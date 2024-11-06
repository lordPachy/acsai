#include <stdio.h>
#include <stdlib.h>

int int_array_min(int length, int* arr){
    int min = arr[0];
    for (int i = 0; i < length; i++){
        if (min > arr[i]){
            min = arr[i];
        }
    }

    return min;
}

int int_array_max(int length, int* arr){
    int max = arr[0];
    for (int i = 0; i < length; i++){
        if (max < arr[i]){
            max = arr[i];
        }
    }

    return max;
}

int main(){
    int *vec;
    char input[10];
    int i = 0;

    while(1){
        vec = realloc(vec, sizeof(int)*(i+1));
        printf("Please insert a number or 0 to terminate operations:\n");
        scanf(" %s", input);
        vec[i] = atoi(input);
        if (*input == '0'){
            break;
        }
        i++;
    }

    int min = int_array_min(i, vec);
    int max = int_array_max(i, vec);
    printf("The minimum of all the vector elements is %d\n", min);
    printf("The maximum of all the vector elements is %d\n", max);

    return 0;
}