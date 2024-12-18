// LIBRARIES

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "my_timer.h"

// FUNCTIONS

/**
 * It returns a dinamically allocated vector full of random ints between
 * min and max. It requires stdlib.h and time.h.
 * 
 * Input:
 * - int* v: (optional) the vector
 * - int length: a non-zero positive integer
 * - int min: the minimum possible value
 * - int max: the maximum possible value
 * 
 * Output:
 * - int*
 */
int* random_int_vector(int* v, int length, int min, int max){
    if (v == NULL){
        v = (int*) malloc(sizeof(int) * length);
    }
    unsigned int seed = time(0);    // taking current time as seed        

    for (int i = 0; i < length; i++){
        v[i] = (int) rand_r(&seed) % (max - min + 1) + min;  // it generates numbers between 0 and 5
    }

    return v;
}

/**
 * It prints a vector.
 * 
 * Input:
 * - int* v: the vector to be printed
 * - int length: the function will print rows in the range [0, length). It must be less or equal to the real number of elements of v
 * 
 * Output:
 * - NULL
 */
void print_int_vector(int* v, int length){
    for (int i = 0; i < length; i++){
        printf("%d ", v[i]);
    }
    printf("\n");
    fflush(stdout);         // Sometimes, prints are stored in a buffer that needs to flushed out
}

// MAIN

int main(int argc, char** argv){
    // Input checking
    if (argc != 5){
        printf("Wrong number of matrix dimensions\n");
        return 0;
    }

    // MPI initialization
    int r = MPI_Init(&argc, &argv);
    if (r != MPI_SUCCESS){
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, r);
        MPI_Finalize();
        return 0;
    }

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Storing the data length, extrema and bin count
    int data_count = atoi(argv[1]);
    int min_meas = atoi(argv[2]);
    int max_meas = atoi(argv[3]);
    int bin_count = atoi(argv[4]);

    // Timing
    double start, stop, endtime;

    // Defining communication 
    int* buffer = (int*) malloc(sizeof(int) * (data_count/size));
    int* data;
    int* extrema;
    // IMPORTANT: histograms are given with the assumption of being set to 0
    int* histogram = (int*) calloc (sizeof(int), bin_count);

    // Creating data and sending them
    if (rank == 0){
        data = random_int_vector(NULL, data_count, min_meas, max_meas);
        //print_int_vector(data, data_count);

        GET_TIME(start);    // timing

        extrema = (int*) malloc (sizeof(int*) * size * 2);
        
        MPI_Request req;    // phantom request

        for (int i = 0; i < size; i++){
            extrema[2*i] = data_count / size * i;
            extrema[2*i+1] = data_count / size * (i+1);
            if (i == size - 1){
                extrema[2*i+1] = data_count;
            }
            MPI_Isend(&data[extrema[2*i]], extrema[2*i+1] - extrema[2*i], MPI_INT, i, 0, MPI_COMM_WORLD, &req);
        }
    }

    // Receiving the data
    int count = data_count/size;
    if (rank == size - 1){
        count = data_count - ((size - 1) * (data_count/size));
    }
    MPI_Recv(buffer, count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Filling up the buckets
    int index;
    for (int i = 0; i < count; i++){
        // Assigning a value to a specific bin
        index = bin_count * (buffer[i] - min_meas) / (max_meas - min_meas);
        if (index == bin_count){
            index--;
        }

        // Increasing the associated bin counter
        histogram[index]++;
    }

    // Sending the data back to rank 0
    if (rank != 0){
        MPI_Send(histogram, bin_count, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        // Instatiating a receiving buffer
        int* recv_buffer = (int*) malloc(sizeof(int) * bin_count);

        for (int i = 1; i < size; i++){
            MPI_Recv(recv_buffer, bin_count, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Adding up the results
            for (int j = 0; j < bin_count; j++){
                histogram[j] += recv_buffer[j];
            }
        }

        // Printing the final result
        print_int_vector(histogram, bin_count);

        // Timing
        GET_TIME(stop);    
        endtime = stop - start;
        printf("The final calculation time was %e\n", endtime);

        // Cleaning up
        free(recv_buffer);
        free(extrema);
        free(data); 
    }
    free(buffer);
    free(histogram);

    MPI_Finalize();
    return 0;
}