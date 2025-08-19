#include <stdio.h>
#include <string.h>

int minChanges(char s[]) {
    int n = strlen(s);
    if (n % 2) return -1;

    int half = n / 2;
    int countLeft[26] = {0}, countRight[26] = {0};
    
    for (int i = 0; i < half; i++) countLeft[s[i] - 'a']++;
    for (int i = half; i < n; i++) countRight[s[i] - 'a']++;

    int changes = 0;
    for (int i = 0; i < 26; i++) {
        if (countLeft[i] > countRight[i]) {
            changes += (countLeft[i] - countRight[i]);
        }
    }
    return changes;
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        char str[100010];
        scanf("%s", str);
        printf("%d\n", minChanges(str));
    }
    return 0;
}
