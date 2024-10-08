#include <stdio.h>

void sub_num_with_ast(int l, char string[l]){
    for (int i = 0; i < l; i++){
        if (string[i] >= '0' && string[i] <= '9'){
            string[i] = '*';
        }
    }
}

int main(){
    int length = 20;
    char seq[length];

    printf("Insert a string:\n");
    scanf("%s", seq);
    sub_num_with_ast(length, seq);
    printf("Your new string is %s\n", seq);

    return 0;
}