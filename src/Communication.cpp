#include <iostream>

#include "Communication.hpp"
#include "mpi.h"

int Communication::_my_rank = 0;

Communication::Communication(int my_rank){
    _my_rank = my_rank;
}

void Communication::init_parallel(int argn, char** args) {
    MPI_Init(&argn, &args);
}

void Communication::finalize() {
    MPI_Finalize();
}

/*void Communication::communicate(Fields &field){

}*/

double Communication::reduce_min(double min_dt){
    std::cout << min_dt <<  " rank " << _my_rank<<"\n";
    MPI_Reduce(&_my_rank, &min_dt, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    std::cout << min_dt << " rank " << _my_rank<<"\n";
    return min_dt;
}