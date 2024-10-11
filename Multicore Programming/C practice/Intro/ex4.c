#include <stdio.h>

int main(){
    int length = 5;
    int vec[length];
    printf("You are going to write 5 numbers:\n");

    for(int i = 0; i < length; i++){
        printf("Insert number %d: \n", i);
        scanf("%d", &vec[i]);
    }

    int max_vec = max(length, vec);
    int min_vec = min(length, vec);
    int sum_vec = sum(length, vec);

    printf("The biggest number is %d\n", max_vec);
    printf("The smallest number is %d\n", min_vec);
    printf("The sum of all array's elements is %d", sum_vec);
}

int min(int l, int arr[l]){
    int min = arr[0];
    for (int i = 1; i < l; i++){
        if (arr[i]  < min){
            min = arr[i];
        }
    }

    return min;
}

int max(int l, int arr[l]){
    int max = arr[0];
    for (int i = 1; i < l; i++){
        if (arr[i] > max){
            max = arr[i];
        }
    }

    return max;
}

int sum(int l, int arr[l]){
    int sum = 0;
    for (int i = 0; i < l; i++){
        sum += arr[i];
    }

    return sum;
}