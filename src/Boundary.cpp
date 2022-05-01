#include "Boundary.hpp"
#include <cmath>
#include <iostream>

FixedWallBoundary::FixedWallBoundary(std::vector<Cell *> cells) : _cells(cells) {}

FixedWallBoundary::FixedWallBoundary(std::vector<Cell *> cells, std::map<int, double> wall_temperature)
    : _cells(cells), _wall_temperature(wall_temperature) {}

void FixedWallBoundary::apply(Fields &field) {
    for(const auto c : _cells){
        if(c->is_border(border_position::BOTTOM)){
            field.setv(c->i(),c->j(),0.0);
            field.setu(c->i(),c->j(),-(field.u(c->i(),(c->j())+1)));
            field.setp(c->i(),c->j(),field.p(c->i(),(c->j())+1));
        }
        if(c->is_border(border_position::LEFT)){
            field.setu(c->i(),c->j(),0.0);
            field.setv(c->i(),c->j(),-(field.v(c->i()+1,c->j())));
            field.setp(c->i(),c->j(),field.p(c->i()+1,c->j()));
        }
        if(c->is_border(border_position::RIGHT)){
            field.setu(c->i()-1,c->j(),0.0);
            field.setv(c->i(),c->j(),-(field.v(c->i()-1,c->j())));
            field.setp(c->i(),c->j(),field.p(c->i()-1,c->j()));
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
        if(c->is_border(border_position::TOP)){
            field.setv(c->i(),c->j()-1,0.0);
            field.setu(c->i(),c->j(),2*_wall_velocity.at(c->i())-(field.u(c->i(),(c->j())-1)));
            field.setp(c->i(),c->j(),field.p(c->i(),(c->j())-1));
        }
    }
}
