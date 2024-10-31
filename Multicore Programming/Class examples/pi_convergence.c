#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int NUM_CORES = 2;

// compute_partial_pi_series() needs its args in a struct. result is the return value and it is modified in-place

typedef struct compute_pps_args{
    int num_cores;
    int index;
    int num_terms;
    float result;
}pps_args;

/**
 * A function that computes a portion of the pi series iteratively.
 * 
 * Input:
 * - void* args: since this function will be passed to pthreads, it requires a void*-casted of a pointer to a compute_pps_args object.
 * 
 * Output:
 * - void* to NULL, but the result field of compute_pps_args will be modified with the result.
 */
void* compute_partial_pi_series(void* args){
    // We re-cast args to its proper type and create internal variables for readibility
    pps_args* params = (pps_args*) args;
    float partial_result = params->result;
    int num_cores = params->num_cores;
    int num_terms = params->num_terms;
    int offset = params->index;
    int n;      // this is the new index, multiplied by the lowered frequency (thanks to the higher number of cores) and correctly offset

    // Pi iterative partial calculation
    for (int i = 0; i < num_terms/num_cores; i++){
        n = num_cores * i + offset;
        partial_result += pow((double) -1, (double) n) / (2 * n + 1);
    }
    params->result = 4 * partial_result;

    return NULL;
}

int main(int argc, char** argv){
    if (argc != 2){
        printf("Wrong number of user inputs\n");
    } else {
        // We first need to decide how many terms of the series we need
        int terms_number = atoi(argv[1]);

        // Variables initialization
        float total = 0;
        pthread_t** thread_handles = malloc(sizeof(pthread_t*) * NUM_CORES);
        pps_args* params = malloc(sizeof(pps_args) * NUM_CORES);

        for (int i = 0; i < NUM_CORES; i++){
            // Initializing the thread handle
            thread_handles[i] = malloc(sizeof(pthread_t));

            // Initializing the parameters for the function
            params[i] = (pps_args) {NUM_CORES, i, terms_number, 0};

            // Creating the thread
            pthread_create(thread_handles[i], NULL, compute_partial_pi_series, (void*) &params[i]);
        }

        // Summing up sequentially all the results
        for (int i = 0; i < NUM_CORES; i++){
            // We need to wait for the thread to finish
            pthread_join(*thread_handles[i], NULL);
            total += params[i].result;
            free(thread_handles[i]);
        }

        // Freeing up allocated now-useless pointers
        free(thread_handles);
        free(params);

        printf("The estimate of pi is %f\n", total);
    }

    return 0;
}