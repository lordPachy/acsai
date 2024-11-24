// LIBRARIES

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// FUNCTIONS

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
void element_cardinal_sum(int** m1, int** m2, int num_rows, int num_cols, int i, int j){
    m2[i][j] = 0;

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
 * A function that applies a single iteration from a matrix m1 and copies the results on a matrix m2.
 * Input:
 *  - int** m1 = the matrix with the values inside
 *  - int** m2 = OUT
 *  - int num_rows = the number of rows of both matrices, which needs to be the same
 *  - int num_cols = the number of columns of both matrices, which needs to be the same
 *  - int rank = the rank of the process calling the function
 *  - int size = the total number of processes
 * Output:
 *  - int** m2 = the matrix where the result of the operation is applied
 */
void apply_iteration(int** m1, int** m2, int num_rows, int num_cols, int rank, int size){
    int isFirst = 0;
    if (rank == 0){
        isFirst = 1;
    }
    int isLast = 0;
    if (rank == size - 1){
        isLast = 1;
    }
    
    for (int i = 1 - isFirst; i < num_rows - 1 + isLast; i++){      // The first and last rank will still compute the correct dimensions
        for (int j = 0; j < num_cols; j++){
            element_cardinal_sum(m1, m2, num_rows, num_cols, i, j);
        }
    }
}

/**
 * A function calculating the pointer of the starting rows for each rank.
 * 
 * Input:
 *  - int num_rows: the number of rows of the matrix
 *  - int size: the number of processes
 *  - int rank: the rank of the current process
 *  - int** base_pointer: the pointer to the start of the matrix
 * 
 * Output:
 *  - int** prev_to_min: a matrix which will be passed to MPI_send.
 *                  It corresponds to the address of the first row of the matrix that is needed to calculate the considered portion.
 *                  Note that if rank == 0, this is 0, which is also the same row that is first calculated.
 */
int** calculate_base_pointer(int num_rows, int size, int rank, int** base_pointer){
    int** prev_to_min = base_pointer;    // the index of the first row used to recalculate m1, but which is not recalculated

    if (rank > 0){
        prev_to_min = &(base_pointer[(num_rows/size) * rank - 1]);      // We cannot share the row before the first. Rank 0 is the one holding the first row 
    }

    return prev_to_min;
}   

/**
 * A function calculating the number of rows that must be passed and communicated for each rank.
 * This function assumes there at least 2 processes.
 * 
 * Input:
 *  - int num_rows: the number of rows of the matrix
 *  - int size: the number of processes
 *  - int rank: the rank of the current process
 * 
 * Output:.
 *  - int count_considered: the number of rows used the process, that must be communicated by MPI_Send.
 *                It corresponds to the number of recalculated elements + 2, since the recalculation needs to consider 2 additional lines:
 *                the one above the top one and the below the bottom. Note that the first and the last rank will have 1 less.
 */
int calculate_considered_rows(int num_rows, int size, int rank){
    int count_recalculated = num_rows/size;
    int count_considered = count_recalculated + 2;  // the number of rows used to calculate the matrix, which is two more the number of rows recalculated (top and bottom are untouched)
    // The first rank does not have a row before its first, so it only considers another row
    if (rank == 0){
        count_considered = count_recalculated + 1;
    }
    // The last rank does not have a row after its last, so it only considers another row. Moreover, its last row must be the last.
    if (rank == size - 1){
        count_recalculated = num_rows - num_rows/size * (size - 1);
        count_considered = count_recalculated + 1;
    }

    return count_considered;
}

/**
 * A function calculating the number of rows that must be changed by each rank.
 * This function assumes there at least 2 processes.
 * 
 * Input:
 *  - int num_rows: the number of rows of the matrix
 *  - int size: the number of processes
 *  - int rank: the rank of the current process
 * 
 * Output:.
 *  - int count_changed: the number of rows calculated by the process, that must be returned to rank 0.
 *                       Note that the first and the last rank will have 1 less.
 */
int calculate_changed_rows(int num_rows, int size, int rank){
    int count_recalculated = num_rows/size;

    // The last rank does not have a row after its last, so it only considers another row. Moreover, its last row must be the last.
    if (rank == size - 1){
        count_recalculated = num_rows - num_rows/size * (size - 1);
    }

    return count_recalculated;
}

// MAIN

int main(int argc, char** argv){
    if (argc != 4){
        printf("Wrong number of matrix dimensions\n");
    } else {
        // MPI initialization
        int r = MPI_Init(&argc, &argv);
        if (r != MPI_SUCCESS){
            printf("Error starting MPI program. Terminating.\n");
            MPI_Abort(MPI_COMM_WORLD, r);
        } else {
            int size, rank;

            MPI_Comm_size(MPI_COMM_WORLD, &size);
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);

            // Storing the matrix dimensions and the number of iterations
            int num_rows = atoi(argv[1]);
            int num_cols = atoi(argv[2]);
            int num_iter = atoi(argv[3]);

            // Defining communication info
            int*** buffers;
            int* counts_considered;
            int* counts_recalculated;
            MPI_Request** reqs_send;
            MPI_Request req_comm1, req_comm2;
            MPI_Status stat_comm1, stat_comm2;

            // Allocating matrices
            int num_considered_rows = calculate_considered_rows(num_rows, size, rank);
            int num_calculated_rows = calculate_changed_rows(num_rows, size, rank);
            int** m1 = allocate_int_matrix(num_considered_rows, num_cols);
            int** m2 = allocate_int_matrix(num_considered_rows, num_cols);

            if (rank == 0){
                // Instatiating the matrix
                m1 = random_int_matrix(NULL, num_rows, num_cols);
                printf("The original matrix is:");
                print_int_matrix(m1, num_rows, num_cols);  

                // Instatiating communication info per process
                buffers = (int***) malloc(sizeof(int**) * (size - 1));
                counts_considered = (int*) malloc(sizeof(int) * (size - 1));
                counts_recalculated = (int*) malloc(sizeof(int) * (size - 1));
                
                // Instantiating the requests for Isend
                reqs_send = (MPI_Request**) malloc(sizeof(MPI_Request*) * (size - 1));

                // Sending pieces of the matrix to each rank
                for (int r = 1; r < size; r++){
                    // Calculating the information to be sent to each rank
                    buffers[r-1] = calculate_base_pointer(num_rows, size, r, m1);
                    counts_considered[r-1] = calculate_considered_rows(num_rows, size, r);

                    // Instantiating the requests for Isend
                    reqs_send[r-1] = (MPI_Request*) malloc(sizeof(MPI_Request) * counts_considered[r-1]);

                    // Sending the matrix
                    for (int i = 0; i < counts_considered[r-1]; i++){
                        MPI_Isend(buffers[r-1][i], num_cols, MPI_INT, r, 0, MPI_COMM_WORLD, &reqs_send[r-1][i]);
                    }
                }

                free(counts_considered);
            } else {
                // Receiving pieces of the matrix
                for (int i = 0; i < num_considered_rows; i++){
                    MPI_Recv(m1[i], num_cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }

            // Applying n-1 iterations
            int** tmp;
            for (int i = 0; i < num_iter - 1; i++){
                apply_iteration(m1, m2, num_considered_rows, num_cols, rank, size);

                // Communication
                if (size > 1 && rank == 0){
                    MPI_Isend(m2[num_considered_rows - 2], num_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &req_comm1);
                    MPI_Recv(m2[num_considered_rows - 1], num_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &stat_comm1);
                } else if (size > 1 && rank == size - 1){
                    MPI_Isend(m2[1], num_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &req_comm2);
                    MPI_Recv(m2[0], num_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &stat_comm2);
                } else if (size > 1){
                    MPI_Isend(m2[num_considered_rows - 2], num_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &req_comm1);
                    MPI_Isend(m2[1], num_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &req_comm2);

                    MPI_Recv(m2[num_considered_rows - 1], num_cols, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &stat_comm1);
                    MPI_Recv(m2[0], num_cols, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &stat_comm2);
                }
                
                tmp = m1;
                m1 = m2;
                m2 = tmp;
            }

            // Applying the last iteration
            if (num_iter > 0){
                apply_iteration(m1, m2, num_considered_rows, num_cols, rank, size);
                tmp = m1;
                m1 = m2;
                m2 = tmp;

                // Sending the results to rank 0
                if (rank != 0){
                    // Sending the matrix
                    for (int i = 0; i < num_calculated_rows; i++){
                        MPI_Send(m1[i+1], num_cols, MPI_INT, 0, 0, MPI_COMM_WORLD);
                    }
                }  else {
                    // Meanwhile, rank 0 starts hearing for results
                    // Initializing the receive requests
                    MPI_Request** reqs_recv = (MPI_Request**) malloc(sizeof(MPI_Request*) * (size - 1));

                    for (int r = 1; r < size; r++){
                        // Initializing the receive requests
                        counts_recalculated[r-1] = calculate_changed_rows(num_rows, size, r);
                        reqs_recv[r-1] = (MPI_Request*) malloc(sizeof(MPI_Request) * (counts_recalculated[r-1]));

                        for (int i = 0; i < counts_recalculated[r-1]; i++){
                            MPI_Irecv(buffers[r-1][i+1], num_cols, MPI_INT, r, 0, MPI_COMM_WORLD, &reqs_recv[r-1][i]);
                        }
                    }

                    // Rank 0 must copy its own results on m1 if they are not already there
                    if (num_iter % 2 != 0){
                        tmp = m1;
                        m1 = m2;
                        m2 = tmp;
                        for (int i = 0; i < num_considered_rows - 1; i++){
                            for (int j = 0; j < num_cols; j++){
                                m1[i][j] = m2[i][j];
                            }
                        }
                    }

                    // Rank 0 can now wait to have all pieces
                    for (int r = 1; r < size; r++){
                        MPI_Waitall(counts_recalculated[r-1], reqs_recv[r-1], MPI_STATUSES_IGNORE);
                    }

                    // And finally print the result matrix
                    printf("\nThe final result is:");
                    print_int_matrix(m1, num_rows, num_cols);

                    // Freeing up memory
                    free(buffers);
                    free(counts_recalculated);

                    for (int r = 1; r < size; r++){
                        free(reqs_send[r-1]);
                        free(reqs_recv[r-1]);
                    }
                    free(reqs_send);
                    free(reqs_recv);
                }
            }
            if (rank == 0){
                for (int i = num_considered_rows; i < num_rows; i++){
                    free(m1[i]);
                }
            }
            for (int i = 0; i < num_considered_rows; i++){
                free(m1[i]);
                free(m2[i]);
            }
            free(m1);
            free(m2);
        }
        MPI_Finalize();
    }
    return 0;
}