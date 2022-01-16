#include "all.h"

int f (int a, int b) {
    return a + b;
}

int main() {
    int a = -5, b = 2;
    -- a;
    a = f(a, b);
    printf("%d\n", a);
}
