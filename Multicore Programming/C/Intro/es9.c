#include <stdio.h>

void insert_number(int l, int seq[l]){
    for (int i = 0; i < l; i++){
        printf("Insert number %d out of %d:\n", i+1, l);
        scanf("%d", &seq[i]);
    }
}

void find_in_array(int x, int l, int seq[l]){
    int counter = 0;

    for (int i = 0; i < l; i++){
        if (seq[i] == '\0'){
            printf("Breaking at position %d\n", i);
            break;
        }

        if (seq[i] == x){
            printf("The number %d is in position %d\n", x, i);
            counter++;
        }
    }

    if (counter == 0){
        printf("Error 404: %d not found in the array\n", x);
    } else {
        printf("The number %d was found %d time(s) in the array\n", x, counter);
    }
}

int main(){
    int x = 5;
    int length = 10;
    int seq[length];

    insert_number(length, seq);
    find_in_array(x, length, seq);
}