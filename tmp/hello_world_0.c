#include <stdio.h>
#include <mpi.h>

int main(void){
    /*
      Per consultare il manuale di MPI, cercare man <nome funzione>.
      La funzione può ritornare uno stato MPI_SUCCESS se è andato a buon fine.

      I programmi MPI si compilano con mpicc.
      Alcuni suggerimenti per mpicc:
      1. Inserire -o nel comando per rinominare l'eseguibile
      2. Con -Wall si attivano tutti i warning

      Invece, con mpirun -n <numero di core> <nome del file> si esegue su # core.
      Le # copie verranno dunque eseguite in parallelo, senza condividere la memoria.

      C'è la possibilità che il numero di core effettivo sia inferiore a quello
      dichiarato dal processore, a causa dell'hyperthreading.
      Si può lanciare un numero arbitrario di processsi con --oversubscribe. Ciò chiaramente
      non garantisce il lancio dei suddetti processi.

      Inoltre, si consiglia di non utilizzare il core 0, in quanto esegue il SO.
    */
    int r = MPI_Init(NULL, NULL);   // La funzione MPI_Init prende due puntatori in input.
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Passo il comunicatore globale, e il puntatore
                                            // alla dimensione.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (r != MPI_SUCCESS){
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, r);   // Questo serve a comunicare a tutti i thread
                                        // che il comando è fallito: non può funzionare
                                        // con un return.
    }

    printf("Hello World! I'm process %d, on a total of %d\n", rank, size);
    MPI_Finalize();     // Questo libera tutte le risorse prese dalla libreria.
                        // Si accompagna sempre all'init.
                        // MPI_Finalize() viene eseguita solo nel momento in cui tutti 
                        // i processi sono in grado di eseguirla
    return 0;
}