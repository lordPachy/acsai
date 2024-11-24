// LIBRARIES

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// FUNCTIONS

/**
 * It allocates the 3d tensor dinamically, as a pointer to a vector of pointers.
 * 
 * Input:
 * - int num_matrices: a non-zero positive integer
 * - int num_rows: a non-zero positive integer
 * - int num_cols: a non-zero positive integer
 * 
 * Output:
 * - int***
 */
int*** allocate_int_3d_tensor(int num_matrices, int num_rows, int num_cols){
    int*** m;

    m = (int***) malloc(sizeof(int**)*num_matrices);
    for (int k = 0; k < num_matrices; k++){
        m[k] = (int**) malloc(sizeof(int*)*num_rows);
        for(int i = 0; i < num_rows; i++){
            m[k][i] = (int*) malloc(sizeof(int)*num_cols);
        }
    }

    return m;
}

/**
 * It returns a dinamically allocated 3d tensor full of random ints between
 * 0 and 5. It requires stdlib.h and time.h.
 * 
 * Input:
 * - int*** m: (optional) the 3d tensor
 * - int num_matrices: a non-zero positive integer
 * - int num_rows: a non-zero positive integer
 * - int num_cols: a non-zero positive integer
 * 
 * Output:
 * - int***
 */
int*** random_int_3d_tensor(int*** m, int num_matrices, int num_rows, int num_cols){
    if (m == NULL){
        m = allocate_int_3d_tensor(num_matrices, num_rows, num_cols);
    }
    unsigned int seed = time(0);    // taking current time as seed        

    for (int k = 0; k < num_matrices; k++){
        for (int i = 0; i < num_rows; i++){
            for (int j = 0; j < num_cols; j++){
                m[k][i][j] = (int) rand_r(&seed) % (5 + 1);  // it generates numbers between 0 and 5
            }
        }
    }

    return m;
}

/**
 * It prints a dinamically allocated 3d tensor stored in a pointer to pointers.
 * 
 * Input:
 * - int*** m: the 3d tensor to be printed
 * - int num_matrices: the function will print matrices in the range [0, num_matrices). It must be less or equal to the real number of matrices of m
 * - int num_rows: the function will print rows in the range [0, num_rows). It must be less or equal to the real number of rows of m
 * - int num_cols: the function will print columns in the range [0, num_cols). It must be less or equal to the real number of columns of m
 * 
 * Output:
 * - NULL
 */
void print_int_3d_tensor(int*** m, int num_matrices, int num_rows, int num_cols){
    printf("\n");
    for (int k = 0; k < num_matrices; k++){
        printf("[");
        for (int i = 0; i < num_rows; i++){
            for (int j = 0; j < num_cols; j++)
                printf("%d ", m[k][i][j]);
            if (i != num_rows - 1){
                printf("\n");
            }
        }
        printf("]\n");
    }

    printf("\n");
    fflush(stdout);         // Sometimes, prints are stored in a buffer that needs to flushed out
}

/**
 * This function applies the operation of cardinal sum on a single element (k, i, j), defined as,
 * A'[k][i][j] = A[k-1][i][j] + A[k+1][i][j] + A[k][i+1][j] + A[k][i][j+1] + A[k][i-1][j] + A[k][i][j-1] (with A[m][n][o] = 0 if undefined)
 * by using A = m1 and A' = m2. Thus, it is not performed in place. m2 is unchanged apart for m2[k][i][j].
 * 
 * Input:
 * - int*** m1: the original 3d tensor
 * - int*** m2: the 3d tensor on which the cardinal sum is applied. Note how m1 and m2 may not be equal before the function is applied.
 * - int num_matrices: the number of rows of both 3d tensors
 * - int num_rows: the number of rows of both 3d tensors
 * - int num_rows: the number of columns of both 3d tensors
 * - int k: matrix of the element where the cardinal sum will be applied
 * - int i: row of the element where the cardinal sum will be applied
 * - int j: column of the element where the cardinal sum will be applied
 */
void element_cardinal_sum(int*** m1, int*** m2, int num_matrices, int num_rows, int num_cols, int k, int i, int j){
    m2[k][i][j] = 0;

    for (int l = 0; l < 2; l++){
        if (k - 1 + 2*l >= 0 && k - 1 + 2*l < num_matrices){
            m2[k][i][j] += m1[k - 1 + 2*l][i][j];
        }
    }

    for (int n = 0; n < 2; n++){
        if (i - 1 + 2*n >= 0 && i - 1 + 2*n < num_rows){
            m2[k][i][j] += m1[k][i - 1 + 2*n][j];
        }
    }
            
    for (int h = 0; h < 2; h++){
        if (j - 1 + 2*h >= 0 && j - 1 + 2*h < num_cols){
            m2[k][i][j] += m1[k][i][j - 1 + 2*h];
        }
    }
}

/**
 * A function that applies a single iteration from a 3d vector m1 and copies the results on a 3d vector m2.
 * Input:
 *  - int*** m1 = the 3d vector with the values inside
 *  - int*** m2 = OUT
 *  - int num_matrices = the number of matrices of both 3d vectors, which needs to be the same
 *  - int num_rows = the number of rows of both 3d vectors, which needs to be the same
 *  - int num_cols = the number of columns of both 3d vectors, which needs to be the same
 *  - int rank = the rank of the process calling the function
 *  - int size = the total number of processes
 * Output:
 *  - int*** m2 = the 3d vector where the result of the operation is applied
 */
void apply_iteration(int*** m1, int*** m2, int num_matrices, int num_rows, int num_cols, int rank, int size){
    int isFirst = 0;
    if (rank == 0){
        isFirst = 1;
    }
    int isLast = 0;
    if (rank == size - 1){
        isLast = 1;
    }
    
    for (int k = 1 - isFirst; k < num_matrices - 1 + isLast; k++){           // The first and last rank will still compute the correct dimensions
        for (int i = 0; i < num_rows; i++){      
            for (int j = 0; j < num_cols; j++){
                element_cardinal_sum(m1, m2, num_matrices, num_rows, num_cols, k, i, j);
            }
        }
    }
}

/**
 * A function calculating the pointer of the starting matrices for each rank.
 * 
 * Input:
 *  - int num_matrices: the number of matrices of the 3d tensor
 *  - int size: the number of processes
 *  - int rank: the rank of the current process
 *  - int*** base_pointer: the pointer to the start of the 3d tensor
 * 
 * Output:
 *  - int*** prev_to_min: a 3d tensor pointer which will be passed to MPI_send.
 *                  It corresponds to the address of the first matrix of the 3d tensor that is needed to calculate the considered portion.
 *                  Note that if rank == 0, this is 0, which is also the same matrix that is first calculated.
 */
int*** calculate_base_pointer(int num_matrices, int size, int rank, int*** base_pointer){
    int*** prev_to_min = base_pointer;    // the index of the first matrix used to recalculate m1, but which is not recalculated

    if (rank > 0){
        prev_to_min = &(base_pointer[(num_matrices/size) * rank - 1]);      // We cannot share the matrix before the first. Rank 0 is the one holding the first matrix 
    }
    return prev_to_min;
}   

/**
 * A function calculating the number of matrices that must be passed and communicated for each rank.
 * This function assumes there at least 2 processes.
 * 
 * Input:
 *  - int num_matrices: the number of matrices of the 3d tensor
 *  - int size: the number of processes
 *  - int rank: the rank of the current process
 * 
 * Output:.
 *  - int count_considered: the number of matrices used the process, that must be communicated by MPI_Send.
 *                It corresponds to the number of recalculated elements + 2, since the recalculation needs to consider 2 additional lines:
 *                the one above the top one and the below the bottom. Note that the first and the last rank will have 1 less.
 */
int calculate_considered_matrices(int num_matrices, int size, int rank){
    int count_recalculated = num_matrices/size;
    int count_considered = count_recalculated + 2;  // the number of matrices used to calculate the 3d tensor, which is two more the number of matrices recalculated (top and bottom are untouched)
    // The first rank does not have a matrix before its first, so it only considers another matrix
    if (rank == 0){
        count_considered = count_recalculated + 1;
    }
    // The last rank does not have a matrix after its last, so it only considers another matrix. Moreover, its last matrix must be the last.
    if (rank == size - 1){
        count_recalculated = num_matrices - num_matrices/size * (size - 1);
        count_considered = count_recalculated + 1;
    }

    return count_considered;
}

/**
 * A function calculating the number of matrices that must be changed by each rank.
 * This function assumes there at least 2 processes.
 * 
 * Input:
 *  - int num_matrices: the number of matrices of the 3d tensor
 *  - int size: the number of processes
 *  - int rank: the rank of the current process
 * 
 * Output:.
 *  - int count_changed: the number of matrices calculated by the process, that must be returned to rank 0.
 *                       Note that the first and the last rank will have 1 less.
 */
int calculate_changed_matrices(int num_matrices, int size, int rank){
    int count_recalculated = num_matrices/size;

    // The last rank does not have a matrix after its last, so it only considers another matrix. Moreover, its last matrix must be the last.
    if (rank == size - 1){
        count_recalculated = num_matrices - num_matrices/size * (size - 1);
    }

    return count_recalculated;
}

// MAIN

int main(int argc, char** argv){
    if (argc != 5){
        printf("Wrong number of 3d tensor dimensions\n");
    } else {
        // MPI initialization
        int r = MPI_Init(&argc, &argv);
        if (r != MPI_SUCCESS){
            printf("Error starting MPI program. Terminating.\n");
            MPI_Abort(MPI_COMM_WORLD, r);
        } else {
            int size, rank;
            double start, end;

            MPI_Comm_size(MPI_COMM_WORLD, &size);
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);

            // Timing
            if (rank == 0){
                start = MPI_Wtime();
            }

            // Storing the 3d tensor dimensions and the number of iterations
            int num_matrices = atoi(argv[1]);
            int num_rows = atoi(argv[2]);
            int num_cols = atoi(argv[3]);
            int num_iter = atoi(argv[4]);

            // Defining communication info
            int**** buffers;
            int* counts_considered;
            int* counts_recalculated;
            MPI_Request req;
            MPI_Status stat;

            // Allocating matrices
            int num_considered_matrices = calculate_considered_matrices(num_matrices, size, rank);
            int num_calculated_matrices = calculate_changed_matrices(num_matrices, size, rank);
            int*** m1 = allocate_int_3d_tensor(num_considered_matrices, num_rows, num_cols);
            int*** m2 = allocate_int_3d_tensor(num_considered_matrices, num_rows, num_cols);

            if (rank == 0){
                // Instatiating the matrix
                m1 = random_int_3d_tensor(NULL, num_matrices, num_rows, num_cols);
                //printf("The original matrix is:");
                //print_int_3d_tensor(m1, num_matrices, num_rows, num_cols);  

                // Instatiating communication info per process
                buffers = (int****) malloc(sizeof(int***) * (size - 1));
                counts_considered = (int*) malloc(sizeof(int) * (size - 1));
                counts_recalculated = (int*) malloc(sizeof(int) * (size - 1));
                
                // Sending pieces of the matrix to each rank
                for (int r = 1; r < size; r++){
                    // Calculating the information to be sent to each rank
                    buffers[r-1] = calculate_base_pointer(num_matrices, size, r, m1);
                    counts_considered[r-1] = calculate_considered_matrices(num_matrices, size, r);

                    // Sending the 3d tensor
                    for (int k = 0; k < counts_considered[r-1]; k++){
                        for (int i = 0; i < num_rows; i++){
                            MPI_Isend(buffers[r-1][k][i], num_cols, MPI_INT, r, 0, MPI_COMM_WORLD, &req);
                        }
                    }
                }

                free(counts_considered);
            } else {
                // Receiving pieces of the 3d tensor
                for (int k = 0; k < num_considered_matrices; k++){
                    for (int i = 0; i < num_rows; i++){
                        MPI_Recv(m1[k][i], num_cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    }
                }
            }

            // Applying n-1 iterations
            int*** tmp;
            for (int iter = 0; iter < num_iter - 1; iter++){
                apply_iteration(m1, m2, num_considered_matrices, num_rows, num_cols, rank, size);

                // Communication
                if (size > 1 && rank == 0){
                    for (int i = 0; i < num_rows; i++){
                        MPI_Isend(m2[num_considered_matrices - 2][i], num_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &req);
                    }

                    for (int i = 0; i < num_rows; i++){
                        MPI_Recv(m2[num_considered_matrices - 1][i], num_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &stat);
                    }
                } else if (size > 1 && rank == size - 1){
                    for (int i = 0; i < num_rows; i++){
                        MPI_Isend(m2[1][i], num_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &req);
                    }

                    for (int i = 0; i < num_rows; i++){
                        MPI_Recv(m2[0][i], num_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &stat);
                    }
                } else if (size > 1){
                    for (int i = 0; i < num_rows; i++){
                        MPI_Isend(m2[num_considered_matrices - 2][i], num_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &req);
                    }

                    for (int i = 0; i < num_rows; i++){
                        MPI_Isend(m2[1][i], num_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &req);
                    }

                    for (int i = 0; i < num_rows; i++){
                        MPI_Recv(m2[num_considered_matrices - 1][i], num_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &stat);
                    }

                    for (int i = 0; i < num_rows; i++){
                        MPI_Recv(m2[0][i], num_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &stat);
                    }
                }
                
                tmp = m1;
                m1 = m2;
                m2 = tmp;
            }

            // Applying the last iteration
            if (num_iter > 0){
                apply_iteration(m1, m2, num_considered_matrices, num_matrices, num_cols, rank, size);
                tmp = m1;
                m1 = m2;
                m2 = tmp;

                // Sending the results to rank 0
                if (rank != 0){
                    // Sending the 3d tensor
                    for (int k = 0; k < num_calculated_matrices; k++){
                        for (int i = 0; i < num_rows; i++){
                            MPI_Send(m1[k+1][i], num_cols, MPI_INT, 0, 0, MPI_COMM_WORLD);
                        }
                    }
                }  else {
                    // Meanwhile, rank 0 starts hearing for results
                    // Initializing the receive requests
                    MPI_Request*** reqs_recv = (MPI_Request***) malloc(sizeof(MPI_Request**) * (size - 1));

                    for (int r = 1; r < size; r++){
                        // Initializing the receive requests
                        counts_recalculated[r-1] = calculate_changed_matrices(num_matrices, size, r);
                        reqs_recv[r-1] = (MPI_Request**) malloc(sizeof(MPI_Request*) * (counts_recalculated[r-1]));

                        for (int k = 0; k < counts_recalculated[r-1]; k++){
                            reqs_recv[r-1][k] = (MPI_Request*) malloc(sizeof(MPI_Request) * num_rows);
                            for (int i = 0; i < num_rows; i++){
                                MPI_Irecv(buffers[r-1][k+1][i], num_cols, MPI_INT, r, 0, MPI_COMM_WORLD, &reqs_recv[r-1][k][i]);
                            }
                        }
                    }

                    // Rank 0 must copy its own results on m1 if they are not already there
                    if (num_iter % 2 != 0){
                        tmp = m1;
                        m1 = m2;
                        m2 = tmp;
                        for (int k = 0; k < num_considered_matrices - 1; k++){
                            for (int i = 0; i < num_rows - 1; i++){
                                for (int j = 0; j < num_cols; j++){
                                    m1[k][i][j] = m2[k][i][j];
                                }
                            }
                        }
                    }

                    // Rank 0 can now wait to have all pieces
                    for (int r = 1; r < size; r++){
                        for (int k = 0; k < counts_recalculated[r-1]; k++){
                            MPI_Waitall(num_rows, reqs_recv[r-1][k], MPI_STATUSES_IGNORE);
                        }
                    }

                    // And finally print the result 3d tensor
                    //printf("\nThe final result is:");
                    //print_int_3d_tensor(m1, num_matrices, num_rows, num_cols);

                    // Freeing up memory
                    free(buffers);
                    free(counts_recalculated);
                }
            }
            if (rank == 0){
                for (int k = num_considered_matrices; k < num_matrices; k++){
                    for (int i = 0; i < num_rows; i++){
                        free(m1[k][i]);
                    }
                    free(m1[k]);
                }
            }
            
            for (int k = 0; k < num_considered_matrices; k++){
                for (int i = 0; i < num_rows; i++){
                    free(m1[k][i]);
                    free(m2[k][i]);
                }
                free(m1[k]);
                free(m2[k]);
            }
            free(m1);
            free(m2);

            // Timing
            if (rank == 0){
                end = MPI_Wtime();
                printf("Elapsed time is %e s\n", end-start);
            }
        }
        MPI_Finalize();
    }
    return 0;
}