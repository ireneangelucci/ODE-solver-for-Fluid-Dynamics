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
    std::cout<<"Entering communicate: "<< _my_rank << "\n";
    std::vector<double> p(_domain.size_y+2);
    // Transfering data to left
    if(_domain.neighbours[0]!= MPI_PROC_NULL){
        if(_my_rank==1){
            std::cout<<"Sending data: "<< _my_rank << "\n";
        }
        p = field.p_matrix().get_col(1);
        MPI_Send(&p, _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[0], 51, MPI_COMM_WORLD);
        if(_my_rank==1){
            std::cout<<"Existing send data: "<< _my_rank << "\n";
        }
    }
    // Recieving data from right
    if(_domain.neighbours[2]!= MPI_PROC_NULL){
        MPI_Status status;
        if(_my_rank==0){
            //std::cout<<"R'ing data: "<< _my_rank << "\n";
        }
        int tag = _my_rank + 10*static_cast<int>(field.dt());
        MPI_Recv(&p, _domain.size_y+2, MPI_DOUBLE, _domain.neighbours[2], tag, MPI_COMM_WORLD, &status);
        (field.p_matrix()).set_col(p, _domain.imax-1);
        if(_my_rank==0){
            //std::cout<<"exit R'ing data: "<< _my_rank << "\n";
        }
    }
    //std::cout<<"exit communicate: "<< _my_rank << "\n";
}

double Communication::reduce_min(double min_dt){
    std::cout << min_dt <<  " rank " << _my_rank<<"\n";
    MPI_Reduce(&_my_rank, &min_dt, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    std::cout << min_dt << " rank " << _my_rank<<"\n";
    return min_dt;
}