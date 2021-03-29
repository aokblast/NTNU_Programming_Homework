#include <stdio.h>
#include "bignum.h"

int main() {
    sBigNum num01, num02, ans, ans2;
    char num[10000];
    printf("Num01: ");
    scanf(" %s", num);
    set(&num01, num);
    printf("Num02: ");
    scanf(" %s", num);
    set(&num02, num);
    printf("Origin:\n");
    printf("Num01:");
    print(num01);
    printf("Num02:");
    print(num02);
    add(&ans, num01, num02);
    printf("Add:");
    print(ans);
    printf("Sub:");
    subtract(&ans, num01, num02);
    print(ans);
    printf("Mul:");
    multiply(&ans, num01, num02);
    print(ans);
    divide(&ans, &ans2, num01, num02);
    printf("Div:\n");
    printf("Q:");
    print(ans);
    printf("R:");
    print(ans2);
    printf("Power:");
    power(&ans, 2, 1024);
    print(ans);
    printf("Combine:");
    combine(&ans, 200, 100);
    print(ans);
}