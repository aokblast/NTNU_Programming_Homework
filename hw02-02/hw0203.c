#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef union 
{
    struct 
    {
        unsigned char b1:1;
        unsigned char b2:1;
        unsigned char b3:1;
        unsigned char b4:1;
        unsigned char b5:1;
        unsigned char b6:1;
        unsigned char b7:1;
        unsigned char b8:1;   
    } bits;
    unsigned char byte;
    
} uByte;

void printuByte(const uByte *num) {
    printf("Data: %d %d%d%d%d%d%d%d%d\n", num->byte, num->bits.b8, num->bits.b7, num->bits.b6, num->bits.b5, num->bits.b4, num->bits.b3, num->bits.b2, num->bits.b1);
}


int main(){
    uByte num;
    printf("Plrase enter a byte (0-255): ");
    scanf("%d", &num.byte);
    printuByte(&num);
    uint32_t mode = 0;
    while(1) {
        printf("Flip bit (1-8, 0: exit): ");
        scanf(" %d", &mode);
        if(mode == 0) break;
        else if (mode > 8 || mode < 0) {
            printf("Wrong input, Please retry.\n");
        }else {
            switch(mode) {
                case 1:
                    num.bits.b8 ^= 1;
                    break;
                case 2:
                    num.bits.b7 ^= 1;
                    break;
                case 3:
                    num.bits.b6 ^= 1;
                    break;
                case 4:
                    num.bits.b5 ^= 1;
                    break;
                case 5:
                    num.bits.b4 ^= 1;
                    break;                
                case 6:
                    num.bits.b3 ^= 1;
                    break;
                case 7:
                    num.bits.b2 ^= 1;
                    break;
                case 8:
                    num.bits.b1 ^= 1;
                    break;                                
            }
            printuByte(&num);
        }
    }
    printf("Bye.\n");

}