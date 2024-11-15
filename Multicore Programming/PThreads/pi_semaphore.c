// LIBRARIES

#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
// A macro for measuring time accurately
#define GET_TIME(now) { \
    struct timeval t; \
    gettimeofday(&t, NULL); \
    now = t.tv_sec + t.tv_usec/1000000.0; \
}
#include <stdio.h>
#include <stdlib.h>

// GLOBAL VARIABLES

int tot_samples_in_circle, trials_per_thread;
sem_t sem;

// FUNCTIONS

/**
 * Montecarlo sampling for a circle of radius 1 in a square of side 2.
 * It is an efficient multi-threaded variant: it first increases a local counter and then applies the result globally.
 * This variant employs a semaphore as a mutex.
 * 
 * Input:
 *  - int num_samples: the number of trials
 *  - int* counter: the counter of successful trials
 * 
 * Output:
 *  void
 */
void* montecarlo_circle(void* args){
    int local_counter = 0;

    for (int i = 0; i < trials_per_thread; i++){
            double x = 2 * (double)rand() / RAND_MAX - 1;
            double y = 2 * (double)rand() / RAND_MAX - 1;

            double l2 = x * x + y * y;
            if (l2 <= 1.0){
                local_counter++;
            }
        }

    sem_wait(&sem);
    tot_samples_in_circle += local_counter;
    sem_post(&sem);

    return NULL;
}

// MAIN

int main(int argc, char** argv){
    // Measuring time of execution
    double start, finish, thread_creation_start, thread_creation_finish, thread_joining_finish;
    GET_TIME(start);

    if (argc != 3){
        printf("Wrong number of input characters\n");
    }
    else{
        // First, we can take the program arguments and calculate the number of trials per thread
        int num_threads = atoi(argv[1]);
        int num_trials = atoi(argv[2]);

        trials_per_thread = num_trials / num_threads;

        // Initializing global variables; the lock and the total variable
        sem_init(&sem, 0, 1);       // the semaphore starts with a write available since the last arg is 1
        tot_samples_in_circle = 0;

        // Creating the array of variables that will be used by each thread: thread handles
        pthread_t** thread_handles = malloc(sizeof(pthread_t*) * num_threads);

        // Creating the threads
        GET_TIME(thread_creation_start);

        for (int i = 0; i < num_threads; i++){
            // Initializing the specific variable for this thread
            thread_handles[i] = malloc(sizeof(pthread_t));

            // Creating the specific thread
            pthread_create(thread_handles[i], NULL, montecarlo_circle, NULL);
        }

        GET_TIME(thread_creation_finish);

        // Stopping all threads and cleaning
        for (int i = 0; i < num_threads; i++){
            pthread_join(*thread_handles[i], NULL);
            free(thread_handles[i]);
        }

        GET_TIME(thread_joining_finish);

        free(thread_handles);
        sem_destroy(&sem);

        // Calculating the actual pi value
        double pi = 4.0 * (double)tot_samples_in_circle / (double)num_trials;

        // Measuring time of execution
        GET_TIME(finish);

        // Printing the final result and time of execution
        printf("Pi is %f\n", pi);
        printf("The time of execution was %e seconds\n", finish - start);      // This is the way of calculating elapsed time with processor clock
        printf("The time for thread creation was %e seconds\n", thread_creation_finish - thread_creation_start);
        printf("The time for thread joining was %e seconds\n", thread_joining_finish - thread_creation_finish);

    }

    return 0;
}