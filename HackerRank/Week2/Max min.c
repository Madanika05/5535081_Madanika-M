#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
int parse_int(char*);

// comparator for qsort
int cmpfunc(const void *p1, const void *p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/*
 * Complete the 'maxMin' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER k
 *  2. INTEGER_ARRAY arr
 */

int maxMin(int k, int arr_count, int* arr) {
    // Sort numbers
    qsort(arr, arr_count, sizeof(int), cmpfunc);

    int best_gap = INT_MAX;

    // Slide a window of size k and track smallest unfairness
    for (int start = 0; start + k - 1 < arr_count; start++) {
        int gap = arr[start + k - 1] - arr[start];
        if (gap < best_gap) {
            best_gap = gap;
        }
    }
    return best_gap;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int n = parse_int(ltrim(rtrim(readline())));
    int k = parse_int(ltrim(rtrim(readline())));

    int* arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        int arr_item = parse_int(ltrim(rtrim(readline())));
        arr[i] = arr_item;
    }

    int result = maxMin(k, n, arr);

    fprintf(fptr, "%d\n", result);

    fclose(fptr);
    free(arr);
    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) break;
        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;

        alloc_length <<= 1;
        data = realloc(data, alloc_length);
        if (!data) return '\0';
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
        if (!data) return '\0';
    } else {
        data = realloc(data, data_length + 1);
        if (!data) return '\0';
        data[data_length] = '\0';
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) return '\0';
    if (!*str) return str;
    while (*str != '\0' && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return '\0';
    if (!*str) return str;
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') exit(EXIT_FAILURE);
    return value;
}
