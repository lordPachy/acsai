// IMPORTS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "my_timer.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// CONSTANTS AND GLOBAL VARIABLES /////////////////////////////////////////////////////////////////////////////////////////
int** m1;
int** m2;
int num_rows, num_cols, num_iter, num_threads, counter;

double start, stop, end;

pthread_mutex_t mutex;
pthread_cond_t cond_var;
sem_t sem;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// STRUCTS AND CUSTOM TYPES ///////////////////////////////////////////////////////////////////////////////////////////////
typedef void (*cardinal_sum)(int** m1, int** m2, int num_rows, int num_cols, int i, int j, char dir);

typedef struct iter_params{
    cardinal_sum f;
    char dir;
    int id;
}iter_params;
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
 */
void total_cardinal_sum(int** m1, int** m2, int num_rows, int num_cols, int i, int j){
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
 * This function applies the operation of cardinal sum on a single element (i, j), defined as,
 * A'[i][j] += A[i+1][j] + A[i-1][j] (with A[h][k] = 0 if undefined)
 * by using A = m1 and A' = m2. Thus, it is not performed in place. m2 is unchanged apart for m2[i][j].
 * 
 * Input:
 * - int** m1: the original matrix
 * - int** m2: the matrix on which the cardinal sum is applied. Note how m1 and m2 may not be equal before the function is applied.
 * - int num_rows: the number of rows of both matrices
 * - int num_rows: the number of columns of both matrices
 * - int i: row of the element where the cardinal sum will be applied
 * - int j: column of the element where the cardinal sum will be applied
 * - char dir: not used
 */
void northsouth_cardinal_sum(int** m1, int** m2, int num_rows, int num_cols, int i, int j, char dir){
    for (int k = 0; k < 2; k++){
        if (i - 1 + 2*k >= 0 && i - 1 + 2*k < num_rows){
            m2[i][j] += m1[i - 1 + 2*k][j];
        }
    }
}

/**
 * This function applies the operation of cardinal sum on a single element (i, j), defined as,
 * A'[i][j] += A[i][j+1] + A[i][j-1] (with A[h][k] = 0 if undefined)
 * by using A = m1 and A' = m2. Thus, it is not performed in place. m2 is unchanged apart for m2[i][j].
 * 
 * Input:
 * - int** m1: the original matrix
 * - int** m2: the matrix on which the cardinal sum is applied. Note how m1 and m2 may not be equal before the function is applied.
 * - int num_rows: the number of rows of both matrices
 * - int num_rows: the number of columns of both matrices
 * - int i: row of the element where the cardinal sum will be applied
 * - int j: column of the element where the cardinal sum will be applied
 * - char dir: not used
 */
void eastwest_cardinal_sum(int** m1, int** m2, int num_rows, int num_cols, int i, int j, char dir){       
    for (int h = 0; h < 2; h++){
        if (j - 1 + 2*h >= 0 && j - 1 + 2*h < num_cols){
            m2[i][j] += m1[i][j - 1 + 2*h];
        }
    }
}

/**
 * This function applies the operation of cardinal sum on a single element (i, j) on a single direction:
 * A'[i][j] += A[i][j+1] or A[i][j-1] or A[i+1][j] or A[i-1][j] (with A[h][k] = 0 if undefined)
 * by using A = m1 and A' = m2. Thus, it is not performed in place. m2 is unchanged apart for m2[i][j].
 * 
 * Input:
 * - int** m1: the original matrix
 * - int** m2: the matrix on which the cardinal sum is applied. Note how m1 and m2 may not be equal before the function is applied.
 * - int num_rows: the number of rows of both matrices
 * - int num_rows: the number of columns of both matrices
 * - int i: row of the element where the cardinal sum will be applied
 * - int j: column of the element where the cardinal sum will be applied
 * - char dir: the direction of the element that will be summed, among "n", "e", "w", "s"
 */
void single_cardinal_sum(int** m1, int** m2, int num_rows, int num_cols, int i, int j, char dir){       
    if (dir = 'n' && i - 1 >= 0){
        m2[i][j] += m1[i-1][j];
    }

    if (dir = 's' && i + 1 < num_rows){
        m2[i][j] += m1[i+1][j];
    }
    
    if (dir = 'w' && j - 1 >= 0){
        m2[i][j] += m1[i][j-1];
    }

    if (dir = 'e' && j + 1 < num_cols){
        m2[i][j] += m1[i][j+1];
    }
}

/**
 * A function that applies a number of iterations from a matrix m1 and copies the results on a matrix m2.
 * The iteration type is chosen among the various functions.
 * The various dimension are taken from global variables for a faster evaluation.
 * Iterations are syncronized among all threads
 * 
 * Input:
 *  - void (args): it contains the parameters in a iter_params* struct that must be cast
 */
void* apply_iteration(void* args){
    iter_params* params = (iter_params*) args;
    int **m1_own, **m2_own;

    for (int k = 0; k < num_iter; k++){
        // At every iterations, the computing and computed matrices need to be switched
        if (k % 2 == 0){
            m1_own = m1;
            m2_own = m2;
        } else {
            m1_own = m2;
            m2_own = m1;
        }

        // Actual computation
        for (int i = 0; i < num_rows; i++){      
            for (int j = 0; j < num_cols; j++){
                params->f(m1, m2, num_rows, num_cols, i, j, params->dir);
            }
        }

        // Barrier
        pthread_mutex_lock (&mutex);
        counter++;
        if (counter == num_threads){
        counter = 0;
        pthread_cond_broadcast (&cond_var);
        } else {
            while(pthread_cond_wait(&cond_var, &mutex) != 0);
        }
        pthread_mutex_unlock(&mutex);

        // Zeroing out the next computed matrix
        if (params->id == 0){
            zero_int_matrix(m1, num_rows, num_cols);
            for (int n = 0; n < num_threads; n++){
                sem_post(&sem);
            }
        }
        sem_wait(&sem);
    }

    return NULL;
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
        m2 = allocate_int_matrix(num_rows, num_cols);

        printf("The original matrix is:");
        print_int_matrix(m1, num_rows, num_cols);  

        // Initializing lock constructs
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond_var, NULL);
        sem_init(&sem, 0, 0);
        counter = 0;

        // Thread-specific variable definitions
        pthread_t** thread_handles = malloc(sizeof(pthread_t*) * num_threads);
        iter_params* params = malloc(sizeof(iter_params) * num_threads);

        // Thread-specific variable initializations
        switch (num_threads){
            case 1:
                params[0] = (iter_params) {&total_cardinal_sum, 'n', 0};
                break;
            case 2:
                params[0] = (iter_params) {&northsouth_cardinal_sum, 'n', 0};
                params[1] = (iter_params) {&eastwest_cardinal_sum, 'n', 1};
                break;
            case 3:
                params[0] = (iter_params) {&northsouth_cardinal_sum, 'n', 0};
                params[1] = (iter_params) {&single_cardinal_sum, 'w', 1};
                params[2] = (iter_params) {&single_cardinal_sum, 'e', 2};
                break;
            case 4:
                params[0] = (iter_params) {&single_cardinal_sum, 'n', 0};
                params[1] = (iter_params) {&single_cardinal_sum, 'w', 1};
                params[2] = (iter_params) {&single_cardinal_sum, 'e', 2};
                params[3] = (iter_params) {&single_cardinal_sum, 's', 3};
            default:
                printf("The number of threads is either less than 1 or bigger than 4, which does not make sense\n");
                return 0;
        }

        // Starting calculations
        GET_TIME(start);

        for (int i = 0; i < num_threads; i++){
            // Initializing the thread handle
            thread_handles[i] = malloc(sizeof(pthread_t));

            // Creating the thread
            pthread_create(thread_handles[i], NULL, apply_iteration, (void*) &params[i]);
        }

        // Joining threads
        for (int i = 0; i < num_threads; i++){
            // We need to wait for the thread to finish
            pthread_join(*thread_handles[i], NULL);
        }

        GET_TIME(stop);
        end = stop - start;

        if (num_iter % 2 == 0){
            print_int_matrix(m1, num_rows, num_cols);
        } else {
            print_int_matrix(m2, num_rows, num_cols);
        }
        printf("Parallel time: %f seconds\n", end);

        // Freeing up allocated, now-useless pointers
        for (int i = 0; i < num_threads; i++){
            free(thread_handles[i]);
        }
        free(thread_handles);
        free(params);
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond_var);
        sem_destroy(&sem);
    }
    return 0;
}