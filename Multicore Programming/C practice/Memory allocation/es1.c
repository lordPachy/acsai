#include <stdio.h>
#include <stdlib.h>

int int_array_sum(int length, int* arr){
    int total = 0;
    for (int i = 0; i < length; i++){
        total += arr[i];
    }

    return total;
}

int main(){
    int *vec;
    char input;
    int i = 0;

    while(1){
        vec = realloc(vec, sizeof(int)*(i+1));
        printf("Please insert a number or 0 to terminate operations:\n");
        scanf(" %c", &input);
        vec[i] = atoi(&input);
        if (input == '0'){
            break;
        }
        i++;
    }

    int total = int_array_sum(i, vec);
    printf("The sum of all the vector elements is %d\n", total);

    return 0;
}