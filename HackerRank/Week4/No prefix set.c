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

/* Trie node definition */
typedef struct TrieNode {
    struct TrieNode* child[26];
    int isEnd;
} TrieNode;

static TrieNode* newTrieNode(void) {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isEnd = 0;
    for (int i = 0; i < 26; ++i) node->child[i] = NULL;
    return node;
}

void noPrefix(int words_count, char** words) {
    TrieNode* root = newTrieNode();
    for (int i = 0; i < words_count; ++i) {
        char* w = words[i];
        TrieNode* cur = root;
        int conflict = 0;
        for (int j = 0; w[j] != '\0'; ++j) {
            int idx = w[j] - 'a';
            if (idx < 0 || idx >= 26) idx = 0;

            if (!cur->child[idx]) {
                cur->child[idx] = newTrieNode();
            }
            cur = cur->child[idx];

            if (cur->isEnd && w[j+1] != '\0') {
                conflict = 1;
                break;
            }
        }
       
        if (conflict || cur->isEnd) {
            printf("BAD SET\n%s\n", w);
            return;
        }

        
        for (int k = 0; k < 26; ++k) {
            if (cur->child[k]) {
                printf("BAD SET\n%s\n", w);
                return;
            }
        }
        cur->isEnd = 1;
    }
    printf("GOOD SET\n");
}

int main() {
    int n = parse_int(ltrim(rtrim(readline())));
    char** words = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        words[i] = readline();
    }
    noPrefix(n, words);
    return 0;
}

char* readline() {
    size_t alloc_length = 1024, data_length = 0;
    char* data = malloc(alloc_length);
    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);
        if (!line) break;
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;
        alloc_length <<= 1;
        data = realloc(data, alloc_length);
    }
    if (data[data_length - 1] == '\n') data[data_length - 1] = '\0';
    return data;
}
char* ltrim(char* str) { while (*str && isspace(*str)) str++; return str; }
char* rtrim(char* str) { char* end = str + strlen(str) - 1; while (end >= str && isspace(*end)) *end-- = '\0'; return str; }
int parse_int(char* str) { char* endptr; return strtol(str, &endptr, 10); }
