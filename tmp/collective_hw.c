#include <stdio.h>
#include <mpi.h>

int main(void){
    /*
      I vari thread costituiscono le proprie e stringhe e le inviano al nodo 0,
      il quale le stampa in ordine.
    */
    int r = MPI_Init(NULL, NULL);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (r != MPI_SUCCESS){
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, r);
    }
    if (rank == 0){
        printf("Hello World! I'm process %d, on a total of %d\n", rank, size);
        int i;
        for (i = 1; i < size; i++){
            char str[256];      // è necessario allocare un buffer di dimensione opportuna
            MPI_Recv(str, 256, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s", str);
            }
    } else {
        char str[256];
        sprintf(str, "Hello World! I'm process %d, on a total of %d\n", rank, size);
        MPI_Send(str, 256, MPI_CHAR, 0, 0, MPI_COMM_WORLD);      
        // attenzione: il tipo deve essere MPI! Se è stata definita una struct
    }
    MPI_Finalize();
    return 0;
}