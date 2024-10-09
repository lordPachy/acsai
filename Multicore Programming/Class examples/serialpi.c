#include <stdio.h>
#include <stdlib.h>

int main(){
    int total;
    int my_samples = 1000000;

    int inside = 0;

    for (int i = 0; i < my_samples; i++){
        double x = 2 * (double)rand() / RAND_MAX - 1;
        double y = 2 * (double)rand() / RAND_MAX - 1;
        double l2 = x * x + y * y;
        //printf("l2 is %lf\n", l2);
        //printf("x is %lf\n", x);
        if (l2 <= 1.0){
            inside++;
        }
    }

    total = inside;
    double pi = 4.0 * (double)total / (double)my_samples; 
    printf("Pi is %f", pi);

    return 0;
}