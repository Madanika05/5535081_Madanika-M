#include <stdio.h>
#include <stdlib.h>


int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Find median from a sorted array
int findMedian(int arr_count, int* arr) {
    qsort(arr, arr_count, sizeof(int), compare); 
    return arr[arr_count / 2]; // Return middle element
}

int main() {
    int n;
    scanf("%d", &n); 

    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]); 
    }

    int result = findMedian(n, arr);
    printf("%d\n", result); 

    free(arr); 
    return 0;
}