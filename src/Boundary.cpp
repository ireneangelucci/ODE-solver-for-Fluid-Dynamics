#include "Boundary.hpp"
#include <cmath>
#include <iostream>

FixedWallBoundary::FixedWallBoundary(std::vector<Cell *> cells) : _cells(cells) {}

FixedWallBoundary::FixedWallBoundary(std::vector<Cell *> cells, std::map<int, double> wall_temperature)
    : _cells(cells), _wall_temperature(wall_temperature) {}

void FixedWallBoundary::apply(Fields &field) {
    for(const auto c : _cells){
        if(c->is_border(border_position::TOP)){ //Bottom cells
            field.setv(c->i(),c->j(),0.0);
            field.setu(c->i(),c->j(),-(field.u(c->neighbour(border_position::TOP)->i(),c->neighbour(border_position::TOP)->j())));
            field.setp(c->i(),c->j(),field.p(c->neighbour(border_position::TOP)->i(),c->neighbour(border_position::TOP)->j()));
            field.setg(c->i(),c->j(),field.v(c->i(),c->j()));
        }
        
        if(c->is_border(border_position::RIGHT)){   //Left cells
            field.setu(c->i(),c->j(),0.0);
            field.setv(c->i(),c->j(),-(field.v(c->neighbour(border_position::RIGHT)->i(),c->neighbour(border_position::RIGHT)->j())));
            field.setp(c->i(),c->j(),field.p(c->neighbour(border_position::RIGHT)->i(),c->neighbour(border_position::RIGHT)->j()));
            field.setf(c->i(),c->j(),field.u(c->i(),c->j()));
        }       
        if(c->is_border(border_position::LEFT)){    // Right cells
            field.setu(c->neighbour(border_position::LEFT)->i(),c->neighbour(border_position::LEFT)->j(),0.0);
            field.setv(c->i(),c->j(),-(field.v(c->neighbour(border_position::LEFT)->i(),c->neighbour(border_position::LEFT)->j())));
            field.setp(c->i(),c->j(),field.p(c->neighbour(border_position::LEFT)->i(),c->neighbour(border_position::LEFT)->j()));
            field.setf(c->neighbour(border_position::LEFT)->i(),c->neighbour(border_position::LEFT)->j(),field.u(c->neighbour(border_position::LEFT)->i(),c->neighbour(border_position::LEFT)->j()));
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
    for(const auto c : _cells){
        if(c->is_border(border_position::BOTTOM)){
            field.setv(c->neighbour(border_position::BOTTOM)->i(),c->neighbour(border_position::BOTTOM)->j(),0.0);
            field.setu(c->i(),c->j(),2*_wall_velocity.at(c->wall_id())-(field.u(c->neighbour(border_position::BOTTOM)->i(),c->neighbour(border_position::BOTTOM)->j())));
            field.setp(c->i(),c->j(),field.p(c->neighbour(border_position::BOTTOM)->i(),c->neighbour(border_position::BOTTOM)->j()));
            field.setg(c->neighbour(border_position::BOTTOM)->i(),c->neighbour(border_position::BOTTOM)->j(),field.v(c->neighbour(border_position::BOTTOM)->i(),c->neighbour(border_position::BOTTOM)->j()));
        }
    }
}
