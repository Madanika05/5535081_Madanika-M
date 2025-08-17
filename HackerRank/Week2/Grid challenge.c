#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to compare two characters (for qsort)
int cmp(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}

// Function to check grid challenge
char* gridChallenge(int n, char grid[n][101]) {
    // Step 1: Sort each row
    for (int i = 0; i < n; i++) {
        qsort(grid[i], strlen(grid[i]), sizeof(char), cmp);
    }

    // Step 2: Verify column order
    for (int col = 0; col < n; col++) {
        for (int row = 1; row < n; row++) {
            if (grid[row][col] < grid[row - 1][col]) {
                return "NO";
            }
        }
    }
    return "YES";
}

int main() {
    int t;
    scanf("%d", &t);
    
    while (t--) {
        int n;
        scanf("%d", &n);
        
        char grid[n][101];  // each string max length = 100
        for (int i = 0; i < n; i++) {
            scanf("%s", grid[i]);
        }
        
        printf("%s\n", gridChallenge(n, grid));
    }
    
    return 0;
}
