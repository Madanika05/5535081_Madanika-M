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
char** split_string(char*);

int parse_int(char*);

/* comparator for qsort if needed (not used here) */
static int cmp_int(const void *a, const void *b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

int* solve(int arr_count, int* arr, int queries_count, int* queries, int* result_count) {
    int n = arr_count;
    *result_count = queries_count;
    int *res = malloc(sizeof(int) * queries_count);
    if (n == 0) {
        for (int i = 0; i < queries_count; ++i) res[i] = 0;
        return res;
    }

    int *left = malloc(n * sizeof(int));
    int *right = malloc(n * sizeof(int));
    int *st = malloc(n * sizeof(int));
    int top = 0;

    /* previous greater element (strict >) */
    top = 0;
    for (int i = 0; i < n; ++i) {
        while (top > 0 && arr[st[top-1]] <= arr[i]) top--;
        left[i] = (top == 0) ? -1 : st[top-1];
        st[top++] = i;
    }

    /* next greater or equal (>=) */
    top = 0;
    for (int i = n-1; i >= 0; --i) {
        while (top > 0 && arr[st[top-1]] < arr[i]) top--;
        right[i] = (top == 0) ? n : st[top-1];
        st[top++] = i;
    }

    free(st);

    int *best = malloc((n + 2) * sizeof(int));
    for (int i = 0; i <= n+1; ++i) best[i] = INT_MAX;

    for (int i = 0; i < n; ++i) {
        int span = right[i] - left[i] - 1;
        if (arr[i] < best[span]) best[span] = arr[i];
    }

    for (int k = n-1; k >= 1; --k) {
        if (best[k+1] < best[k]) best[k] = best[k+1];
    }

    for (int qi = 0; qi < queries_count; ++qi) {
        int k = queries[qi];
        if (k >= 1 && k <= n) {
            if (best[k] == INT_MAX) res[qi] = 0;
            else res[qi] = best[k];
        } else res[qi] = 0;
    }

    free(left);
    free(right);
    free(best);
    return res;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int q = parse_int(*(first_multiple_input + 1));

    char** arr_temp = split_string(rtrim(readline()));

    int* arr = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int arr_item = parse_int(*(arr_temp + i));

        *(arr + i) = arr_item;
    }

    int* queries = malloc(q * sizeof(int));

    for (int i = 0; i < q; i++) {
        int queries_item = parse_int(ltrim(rtrim(readline())));

        *(queries + i) = queries_item;
    }

    int result_count;
    int* result = solve(n, arr, q, queries, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", *(result + i));

        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");

    fclose(fptr);

    free(arr);
    free(queries);
    free(result);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
