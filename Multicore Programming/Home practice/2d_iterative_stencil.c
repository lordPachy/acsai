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
    fflush(stdout);         // Sometimes, prints are stored in a buffer that needs to flushed out
}

int main(int argc, char** argv){
    if (argc != 4){
        printf("Wrong number of matrix dimensions\n");
    } else {
        // MPI initialization
        int r = MPI_Init(&argc, &argv);
        int size, rank, num_rows, num_cols, num_iter;     

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
        }
        MPI_Finalize();
    }
    return 0;
}