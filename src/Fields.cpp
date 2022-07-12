#include "Fields.hpp"
#include "Communication.hpp"

#include<cmath>
#include <algorithm>
#include <iostream>


Fields::Fields(double nu, double dt, double tau, double alpha, double beta, int imax, int jmax, double UI, double VI, double PI, double TI, double GX, double GY, Grid &grid, std::string energy_eq, std::string nonnewton_vis)
    : _nu(nu), _dt(dt), _tau(tau), _alpha(alpha), _beta(beta), _energy_eq(energy_eq), _nonnewton_vis(nonnewton_vis) {
    // intializing u, v and p
    _U = Matrix<double>(imax + 2, jmax + 2, 0.0);
    _V = Matrix<double>(imax + 2, jmax + 2, 0.0);
    _P = Matrix<double>(imax + 2, jmax + 2, 0.0);
    _T = Matrix<double>(imax + 2, jmax + 2, 0.0);

    _F = Matrix<double>(imax + 2, jmax + 2, 0.0);
    _G = Matrix<double>(imax + 2, jmax + 2, 0.0);
    _RS = Matrix<double>(imax + 2, jmax + 2, 0.0);
    _NU = Matrix<double>(imax + 2, jmax + 2, _nu);

    _gx = GX;
    _gy = GY;

    for(auto &currentCell: grid.fluid_cells()){  
        int i = currentCell->i();
        int j = currentCell->j();
        setu(i,j,UI);
        setv(i,j,VI);
        setp(i,j,PI);
        if(_energy_eq == "on"){
            setT(i,j,TI);
        }
    }
    for(auto &currentCell: grid.boundary_fluid_cells()){  
        int i = currentCell->i();
        int j = currentCell->j();
        setu(i,j,UI);
        setv(i,j,VI);
        setp(i,j,PI);
        if(_energy_eq == "on"){
            setT(i,j,TI);
        }
    }
    if(_energy_eq == "on"){
        for(auto &currentCell: grid.inflow_cells()){
            setT(currentCell->i(),currentCell->j(),TI);
        }
    }
}

void Fields::calculate_fluxes(Grid &grid) {
    for(auto &currentCell: grid.fluid_cells()){
        //std::cout << "entered for loop \n";
        int i = currentCell->i();
        int j = currentCell->j();
        if(i != grid.imax() || (i == grid.imax() && currentCell->neighbour(border_position::RIGHT)->type() == cell_type::BOUNDARY_FLUID) ){   //excluding imax as f_imax is part of the fixed boundary and set as 0.0
            setf(i,j,u(i,j)+dt()*(0.5*(nu(i,j)+nu(i,j-1))*(Discretization::diffusion(_U, i, j))-Discretization::convection_u(_U, _V, i, j) - 0.5*_beta*_gx*(T(i,j)+T(i+1,j)) ));
        }
        if(j != grid.jmax() || (j == grid.jmax() && currentCell->neighbour(border_position::TOP)->type() == cell_type::BOUNDARY_FLUID)){   // excluding jmax as g_jmax is part of the moving boundary and set as 0.0
            setg(i,j,v(i,j)+dt()*(0.5*(nu(i,j)+nu(i-1,j))*(Discretization::diffusion(_V, i, j))-Discretization::convection_v(_U, _V, i, j) - 0.5*_beta*_gy*(T(i,j)+T(i,j+1)) ));
        }
    }
}

void Fields::calculate_rs(Grid &grid) {
    for(auto &currentCell: grid.fluid_cells()){
        int i = currentCell->i();
        int j = currentCell->j();
        // calculating the rhs of pressure equation
        double val = ((f(i,j)-f(i-1,j))/grid.dx() + (g(i,j)-g(i,j-1))/grid.dy())/dt();
        setrs(i, j, val);        
    }
}

void Fields::calculate_velocities(Grid &grid) {
    for(auto &currentCell: grid.fluid_cells()){
        int i = currentCell->i();
        int j = currentCell->j();
        if(i != grid.imax() || (i == grid.imax() && currentCell->neighbour(border_position::RIGHT)->type() == cell_type::BOUNDARY_FLUID)){   //excluding imax as u_imax is part of the fixed boundary and set as 0.0
            setu(i, j, f(i,j) - _dt/grid.dx()*(p(i+1,j)-p(i,j)));
        }
        if(j != grid.jmax() || (j == grid.jmax() && currentCell->neighbour(border_position::TOP)->type() == cell_type::BOUNDARY_FLUID)){   // excluding jmax as v_jmax is part of the moving boundary (in x direction) and set as 0.0
            setv(i, j, g(i,j) - _dt/grid.dy()*(p(i,j+1)-p(i,j)));       
        }
    }
}

void Fields::calculate_viscosity(Grid &grid){
    double uy, vx, gamma;
    for(auto &currentCell: grid.fluid_cells()){
        int i = currentCell->i();
        int j = currentCell->j();
        //bool x = (i != grid.imax()) || (i == grid.imax() && currentCell->neighbour(border_position::RIGHT)->type() == cell_type::BOUNDARY_FLUID);
        //bool y = (j != grid.jmax()) || (j == grid.jmax() && currentCell->neighbour(border_position::TOP)->type() == cell_type::BOUNDARY_FLUID);
        //if (x || y){
            uy = (u(i,j+1)-u(i,j))/grid.dy();
            vx = (v(i+1,j)-v(i,j))/grid.dx(); 
            gamma = 0.5 * (std::abs(uy) + std::abs(vx));
            if (pow(gamma, -0.0225) > 0.0034 ) {
                setnu(i,j,_nu*pow(gamma, -0.225) );
            }
            else {setnu(i,j,_nu*0.0034);}          
        //}
            
    }
}

//calculating temperature at new timestep
void Fields::calculate_Temperature(Grid &grid){
    Matrix<double> tempT = _T;
    for(auto &currentCell: grid.fluid_cells()){
        int i = currentCell->i();
        int j = currentCell->j();
        tempT(i,j) = _T(i,j) + _dt*(_alpha * (Discretization::diffusion(_T,i,j)) - Discretization::convection_T(_T,_U,_V,i,j));
    }
    _T = tempT;
}

//calculating the timestep keeping in mind the stability crtiteria
double Fields::calculate_dt(Grid &grid) { 
    double dt;
    double nu_max = _nu;
    if(_nonnewton_vis == "on"){
        for(auto &currentCell: grid.fluid_cells()){
        int i = currentCell->i();
        int j = currentCell->j();
        if (nu(i,j) > nu_max) {nu_max = nu(i,j);}
        }
    }
    if((nu_max < _alpha) && (_energy_eq != "on")){
        dt = 0.5/_alpha/(1/grid.dx()/grid.dx() + 1/grid.dy()/grid.dy())*_tau;
    }else{
        dt = 0.5/nu_max/(1/grid.dx()/grid.dx() + 1/grid.dy()/grid.dy())*_tau;
    }
    
    for(auto &currentCell: grid.fluid_cells()){
        int i = currentCell->i();
        int j = currentCell->j();
        double dt1 = std::abs(grid.dx()/u(i,j))*_tau;
        double dt2 = std::abs(grid.dy()/v(i,j))*_tau;
        if(dt > dt1){  
            dt = dt1;
        }
        if(dt > dt2){  
            dt = dt2;
        }
    }
    _dt = Communication::reduce_min(dt);
    return _dt; 
}

// get functions
double &Fields::p(int i, int j) { return _P(i, j); }
double &Fields::u(int i, int j) { return _U(i, j); }
double &Fields::v(int i, int j) { return _V(i, j); }
double &Fields::f(int i, int j) { return _F(i, j); }
double &Fields::g(int i, int j) { return _G(i, j); }
double &Fields::rs(int i, int j) { return _RS(i, j); }
double &Fields::T(int i, int j) { return _T(i, j); }
double &Fields::nu(int i, int j) { return _NU(i, j); }
std::string &Fields::Energy() { return _energy_eq; }
std::string &Fields::NonNewtonian() { return _nonnewton_vis; }

Matrix<double> &Fields::p_matrix() { return _P; }
Matrix<double> &Fields::u_matrix() { return _U; }
Matrix<double> &Fields::v_matrix() { return _V; }
Matrix<double> &Fields::T_matrix() { return _T; }
Matrix<double> &Fields::f_matrix() { return _F; }
Matrix<double> &Fields::g_matrix() { return _G; }
Matrix<double> &Fields::rs_matrix() { return _RS; }
Matrix<double> &Fields::nu_matrix() { return _NU; }

double Fields::dt() const { return _dt; }

// set functions
void Fields::setp(int i, int j, double val) { _P(i, j) = val; }
void Fields::setT(int i, int j, double val) { _T(i, j) = val; }
void Fields::setu(int i, int j, double val) { _U(i, j) = val; }
void Fields::setv(int i, int j, double val) { _V(i, j) = val; }
void Fields::setf(int i, int j, double val) { _F(i, j) = val; }
void Fields::setg(int i, int j, double val) { _G(i, j) = val; }
void Fields::setrs(int i, int j, double val) { _RS(i, j) = val; }
void Fields::setnu(int i, int j, double val) { _NU(i, j) = val; }