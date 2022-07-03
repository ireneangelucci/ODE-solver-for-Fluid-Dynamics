#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include "Fields.hpp"
#include "Domain.hpp"

class Communication{
    public:

        Communication() = default;
        //~Communication();
        /**
         * @brief Constructor
         * 
         */
        Communication(int my_rank, Domain domain);

        /**
         * @brief Initializing
         * 
         */
        static void init_parallel(int argn, char** args);

        /**
         * @brief Finalizing
         * 
         */
        static void finalize();

        /**
         * @brief Communication
         * 
         */
        static void communicate(Matrix<double>& matrix);

        /**
         * @brief Find minimum value across all procresses
         * 
         */
        static double reduce_min(double x);

        /**
         * @brief Find sum across all processed
         * 
         */
        static double reduce_max(double x);
        static int getrank();

        static int _my_rank;
        static Domain _domain;

};

#endif