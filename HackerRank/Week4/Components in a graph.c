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
 * Complete the 'componentsInGraph' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts 2D_INTEGER_ARRAY gb as parameter.
 *
 * It must return an array of two ints: {min_size, max_size}
 * where sizes are taken over components that have more than 1 node.
 */

int find_root(int *parent, int x) {
    if (parent[x] != x) parent[x] = find_root(parent, parent[x]);
    return parent[x];
}

void union_sets(int *parent, int *size, int a, int b) {
    int ra = find_root(parent, a);
    int rb = find_root(parent, b);
    if (ra == rb) return;
    if (size[ra] < size[rb]) {
        parent[ra] = rb;
        size[rb] += size[ra];
    } else {
        parent[rb] = ra;
        size[ra] += size[rb];
    }
}

int* componentsInGraph(int gb_rows, int gb_columns, int** gb, int* result_count) {
    int max_node = 0;
    for (int i = 0; i < gb_rows; ++i) {
        if (gb[i][0] > max_node) max_node = gb[i][0];
        if (gb[i][1] > max_node) max_node = gb[i][1];
    }

  
    int *parent = malloc((max_node + 1) * sizeof(int));
    int *size = malloc((max_node + 1) * sizeof(int));
    if (!parent || !size) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= max_node; ++i) {
        parent[i] = i;
        size[i] = 1;
    }

    
    for (int i = 0; i < gb_rows; ++i) {
        int a = gb[i][0];
        int b = gb[i][1];
        union_sets(parent, size, a, b);
    }


    int min_sz = INT_MAX;
    int max_sz = 0;


    for (int i = 1; i <= max_node; ++i) {
        if (parent[i] == i) { // i is a root
            if (size[i] > 1) {
                if (size[i] < min_sz) min_sz = size[i];
                if (size[i] > max_sz) max_sz = size[i];
            }
        }
    }


    if (min_sz == INT_MAX) min_sz = 0;

    int *result = malloc(2 * sizeof(int));
    if (!result) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    result[0] = min_sz;
    result[1] = max_sz;
    *result_count = 2;

    free(parent);
    free(size);

    return result;
}

int main() {
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");
    int n = parse_int(ltrim(rtrim(readline())));
    int** gb = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        *(gb + i) = malloc(2 * (sizeof(int)));
        char** gb_item_temp = split_string(rtrim(readline()));
        for (int j = 0; j < 2; j++) {
            int gb_item = parse_int(*(gb_item_temp + j));
            *(*(gb + i) + j) = gb_item;
        }
    }
    int result_count;
    int* result = componentsInGraph(n, 2, gb, &result_count);
    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", *(result + i));
        if (i != result_count - 1) {
            fprintf(fptr, " ");
        }
    }
    fprintf(fptr, "\n");
    fclose(fptr);


    for (int i = 0; i < n; ++i) free(gb[i]);
    free(gb);
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
