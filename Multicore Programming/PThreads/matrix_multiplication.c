// IMPORTS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "my_timer.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// CONSTANTS AND GLOBAL VARIABLES /////////////////////////////////////////////////////////////////////////////////////////
int** m1;
int** m2;
int** m3;
int num_rows_1, num_cols_1, num_rows_2, num_cols_2, num_threads;

double start, stop, end;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// STRUCTS AND CUSTOM TYPES ///////////////////////////////////////////////////////////////////////////////////////////////
typedef struct imml_params{
    int start_i;
    int end_i;
}imml_params;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////////
int** allocate_matrix(int num_rows, int num_cols){
    // It returns a dinamically allocated memory space of dimension
    // num_rows x num_cols. It returns a pointer to a vector of pointers,
    // pointing to each future row.

    int** m;       

    m = (int**) malloc(sizeof(int*)*num_rows);
    for(int i = 0; i < num_rows; i++){
        m[i] = (int*) malloc(sizeof(int)*num_cols);
    }

    return m;
}

int** random_matrix(int num_rows, int num_cols){
    // It returns a dinamically allocated random matrix of dimension
    // num_rows x num_cols. It returns a pointer to a vector of pointers,
    // pointing to each row.

    int** m = allocate_matrix(num_rows, num_cols);
    unsigned int seed = time(0);    // taking current time as seed        

    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++){
            m[i][j] = (int) rand_r(&seed) % (5 + 1);  // it generates numbers between 0 and 5
        }
    }

    return m;
}

void print_matrix(int** m, int num_rows, int num_cols){
    // It supports matrices in double pointer form.

    for (int i = 0; i < num_rows; i++){
        printf("\n");
        for (int j = 0; j < num_cols; j++)
            printf("%d ", m[i][j]);
    }

    printf("\n");
    fflush(stdout);         // Sometimes, prints are stored in a buffer that needs to flushed out
}

int** matrix_matrix_mul(int** m1, int num_rows_1, int num_cols_1, int** m2, int num_rows_2, int num_cols_2){
    // It creates another matrix which is m1@m2, whose pointer-pointer is returned.

    if (num_cols_1 != num_rows_2){
        printf("Matrix dimensions are not compatible\n");
        return 0;
    } else {
        int** m = allocate_matrix(num_rows_1, num_cols_2);

        for (int i = 0; i < num_rows_1; i++){
            for (int j = 0; j < num_cols_2; j++){
                m[i][j] = 0;
                for (int k = 0; k < num_rows_2; k++){
                    m[i][j] += m1[i][k] * m2[k][j];
                }
            }
        }

        return m;
    }
}

/**
 * It performs matrix-matrix multiplication only on a specific range of rows and columns.
 * It employs m1, m2, m3, and relative dimensions as global constants to avoid spending time copying.
 * 
 * Inputs:
 *  - int start_i, end_i: the starting (included) and ending (excluded) row
 *  - int start_j, end_j: the starting (included) and ending (excluded) column
 */
void idx_matrix_matrix_mul(int start_i, int end_i, int start_j, int end_j){
    // Some ordinary checks on dimensions
    if (num_cols_1 != num_rows_2){
        printf("Matrix dimensions are not compatible\n");
        return;
    }

    if (start_i < 0 || start_j < 0 || end_i > num_rows_1 || end_j > num_cols_2){
        printf("The modified range goes out of the matrix dimensions\n");
        return;
    }

    for (int i = start_i; i < end_i; i++){
        for (int j = start_j; j < end_j; j++){
            m3[i][j] = 0;
            for (int k = 0; k < num_rows_2; k++){
                m3[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

/**
 * A wrapper for idx_matrix_matrix_mul that can be useful for PThreads.
 * 
 * Input: 
 *  - void* args: all the arguments
 * 
 * Output:
 *  - void*: it will return NULL
 */
void* pthread_idx_matrix_matrix_mul(void* args){
    imml_params* params = (imml_params*) args;

    idx_matrix_matrix_mul(params->start_i, params->end_i, 0, num_cols_2);

    return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// MAIN ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv){
    if (argc != 6){
        printf("Wrong number of inputs\n");
    } else {
        // First, we store the matrix dimensions
        num_rows_1 = atoi(argv[1]);
        num_cols_1 = atoi(argv[2]);
        num_rows_2 = atoi(argv[3]);
        num_cols_2 = atoi(argv[4]);
        num_threads = atoi(argv[5]);

        // We can now create the 2 matrices and print them
        m1 = random_matrix(num_rows_1, num_cols_1);
        m2 = random_matrix(num_rows_2, num_cols_2);

        /*
        printf("The original matrices are:\n");
        print_matrix(m1, num_rows_1, num_cols_1);
        print_matrix(m2, num_rows_2, num_cols_2);
        */

        // Sequential execution
        GET_TIME(start);

        m3 = matrix_matrix_mul(m1, num_rows_1, num_cols_1, m2, num_rows_2, num_cols_2);

        GET_TIME(stop);
        end = stop - start;

        //print_matrix(m3, num_rows_1, num_cols_2);
        printf("Serial time: %f seconds\n", end);

        // Parallel execution
        int start_i, end_i;

        // Variable initialization
        pthread_t** thread_handles = malloc(sizeof(pthread_t*) * num_threads);
        imml_params* params = malloc(sizeof(imml_params) * num_threads);

        GET_TIME(start);

        for (int i = 0; i < num_threads; i++){
            // Initializing the thread handle
            thread_handles[i] = malloc(sizeof(pthread_t));

            // Initializing the parameters for the function
            start_i = num_rows_1/num_threads * i;
            if (i == num_threads - 1){
                end_i = num_rows_1;
            } else {
                end_i = num_rows_1/num_threads * (i + 1);
            }
            params[i] = (imml_params) {start_i, end_i};

            // Creating the thread
            pthread_create(thread_handles[i], NULL, pthread_idx_matrix_matrix_mul, (void*) &params[i]);
        }

        // Joining threads
        for (int i = 0; i < num_threads; i++){
            // We need to wait for the thread to finish
            pthread_join(*thread_handles[i], NULL);
        }

        GET_TIME(stop);
        end = stop - start;

        //print_matrix(m3, num_rows_1, num_cols_2);
        printf("Parallel time: %f seconds\n", end);

        // Freeing up allocated now-useless pointers
        for (int i = 0; i < num_threads; i++){
            free(thread_handles[i]);
        }
        free(thread_handles);
        free(params);
        
        return 0;
    }
}