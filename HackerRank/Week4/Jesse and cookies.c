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
char* ltrim(char* str);
char* rtrim(char* str);
char** split_string(char* str);
int parse_int(char* str);

typedef struct {
    int *data;
    int size;
    int capacity;
} MinHeap;

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

MinHeap* createHeap(int capacity) {
    MinHeap* h = malloc(sizeof(MinHeap));
    h->data = malloc(sizeof(int) * (capacity + 1));
    h->size = 0;
    h->capacity = capacity;
    return h;
}

void heapifyUp(MinHeap* h, int idx) {
    while (idx > 1 && h->data[idx] < h->data[idx/2]) {
        swap(&h->data[idx], &h->data[idx/2]);
        idx /= 2;
    }
}

void heapifyDown(MinHeap* h, int idx) {
    int smallest = idx;
    int left = idx * 2;
    int right = idx * 2 + 1;

    if (left <= h->size && h->data[left] < h->data[smallest]) smallest = left;
    if (right <= h->size && h->data[right] < h->data[smallest]) smallest = right;

    if (smallest != idx) {
        swap(&h->data[idx], &h->data[smallest]);
        heapifyDown(h, smallest);
    }
}

void push(MinHeap* h, int val) {
    h->data[++h->size] = val;
    heapifyUp(h, h->size);
}

int pop(MinHeap* h) {
    if (h->size == 0) return -1;
    int root = h->data[1];
    h->data[1] = h->data[h->size--];
    heapifyDown(h, 1);
    return root;
}

int top(MinHeap* h) {
    if (h->size == 0) return -1;
    return h->data[1];
}

int cookies(int k, int A_count, int* A) {
    MinHeap* h = createHeap(A_count);
    for (int i = 0; i < A_count; i++) {
        push(h, A[i]);
    }

    int operations = 0;
    while (h->size > 1 && top(h) < k) {
        int least = pop(h);
        int second = pop(h);
        long long newSweet = (long long)least + 2LL * second;
        push(h, (int)newSweet);
        operations++;
    }

    if (top(h) >= k) {
        return operations;
    }
    return -1;
}

int main() {
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));
    int n = parse_int(*(first_multiple_input + 0));
    int k = parse_int(*(first_multiple_input + 1));

    char** A_temp = split_string(rtrim(readline()));
    int* A = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        int A_item = parse_int(*(A_temp + i));
        *(A + i) = A_item;
    }

    int result = cookies(k, n, A);
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
        else data[data_length] = '\0';
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
    int value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') exit(EXIT_FAILURE);
    return value;
}
