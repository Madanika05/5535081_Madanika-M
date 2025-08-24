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
 * Complete the 'cutTheTree' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY data
 *  2. 2D_INTEGER_ARRAY edges
 */

int cutTheTree(int data_count, int* data, int edges_rows, int edges_columns, int** edges) {
    int n = data_count;
    if (n <= 1) return 0;
    int m = edges_rows; // should be n-1
    int *u = malloc(m * sizeof(int));
    int *v = malloc(m * sizeof(int));
    if (!u || !v) exit(EXIT_FAILURE);
    for (int i = 0; i < m; ++i) {
        u[i] = edges[i][0];
        v[i] = edges[i][1];
    }

    // degree count
    int *deg = calloc(n + 1, sizeof(int));
    if (!deg) exit(EXIT_FAILURE);
    for (int i = 0; i < m; ++i) {
        ++deg[u[i]];
        ++deg[v[i]];
    }

    int totalAdj = 2 * m;
    int *start = malloc((n + 2) * sizeof(int)); 
    if (!start) exit(EXIT_FAILURE);
    start[1] = 0;
    for (int i = 2; i <= n; ++i) start[i] = start[i - 1] + deg[i - 1];
    start[n + 1] = totalAdj;

    int *cur = malloc((n + 1) * sizeof(int));
    if (!cur) exit(EXIT_FAILURE);
    for (int i = 1; i <= n; ++i) cur[i] = start[i];

    int *adj = malloc(totalAdj * sizeof(int));
    if (!adj) exit(EXIT_FAILURE);

    for (int i = 0; i < m; ++i) {
        int a = u[i];
        int b = v[i];
        adj[cur[a]++] = b;
        adj[cur[b]++] = a;
    }

    long long total = 0;
    for (int i = 0; i < n; ++i) total += data[i];

    int *stack = malloc(n * sizeof(int));
    int top = 0;
    int *parent = malloc((n + 1) * sizeof(int));
    int *order = malloc(n * sizeof(int));
    if (!stack || !parent || !order) exit(EXIT_FAILURE);

    for (int i = 1; i <= n; ++i) parent[i] = 0;
    stack[top++] = 1;
    parent[1] = -1; // mark root's parent

    int ordIdx = 0;
    while (top > 0) {
        int node = stack[--top];
        order[ordIdx++] = node;
        int s = start[node], e = (node == n ? totalAdj : start[node + 1]);
        for (int i = s; i < e; ++i) {
            int nei = adj[i];
            if (parent[nei] == 0) { // unvisited
                parent[nei] = node;
                stack[top++] = nei;
            }
        }
    }

   
    long long *sub = malloc((n + 1) * sizeof(long long));
    if (!sub) exit(EXIT_FAILURE);
    for (int i = 1; i <= n; ++i) sub[i] = 0;

    
    for (int i = 1; i <= n; ++i) sub[i] = 0;
    for (int i = 0; i < n; ++i) {
        int node = i + 1; // data[] is 0-based, nodes 1..n
        sub[node] = data[i];
    }


    for (int i = ordIdx - 1; i >= 0; --i) {
        int node = order[i];
        int p = parent[node];
        if (p > 0) {
            sub[p] += sub[node];
        }
    }

    // find minimal difference when cutting an edge (i.e., removing the subtree of some node != root)
    long long best = LLONG_MAX;
    for (int i = 2; i <= n; ++i) { // nodes are 1..n; root is 1
        long long part = sub[i];
        long long diff = llabs(total - 2 * part);
        if (diff < best) best = diff;
    }

    
    free(u); free(v);
    free(deg); free(start); free(cur); free(adj);
    free(stack); free(parent); free(order); free(sub);

    return (int)best;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int n = parse_int(ltrim(rtrim(readline())));

    char** data_temp = split_string(rtrim(readline()));

    int* data = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int data_item = parse_int(*(data_temp + i));

        *(data + i) = data_item;
    }

    int** edges = malloc((n - 1) * sizeof(int*));

    for (int i = 0; i < n - 1; i++) {
        *(edges + i) = malloc(2 * (sizeof(int)));

        char** edges_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < 2; j++) {
            int edges_item = parse_int(*(edges_item_temp + j));

            *(*(edges + i) + j) = edges_item;
        }
    }

    int result = cutTheTree(n, data, n - 1, 2, edges);

    fprintf(fptr, "%d\n", result);

    fclose(fptr);

    
    for (int i = 0; i < n - 1; ++i) free(edges[i]);
    free(edges);
    free(data);

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
