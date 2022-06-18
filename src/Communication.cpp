#include <iostream>

#include "Communication.hpp"
#include "mpi.h"

int Communication::_my_rank = 0;
Domain Communication::_domain;

Communication::Communication(int my_rank, Domain domain){
    _my_rank = my_rank;
    _domain = domain;
}

void Communication::init_parallel(int argn, char** args) {
    MPI_Init(&argn, &args);
}

void Communication::finalize() {
    MPI_Finalize();
}

void Communication::communicate(Matrix<double>& matrix){
    MPI_Status status;
    // Transfering data to left
    auto send_left = matrix.get_col(1);
    auto recv_left = matrix.get_col(_domain.size_x+1);
    MPI_Sendrecv(send_left.data(), send_left.size(), MPI_DOUBLE, _domain.neighbours[0], 51,
                 recv_left.data(), recv_left.size(), MPI_DOUBLE, _domain.neighbours[2], 51, MPI_COMM_WORLD, &status);
    matrix.set_col(recv_left, _domain.size_x+1);
        
    // Transfering data to right
    auto send_right = matrix.get_col(_domain.size_x);
    auto recv_right = matrix.get_col(0);
    MPI_Sendrecv(send_right.data(), send_right.size(), MPI_DOUBLE, _domain.neighbours[2], 151,
                    recv_right.data(), recv_right.size(), MPI_DOUBLE, _domain.neighbours[0], 151, MPI_COMM_WORLD, &status);
    matrix.set_col(recv_right, 0);
    
    // Transfering data to bottom
    auto send_bottom = matrix.get_row(1);
    auto recv_bottom = matrix.get_row(_domain.size_y+1);
    MPI_Sendrecv(send_bottom.data(), send_bottom.size(), MPI_DOUBLE, _domain.neighbours[1], 251,
                 recv_bottom.data(), recv_bottom.size(), MPI_DOUBLE, _domain.neighbours[3], 251, MPI_COMM_WORLD, &status);
    matrix.set_row(recv_bottom, _domain.size_y+1);

    // Transfering data to top
    auto send_top = matrix.get_row(_domain.size_y);
    auto recv_top = matrix.get_row(0);
    MPI_Sendrecv(send_top.data(), send_top.size(), MPI_DOUBLE, _domain.neighbours[3], 351,
             recv_top.data(), recv_top.size(), MPI_DOUBLE, _domain.neighbours[1], 351, MPI_COMM_WORLD, &status);
    matrix.set_row(recv_top, 0);
    return;
}

double Communication::reduce_min(double dt){
    double min_dt = dt;
    //std::cout<<_my_rank<<" Rank - before dt: "<<min_dt<<"\n";
    MPI_Allreduce(&dt, &min_dt, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    //std::cout<<_my_rank<<" Rank - after dt: "<<min_dt<<"\n";
    return min_dt;
}