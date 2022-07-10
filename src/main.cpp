#include <iostream>
#include <string>
#include <chrono>

#include "Communication.hpp"
#include "Case.hpp"

// make it true to print time for the simulation
#define PrintTime false

/*
void printProgressBar( double t, double t_end ){

    std::string bar;
    int percent = (t/t_end) * 100;

    for(int i = 0; i < 50; i++){
        if( i < (percent/2)){
            bar.replace(i,1,"=");
        }else if( i == (percent/2)){
            bar.replace(i,1,">");
        }else{
            bar.replace(i,1," ");
        }
    }
    std::cout<< "\r" "[" << bar << "] ";
    std::cout.width( 3 );
    std::cout<< percent << "%     \r" << std::flush;
}*/

int main(int argn, char **args) {
#if(PrintTime) 
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
#endif
    Communication::init_parallel(argn, args);
    if (argn > 1) {
        std::string file_name{args[1]};
        Case problem(file_name, argn, args);
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
    Communication::finalize();
    return 0;
}
