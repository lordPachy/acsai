#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int MPI_Allreduce_custom(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm){
    // A function that mimicks MPI_Allreduce

    // I first need to retrieve size and rank
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Rank 0 makes space to accomodate all resources. Note that we create different buffers for different positions
    if (rank == 0){
        datatype** = (datatype**) malloc

        for (int i = 0; i < count; i++){
            MPI_INT*  
        }
    }
    MPI_Send(sendbuf, )
}

int main(void){
    // MPI initialization
    int r = MPI_Init(NULL, NULL);
    int size, rank, a, sum;     

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (r != MPI_SUCCESS){
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, r);
    } else {
        sleep(rank);     // Let's see if it is actually random
        unsigned int seed = time(0);    // taking current time as seed

        a = (int) rand_r(&seed) % (100 + 1);  // it generates numbers between 0 and 100
        printf("I'm rank %d and I generated number %d\n", rank, a);

        MPI_Allreduce(&a, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        printf("I'm rank %d and the sum that I received is %d\n", rank, sum);
    }
    MPI_Finalize();
    return 0;
}