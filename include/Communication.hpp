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
        static void communicate(Fields &field);

        /**
         * @brief Find minimum value across all procresses
         * 
         */
        static double reduce_min(double dt);

        /**
         * @brief Find sum across all processed
         * 
         */
        static double reduce_sum(double sum);

};