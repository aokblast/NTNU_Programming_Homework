#include <stdint.h>
#include <stdio.h>

typedef union 
{
    double flp;
    struct  {
        uint64_t fraction : 52;
        uint64_t exponent : 11;
        uint64_t sign : 1;
    } ieee;
} flt;

void printbin(uint64_t num, size_t times) {
    if(times != 1) printbin(num >> 1, times - 1);
    printf("%d", num & 1);
}

void frachandler(uint64_t num, size_t times) {
    if(times != 1) frachandler(num >> 1, times - 1);
    if(num & 1) printf(" + 2^-%lu", times);
}

int main() {
    flt num;
    printf("Please enter a floating -point number (double precision): ");
    scanf("%lf", &num.flp);
    printf("Sign : %d\n", num.ieee.sign);
    printf("Exponent: ");
    printbin(num.ieee.exponent, 11);
    printf("\n");
    printf("Fraction: ");
    printbin(num.ieee.fraction, 52);
    printf("\n");
    printf("%lf = (-1)^%d * (1", num.flp, num.ieee.sign);
    frachandler(num.ieee.fraction, 52);
    printf(") * 2^(%d- 1023)\n", num.ieee.exponent);
}