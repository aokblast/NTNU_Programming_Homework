#include <ctype.h>
#include <stdio.h>

int gotaline(char target[], int buffer) {
    int i, c;
    for (i = 0; (c = getchar()) && c != '\n' && c != EOF && i < buffer - 1; ++i) target[i] = c;
    target[i] = '\0';
    if((i % 2 == 1) || (target[i - 1] != '0' || target[i - 2] != '0')) return 0;
    else return 1;
}

void getaline(char target[], int buffer) {
    int i, c;
    for (i = 0; (c = getchar()) && c != '\n' && c != EOF && i < buffer - 1; ++i) target[i] = c;
    target[i] = '\0';
}
