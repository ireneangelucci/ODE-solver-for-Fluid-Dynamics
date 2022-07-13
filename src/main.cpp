#include <iostream>
#include <string>
#include <chrono>

#include "Communication.hpp"
#include "Case.hpp"

// make it true to print time for the simulation
#define PrintTime false

int main(int argn, char **args) {
#if(PrintTime) 
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
#endif
    if (argn > 1) {
        Communication::init_parallel(argn, args);
        Communication  communication;
        std::string file_name{args[1]};
        Case problem(file_name, argn, args);
        MPI_Barrier(MPI_COMM_WORLD);
        problem.simulate();
    } else {
        std::cout << "Error: No input file is provided to fluidchen." << std::endl;
        std::cout << "Example usage: /path/to/fluidchen /path/to/input_data.dat" << std::endl;
    }
#if(PrintTime) 
    if(Communication::getrank()==0){
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;
        std::cout << "\n" <<"Elapsed time: " << elapsed_time.count() << "s\n";
    }
#endif
    return 0;
}
