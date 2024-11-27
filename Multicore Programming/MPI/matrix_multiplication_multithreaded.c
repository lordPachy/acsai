#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double start, stop, end;
int val;

/**
 * It checks the equality of two matrices.
 * 
 * Input:
 * - int** m1, m2: the two matrices
 * - int num_rows: a non-zero positive integer
 * - int num_cols: a non-zero positive integer
 */
int check_eq_int_matrix(int** m1, int** m2, int num_rows, int num_cols){
    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++){
            if (m1[i][j] != m2[i][j]){
                return 0;
            }
        }
    }

    return 1;
}

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

int main(int argc, char** argv){
    if (argc != 5){
        printf("Wrong number of matrix dimensions\n");
    } else {
        // MPI initialization
        int r = MPI_Init(&argc, &argv);
        int size, rank;
        int **m_serial;    

        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        
        if (r != MPI_SUCCESS){
            printf("Error starting MPI program. Terminating.\n");
            MPI_Abort(MPI_COMM_WORLD, r);
        } else {
            // First, we store the matrix dimensions
            int num_rows_1 = atoi(argv[1]);
            int num_cols_1 = atoi(argv[2]);
            int num_rows_2 = atoi(argv[3]);
            int num_cols_2 = atoi(argv[4]);

            // let's assume the number of rows is divisible by the number of processes, for now
            int** m1 = allocate_matrix(num_rows_1/size, num_cols_1);
            int** m2 = allocate_matrix(num_rows_2, num_cols_2);

            if (rank == size - 1){
                // We can now create the 2 matrices and print them
                int** m1_full = random_matrix(num_rows_1, num_cols_1);
                /*
                printf("I'm rank %d, and the original matrices are:\n", rank);
                print_matrix(m1_full, num_rows_1, num_cols_1);
                */
                m2 = random_matrix(num_rows_2, num_cols_2);
                //print_matrix(m2, num_rows_2, num_cols_2);

                // SEQUENTIAL VERSION
                start = MPI_Wtime();

                m_serial = matrix_matrix_mul(m1_full, num_rows_1, num_cols_1, m2, num_rows_2, num_cols_2);
                
                stop = MPI_Wtime();
                end = stop - start;
                printf("Sequential time: %e seconds\n", end);

                // PARALLEL VERSION
                start = MPI_Wtime();

                // Communication: sending the i-th num_rows_1/size rows to the first num_rows_1/size rows of process i, from m1_full
                for(int process = 0; process < size - 1; process++){   
                    for (int i = 0; i < num_rows_1/size; i++){
                        MPI_Send(m1_full[process * num_rows_1/size + i], num_cols_1, MPI_INT, process, 0, MPI_COMM_WORLD);  // Matrices must be sent row by row
                    }
                }

                // Copying the last part of the matrix, in the last process, to the array where it will be processed
                for (int i = num_rows_1 - num_rows_1/size; i < num_rows_1; i++){
                    for (int j = 0; j < num_cols_1; j++){
                        m1[i - (num_rows_1 - num_rows_1/size)][j] = m1_full[i][j];
                    }
                }
            } else {
            // Communication: receiving the i-th num_rows_1/size rows to the first num_rows_1/size rows of process i, on m1
            for (int i = 0; i < num_rows_1/size; i++){
                MPI_Recv(m1[i], num_cols_1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }


            // Communication : just broadcast matrix 2
            for(int i = 0; i < num_rows_2; i++){      
                MPI_Bcast(m2[i], num_cols_2, MPI_INT, size - 1, MPI_COMM_WORLD);
            }

            // Matrix multiplication
            int** m = matrix_matrix_mul(m1, num_rows_1/size, num_cols_1, m2, num_rows_2, num_cols_2);
            
            if (rank != size - 1){
                // Communication: sending the first num_rows_1/size rows from process i to the i-th num_rows_1/size rows of process size - 1, from m
                for (int i = 0; i < num_rows_1/size; i++){   
                    MPI_Send(m[i], num_cols_2, MPI_INT, 0, 0, MPI_COMM_WORLD);
                }
                printf("Done: m communication from %d\n", rank);
            } else {
                int** m_parallel = allocate_matrix(num_rows_1, num_cols_2);

                // Communication: receiving the first num_rows_1/size rows from process i to the i-th num_rows_1/size rows of process 0, on m_parallel
                for(int process = 0; process < size - 1; process++){   
                    for (int i = 0; i < num_rows_1/size; i++){   
                        MPI_Recv(m_parallel[process * num_rows_1/size + i], num_cols_2, MPI_INT, process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        printf("Row %d has been received from rank %d\n", process * num_rows_1/size + i, process);
                        fflush(stdout);
                    }

                }
                printf("Done: m communication\n");
                fflush(stdout);

                // Copying the last part of the matrix, in the last process, to the array where it will be evaluated
                for (int i = num_rows_1 - num_rows_1/size; i < num_rows_1; i++){
                    for (int j = 0; j < num_cols_1; j++){
                        m_parallel[i][j] = m1[i - (num_rows_1 - num_rows_1/size)][j];
                    }
                }

                // Timing
                stop = MPI_Wtime();
                end = stop - start;
                printf("Parallel time: %e seconds\n", end);

                // Checking correctness
                val = check_eq_int_matrix(m_serial, m_parallel, num_rows_1, num_cols_2);
                printf("The calculations are correct: %d\n", val);
            }
        }

        MPI_Finalize();
    }

    return 0;
}