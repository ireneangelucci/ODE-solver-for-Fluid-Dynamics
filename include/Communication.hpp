#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include "Fields.hpp"

class Communication{
    public:

        Communication() = default;
        /**
         * @brief Constructor
         * 
         */
        Communication(int my_rank);

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
        //static void communicate(Fields &field);

        /**
         * @brief Find minimum value across all procresses
         * 
         */
        static double reduce_min(double min_dt);

        /**
         * @brief Find sum across all processed
         * 
         */
        static double reduce_sum(double sum);
    
        static int _my_rank;

};

#endif