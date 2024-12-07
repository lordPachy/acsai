#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include "my_timer.h"

int num_threads, num_samples;
double start, end, stop;

double sequential_pi(num_samples){
    long inside = 0;

    for (int i = 0; i < num_samples; i++){
        double x = 2 * (double)rand() / RAND_MAX - 1;
        double y = 2 * (double)rand() / RAND_MAX - 1;

        // Checking the trial is correct
        double l2 = x * x + y * y;
        if (l2 <= 1.0){
            inside++;
        }
    }
    double pi = 4.0 * (double)inside / (double)num_samples; 
    printf("Pi is %f\n", pi);

    return pi;
}

double parallel_pi(num_samples){
    long inside = 0;

    #pragma omp parallel{
    // This is needed for rand_r
    unsigned int seed = omp_get_thread_num();
    
        #pragma omp for reduction(+: inside) num_threads(num_threads)
        for (int i = 0; i < num_samples; i++){
            // rand is thread safe but it is globally locking
            double x = 2 * (double)rand_r(&seed) / RAND_MAX - 1;
            double y = 2 * (double)rand_r(&seed) / RAND_MAX - 1;

            // Checking the trial is correct
            double l2 = x * x + y * y;
            if (l2 <= 1.0){
                inside++;
            }
        }
    }

    double pi = 4.0 * (double)inside / (double)num_samples; 
    printf("Pi is %f\n", pi);

    return pi;
}

int main(int argc, char **argv){
    // Taking program inputs
    if (argc != 3){
        printf("Wrong number of inputs\n");
        return 0;
    }
    num_threads = atoi(argv[1]);
    num_samples = atol(argv[2]);
    srand(time(NULL));

    // Serial test
    GET_TIME(start);

    sequential_pi(num_samples);

    GET_TIME(stop)
    end = stop - start;
    printf("The sequential time is %e\n", end);

    // Parallel test
    GET_TIME(start);

    parallel_pi(num_samples);

    GET_TIME(stop)
    end = stop - start;
    printf("The parallel time is %e\n", end);

    return 0;
}