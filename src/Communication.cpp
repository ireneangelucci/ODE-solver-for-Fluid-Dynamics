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

void Communication::communicate(Fields &field){
    
    // Transfering data to left
    if(_domain.neighbours[0]!= MPI_PROC_NULL){
        auto p = field.p_matrix().get_col(1);
        MPI_Send(p.data(), _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[0], 51, MPI_COMM_WORLD);
    }
    // Recieving data from right
    if(_domain.neighbours[2]!= MPI_PROC_NULL){
        MPI_Status status;
        auto p = field.p_matrix().get_col(_domain.imax-1);
        auto st = MPI_Recv(p.data(), _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[2], 51, MPI_COMM_WORLD, &status);
        field.p_matrix().set_col(p, _domain.imax-1);
    }
    return;
}

double Communication::reduce_min(double min_dt){
    std::cout << min_dt <<  " rank " << _my_rank<<"\n";
    MPI_Reduce(&_my_rank, &min_dt, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    std::cout << min_dt << " rank " << _my_rank<<"\n";
    return min_dt;
}