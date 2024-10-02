#include <stdio.h>

void insert_number(int l, int seq[l]){
    for (int i = 0; i < l; i++){
        printf("Insert number %d out of %d:\n", i+1, l);
        scanf("%d", &seq[i]);
    }
}

void find_in_array(int x, int l, int seq[l]){
    for (int i = 0; i < l; i++){
        if (seq[i] == '\0'){
            break;
        }

        if (seq[i] == x){
            printf("The number %d is in position %d\n", x, i);
            return;
        }
    }

    printf("Error 404: %d not found in the array", x);
}

int main(){
    int x = 5;
    int length = 10;
    int seq[length];

    insert_number(length, seq);
    find_in_array(x, length, seq);
}