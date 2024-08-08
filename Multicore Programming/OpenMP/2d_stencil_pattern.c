// IMPORTS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "my_timer.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// CONSTANTS AND GLOBAL VARIABLES /////////////////////////////////////////////////////////////////////////////////////////
int** m1;
int** m1_copy;
int** m2;
int** m2_copy;
int num_rows, num_cols, num_iter, num_threads, counter;

double start, stop, end;

pthread_mutex_t mutex;
pthread_cond_t cond_var;
pthread_mutex_t* row_available;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * It allocates the matrix dinamically, as a pointer to a vector of pointers.
 * 
 * Input:
 * - int num_rows: a non-zero positive integer
 * - int num_cols: a non-zero positive integer
 * 
 * Output:
 * - int**
 */
int** allocate_int_matrix(int num_rows, int num_cols){
    int** m;       

    m = (int**) malloc(sizeof(int*)*num_rows);
    for(int i = 0; i < num_rows; i++){
        m[i] = (int*) malloc(sizeof(int)*num_cols);
    }

    return m;
}

/**
 * It returns a dinamically allocated matrix full of random ints between
 * 0 and 5. It requires stdlib.h and time.h.
 * 
 * Input:
 * - int** m: (optional) the matrix
 * - int num_rows: a non-zero positive integer
 * - int num_cols: a non-zero positive integer
 * 
 * Output:
 * - int**
 */
int** random_int_matrix(int** m, int num_rows, int num_cols){
    if (m == NULL){
        m = allocate_int_matrix(num_rows, num_cols);
    }
    unsigned int seed = time(0);    // taking current time as seed        

    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++){
            m[i][j] = (int) rand_r(&seed) % (5 + 1);  // it generates numbers between 0 and 5
        }
    }

    return m;
}

/**
 * It zeroes out a matrix.
 * 
 * Input:
 * - int** m: (optional) the matrix
 * - int num_rows: a non-zero positive integer
 * - int num_cols: a non-zero positive integer
 */
void zero_int_matrix(int** m, int num_rows, int num_cols){
    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++){
            m[i][j] = 0;
        }
    }
}

/**
 * It prints a dinamically allocated matrix stored in a pointer to pointers.
 * 
 * Input:
 * - int** m: the matrix to be printed
 * - int num_rows: the function will print rows in the range [0, num_rows). It must be less or equal to the real number of rows of m
 * - int num_cols: the function will print columns in the range [0, num_cols). It must be less or equal to the real number of columns of m
 * 
 * Output:
 * - NULL
 */
void print_int_matrix(int** m, int num_rows, int num_cols){
    for (int i = 0; i < num_rows; i++){
        printf("\n");
        for (int j = 0; j < num_cols; j++)
            printf("%d ", m[i][j]);
    }

    printf("\n");
    fflush(stdout);         // Sometimes, prints are stored in a buffer that needs to flushed out
}

/**
 * This function applies the operation of cardinal sum on a single element (i, j), defined as,
 * A'[i][j] = A[i+1][j] + A[i][j+1] + A[i-1][j] + A[i][j-1] (with A[h][k] = 0 if undefined)
 * by using A = m1 and A' = m2. Thus, it is not performed in place. m2 is unchanged apart for m2[i][j].
 * 
 * Input:
 * - int** m1: the original matrix
 * - int** m2: the matrix on which the cardinal sum is applied. Note how m1 and m2 may not be equal before the function is applied.
 * - int num_rows: the number of rows of both matrices
 * - int num_rows: the number of columns of both matrices
 * - int i: row of the element where the cardinal sum will be applied
 * - int j: column of the element where the cardinal sum will be applied
 * - char dir: unused argument added for compatibility
 */
void total_cardinal_sum(int** m1, int** m2, int num_rows, int num_cols, int i, int j, char dir){
    for (int k = 0; k < 2; k++){
        if (i - 1 + 2*k >= 0 && i - 1 + 2*k < num_rows){
            m2[i][j] += m1[i - 1 + 2*k][j];
        }
    }
            
    for (int h = 0; h < 2; h++){
        if (j - 1 + 2*h >= 0 && j - 1 + 2*h < num_cols){
            m2[i][j] += m1[i][j - 1 + 2*h];
        }
    }
}

/**
 * A function that applies a number of iterations from a matrix m1 and copies the results on a matrix m2.
 * The various dimension are taken from global variables for a faster evaluation.
 * Iterations are syncronized among all threads
 * 
 * Input:
 *  - void (args): it contains the row range that must be computed by the given thread
 */
void parallel_row_iterations(){
    int **m1_own, **m2_own;
    int tmp;

    for (int k = 0; k < num_iter; k++){
        // At every iterations, the computing and computed matrices need to be switched
        if (k % 2 == 0){
            m1_own = m1_copy;
            m2_own = m2_copy;
        } else {
            m1_own = m2_copy;
            m2_own = m1_copy;
        }

        // Actual computation
        for (int i = 0; i < num_rows; i++){      
            for (int j = 0; j < num_cols; j++){
                tmp = m1_own[i][j];
                m1_own[i][j] = 0;

                for (int l = 0; l < 2; l++){
                    if (i - 1 + 2*l >= 0 && i - 1 + 2*l < num_rows){
                            #pragma omp atomic
                            m2_own[i - 1 + 2*l][j] += tmp;
                        }
                    }
                        
                for (int h = 0; h < 2; h++){
                    if (j - 1 + 2*h >= 0 && j - 1 + 2*h < num_cols){
                        m2_own[i][j - 1 + 2*h] += tmp;
                    }
                }
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MAIN ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv){
    if (argc != 5){
        printf("Wrong number of inputs\n");
    } else {
        // Storing the matrix dimensions and the number of iterations and threads
        num_rows = atoi(argv[1]);
        num_cols = atoi(argv[2]);
        num_iter = atoi(argv[3]);
        num_threads = atoi(argv[4]);
        
        // Instatiating the matrix
        m1 = random_int_matrix(m1, num_rows, num_cols);
        m1_copy = copy_int_matrix(m1, num_rows, num_cols);
        m2 = allocate_int_matrix(num_rows, num_cols);
        m2_copy = allocate_int_matrix(num_rows, num_cols);

        /*
        printf("The original matrix is:");
        print_int_matrix(m1, num_rows, num_cols);
        */

        // SEQUENTIAL VERSION
        GET_TIME(start);

        sequential_iterations();

        GET_TIME(stop);
        end = stop - start;
        printf("Serial time: %e seconds\n", end);

        // PARALLEL VERSION
        GET_TIME(start);

        parallel_iterations();

        GET_TIME(stop);
        end = stop - start;
        printf("Parallel time: %e seconds\n", end);

        // Checking that computations are correct
        int val;
        if (num_iter % 2 == 0){
            val = check_eq_int_matrix(m1, m1_copy, num_rows, num_cols);
        } else {
            val = check_eq_int_matrix(m2, m2_copy, num_rows, num_cols);
        }
        printf("The calculations are correct: %d\n", val);

        for (int i = 0; i < num_rows; i++){
            free(m1[i]);
            free(m1_copy[i]);
            free(m2[i]);
            free(m2_copy[i]);
        }
        free(m1);
        free(m1_copy);
        free(m2);
        free(m2_copy);
    }
    return 0;
}