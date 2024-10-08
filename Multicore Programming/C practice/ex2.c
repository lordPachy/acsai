#include <stdio.h>

int main(){
    int a = 0;

    printf("Insert a number:\n");
    scanf("%d", &a);

    printf("The numbers from 0 up to %d are:\n", a);
    for (int i = 0; i <=a; i++){
        printf("%d\n", i);
    }

    printf("The numbers from %d up to 0 are:\n", a);
    for (int i = a; i >=0; i--){
        printf("%d\n", i);
    }

}