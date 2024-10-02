#include <stdio.h>

void swap_and_invert_arrays(int l, char *ptr1, char *ptr2){
    char tmp;
    ptr2 += l - 1;

    for (int i = 0; i < l; i++){
        tmp = *ptr1;
        *ptr1 = *ptr2;
        *ptr2 = tmp;
        ptr1++;
        ptr2--;
    }
}

int main(){
    int length = 5;
    char arr1[length];
    char arr2[length];

    printf("Please fill the first array with 5 chars:\n");
    scanf("%s", arr1);
    printf("Please fill the second array with 5 chars:\n");
    scanf("%s", arr2);

    char *ptr1 = &arr1[0];
    char *ptr2 = &arr2[0];

    swap_and_invert_arrays(length, ptr1, ptr2);

    printf("The 2 new sequences are\n%s\nand\n%s\n", arr1, arr2);

    return 0;
}