#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include "my_timer.h"

int** m1;
int** m2;
int** m3;
int num_rows_1, num_cols_1, num_rows_2, num_cols_2, num_threads;

double start, stop, end;

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
            m[i][j] = (int) rand_r(&seed) % (100 + 1);  // it generates numbers between 0 and 100
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

int** parallel_matrix_matrix_mul(int** m1, int num_rows_1, int num_cols_1, int** m2, int num_rows_2, int num_cols_2){
    // It creates another matrix which is m1@m2, whose pointer-pointer is returned.
    // It employs OpenMP parallelization.

    if (num_cols_1 != num_rows_2){
        printf("Matrix dimensions are not compatible\n");
        return 0;
    } else {
        int** m = allocate_matrix(num_rows_1, num_cols_2);

        #pragma omp parallel for num_threads(num_threads)
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
        GET_TIME(start);

        m3 = parallel_matrix_matrix_mul(m1, num_rows_1, num_cols_1, m2, num_rows_2, num_cols_2);

        GET_TIME(stop);
        end = stop - start;

        //print_matrix(m3, num_rows_1, num_cols_2);
        printf("Parallel time: %f seconds\n", end);
        return 0;
    }
}