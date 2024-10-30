#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int NUM_CORES = 2;

typedef struct compute_pps_args{
    int num_cores;
    int index;
    int terms_number;
    float result;
}pps_args;

void* compute_partial_pi_series(void* args){
    pps_args* params = (pps_args*) args;
    params->result = 0;
    for (int i = 0; i < (params->terms_number)/(params->num_cores); i++){
        params->result += pow((double) -1, (double) i) / (params->num_cores * i + params->index + 1);
    }

    printf("The partial result inside the functions is %f\n", params->result);
    return NULL;
}

int main(int argc, char** argv){
    if (argc != 2){
        printf("Wrong number of user inputs\n");
    } else {
        // We first need to decide how many terms of the series we need
        int terms_number = atoi(argv[1]);

        // Variable initialization
        float total = 0;
        void* params_void;
        pthread_t** thread_handles = malloc(sizeof(pthread_t*) * NUM_CORES);
        pps_args* params = malloc(sizeof(pps_args) * NUM_CORES);

        for (int i = 0; i < NUM_CORES; i++){
            // Initializing the parameters for the function
            params[i] = (pps_args) {NUM_CORES, i, terms_number, 0};
            params_void = (void*) &params[i];

            // Initializing the thread handle
            thread_handles[i] = malloc(sizeof(pthread_t));

            // Creating the thread
            pthread_create(thread_handles[i], NULL, compute_partial_pi_series, params_void);
        }

        // Summing up sequentially all the results
        for (int i = 0; i < NUM_CORES; i++){
            total += params[i].result;
        }

        printf("The estimate of pi is %f\n", total);
    }

    return 0;
}