#include "Discretization.hpp"

#include <cmath>

double Discretization::_dx = 0.0;
double Discretization::_dy = 0.0;
double Discretization::_gamma = 0.0;

Discretization::Discretization(double dx, double dy, double gamma) {
    _dx = dx;
    _dy = dy;
    _gamma = gamma;
}

double Discretization::convection_u(const Matrix<double> &U, const Matrix<double> &V, int i, int j) {
    double u2x = ((U(i,j)+U(i+1,j))/2*(U(i,j)+U(i+1,j))/2 - (U(i,j)+U(i-1,j))/2*(U(i,j)+U(i-1,j))/2 )/_dx 
                            + _gamma * (std::abs(U(i,j)+U(i+1,j))/2*(U(i,j)-U(i+1,j))/2 - std::abs(U(i-1,j)+U(i,j))/2*(U(i-1,j)-U(i,j))/2)/_dx;
    double uvy = ((U(i,j)+U(i,j+1))/2 * (V(i,j)+V(i+1,j))/2 - (V(i,j-1)+V(i+1,j-1))/2 * (U(i,j-1)+U(i,j))/2)/_dy
                        +_gamma*(std::abs(V(i,j)+V(i+1,j))/2 * (U(i,j)-U(i,j+1))/2 - std::abs(V(i,j-1)+V(i+1,j-1))/2 * (U(i,j-1)-U(i,j))/2)/_dy;
    return u2x+uvy;
}

double Discretization::convection_v(const Matrix<double> &U, const Matrix<double> &V, int i, int j) {
    double v2y = ((V(i,j)+V(i,j+1))/2*(V(i,j)+V(i,j+1))/2 - (V(i,j)+V(i,j-1))/2*(V(i,j)-V(i,j-1))/2 )/_dy 
                            + _gamma * (std::abs(V(i,j)+V(i,j+1))/2*(V(i,j)-V(i,j+1))/2 - std::abs(V(i,j-1)+V(i,j))/2*(V(i,j-1)-V(i,j))/2)/_dy;
    double uvx = ((U(i,j)+U(i,j+1))/2 * (V(i,j)+V(i+1,j))/2 - (U(i-1,j)+U(i-1,j+1))/2 * (V(i-1,j)+V(i,j))/2)/_dx
                        + _gamma*(std::abs(U(i,j)+U(i,j+1))/2 * (V(i,j)-V(i+1,j))/2 - std::abs(U(i-1,j)+U(i-1,j+1))/2 * (V(i-1,j)-V(i,j))/2)/_dx;
    return uvx+v2y;
}

double Discretization::diffusion(const Matrix<double> &A, int i, int j) {
    return laplacian(A, i ,j);
}

double Discretization::laplacian(const Matrix<double> &P, int i, int j) {
    double result = (P(i + 1, j) - 2.0 * P(i, j) + P(i - 1, j)) / (_dx * _dx) +
                    (P(i, j + 1) - 2.0 * P(i, j) + P(i, j - 1)) / (_dy * _dy);
    return result;
}

double Discretization::sor_helper(const Matrix<double> &P, int i, int j) {
    double result = (P(i + 1, j) + P(i - 1, j)) / (_dx * _dx) + (P(i, j + 1) + P(i, j - 1)) / (_dy * _dy);
    return result;
}

double Discretization::interpolate(const Matrix<double> &A, int i, int j, int i_offset, int j_offset) {}