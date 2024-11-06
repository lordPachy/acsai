#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void random_vector(int length, int vector[length]){
    for (int i = 0; i < length; i++){
        vector[i] = rand()%20;
    }
}

void partial_vector_sum(int length, int vec1[length], int vec2[length], int vec3[length]){
    for (int i = 0; i < length; i++){
        vec3[i] = vec1[i] + vec2[i];
    }
}

int main(){
    int r = MPI_Init(NULL, NULL);
    int size, rank;                 // MPI variables
    int rel_vec_size = 4;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (r != MPI_SUCCESS){
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, r);
    }
    else{
        int vec_size = size * rel_vec_size;
        int whole_vec1[vec_size], whole_vec2[vec_size], whole_vec3[vec_size];
        int vec1[rel_vec_size], vec2[rel_vec_size], vec3[rel_vec_size];
        
        if (rank == 0){
            random_vector(vec_size, whole_vec1);
            random_vector(vec_size, whole_vec2);
        }

        MPI_Scatter(whole_vec1, rel_vec_size, MPI_INT,
                    vec1, rel_vec_size, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(whole_vec2, rel_vec_size, MPI_INT,
                    vec2, rel_vec_size, MPI_INT, 0, MPI_COMM_WORLD);

        partial_vector_sum(rel_vec_size, vec1, vec2, vec3);

        if (rank == 1){
            printf("I'm rank %d, and vec1 is\n", rank);
            for (int i = 0; i < rel_vec_size; i++){
                printf("%d ", vec1[i]);
            }
            printf("\nI'm rank %d, and vec2 is\n", rank);
            for (int i = 0; i < rel_vec_size; i++){
                printf("%d ", vec2[i]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}