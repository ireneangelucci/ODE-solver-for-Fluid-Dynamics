#include "Boundary.hpp"
#include <cmath>
#include <iostream>


FixedWallBoundary::FixedWallBoundary(std::vector<Cell *> cells) : _cells(cells) {}

FixedWallBoundary::FixedWallBoundary(std::vector<Cell *> cells, std::map<int, double> wall_temperature)
    : _cells(cells), _wall_temperature(wall_temperature) {}

void FixedWallBoundary::apply(Fields &field) {
    for(const auto& currentCell: _cells){
        if(currentCell->is_border(border_position::TOP)){ //Bottom cells
            // setting boundary conditions
            field.setv(currentCell->i(),currentCell->j(),0.0);     // v lies on the boundary, setting it as 0.0
            // u doesn't lie on the boundary, hence setting average with neighbouring cell on top as 0.0
            field.setu(currentCell->i(),currentCell->j(),-(field.u(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j())));
            // setting pressure gradient as 0.0
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()));
            field.setg(currentCell->i(),currentCell->j(),field.v(currentCell->i(),currentCell->j()));
        }
        
        if(currentCell->is_border(border_position::BOTTOM)){ //Top cells
            // setting boundary conditions
            field.setv(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),0.0);     // v lies on the boundary, setting it as 0.0
            // u doesn't lie on the boundary, hence setting average with neighbouring cell on top as 0.0
            field.setu(currentCell->i(),currentCell->j(),-(field.u(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j())));
            // setting pressure gradient as 0.0
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
            field.setg(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),field.v(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
        }

        if(currentCell->is_border(border_position::RIGHT)){   //Left cells
            field.setu(currentCell->i(),currentCell->j(),0.0);
            field.setv(currentCell->i(),currentCell->j(),-(field.v(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));
            field.setf(currentCell->i(),currentCell->j(),field.u(currentCell->i(),currentCell->j()));
        }       
        if(currentCell->is_border(border_position::LEFT)){    // Right cells
            field.setu(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),0.0);
            field.setv(currentCell->i(),currentCell->j(),-(field.v(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
            field.setf(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),field.u(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
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

InflowBoundary::InflowBoundary(std::vector<Cell *> cells, double inlet_velocity) 
    : _cells(cells), _inlet_velocity(inlet_velocity) {};

void InflowBoundary::apply(Fields &field) {
    for(const auto& currentCell: _cells){
        if(currentCell->is_border(border_position::RIGHT)){   //Left cells
            field.setu(currentCell->i(),currentCell->j(),_inlet_velocity);
            // v doesn't lie on boundary, setting the avg value with neighbour (bottom) cell as 0 (inflow perpendicular)
            field.setv(currentCell->i(),currentCell->j(),-(field.v(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));
            field.setf(currentCell->i(),currentCell->j(),field.u(currentCell->i(),currentCell->j()));
        }
        if(currentCell->is_border(border_position::LEFT)){   //RIGHT cells
            field.setu(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),_inlet_velocity);
            // v doesn't lie on boundary, setting the avg value with neighbour (bottom) cell as 0 (inflow perpendicular)
            field.setv(currentCell->i(),currentCell->j(),-(field.v(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
            field.setf(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),field.u(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
        }
    }
}

OutflowBoundary::OutflowBoundary(std::vector<Cell *> cells, double outflow_pressure) 
    : _cells(cells), _outflow_pressure(outflow_pressure) {};

void OutflowBoundary::apply(Fields &field) {
    for(const auto& currentCell: _cells){
        if(currentCell->is_border(border_position::LEFT)){   //Right cells
            //field.setu(currentCell->i(),currentCell->j(),field.u(currentCell->neighbour(border_position::LEFT)->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->neighbour(border_position::LEFT)->j()));
            field.setu(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),field.u(currentCell->neighbour(border_position::LEFT)->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->neighbour(border_position::LEFT)->j()));
            // v doesn't lie on boundary, setting the avg value with neighbour (bottom) cell as 0 (inflow perpendicular)
            field.setv(currentCell->i(),currentCell->j(),field.v(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
            field.setp(currentCell->i(),currentCell->j(),2 * _outflow_pressure - field.p(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
            field.setf(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j(),field.u(currentCell->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->j()));
        }
        if(currentCell->is_border(border_position::RIGHT)){   //Left cells
            //field.setu(currentCell->i(),currentCell->j(),field.u(currentCell->neighbour(border_position::LEFT)->neighbour(border_position::LEFT)->i(),currentCell->neighbour(border_position::LEFT)->neighbour(border_position::LEFT)->j()));
            field.setu(currentCell->i(),currentCell->j(),field.u(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));
            // v doesn't lie on boundary, setting the avg value with neighbour (bottom) cell as 0 (inflow perpendicular)
            field.setv(currentCell->i(),currentCell->j(),field.v(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));
            field.setp(currentCell->i(),currentCell->j(),2 * _outflow_pressure - field.p(currentCell->neighbour(border_position::RIGHT)->i(),currentCell->neighbour(border_position::RIGHT)->j()));
            field.setf(currentCell->i(),currentCell->j(),field.u(currentCell->i(),currentCell->j()));
        }
    }
}
