#include <stdio.h>
#include <stdint.h>

int main() {
    int32_t number = 0;
   
    scanf("%d", &number);
    
    uint32_t bit = 1;
    bit <<= 31;
    
    for(int i = 0; i < 32; ++i) {
		if(bit & number) printf("1");
        else printf("0");
        bit = bit >> 1;
    }
    return 0;
}