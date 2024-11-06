//LIBRARIES

#include <stdlib.h>
#include <time.h>
#include <stdio.h>



//STRUCTS AND CUSTOM TYPES

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



//GLOBAL VARIABLES

// It is useful to declare data and (the final) histogram as global variables
int* data;
int* histogram;



//FUNCTIONS

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

void* process_data_to_hist(void* args){
    process_data_args vars = *((process_data_args*) args);

    // Allocating data into the histogram
    int index;
    for (int i = vars.start_idx; i < vars.end_idx; i++){
        // We can avoid using a for loop for the bin calculation
        index = (int) ((vars.num_bins * (data[i] - vars.data_lowerb)) / (vars.data_upperb - vars.data_lowerb));
        if (index == vars.num_bins){
            index -= 1;     // if the datapoint is exactly equal to the maximum, it should still fall in the last bin; this happens because bins are of type [a, b)
        }
        vars.private_his[index] += 1;
    }

    return NULL;
}



//MAIN

int main(int argc, char** argv){
    // Measuring time of execution
    clock_t start_t = clock();

    // Insert input data: nubmer of elements, lower and upper bound of data, number of histogram bins
    if (argc != 5){
        printf("Wrong number of input characters!\n");
    } else {
        int data_length = atoi(argv[1]);
        int data_lowerb = atoi(argv[2]);        // since vectors are random, we are not guaranteed that these are min
        int data_upperb = atoi(argv[3]);        // and max
        int num_bins = atoi(argv[4]);

        // Initializing global variables; the lock, the histogram and random data
        histogram = (int*) malloc(sizeof(int) * num_bins);
        data = random_int_vector(data_length, data_lowerb, data_upperb);
        //print_int_vector(data, data_length);
        
        // Initializing the function arguments
        process_data_args args = (process_data_args) {0, data_length, histogram, num_bins, data_lowerb, data_upperb};

        // Calculating the histogram
        process_data_to_hist((void*) &args);

        // Measuring time of execution
        clock_t end_t = clock();

        // Printing the final result and time of execution
        print_int_vector(histogram, num_bins);
        printf("The time of execution was %f seconds\n", ((double) end_t - start_t) / CLOCKS_PER_SEC);      // This is the way of calculating elapsed time with processor clock
    }

    return 0;
}