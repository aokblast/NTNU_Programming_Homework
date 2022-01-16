#include "all.h"
#define myassert(s) if(s) printf("Pass " #s "\n");\
					else printf("Failed " #s "\n")

#define getBits(DIR, NUM) DIR##NUM

#define LEFT2 0xc0
#define RIGHT2 0x03
#define RIGHT4 0x0f
#define LEFT4 0xf0
#define LEFT6 0x3f
#define RIGHT6 0xfc




int main() {
    myassert(-1 == 1);
    printf("%u", 'a' & (getBits(RIGHT, 4)));

}