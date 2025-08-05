#include <stdio.h>

int main() {
    long arr[5];
    long total = 0;
    long min = __LONG_MAX__;
    long max = 0;

    
    for (int i = 0; i < 5; i++) {
        scanf("%ld", &arr[i]);
        total += arr[i];

        if (arr[i] < min)
            min = arr[i];
        if (arr[i] > max)
            max = arr[i];
    }

    long minSum = total - max;
    long maxSum = total - min;

    printf("%ld %ld\n", minSum, maxSum);

    return 0;
}
