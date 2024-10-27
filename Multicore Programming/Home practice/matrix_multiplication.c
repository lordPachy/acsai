#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
        int size, rank;                 // MPI variables

        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (rank == size - 1){
            // First, we store the matrix dimensions
            int num_rows_1 = atoi(argv[1]);
            int num_cols_1 = atoi(argv[2]);
            int num_rows_2 = atoi(argv[3]);
            int num_cols_2 = atoi(argv[4]);

            // We can now create the 2 matrices and print them
            int** m1 = random_matrix(num_rows_1, num_cols_1);
            print_matrix(m1, num_rows_1, num_cols_1);

            int** m2 = random_matrix(num_rows_2, num_cols_2);
            print_matrix(m2, num_rows_2, num_cols_2);

            // Sequential execution
            int** m = matrix_matrix_mul(m1, num_rows_1, num_cols_1, m2, num_rows_2, num_cols_2);
            print_matrix(m, num_rows_1, num_cols_2);
            }

            MPI_Scatter( const void* sendbuf , num_rows_1/p, MPI_int , void* recvbuf , num_rows_1/p, MPI_Datatype recvtype , int root , MPI_Comm comm);

        

        // Parallel execution
        int r = MPI_Init(NULL, NULL);
        int size, rank;                 // MPI variables

        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int my_num_samples = num_samples/size;      // number of samples per thread

        if (r != MPI_SUCCESS){
            printf("Error starting MPI program. Terminating.\n");
            MPI_Abort(MPI_COMM_WORLD, r);
        }
        else{
        }

        MPI_Finalize();
    }

    return 0;
}