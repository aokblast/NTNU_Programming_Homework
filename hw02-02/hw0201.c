#include "mixed.h"
#include <stdio.h>

int main() {
    sMixedNumber a, b, ans;
    if(!mixed_set(&a, -1,  0, 0) || !mixed_set(&b, 3, 0, 0)) {
        printf("Wrong input.\n");
        return 0;
    }
    printf("Origin A:");
    mixed_print(a);
    printf("Origin B:");
    mixed_print(b);
    printf("Add:");
    mixed_add(&ans, a, b);
    mixed_print(ans);
    printf("Sub:");
    mixed_sub(&ans, a, b);
    mixed_print(ans);
    printf("Mul:");
    mixed_mul(&ans, a, b);
    mixed_print(ans);
    printf("Div:");
    mixed_div(&ans, a, b);
    mixed_print(ans);
}