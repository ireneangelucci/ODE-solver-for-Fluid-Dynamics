#include "Boundary.hpp"
#include <cmath>
#include <iostream>


FixedWallBoundary::FixedWallBoundary(std::vector<Cell *> cells) : _cells(cells) {}

FixedWallBoundary::FixedWallBoundary(std::vector<Cell *> cells, std::map<int, double> wall_temperature)
    : _cells(cells), _wall_temperature(wall_temperature) {}

void FixedWallBoundary::apply(Fields &field) {
    for(const auto& currentCell: _cells){
        if(currentCell->is_border(border_position::TOP)){ //Bottom cells
            field.setv(currentCell->i(),currentCell->j(),0.0);
            field.setu(currentCell->i(),currentCell->j(),-(field.u(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::TOP)->i(),currentCell->neighbour(border_position::TOP)->j()));
            field.setg(currentCell->i(),currentCell->j(),field.v(currentCell->i(),currentCell->j()));
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
            field.setu(currentCell->i(),currentCell->j(),2*_wall_velocity.at(currentCell->wall_id())-(field.u(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j())));
            field.setp(currentCell->i(),currentCell->j(),field.p(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
            field.setg(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j(),field.v(currentCell->neighbour(border_position::BOTTOM)->i(),currentCell->neighbour(border_position::BOTTOM)->j()));
        }
    }
}
