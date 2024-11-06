#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int MPI_Allreduce_custom(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm){
    // A function that mimicks MPI_Allreduce. This implements only sum and ints, but it is possible to build a switch statement that includes all of them.

    int size, rank;
    int** int_buf;
    MPI_Request req;        // Non-blocking communications need always a handle

    // I first need to retrieve size and rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
        // Rank 0 makes space to accomodate all resources. Note that we create different buffers for different positions
        int_buf = (int**) malloc(sizeof(int*) * size);     // rows are different ranks
        for (int i = 0; i < size; i++){
            int_buf[i] = (int*) malloc(sizeof(int) * count);     // columns each buffer's rank
        }
    }

    // Rank i sends its contents to rank 0
    MPI_Isend(sendbuf, count, datatype, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Request_free(&req);

    if (rank == 0){
        for (int r = 0; r < size; r++){
            MPI_Recv(int_buf[r], count, datatype, r, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Now, sums can be performed on each column separately
        int* results = (int*) malloc(sizeof(int) * count);
        for (int j = 0; j < count; j++){
            results[j] = 0;
            for (int i = 0; i < size; i++){
                results[j] += int_buf[i][j];
            }
        }

        // Results can now be sent to everyone
        MPI_Request* reqs = (MPI_Request*) malloc(sizeof(MPI_Request) * size);

        for (int i = 0; i < size; i++){
            MPI_Isend(results, count, datatype, i, 0, MPI_COMM_WORLD, &reqs[i]);
            MPI_Request_free(&reqs[i]);
        }
    }

    MPI_Recv(recvbuf, count, datatype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    return 0;    
}

int MPI_Allreduce_custom_nonblocking_only(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm){
    // A function that mimicks MPI_Allreduce. This implements only sum and ints, but it is possible to build a switch statement that includes all of them.

    int size, rank;
    int** int_buf;
    MPI_Request req;        // Non-blocking communications need always a handle

    // I first need to retrieve size and rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
        // Rank 0 makes space to accomodate all resources. Note that we create different buffers for different positions
        int_buf = (int**) malloc(sizeof(int*) * size);     // rows are different ranks
        for (int i = 0; i < size; i++){
            int_buf[i] = (int*) malloc(sizeof(int) * count);     // columns each buffer's rank
        }
    }

    // Rank i sends its contents to rank 0
    MPI_Isend(sendbuf, count, datatype, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Request_free(&req);

    if (rank == 0){
        MPI_Request* recv_reqs = (MPI_Request*) malloc(sizeof(MPI_Request) * size);
        for (int r = 0; r < size; r++){
            MPI_Irecv(int_buf[r], count, datatype, r, 0, MPI_COMM_WORLD, &recv_reqs[r]);
        }

        // Since this is the non-blocking version, we need to wait everyone after the call
        MPI_Waitall(size, recv_reqs, MPI_STATUSES_IGNORE);

        // Now, sums can be performed on each column separately
        int* results = (int*) malloc(sizeof(int) * count);
        for (int j = 0; j < count; j++){
            results[j] = 0;
            for (int i = 0; i < size; i++){
                results[j] += int_buf[i][j];
            }
        }

        // Results can now be sent to everyone
        MPI_Request* reqs = (MPI_Request*) malloc(sizeof(MPI_Request) * size);

        for (int i = 0; i < size; i++){
            MPI_Isend(results, count, datatype, i, 0, MPI_COMM_WORLD, &reqs[i]);
            MPI_Request_free(&reqs[i]);
        }
    }

    MPI_Irecv(recvbuf, count, datatype, 0, 0, MPI_COMM_WORLD, &req);
    // Since this is the non-blocking version, we need to wait after the call
    MPI_Wait(&req, MPI_STATUS_IGNORE);

    return 0;    
}

int main(int argc, char** argv){
    // MPI initialization
    int r = MPI_Init(&argc, &argv);
    int array_length = atoi(argv[1]);
    int size, rank, a[array_length], sum[array_length];
    double start_time, end_time;     

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (r != MPI_SUCCESS){
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, r);
    } else {
        for (int i = 0; i < 3; i++){
            sleep(rank);     // Let's see if it is actually random
            unsigned int seed = time(0);    // taking current time as seed
            
            for (int i = 0; i < array_length; i++){
                a[i] = (int) rand_r(&seed) % (100 + 1);  // it generates numbers between 0 and 100
            }

            // MPI trial
            if (i == 0){
                start_time = MPI_Wtime();
                MPI_Allreduce(a, sum, array_length, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
                end_time = MPI_Wtime();
            }

            // Custom trial
            if (i == 1){
                start_time = MPI_Wtime();
                MPI_Allreduce_custom(a, sum, array_length, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
                end_time = MPI_Wtime();
            }

            if (i == 2){
                start_time = MPI_Wtime();
                MPI_Allreduce_custom_nonblocking_only(a, sum, array_length, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
                end_time = MPI_Wtime();
            }

            // Performance evaluation
            if (rank == 0){
                if (i == 0){
                    printf("MPI implementation took %lf s\n", end_time - start_time);
                }
                if (i == 1){
                    printf("Custom implementation took %lf s\n", end_time - start_time);
                }
                if (i == 2){
                    printf("Custom implementation with non-blocking calls only took %lf s\n", end_time - start_time);
                }
            }
        }
    }
    MPI_Finalize();
    return 0;
}