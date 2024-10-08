#include <stdio.h>

void compare_numbers(int *num1, int *num2){
    if (*num1 > *num2){
        printf("%d is bigger than %d\n", *num1, *num2);
    } else if (*num2 > *num1){
        printf("%d is bigger than %d\n", *num2, *num1);
    } else if (*num1 == *num2){
        printf("The two numbers are equal\n");
    }
}

void sum_and_product(int *num1, int *num2){
    printf("Their sum is %d\n", *num1 + *num2);
    printf("Their product is %d\n", *num1 * *num2);
}

int main(){
    int x = 0;
    int y = 0;

    printf("Insert two numbers:");
    scanf("%d", &x);
    scanf("%d", &y);
    compare_numbers(&x, &y);
    sum_and_product(&x, &y);
    return 0;
}