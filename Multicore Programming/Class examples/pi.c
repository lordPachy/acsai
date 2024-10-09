#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void montecarlo_circle(int num_samples, int* counter){
    // Montecarlo sampling for a circle of radius 1 in a square of side 2.
    for (int i = 0; i < num_samples; i++){
            double x = 2 * (double)rand() / RAND_MAX - 1;
            double y = 2 * (double)rand() / RAND_MAX - 1;
            double l2 = x * x + y * y;
            if (l2 <= 1.0){
                (*counter)++;
            }
        }
}

int main(){
    int r = MPI_Init(NULL, NULL);
    int size, rank;                 // MPI variables
    int num_samples, samples_in_circle, tot_samples_in_circle;
    num_samples = 1000000;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int my_num_samples = num_samples/size;      // number of samples per thread

    if (r != MPI_SUCCESS){
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, r);
    }
    else{
        // If there are no errors, I can calculate the number of successful trials
        // and communicate to rank 0.
            float start_time = MPI_Wtime();

        montecarlo_circle(my_num_samples, &samples_in_circle);    
        MPI_Reduce(&samples_in_circle, &tot_samples_in_circle, 1, 
                    MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        // Rank 0 can output the pi value
        if (rank == 0){
            double pi = 4.0 * (double)tot_samples_in_circle / (double)num_samples;
            float final_time = MPI_Wtime(); 
            printf("Pi is %lf\n", pi);
            printf("It employed %f seconds.\n", final_time-start_time);
        }
    }

    MPI_Finalize();
    return 0;
}