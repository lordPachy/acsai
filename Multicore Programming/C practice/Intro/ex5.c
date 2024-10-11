#include <stdio.h>

void sub_num_with_ast(int l, char str[l]){
    for (int i = 0; i < l; i++){
        if (str[i] >= "0" && str[i] <= "9"){
            str[i] = "*";
        }
    }
}

int main(){
    int length = 20;
    char seq[length];

    printf("Please insert the string:\n");
    scanf("%s", seq);

    sub_num_with_ast(length, seq);

    printf("The final string is %s\n", seq);
}