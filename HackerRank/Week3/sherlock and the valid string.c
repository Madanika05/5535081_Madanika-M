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

char* isValid(char* s) {
    static char yes[] = "YES";
    static char no[] = "NO";
    int freq[26] = {0};
    int n = strlen(s);

    for(int i = 0; i < n; i++) {
        freq[s[i]-'a']++;
    }

    int values[26], count = 0;
    for(int i = 0; i < 26; i++) {
        if(freq[i] > 0) values[count++] = freq[i];
    }

    int unique[26], freqCount[26], uCount = 0;
    for(int i = 0; i < count; i++) {
        int found = 0;
        for(int j = 0; j < uCount; j++) {
            if(unique[j] == values[i]) {
                freqCount[j]++;
                found = 1;
                break;
            }
        }
        if(!found) {
            unique[uCount] = values[i];
            freqCount[uCount] = 1;
            uCount++;
        }
    }

    if(uCount == 1) return yes;
    if(uCount == 2) {
        if((unique[0] == 1 && freqCount[0] == 1) || (unique[1] == 1 && freqCount[1] == 1)) 
            return yes;
        if((unique[0] == unique[1] + 1 && freqCount[0] == 1) || 
           (unique[1] == unique[0] + 1 && freqCount[1] == 1)) 
            return yes;
    }
    return no;
}

int main() {
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");
    char* s = readline();
    char* result = isValid(s);
    fprintf(fptr, "%s\n", result);
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
        if (!data) return NULL;
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
