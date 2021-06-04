#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min[1000005] = {0};
int max[1000005] = {0};

int main() {
    int totalCase = 0, num = 0, left = 0, right = 0, ans = 0;
    scanf(" %d", &totalCase);
    memset(max, 127, 4 * 1000005);
    for(int i = 1; i <= totalCase; ++i) {
        scanf(" %d %d %d", &num, &left, &right);
        min[(right == -1 ? 0 : right)] = (min[i] < num ? num : min[i]);
        max[(right == -1 ? 0 : right)] = max[i];
        max[(left == -1 ? 0 : left)] = (max[i] > num ? num : max[i]);
        min[(left == -1 ? 0 : left)] = min[i];
        if( min[i] <= num && num <= max[i]) ++ans;
    }
    printf("%d\n", ans);
}

