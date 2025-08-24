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
 * Complete the 'bfs' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. INTEGER m
 *  3. 2D_INTEGER_ARRAY edges
 *  4. INTEGER s
 */
int* bfs(int n, int m, int edges_rows, int edges_columns, int** edges, int s, int* result_count) {
   
    int **adj = malloc((n + 1) * sizeof(int*));
    int *adj_size = calloc(n + 1, sizeof(int));
    int *adj_cap  = calloc(n + 1, sizeof(int));

    if (!adj || !adj_size || !adj_cap) {
        // allocation failed
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= n; ++i) {
        adj[i] = NULL;
        adj_size[i] = 0;
        adj_cap[i] = 0;
    }

  
    for (int i = 0; i < edges_rows; ++i) {
        int u = edges[i][0];
        int v = edges[i][1];

        if (u >= 1 && u <= n && v >= 1 && v <= n) {
          
            if (adj_cap[u] == 0) {
                adj_cap[u] = 2;
                adj[u] = malloc(adj_cap[u] * sizeof(int));
                if (!adj[u]) exit(EXIT_FAILURE);
            } else if (adj_size[u] >= adj_cap[u]) {
                adj_cap[u] *= 2;
                adj[u] = realloc(adj[u], adj_cap[u] * sizeof(int));
                if (!adj[u]) exit(EXIT_FAILURE);
            }
            adj[u][adj_size[u]++] = v;

           
            if (adj_cap[v] == 0) {
                adj_cap[v] = 2;
                adj[v] = malloc(adj_cap[v] * sizeof(int));
                if (!adj[v]) exit(EXIT_FAILURE);
            } else if (adj_size[v] >= adj_cap[v]) {
                adj_cap[v] *= 2;
                adj[v] = realloc(adj[v], adj_cap[v] * sizeof(int));
                if (!adj[v]) exit(EXIT_FAILURE);
            }
            adj[v][adj_size[v]++] = u;
        }
    }


    int *dist = malloc((n + 1) * sizeof(int));
    if (!dist) exit(EXIT_FAILURE);
    for (int i = 1; i <= n; ++i) dist[i] = -1;


    int *queue = malloc((n + 5) * sizeof(int));
    if (!queue) exit(EXIT_FAILURE);
    int head = 0, tail = 0;


    dist[s] = 0;
    queue[tail++] = s;

    while (head < tail) {
        int cur = queue[head++];
        for (int k = 0; k < adj_size[cur]; ++k) {
            int nb = adj[cur][k];
            if (dist[nb] == -1) {
                dist[nb] = dist[cur] + 6; 
                queue[tail++] = nb;
            }
        }
    }

  
    *result_count = n - 1;
    int *result = malloc((*result_count) * sizeof(int));
    if (!result) exit(EXIT_FAILURE);

    int pos = 0;
    for (int i = 1; i <= n; ++i) {
        if (i == s) continue;
        result[pos++] = dist[i];
    }


    for (int i = 0; i <= n; ++i) free(adj[i]);
    free(adj);
    free(adj_size);
    free(adj_cap);
    free(dist);
    free(queue);

    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int q = parse_int(ltrim(rtrim(readline())));

    for (int q_itr = 0; q_itr < q; q_itr++) {
        char** first_multiple_input = split_string(rtrim(readline()));

        int n = parse_int(*(first_multiple_input + 0));
        int m = parse_int(*(first_multiple_input + 1));

        int** edges = malloc(m * sizeof(int*));
        if (!edges) exit(EXIT_FAILURE);

        for (int i = 0; i < m; i++) {
            *(edges + i) = malloc(2 * (sizeof(int)));
            char** edges_item_temp = split_string(rtrim(readline()));
            for (int j = 0; j < 2; j++) {
                int edges_item = parse_int(*(edges_item_temp + j));
                *(*(edges + i) + j) = edges_item;
            }
        }

        int s = parse_int(ltrim(rtrim(readline())));

        int result_count;
        int* result = bfs(n, m, m, 2, edges, s, &result_count);

        for (int i = 0; i < result_count; i++) {
            fprintf(fptr, "%d", *(result + i));
            if (i != result_count - 1) {
                fprintf(fptr, " ");
            }
        }
        fprintf(fptr, "\n");

        for (int i = 0; i < m; ++i) free(edges[i]);
        free(edges);
        free(result);
    }

    fclose(fptr);

    return 0;
}
char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);
    if (!data) exit(EXIT_FAILURE);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, (int)(alloc_length - data_length), stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;
        data = realloc(data, alloc_length);
        if (!data) exit(EXIT_FAILURE);
    }

    if (data_length > 0 && data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
        if (!data) exit(EXIT_FAILURE);
    } else {
        data = realloc(data, data_length + 1);
        if (!data) exit(EXIT_FAILURE);
        data[data_length] = '\0';
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) return NULL;
    while (*str != '\0' && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return NULL;
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");
    int spaces = 0;
    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);
        if (!splits) return splits;
        splits[spaces - 1] = token;
        token = strtok(NULL, " ");
    }
    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = (int) strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') exit(EXIT_FAILURE);
    return value;
}
