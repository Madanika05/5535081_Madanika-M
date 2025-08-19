#include <stdio.h>
#include <string.h>

int validPal(char s[], int n, int skip) {
    int a = 0, b = n - 1;
    while (a < b) {
        if (a == skip) a++;
        if (b == skip) b--;
        if (s[a] != s[b]) return 0;
        a++; b--;
    }
    return 1;
}

int findIndex(char s[]) {
    int n = strlen(s);
    for (int i = 0, j = n - 1; i < j; i++, j--) {
        if (s[i] != s[j]) {
            if (validPal(s, n, i)) return i;
            if (validPal(s, n, j)) return j;
            return -1;
        }
    }
    return -1;
}

int main() {
    int q;
    scanf("%d", &q);
    while (q--) {
        char word[100010];
        scanf("%s", word);
        printf("%d\n", findIndex(word));
    }
    return 0;
}
