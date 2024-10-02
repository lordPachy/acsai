#include <stdio.h>

int compare_array(int A[], int n, int B[], int m){
    int present = 0;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (A[i] == B[j]){
                printf("%d is equal to %d\n", A[i], B[j]);
                present++;
                break;
            }
        }
        if (present <= i){
            return 0;
        }
    }

    return 1;
        
}

int main(){
    int length1 = 5;
    int length2 = 4;
    int arr1[] = {4, 1, 3, 2, 1};
    int arr2[] = {5, 1, 3, 2};

    int answer = compare_array(arr1, length1, arr2, length2);
    printf("%d\n", answer);

    return 0;
}