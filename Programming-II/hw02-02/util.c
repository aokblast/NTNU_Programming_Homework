#include <stdint.h>

void swapi64(int64_t *a, int64_t *b) {
    int64_t tmp = *a;
    *a = *b;
    *b = tmp;
} 

int64_t gcd(int64_t a, int64_t b) {
    if(b > a) swapi64(&b, &a);
    if(b == 0) return 1;
    while(b != 0) {
        int64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}