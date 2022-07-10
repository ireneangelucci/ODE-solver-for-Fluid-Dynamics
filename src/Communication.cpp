#include <iostream>

#include "Communication.hpp"
#include "mpi.h"

int Communication::_my_rank = -1;
Domain Communication::_domain;
int Communication::_nprocs = -1;
std::array<int, 4> Communication::neighbours{MPI_PROC_NULL, MPI_PROC_NULL, MPI_PROC_NULL, MPI_PROC_NULL}; //L, B, R, T;

Communication::Communication(){
    MPI_Comm_rank(MPI_COMM_WORLD, &_my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &_nprocs);
}

Communication::~Communication(){
    MPI_Finalize();
}

void Communication::init_parallel(int argn, char** args) {
    MPI_Init(&argn, &args);
    
}

void Communication::communicate(Matrix<double>& matrix){
    MPI_Status status;
    // Transfering data to left
    auto send_left = matrix.get_col(1);
    auto recv_left = matrix.get_col(_domain.size_x+1);
    if(_my_rank == 0){
        //std::cout << " right " <<  Communication::neighbours[neighbour::RIGHT] << " left " << Communication::neighbours[neighbour::LEFT] << " top " << Communication::neighbours[neighbour::TOP] << " bottom " << Communication::neighbours[neighbour:BOTTOM] << "\n";
    }
    MPI_Sendrecv(send_left.data(), send_left.size(), MPI_DOUBLE, neighbours[neighbour::LEFT], 51,
                 recv_left.data(), recv_left.size(), MPI_DOUBLE, neighbours[neighbour::RIGHT], 51, MPI_COMM_WORLD, &status);
    matrix.set_col(recv_left, _domain.size_x+1);
        
    // Transfering data to right
    auto send_right = matrix.get_col(_domain.size_x);
    auto recv_right = matrix.get_col(0);
    MPI_Sendrecv(send_right.data(), send_right.size(), MPI_DOUBLE, neighbours[neighbour::RIGHT], 151,
                    recv_right.data(), recv_right.size(), MPI_DOUBLE, neighbours[neighbour::LEFT], 151, MPI_COMM_WORLD, &status);
    matrix.set_col(recv_right, 0);
    
    // Transfering data to bottom
    auto send_bottom = matrix.get_row(1);
    auto recv_bottom = matrix.get_row(_domain.size_y+1);
    MPI_Sendrecv(send_bottom.data(), send_bottom.size(), MPI_DOUBLE, neighbours[neighbour::BOTTOM], 251,
                 recv_bottom.data(), recv_bottom.size(), MPI_DOUBLE,neighbours[neighbour::TOP], 251, MPI_COMM_WORLD, &status);
    matrix.set_row(recv_bottom, _domain.size_y+1);

    // Transfering data to top
    auto send_top = matrix.get_row(_domain.size_y);
    auto recv_top = matrix.get_row(0);
    MPI_Sendrecv(send_top.data(), send_top.size(), MPI_DOUBLE, neighbours[neighbour::TOP], 351,
             recv_top.data(), recv_top.size(), MPI_DOUBLE, neighbours[neighbour::BOTTOM], 351, MPI_COMM_WORLD, &status);
    matrix.set_row(recv_top, 0);
    return;
}

double Communication::reduce_min(double x){
    double min_x = x;
    MPI_Allreduce(&x, &min_x, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    return min_x;
}

double Communication::reduce_max(double x){
    double max_x = x;
    MPI_Allreduce(&x, &max_x, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    return max_x;
}

int Communication::getrank() {return _my_rank;}