#include "Fields.hpp"

#include <algorithm>
#include <iostream>

Fields::Fields(double nu, double dt, double tau, int imax, int jmax, double UI, double VI, double PI)
    : _nu(nu), _dt(dt), _tau(tau) {
    _U = Matrix<double>(imax + 2, jmax + 2, UI);
    _V = Matrix<double>(imax + 2, jmax + 2, VI);
    _P = Matrix<double>(imax + 2, jmax + 2, PI);

    _F = Matrix<double>(imax + 2, jmax + 2, 0.0);
    _G = Matrix<double>(imax + 2, jmax + 2, 0.0);
    _RS = Matrix<double>(imax + 2, jmax + 2, 0.0);
}

void Fields::calculate_fluxes(Grid &grid, Discretization& disc) {

    for(auto &cell: grid.fluid_cells()){
        int i = cell->i();
        int j = cell->j();
        setf(i,j,u(i,j)+dt()*(_nu*(disc.diffusion(_U, i, j))-disc.convection_u(_U, _V, i, j)));
        setg(i,j,v(i,j)+dt()*(_nu*disc.diffusion(_V, i, j))-disc.convection_v(_U, _V, i, j));
    }
}

void Fields::calculate_rs(Grid &grid) {
    for(auto &cell: grid.fluid_cells()){
        int i = cell->i();
        int j = cell->j();
        double val = ((f(i,j)-f(i-1,j))/grid.dx() + (g(i,j)-g(i,j-1))/grid.dy())/dt();
        setrs(i, j, val);        
    }
}

void Fields::calculate_velocities(Grid &grid) {
    for(auto &cell: grid.fluid_cells()){
        int i = cell->i();
        int j = cell->j();
        setu(i, j, f(i,j) - _dt/grid.dx()*(p(i+1,j)-p(i,j)));
        setv(i, j, g(i,j) - _dt/grid.dy()*(p(i,j+1)-p(i,j)));       
    }
}

double Fields::calculate_dt(Grid &grid) { 
    _dt = (1/2/_nu)/(1/grid.dx()/grid.dx() + 1/grid.dy()/grid.dy());
    for(auto &cell: grid.fluid_cells()){
        int i = cell->i();
        int j = cell->j();
        double dt1 = grid.dx()/u(i,j);
        double dt2 = grid.dy()/v(i,j);
        if(dt1<_dt){
            _dt = dt1;
        }
        if(dt2<_dt){
            _dt = dt2;
        }  
    }
    return _dt; 
    }

double &Fields::p(int i, int j) { return _P(i, j); }
double &Fields::u(int i, int j) { return _U(i, j); }
double &Fields::v(int i, int j) { return _V(i, j); }
double &Fields::f(int i, int j) { return _F(i, j); }
double &Fields::g(int i, int j) { return _G(i, j); }
double &Fields::rs(int i, int j) { return _RS(i, j); }

Matrix<double> &Fields::p_matrix() { return _P; }

double Fields::dt() const { return _dt; }

void Fields::setp(int i, int j, double val) { _P(i, j) = val; }
void Fields::setu(int i, int j, double val) { _U(i, j) = val; }
void Fields::setv(int i, int j, double val) { _V(i, j) = val; }
void Fields::setf(int i, int j, double val) { _F(i, j) = val; }
void Fields::setg(int i, int j, double val) { _G(i, j) = val; }
void Fields::setrs(int i, int j, double val) { _RS(i, j) = val; }