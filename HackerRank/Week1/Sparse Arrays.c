#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
int parse_int(char*);

int* matchingStrings(int strings_count, char** strings, int queries_count, char** queries, int* result_count) {
    int* res = malloc(queries_count * sizeof(int));
    for (int i = 0; i < queries_count; i++) {
        int count = 0;
        for (int j = 0; j < strings_count; j++) {
            if (strcmp(queries[i], strings[j]) == 0) {
                count++;
            }
        }
        res[i] = count;
    }
    *result_count = queries_count;
    return res;
}

int main() {
    int strings_count = parse_int(ltrim(rtrim(readline())));
    char** strings = malloc(strings_count * sizeof(char*));
    for (int i = 0; i < strings_count; i++) {
        strings[i] = readline();
    }

    int queries_count = parse_int(ltrim(rtrim(readline())));
    char** queries = malloc(queries_count * sizeof(char*));
    for (int i = 0; i < queries_count; i++) {
        queries[i] = readline();
    }

    int res_count;
    int* res = matchingStrings(strings_count, strings, queries_count, queries, &res_count);

    for (int i = 0; i < res_count; i++) {
        printf("%d\n", res[i]);
    }

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (1) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);
        if (!line) break;

        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;

        alloc_length <<= 1;
        data = realloc(data, alloc_length);
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);
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
