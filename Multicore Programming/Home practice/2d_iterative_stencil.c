#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
 * 0 and 100. It requires stdlib.h and time.h.
 * 
 * Input:
 * - int num_rows: a non-zero positive integer
 * - int num_cols: a non-zero positive integer
 * 
 * Output:
 * - int**
 */
int** random_int_matrix(int num_rows, int num_cols){
    int** m = allocate_int_matrix(num_rows, num_cols);
    unsigned int seed = time(0);    // taking current time as seed        

    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++){
            m[i][j] = (int) rand_r(&seed) % (100 + 1);  // it generates numbers between 0 and 100
        }
    }

    return m;
}

/**
 * It prints dinamically allocated matrix stored in a pointer to pointers.
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
            for (int h = 0; h < 2; h++){
                if (j - 1 + 2*h >= 0 && j - 1 + 2*h < num_cols){
                    m2[i][j] += m1[i - 1 + 2*k][j - 1 + 2*h];
                }
            }
        }
    }
}

/**
 * This function creates a dinamically allocated copy of a dinamically allocated matrix of ints.
 * 
 * Input:
 * - int** m1: the matrix to be copied
 * - int num_rows: the number of rows of m1
 * - int num_cols: the number of colums of m1
 * 
 * Output:
 * - int**: a pointer to the copied matrix
 */
int** copy_int_matrix(int** m1, int num_rows, int num_cols){
    int** m2 = allocate_int_matrix(num_rows, num_cols);

    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++){
            m2[i][j] = m1[i][j];
        }
    }

    return m2;
}

int main(int argc, char** argv){
    if (argc != 4){
        printf("Wrong number of matrix dimensions\n");
    } else {
        // MPI initialization
        int r = MPI_Init(&argc, &argv);
        int size, rank, num_rows, num_cols, num_iter;
        int** m;
        int** m1;
        int** m2;     

        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        
        if (r != MPI_SUCCESS){
            printf("Error starting MPI program. Terminating.\n");
            MPI_Abort(MPI_COMM_WORLD, r);
        } else {
            // First, we store the matrix dimensions and the number of iterations
            num_rows = atoi(argv[1]);
            num_cols = atoi(argv[2]);
            num_iter = atoi(argv[3]);

            // We can now instantiate the matrix
            if (rank == 0){
                m = random_int_matrix(num_rows, num_cols);
                m1 = copy_int_matrix(m, num_rows, num_cols);
                print_int_matrix(m, num_rows, num_cols);
                print_int_matrix(m1, num_rows, num_cols);
            }
        }
        MPI_Finalize();
    }
    return 0;
}