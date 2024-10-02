#include <stdio.h>

void sub_num_with_ast(int length, char *ptr){
    for (int i = 0; i < length; i++){
        if (*ptr == '\0'){
            printf("Breaking condition reached\n");
            break;
        }

        if (*ptr >= '0' && *ptr <= '9'){
            *ptr = '*';
        }
        ptr++;
    }
}

int main(){
    int length = 10;
    char seq[length];

    printf("Insert a string shorter than 11 chars:\n");
    scanf("%s", seq);

    char *p = &seq[0];
    sub_num_with_ast(length, p);
    printf("The new string is %s\n", seq);

    return 0;
}