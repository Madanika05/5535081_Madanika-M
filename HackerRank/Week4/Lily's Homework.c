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

/*
 * Complete the 'lilysHomework' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_ARRAY arr as parameter.
 */

int lilysHomework(int arr_count, int* arr) {
    if (arr_count <= 1) return 0;

    typedef struct { int val; int idx; } Pair;
    Pair *pairs = malloc(arr_count * sizeof(Pair));
    if (!pairs) return 0;

    for (int i = 0; i < arr_count; ++i) {
        pairs[i].val = arr[i];
        pairs[i].idx = i;
    }

    int cmp_asc(const void *a, const void *b) {
        const Pair *pa = a;
        const Pair *pb = b;
        if (pa->val < pb->val) return -1;
        if (pa->val > pb->val) return 1;
        return 0;
    }

    qsort(pairs, arr_count, sizeof(Pair), cmp_asc);

    int *to = malloc(arr_count * sizeof(int));
    int *visited = malloc(arr_count * sizeof(int));
    if (!to || !visited) {
        free(pairs);
        free(to);
        free(visited);
        return 0;
    }

    for (int i = 0; i < arr_count; ++i) {
        to[pairs[i].idx] = i;
        visited[i] = 0;
    }

    long long swaps_asc = 0;
    for (int i = 0; i < arr_count; ++i) {
        if (visited[i]) continue;
        int cycle_size = 0;
        int j = i;
        while (!visited[j]) {
            visited[j] = 1;
            j = to[j];
            ++cycle_size;
        }
        if (cycle_size > 1) swaps_asc += (cycle_size - 1);
    }

    /* prepare for descending: reverse the sorted pairs */
    for (int i = 0; i < arr_count; ++i) visited[i] = 0;
    for (int i = 0; i < arr_count; ++i) {
        to[pairs[i].idx] = (arr_count - 1 - i);
    }

    long long swaps_desc = 0;
    for (int i = 0; i < arr_count; ++i) {
        if (visited[i]) continue;
        int cycle_size = 0;
        int j = i;
        while (!visited[j]) {
            visited[j] = 1;
            j = to[j];
            ++cycle_size;
        }
        if (cycle_size > 1) swaps_desc += (cycle_size - 1);
    }

    free(pairs);
    free(to);
    free(visited);

    return (int)((swaps_asc < swaps_desc) ? swaps_asc : swaps_desc);
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int n = parse_int(ltrim(rtrim(readline())));

    char** arr_temp = split_string(rtrim(readline()));

    int* arr = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int arr_item = parse_int(*(arr_temp + i));

        *(arr + i) = arr_item;
    }

    int result = lilysHomework(n, arr);

    fprintf(fptr, "%d\n", result);

    fclose(fptr);

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
