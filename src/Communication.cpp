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
    
    // Transfering data to left
    if(_domain.neighbours[0]!= MPI_PROC_NULL){
        auto send = matrix.get_col(1);
        MPI_Send(send.data(), _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[0], 51, MPI_COMM_WORLD);
    }
    // Recieving data from right
    if(_domain.neighbours[2]!= MPI_PROC_NULL){
        MPI_Status status;
        auto recv = matrix.get_col(_domain.imax-1);
        auto st = MPI_Recv(recv.data(), _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[2], 51, MPI_COMM_WORLD, &status);
        matrix.set_col(recv, _domain.imax-1);
    }

    // Transfering data to right
    if(_domain.neighbours[2]!= MPI_PROC_NULL){
        auto send = matrix.get_col(_domain.imax-2);
        MPI_Send(send.data(), _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[2], 51, MPI_COMM_WORLD);
    }
    // Recieving data from left
    if(_domain.neighbours[0]!= MPI_PROC_NULL){
        MPI_Status status;
        auto recv = matrix.get_col(0);
        auto st = MPI_Recv(recv.data(), _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[0], 51, MPI_COMM_WORLD, &status);
        matrix.set_col(recv, 0);
    }

    // Transfering data to bottom
    if(_domain.neighbours[1]!= MPI_PROC_NULL){
        auto send = matrix.get_row(0);
        MPI_Send(send.data(), _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[1], 51, MPI_COMM_WORLD);
    }
    // Recieving data from top
    if(_domain.neighbours[3]!= MPI_PROC_NULL){
        MPI_Status status;
        auto recv = matrix.get_row(_domain.jmax-2);
        auto st = MPI_Recv(recv.data(), _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[3], 51, MPI_COMM_WORLD, &status);
        matrix.set_row(recv, _domain.jmax-2);
    }

    // Transfering data to top
    if(_domain.neighbours[3]!= MPI_PROC_NULL){
        auto send = matrix.get_row(_domain.jmax-2);
        MPI_Send(send.data(), _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[3], 51, MPI_COMM_WORLD);
    }
    // Recieving data from bottom
    if(_domain.neighbours[1]!= MPI_PROC_NULL){
        MPI_Status status;
        auto recv = matrix.get_row(0);
        auto st = MPI_Recv(recv.data(), _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[1], 51, MPI_COMM_WORLD, &status);
        matrix.set_row(recv, 0);
    }

    return;
}

double Communication::reduce_min(double dt){
    double min_dt = dt;
    //std::cout<<_my_rank<<" Rank - before dt: "<<min_dt<<"\n";
    MPI_Allreduce(&dt, &min_dt, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    //std::cout<<_my_rank<<" Rank - after dt: "<<min_dt<<"\n";
    return min_dt;
}