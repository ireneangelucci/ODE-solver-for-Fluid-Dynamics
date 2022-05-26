#include "Boundary.hpp"
#include <cmath>
#include <iostream>


FixedWallBoundary::FixedWallBoundary(std::vector<Cell *> cells) : _cells(cells) {}

FixedWallBoundary::FixedWallBoundary(std::vector<Cell *> cells, double wall_temperature)
    : _cells(cells), _wall_temperature(wall_temperature) {
    }

void FixedWallBoundary::apply(Fields &field) {
    for(const auto& currentCell: _cells){
        
        if(currentCell->is_border(border_position::TOP)){ //Bottom cells
            //std::cout<<"Bottom Cells fixed: "<<currentCell->i()<<", "<<currentCell->j()<<"\n";
            // setting boundary conditions
            field.setv(currentCell->i(),currentCell->j(),0.0);     // v lies on the boundary, setting it as 0.0
            // u doesn't lie on the boundary, hence setting average with neighbouring cell on top as 0.0
            field.setu(currentCell->i(),currentCell->j(),-(field.u(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j())));
            // setting pressure gradient as 0.0
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()));
            field.setg(currentCell->i(),currentCell->j(),field.v(currentCell->i(),currentCell->j()));
            if(field.Energy() == "on"){
                if (currentCell->type() == cell_type::ADIABATIC_WALL){
                    field.setT(currentCell->i(),currentCell->j(),field.T(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()));            
                }
                if (currentCell->type() == cell_type::HOT_WALL || currentCell->type() == cell_type::COLD_WALL){
                    field.setT(currentCell->i(),currentCell->j(),2*_wall_temperature - field.T(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()));
                }  
            }
        }
        
        if(currentCell->is_border(border_position::BOTTOM)){ //Top cells
            //std::cout<<"Top Cells fixed: "<<currentCell->i()<<", "<<currentCell->j()<<"\n";
            // setting boundary conditions
            field.setv(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),0.0);     // v lies on the boundary, setting it as 0.0
            // u doesn't lie on the boundary, hence setting average with neighbouring cell on top as 0.0
            field.setu(currentCell->i(),currentCell->j(),-(field.u(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j())));
            // setting pressure gradient as 0.0
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
            field.setg(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),field.v(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
            if(field.Energy() == "on"){
                if (currentCell->type() == cell_type::ADIABATIC_WALL){
                    field.setT(currentCell->i(),currentCell->j(),field.T(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));            
                }
                if (currentCell->type() == cell_type::HOT_WALL || currentCell->type() == cell_type::COLD_WALL){
                    field.setT(currentCell->i(),currentCell->j(),2*_wall_temperature - field.T(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
                }  
            }        
        }

        if(currentCell->is_border(border_position::RIGHT)){   //Left cells
            //std::cout<<"Left Cells fixed: "<<currentCell->i()<<", "<<currentCell->j()<<"\n";
            field.setu(currentCell->i(),currentCell->j(),0.0);
            field.setv(currentCell->i(),currentCell->j(),-(field.v(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));
            field.setf(currentCell->i(),currentCell->j(),field.u(currentCell->i(),currentCell->j()));
            if(field.Energy() == "on"){
                if (currentCell->type() == cell_type::ADIABATIC_WALL){
                    field.setT(currentCell->i(),currentCell->j(),field.T(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));            
                }
                if (currentCell->type() == cell_type::HOT_WALL || currentCell->type() == cell_type::COLD_WALL){
                    field.setT(currentCell->i(),currentCell->j(),2*_wall_temperature - field.T(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));
                }  
            }   
        }       
        if(currentCell->is_border(border_position::LEFT)){    // Right cells
            //std::cout<<"Right Cells fixed: "<<currentCell->i()<<", "<<currentCell->j()<<"\n";
            field.setu(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),0.0);
            field.setv(currentCell->i(),currentCell->j(),-(field.v(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
            field.setf(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),field.u(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
            if(field.Energy() == "on"){
                if (currentCell->type() == cell_type::ADIABATIC_WALL){
                    field.setT(currentCell->i(),currentCell->j(),field.T(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));            
                }
                if (currentCell->type() == cell_type::HOT_WALL || currentCell->type() == cell_type::COLD_WALL){
                    field.setT(currentCell->i(),currentCell->j(),2*_wall_temperature - field.T(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
                }  
            }  
        }

        if(currentCell->is_border(border_position::TOP) && currentCell->is_border(border_position::RIGHT)){ //Fluid on top and on right 
            // setting boundary conditions
            field.setv(currentCell->i(),currentCell->j(),0.0);     // v lies on the boundary, setting it as 0.0
            field.setu(currentCell->i(),currentCell->j(),0.0);     // u lies on the boundary, setting it as 0.0
            field.setu(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),-field.u(currentCell->neighbour(border_position::LEFT)->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::LEFT)->neighbour(border_position::TOP)->j()));  //U_(i-1,j) = -U_(i-1,j+1)
            field.setv(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),-field.v(currentCell->neighbour(border_position::BOTTOM)->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::BOTTOM)->neighbour(border_position::RIGHT)->j()));  //V_(i,j-1) = -V_(i+1,j-1)
            // setting pressure gradient as 0.0
            field.setp(currentCell->i(),currentCell->j(),(field.p(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()) + field.p(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()))*0.5);  //P_(i,j) = (P_(i,j+1)+P_(i+1,j))/2
            field.setf(currentCell->i(),currentCell->j(),field.u(currentCell->i(),currentCell->j()));
            field.setg(currentCell->i(),currentCell->j(),field.v(currentCell->i(),currentCell->j()));
            if(field.Energy() == "on"){
                if (currentCell->type() == cell_type::ADIABATIC_WALL){
                    field.setT(currentCell->i(),currentCell->j(),field.T(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()) + field.T(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));            
                }
                if (currentCell->type() == cell_type::HOT_WALL || currentCell->type() == cell_type::COLD_WALL){
                    field.setT(currentCell->i(),currentCell->j(),2*_wall_temperature - 0.5* (field.T(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()) + field.T(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j())));
                }  
            }  
        }

        if(currentCell->is_border(border_position::TOP) && currentCell->is_border(border_position::LEFT)){ //Fluid on top and on left 
            // setting boundary conditions
            field.setv(currentCell->i(),currentCell->j(),0.0);     // v lies on the boundary, setting it as 0.0
            field.setu(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),0.0);     // u lies on the boundary, setting it as 0.0
            field.setu(currentCell->i(),currentCell->j(),-field.u(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()));  
            field.setv(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),-field.v(currentCell->neighbour(border_position::BOTTOM)->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::BOTTOM)->neighbour(border_position::LEFT)->j()));  
            // setting pressure gradient as 0.0
            field.setp(currentCell->i(),currentCell->j(),(field.p(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()) + field.p(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()))*0.5);  
            field.setf(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),field.u(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
            field.setg(currentCell->i(),currentCell->j(),field.v(currentCell->i(),currentCell->j()));
            if(field.Energy() == "on"){
                if (currentCell->type() == cell_type::ADIABATIC_WALL){
                    field.setT(currentCell->i(),currentCell->j(),field.T(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()) + field.T(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));            
                }
                if (currentCell->type() == cell_type::HOT_WALL || currentCell->type() == cell_type::COLD_WALL){
                    field.setT(currentCell->i(),currentCell->j(),2*_wall_temperature - 0.5* (field.T(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()) + field.T(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j())));
                } 
            } 
        }

        if(currentCell->is_border(border_position::BOTTOM) && currentCell->is_border(border_position::RIGHT)){
            // setting boundary conditions
            field.setv(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),0.0);     
            field.setu(currentCell->i(),currentCell->j(),0.0);     
            field.setu(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),-field.u(currentCell->neighbour(border_position::LEFT)->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::LEFT)->neighbour(border_position::BOTTOM)->j()));  //U_(i-1,j) = -U_(i-1,j+1)
            field.setv(currentCell->i(),currentCell->j(),-field.v(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));  //V_(i,j-1) = -V_(i+1,j-1)
            // setting pressure gradient as 0.0
            field.setp(currentCell->i(),currentCell->j(),(field.p(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()) + field.p(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()))*0.5);  //P_(i,j) = (P_(i,j+1)+P_(i+1,j))/2
            field.setf(currentCell->i(),currentCell->j(),field.u(currentCell->i(),currentCell->j()));
            field.setg(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),field.v(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
            if(field.Energy() == "on"){
                if (currentCell->type() == cell_type::ADIABATIC_WALL){
                    field.setT(currentCell->i(),currentCell->j(),field.T(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()) + field.T(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));            
                }
                if (currentCell->type() == cell_type::HOT_WALL || currentCell->type() == cell_type::COLD_WALL){
                    field.setT(currentCell->i(),currentCell->j(),2*_wall_temperature - 0.5* (field.T(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()) + field.T(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j())));
                } 
            } 
        }

        if(currentCell->is_border(border_position::BOTTOM) && currentCell->is_border(border_position::LEFT)){ 
            // setting boundary conditions
            field.setv(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),0.0);     
            field.setu(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),0.0);     
            field.setu(currentCell->i(),currentCell->j(),-field.u(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));  
            field.setv(currentCell->i(),currentCell->j(),-field.v(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));  
            // setting pressure gradient as 0.0
            field.setp(currentCell->i(),currentCell->j(),(field.p(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()) + field.p(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()))*0.5);  //P_(i,j) = (P_(i,j+1)+P_(i+1,j))/2
            field.setf(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),field.u(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
            field.setg(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),field.v(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
            if(field.Energy() == "on"){
                if (currentCell->type() == cell_type::ADIABATIC_WALL){
                    field.setT(currentCell->i(),currentCell->j(),field.T(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()) + field.T(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));            
                }
                if (currentCell->type() == cell_type::HOT_WALL || currentCell->type() == cell_type::COLD_WALL){
                    field.setT(currentCell->i(),currentCell->j(),2*_wall_temperature - 0.5* (field.T(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()) + field.T(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j())));
                } 
            } 
        }
    }
}

MovingWallBoundary::MovingWallBoundary(std::vector<Cell *> cells, double wall_velocity) : _cells(cells) {
    _wall_velocity.insert(std::pair(LidDrivenCavity::moving_wall_id, wall_velocity));
}

MovingWallBoundary::MovingWallBoundary(std::vector<Cell *> cells, std::map<int, double> wall_velocity,
                                       std::map<int, double> wall_temperature)
    : _cells(cells), _wall_velocity(wall_velocity), _wall_temperature(wall_temperature) {}

void MovingWallBoundary::apply(Fields &field) {
    for(const auto& currentCell: _cells){
        if(currentCell->is_border(border_position::BOTTOM)){   //Top cells
            field.setv(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),0.0);
            // u doesn't lie on boundary, setting the avg value with neighbour (bottom) cell as the specified wall velocity
            field.setu(currentCell->i(),currentCell->j(),2*_wall_velocity.at(currentCell->wall_id())-(field.u(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
            field.setg(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),field.v(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
        }
    }
}

InflowBoundary::InflowBoundary(std::vector<Cell *> cells, double inlet_velocity_x, double inlet_velocity_y) 
    : _cells(cells), _inlet_velocity_x(inlet_velocity_x), _inlet_velocity_y(inlet_velocity_y) {};

void InflowBoundary::apply(Fields &field) {
    for(const auto& currentCell: _cells){
        if(currentCell->is_border(border_position::RIGHT)){   //Left cells
            field.setu(currentCell->i(),currentCell->j(), _inlet_velocity_x);
            // v doesn't lie on boundary, setting the avg value with neighbour (bottom) cell as 0 (inflow perpendicular)
            field.setv(currentCell->i(),currentCell->j(), 2*_inlet_velocity_y -(field.v(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));
            field.setf(currentCell->i(),currentCell->j(),field.u(currentCell->i(),currentCell->j()));
        }
        if(currentCell->is_border(border_position::LEFT)){   //RIGHT cells
            field.setu(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),_inlet_velocity_x);
            // v doesn't lie on boundary, setting the avg value with neighbour (bottom) cell as 0 (inflow perpendicular)
            field.setv(currentCell->i(),currentCell->j(),2*_inlet_velocity_y -(field.v(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
            field.setf(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),field.u(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
        }
        if(currentCell->is_border(border_position::TOP)){   //BOTTOM cells
            field.setu(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),2*_inlet_velocity_x - field.p(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()));
            field.setv(currentCell->i(),currentCell->j(),_inlet_velocity_y);
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()));
            field.setg(currentCell->i(),currentCell->j(),field.v(currentCell->i(),currentCell->j()));
        }
        if(currentCell->is_border(border_position::BOTTOM)){   //TOP cells
            field.setv(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),_inlet_velocity_y);
            field.setu(currentCell->i(),currentCell->j(),2*_inlet_velocity_x -(field.u(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
            field.setg(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),field.v(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
        }
    }
}

OutflowBoundary::OutflowBoundary(std::vector<Cell *> cells, double outflow_pressure) 
    : _cells(cells), _outflow_pressure(outflow_pressure) {
        //std::cout<<"Out press is "<<_outflow_pressure<<"\n";
    };

void OutflowBoundary::apply(Fields &field) {
    for(const auto& currentCell: _cells){
        if(currentCell->is_border(border_position::LEFT)){   //Right cells
            field.setu(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),field.u(currentCell->neighbour(border_position::LEFT)->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->neighbour(border_position::LEFT)->j()));
            // v doesn't lie on boundary, setting the avg value with neighbour (bottom) cell as 0 (inflow perpendicular)
            field.setv(currentCell->i(),currentCell->j(),field.v(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
            field.setp(currentCell->i(),currentCell->j(),2 * _outflow_pressure - field.p(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
            field.setf(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),field.u(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
        }
        if(currentCell->is_border(border_position::RIGHT)){   //Left cells
            field.setu(currentCell->i(),currentCell->j(),field.u(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));
            // v doesn't lie on boundary, setting the avg value with neighbour (bottom) cell as 0 (inflow perpendicular)
            field.setv(currentCell->i(),currentCell->j(),field.v(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));
            field.setp(currentCell->i(),currentCell->j(),2 * _outflow_pressure - field.p(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));
            field.setf(currentCell->i(),currentCell->j(),field.u(currentCell->i(),currentCell->j()));
        }
        if(currentCell->is_border(border_position::BOTTOM)){   //Top cells
            field.setv(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),field.v(currentCell->neighbour(border_position::BOTTOM)->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->neighbour(border_position::BOTTOM)->j()));
            // v doesn't lie on boundary, setting the avg value with neighbour (bottom) cell as 0 (inflow perpendicular)
            field.setu(currentCell->i(),currentCell->j(),field.u(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
            field.setp(currentCell->i(),currentCell->j(),2 * _outflow_pressure - field.p(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
            field.setg(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),field.v(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
        }
        if(currentCell->is_border(border_position::TOP)){   //Bottom cells
            field.setv(currentCell->i(),currentCell->j(),field.v(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()));
            // v doesn't lie on boundary, setting the avg value with neighbour (bottom) cell as 0 (inflow perpendicular)
            field.setu(currentCell->i(),currentCell->j(),field.u(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()));
            field.setp(currentCell->i(),currentCell->j(),2 * _outflow_pressure - field.p(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()));
            field.setg(currentCell->i(),currentCell->j(),field.v(currentCell->i(),currentCell->j()));
        }
    }
}
