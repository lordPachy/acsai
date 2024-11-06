#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// It is useful to declare data as a global variable, in such a way that all threads can access it

int* data;

/**
 * It returns a vector of random ints. It requires stdlib.h and time.h.
 * 
 * Input:
 * - int length: a non-zero positive integer
 * - int min: an integer
 * - int max: an integer bigger than min
 * 
 * Output:
 * - int**
 */
int* random_int_vector(int length, int min, int max){
    int* v = (int*) malloc(sizeof(int) * length);
    unsigned int seed = time(0);    // taking current time as seed        

    for (int i = 0; i < length; i++){
        v[i] = (int) rand_r(&seed) % (max - min + 1) + min;  // it generates numbers between min and max
    }

    return v;
}

/**
 * It prints vectors of integers.
 * 
 * Input:
 * - int* v: the vector to be printed
 * - int length: the length of the vector
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

// A struct containing the number of threads and rank position is necessary

typedef struct process_data_to_hist_args{
    // The starting and ending index to be considered in the data array
    int start_idx;      
    int end_idx;
    
    // Histogram information
    int* private_his;
    int num_bins;
    int data_lowerb;
    int data_upperb;
}process_data_args;

void* process_data_to_hist(void* args){
    process_data_args vars = *((process_data_args*) args);

    // Allocating data into the private histogram
    for (int i = vars.start_idx; i < vars.end_idx; i++){
        private_his[] += 1
    }
}

int main(int argc, char** argv){
    // Insert input data: nubmer of elements, lower and upper bound of data, and number of histogram bins
    if (argc != 5){
        printf("Wrong number of input characters!\n");
    } else {
        int data_length = atoi(argv[1]);
        int data_lowerb = atoi(argv[2]);        // since vectors are random, we are not guaranteed that these are min
        int data_upperb = atoi(argv[3]);        // and max
        //int num_bins = atoi(argv[4]);
        
        data = random_int_vector(data_length, data_lowerb, data_upperb);
        print_int_vector(data, data_length);
    }

    return 0;
}