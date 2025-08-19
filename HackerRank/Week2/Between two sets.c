#include <stdio.h>

int dividesAll(int x, int arr[], int n) {
    for (int i = 0; i < n; i++) {
        if (x % arr[i] != 0) return 0;
    }
    return 1;
}

int isFactorForAll(int x, int arr[], int m) {
    for (int j = 0; j < m; j++) {
        if (arr[j] % x != 0) return 0;
    }
    return 1;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int A[60], B[60];
    for (int i = 0; i < n; i++) scanf("%d", &A[i]);
    for (int j = 0; j < m; j++) scanf("%d", &B[j]);

    // find search bounds
    int maxA = A[0], minB = B[0];
    for (int i = 1; i < n; i++) if (A[i] > maxA) maxA = A[i];
    for (int j = 1; j < m; j++) if (B[j] < minB) minB = B[j];

    int result = 0;
    for (int x = maxA; x <= minB; x++) {
        if (dividesAll(x, A, n) && isFactorForAll(x, B, m)) {
            result++;
        }
    }

    printf("%d\n", result);
    return 0;
}
