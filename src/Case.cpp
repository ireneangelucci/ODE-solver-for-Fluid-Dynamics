#include "Case.hpp"
#include "Enums.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <iomanip>

namespace filesystem = std::filesystem;

#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredGrid.h>
#include <vtkStructuredGridWriter.h>
#include <vtkTuple.h>

Case::Case(std::string file_name, int argn, char **args) {
    // Read input parameters
    
    const int MAX_LINE_LENGTH = 1024;
    std::ifstream file(file_name);
    double nu;      /* viscosity   */
    double UI;      /* velocity x-direction */
    double VI;      /* velocity y-direction */
    double PI;      /* pressure */
    double GX;      /* gravitation x-direction */
    double GY;      /* gravitation y-direction */
    double xlength; /* length of the domain x-dir.*/
    double ylength; /* length of the domain y-dir.*/
    double dt;      /* time step */
    int imax;       /* number of cells x-direction*/
    int jmax;       /* number of cells y-direction*/
    double gamma;   /* uppwind differencing factor*/
    double omg;     /* relaxation factor */
    double tau;     /* safety factor for time step*/
    int itermax;    /* max. number of iterations for pressure per time step */
    double eps;     /* accuracy bound for pressure*/

    double dt_value;           /* time for output */
    std::string program;
    std::string energy_eq{"NONE"};
    double TI;                 /* initial temperature  */
    double Pr;                 /* prandtl number   */
    double beta;               /* the coefficient of thermal expansion */
    double alpha;
    double deltaP;
    double UIN; 
    double VIN; 
    int num_walls;
    double coldwall_temp;
    double hotwall_temp;
    int iproc=1;
    int jproc=1;

    if (file.is_open()) {

        std::string var;
        while (!file.eof() && file.good()) {
            file >> var;
            if (var[0] == '#') { /* ignore comment line*/
                file.ignore(MAX_LINE_LENGTH, '\n');
            } else {
                if (var == "xlength") file >> xlength;
                if (var == "ylength") file >> ylength;
                if (var == "nu") file >> nu;
                if (var == "t_end") file >> _t_end;
                if (var == "dt") file >> dt;
                if (var == "omg") file >> omg;
                if (var == "eps") file >> eps;
                if (var == "tau") file >> tau;
                if (var == "gamma") file >> gamma;
                if (var == "dt_value") file >> _output_freq;
                if (var == "UI") file >> UI;
                if (var == "VI") file >> VI;
                if (var == "GX") file >> GX;
                if (var == "GY") file >> GY;
                if (var == "PI") file >> PI;
                if (var == "itermax") file >> itermax;
                if (var == "imax") file >> imax;
                if (var == "jmax") file >> jmax;
                if (var == "iproc") file >> iproc;
                if (var == "jproc") file >> jproc;
                if (var == "program") file >> program;
                if (var == "geo_file") file >> _geom_name;
                if (var == "TI") file >> TI;
                if (var == "energy_eq") file >> energy_eq;
                if (var == "alpha") file >> alpha;
                if (var == "beta") file >> beta;
                if (var == "deltaP") file >> deltaP;
                if (var == "UIN") file >> UIN;
                if (var == "VIN") file >> VIN;
                if (var == "num_walls") file >> num_walls;
                if (var == "wall_temp_5") file >> coldwall_temp;
                if (var == "wall_temp_4") file >> hotwall_temp;
            }
        }
    }
    file.close();

    //int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &_my_rank);
    
    int nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    if (nprocs == 1){
        iproc = 1;
        jproc = 1;
    }
    else if(nprocs != iproc*jproc && _my_rank == 0){
        std::cerr << "No. of processes not compatible with input file: Nprocs must be Iprocs*Jprocs \n"<<"Iprocs: "<<iproc<<", Jprocs: "<<jproc<<"\n";
        exit(1);
    }

    std::map<int, double> wall_vel;
    if (_geom_name.compare("NONE") == 0) {
        wall_vel.insert(std::pair<int, double>(LidDrivenCavity::moving_wall_id, LidDrivenCavity::wall_velocity));
    }

    // Set file names for geometry file and output directory
    set_file_names(file_name);

    // Build up the domain
    Domain domain;
    domain.dx = xlength / (double)imax;
    domain.dy = ylength / (double)jmax;
    domain.domain_size_x = imax;
    domain.domain_size_y = jmax;

    build_domain(domain, imax, jmax, iproc, jproc);
    _communication = Communication(_my_rank, domain);
    _grid = Grid(_geom_name, domain);
    _field = Fields(nu, dt, tau, alpha, beta, _grid.domain().size_x, _grid.domain().size_y, UI, VI, PI, TI, GX, GY, _grid, energy_eq);
    

    _discretization = Discretization(domain.dx, domain.dy, gamma);
    _pressure_solver = std::make_unique<SOR>(omg);
    _max_iter = itermax;
    _tolerance = eps;

    // Construct boundaries
    if (not _grid.boundary_fluid_cells().empty()) {
        _boundaries.push_back(
            std::make_unique<FluidBoundary>(_grid.boundary_fluid_cells()));
    }
    if (not _grid.moving_wall_cells().empty()) {
        _boundaries.push_back(
            std::make_unique<MovingWallBoundary>(_grid.moving_wall_cells(), LidDrivenCavity::wall_velocity));
    }
    if (not _grid.fixed_wall_cells().empty()) {
        _boundaries.push_back(std::make_unique<FixedWallBoundary>(_grid.fixed_wall_cells()));
    }
    if (not _grid.hot_wall_cells().empty()) {
        _boundaries.push_back(std::make_unique<FixedWallBoundary>(_grid.hot_wall_cells(), hotwall_temp));
    }
    if (not _grid.cold_wall_cells().empty()) {
        _boundaries.push_back(std::make_unique<FixedWallBoundary>(_grid.cold_wall_cells(), coldwall_temp));
    }
    if (not _grid.inflow_cells().empty()) {
        _boundaries.push_back(std::make_unique<InflowBoundary>(_grid.inflow_cells(),UIN, VIN));
    }
    double Pout = 0.0;
    if (not _grid.outflow_cells().empty()) {
        _boundaries.push_back(std::make_unique<OutflowBoundary>(_grid.outflow_cells(),Pout));
    }
}

void Case::set_file_names(std::string file_name) {
    std::string temp_dir;
    bool case_name_flag = true;
    bool prefix_flag = false;

    for (int i = file_name.size() - 1; i > -1; --i) {
        if (file_name[i] == '/') {
            case_name_flag = false;
            prefix_flag = true;
        }
        if (case_name_flag) {
            _case_name.push_back(file_name[i]);
        }
        if (prefix_flag) {
            _prefix.push_back(file_name[i]);
        }
    }

    for (int i = file_name.size() - _case_name.size() - 1; i > -1; --i) {
        temp_dir.push_back(file_name[i]);
    }

    std::reverse(_case_name.begin(), _case_name.end());
    std::reverse(_prefix.begin(), _prefix.end());
    std::reverse(temp_dir.begin(), temp_dir.end());

    _case_name.erase(_case_name.size() - 4);
    _dict_name = temp_dir;
    _dict_name.append(_case_name);
    _dict_name.append("_Output");

    if (_geom_name.compare("NONE") != 0) {
        _geom_name = _prefix + _geom_name;
    }
    
    filesystem::path folder(_dict_name);

    //Check if directory already exists, delete it if it does
    if (filesystem::exists(_dict_name) == 1){
        filesystem::remove_all(folder);
    }

    // Create output directory 
    try {
        filesystem::create_directory(folder);
    } catch (const std::exception &e) {
        std::cerr << "Output directory could not be created." << std::endl;
        std::cerr << "Make sure that you have write permissions to the "
                     "corresponding location"
                  << std::endl;
    }
}

/**
 * This function is the main simulation loop. In the simulation loop, following steps are required
 * Calculate and apply boundary conditions for all the boundaries in _boundaries container
 * using apply() member function of Boundary class
 * Calculate fluxes (F and G) using calculate_fluxes() member function of Fields class.
 * Flux consists of diffusion and convection part, which are located in Discretization class
 * Calculate right-hand-side of PPE using calculate_rs() member function of Fields class
 * Iterate the pressure poisson equation until the residual becomes smaller than the desired tolerance
 * or the maximum number of the iterations are performed using solve() member function of PressureSolver class
 * Calculate the velocities u and v using calculate_velocities() member function of Fields class
 * Calculat the maximal timestep size for the next iteration using calculate_dt() member function of Fields class
 * Write vtk files using output_vtk() function
 *
 * Please note that some classes such as PressureSolver, Boundary are abstract classes which means they only provide the
 * interface. No member functions should be defined in abstract classes. You need to define functions in inherited
 * classes such as MovingWallBoundary class.
 *
 * For information about the classes and functions, you can check the header files.
 */
void Case::simulate() {
    if(_my_rank == 0){
        std::cout << "Simulation started \n";
    }
    double t = 0.0;
    //_field.calculate_dt(_grid);
    double dt = _field.dt();
    int timestep = 0;
    int output_counter = 0;
    std::string convergence;
    const int numWidth = 15;
    const char separator = ' ';

    // starting the time loop
    while(t < _t_end){
        
        Communication::communicate(_field.u_matrix());
        Communication::communicate(_field.v_matrix());
        // applying boundary
        for(auto &boundary: _boundaries){
            boundary->apply(_field);
        }
        if(_field.Energy() == "on") {
            _field.calculate_Temperature(_grid);
            Communication::communicate(_field.T_matrix());
        }
        _field.calculate_fluxes(_grid);
        Communication::communicate(_field.f_matrix());
        Communication::communicate(_field.g_matrix());

        _field.calculate_rs(_grid);

        int it = 0;
        double res = 1.0;
        double max_res = res;
        // starting iteration for solving pressure at next time step
        while(it < _max_iter && max_res > _tolerance){
            res = _pressure_solver->solve(_field, _grid, _boundaries);
            Communication::communicate(_field.p_matrix());
            max_res = res;
            MPI_Allreduce(&res, &max_res, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
            it++;
        }

        if(it < _max_iter){
            convergence = "Converged";
        }
        else{
            convergence = "Not Converged";
        }

        // output on screen - time, timestep, residual and convergence status of pressure eqn.
        if(_my_rank == 0){
            std::cout << std::left << std::setw(12) << std::setfill(separator) << "Timestep: " ;
            std::cout << std::left << std::setw(numWidth) << std::setfill(separator) << timestep;
            std::cout << std::left << std::setw(8) << std::setfill(separator) << "Time: ";
            std::cout << std::left << std::setw(numWidth) << std::setfill(separator) << t;
            std::cout << std::left << std::setw(12) << std::setfill(separator) << "Residual: ";
            std::cout << std::left << std::setw(numWidth) << std::setfill(separator) << res;
            std::cout << std::left << std::setw(numWidth) << std::setfill(separator) << convergence;
            std::cout << std::left << std::setw(12) << std::setfill(separator) << "Iterations:";
            std::cout << std::left << std::setw(12) << std::setfill(separator) << it;
            std::cout << std::endl;
        }

        // calculating velocities at next timestep 
        _field.calculate_velocities(_grid);

        if(t >= output_counter*_output_freq){
            output_vtk(timestep, _my_rank);
            output_counter += 1;
        }
        t = t + _field.dt();
        _field.calculate_dt(_grid);
        timestep +=1;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(Communication::_my_rank == 0)
    {
        std::cout << "Simulation Ended \n";
    }
}

void Case::output_vtk(int timestep, int my_rank) {
    
    // Create a new structured grid
    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();

    // Create grid
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    double dx = _grid.dx();
    double dy = _grid.dy();

    double x = _grid.domain().imin * dx;
    double y = _grid.domain().jmin * dy;

    { y += dy; }
    { x += dx; }
    
    double z = 0;
    std::vector<vtkIdType> current_cell;

    for (int col = 0; col < _grid.domain().size_y + 1; col++) {
        x = _grid.domain().imin * dx;
        { x += dx; }
        for (int row = 0; row < _grid.domain().size_x + 1; row++) {
            current_cell.push_back(points->InsertNextPoint(x, y, z));
            x += dx;
        }
        y += dy;
    }

    // Specify the dimensions of the grid
    structuredGrid->SetDimensions(_grid.domain().size_x + 1, _grid.domain().size_y + 1, 1);
    structuredGrid->SetPoints(points);


    // Output obstacle cells as blank cells
    int k=0;
    for (int col = 0; col < _grid.domain().size_y + 1; col++) {
        for (int row = 0; row < _grid.domain().size_x + 1; row++) {
            if((_grid.cell(row,col).type() != cell_type::FLUID) && (_grid.cell(row,col).type() != cell_type::BOUNDARY_FLUID)){
                structuredGrid->BlankPoint(current_cell[k]);
            }
        k++;
        }
    }

    // Pressure Array
    vtkDoubleArray *Pressure = vtkDoubleArray::New();
    Pressure->SetName("pressure");
    Pressure->SetNumberOfComponents(1);

    // Temperature Array
    vtkDoubleArray *Temperature = vtkDoubleArray::New();
    Temperature->SetName("Temperature");
    Temperature->SetNumberOfComponents(1);

    // Velocity Array
    vtkDoubleArray *Velocity = vtkDoubleArray::New();
    Velocity->SetName("velocity");
    Velocity->SetNumberOfComponents(3);

    // Print pressure from bottom to top
    for (int j = 1; j < _grid.domain().size_y + 1; j++) {
        for (int i = 1; i < _grid.domain().size_x + 1; i++) {
            double pressure = _field.p(i, j);
            Pressure->InsertNextTuple(&pressure);
        }
    }
    
    // Print temperature from bottom to top
    for (int j = 1; j < _grid.domain().size_y + 1; j++) {
        for (int i = 1; i < _grid.domain().size_x + 1; i++) {
            double temp = _field.T(i, j);
            Temperature->InsertNextTuple(&temp);
        }
    }

    // Temp Velocity
    float vel[3];
    vel[2] = 0; // Set z component to 0

    // Print Velocity from bottom to top
    for (int j = 0; j < _grid.domain().size_y + 1; j++) {
        for (int i = 0; i < _grid.domain().size_x + 1; i++) {
            vel[0] = (_field.u(i, j) + _field.u(i, j + 1)) * 0.5;
            vel[1] = (_field.v(i, j) + _field.v(i + 1, j)) * 0.5;
            Velocity->InsertNextTuple(vel);
        }
    }

    // Add Pressure to Structured Grid
    structuredGrid->GetCellData()->AddArray(Pressure);

    // Add Temperature to Structured Grid
    if(_field.Energy() == "on"){
        structuredGrid->GetCellData()->AddArray(Temperature);
    }
    // Add Velocity to Structured Grid
    structuredGrid->GetPointData()->AddArray(Velocity);

    // Write Grid
    vtkSmartPointer<vtkStructuredGridWriter> writer = vtkSmartPointer<vtkStructuredGridWriter>::New();

    // Create Filename
    std::string outputname =
        _dict_name + '/' + _case_name + "_" + std::to_string(my_rank) + "." + std::to_string(timestep) + ".vtk";

    writer->SetFileName(outputname.c_str());
    writer->SetInputData(structuredGrid);
    writer->Write();
}

void Case::build_domain(Domain &domain, int imax_domain, int jmax_domain, int iproc, int jproc) {
    if(_my_rank == 0){
        domain.imin = 0;
        domain.jmin = 0;
        domain.imax = imax_domain/iproc + 2;
        domain.jmax = jmax_domain/jproc + 2;
        domain.size_x = imax_domain/iproc;
        domain.size_y = jmax_domain/jproc;
        if(iproc > 1){ domain.neighbours[2]=1; }
        if(jproc > 1){ domain.neighbours[3]=iproc; }

        int imin, jmin, imax, jmax, curr_rank, size_x, size_y;
        for(int j = 0; j < jproc; j++){
            for(int i = 0; i < iproc; i++){
                std::array<int, 4> neighbours{MPI_PROC_NULL, MPI_PROC_NULL, MPI_PROC_NULL, MPI_PROC_NULL}; 
                if(i==0 && j==0){continue;}
                imin = i * (imax_domain/iproc);
                jmin = j * (jmax_domain/jproc);
                imax = (i+1) * (imax_domain/iproc)+2;
                jmax = (j+1) * (jmax_domain/jproc)+2;
                if((i+1) == iproc){
                    imax = imax_domain+2;
                }
                if((j+1) == jproc){
                    jmax = jmax_domain+2;
                }   
                size_x = imax - imin -2;
                size_y = jmax - jmin -2;
                curr_rank = i+j*iproc;
                if(i<iproc-1){neighbours[2] = (i+1)+j*iproc; }
                if(i>0){neighbours[0] = (i-1)+j*iproc; }
                if(j<jproc-1){neighbours[3] = i+(j+1)*iproc; }
                if(j>0){neighbours[1] = i+(j-1)*iproc; }
                MPI_Send(&neighbours, 4, MPI_INT, curr_rank, 123, MPI_COMM_WORLD);
                MPI_Send(&imin, 1, MPI_INT, curr_rank, 1, MPI_COMM_WORLD);
                MPI_Send(&jmin, 1, MPI_INT, curr_rank, 2, MPI_COMM_WORLD);
                MPI_Send(&imax, 1, MPI_INT, curr_rank, 3, MPI_COMM_WORLD);
                MPI_Send(&jmax, 1, MPI_INT, curr_rank, 4, MPI_COMM_WORLD);
                MPI_Send(&size_x, 1, MPI_INT, curr_rank, 5, MPI_COMM_WORLD);
                MPI_Send(&size_y, 1, MPI_INT, curr_rank, 6, MPI_COMM_WORLD);
            }
        }        
    }else{
        MPI_Status status;
        MPI_Recv(&domain.neighbours, 4, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
        MPI_Recv(&domain.imin, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&domain.jmin, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        MPI_Recv(&domain.imax, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
        MPI_Recv(&domain.jmax, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
        MPI_Recv(&domain.size_x, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
        MPI_Recv(&domain.size_y, 1, MPI_INT, 0, 6, MPI_COMM_WORLD, &status);
        //std::cout<<_my_rank<<" Imin: "<<domain.imin<<" Jmin: "<<domain.jmin<<"\n";
        //std::cout<<_my_rank<<" Imax: "<<domain.imax<<" Jmax: "<<domain.jmax<<"\n";         
    }
    //std::cout << "exiting build domain, rank "<< _my_rank <<  "\n";
}
