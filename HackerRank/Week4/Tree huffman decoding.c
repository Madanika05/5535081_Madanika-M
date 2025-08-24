#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char data;
    unsigned freq;
    struct Node *left, *right;
} Node;

typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    Node **array;
} MinHeap;

Node* newNode(char data, unsigned freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (Node**)malloc(capacity * sizeof(Node*));
    return minHeap;
}

void swapNode(Node **a, Node **b) {
    Node *t = *a; *a = *b; *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2*idx + 1;
    int right = 2*idx + 2;
    if (left < (int)minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    if (right < (int)minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;
    if (smallest != idx) {
        swapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

Node* extractMin(MinHeap* minHeap) {
    Node* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap* minHeap, Node* node) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

MinHeap* buildAndCreateMinHeap(unsigned freq[]) {
    unsigned capacity = 0;
    for (int i = 0; i < 256; ++i) if (freq[i]) ++capacity;
    if (capacity == 0) return createMinHeap(1);
    MinHeap* minHeap = createMinHeap(capacity);
    for (int i = 0; i < 256; ++i) {
        if (freq[i]) minHeap->array[minHeap->size++] = newNode((char)i, freq[i]);
    }
    for (int i = (minHeap->size - 1)/2; i >= 0; --i) minHeapify(minHeap, i);
    return minHeap;
}

Node* buildHuffmanTree(unsigned freq[]) {
    MinHeap* minHeap = buildAndCreateMinHeap(freq);
    if (minHeap->size == 0) {
        free(minHeap->array);
        free(minHeap);
        return NULL;
    }
    while (minHeap->size != 1) {
        Node* left = extractMin(minHeap);
        Node* right = extractMin(minHeap);
        Node* top = newNode('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    Node* root = extractMin(minHeap);
    free(minHeap->array);
    free(minHeap);
    return root;
}

void generateCodes(Node* root, char *arr, int top, char *codes[256]) {
    if (!root) return;
    if (!root->left && !root->right) {
        if (top == 0) arr[top++] = '0';
        arr[top] = '\0';
        codes[(unsigned char)root->data] = strdup(arr);
        return;
    }
    arr[top] = '0';
    generateCodes(root->left, arr, top + 1, codes);
    arr[top] = '1';
    generateCodes(root->right, arr, top + 1, codes);
}

void decodeHuff(Node* root, char* s) {
    if (!root) return;
    Node* cur = root;
    for (char *p = s; *p != '\0'; ++p) {
        if (*p == '0') cur = cur->left;
        else cur = cur->right;
        if (!cur->left && !cur->right) {
            putchar(cur->data);
            cur = root;
        }
    }
    putchar('\n');
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void) {
    char buffer[10005];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[--len] = '\0';
    if (len == 0) {
        printf("\n");
        return 0;
    }
    unsigned freq[256] = {0};
    for (size_t i = 0; i < len; ++i) freq[(unsigned char)buffer[i]]++;
    Node* root = buildHuffmanTree(freq);
    char *codes[256] = {0};
    char arr[1024];
    generateCodes(root, arr, 0, codes);
    size_t encoded_len = 0;
    for (size_t i = 0; i < len; ++i) {
        encoded_len += strlen(codes[(unsigned char)buffer[i]]);
    }
    char *encoded = malloc(encoded_len + 1);
    encoded[0] = '\0';
    for (size_t i = 0; i < len; ++i) strcat(encoded, codes[(unsigned char)buffer[i]]);
    decodeHuff(root, encoded);
    free(encoded);
    for (int i = 0; i < 256; ++i) if (codes[i]) free(codes[i]);
    freeTree(root);
    return 0;
}
