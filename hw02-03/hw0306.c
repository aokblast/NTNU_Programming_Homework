#include <stdio.h>
#include <stdint.h>

void max(int32_t a, int32_t b) {
    if(!((a - b) & (1 << 31))){
        printf("%d\n", a);
    }else {
        printf("%d\n", b);
    }
    return;
}

int main(){
    max(1, 3);
}