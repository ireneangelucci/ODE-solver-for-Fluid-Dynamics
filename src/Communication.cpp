#include "Communication.hpp"
#include "mpi.h"

Communication::Communication(int my_rank){
    //constructor
}

void Communication::init_parallel(int argn, char** args) {
    MPI_Init(&argn, &args);
}

void Communication::finalize() {
    MPI_Finalize();
}

void Communication::communicate(Fields &field){

}