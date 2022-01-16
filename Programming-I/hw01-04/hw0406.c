#include <stdio.h>
#include <stdint.h>
#define i32 int32_t
int main(){
    i32 a=0,b=0;
    printf("Return of scanf: %d\n",scanf("%d,%d",&a,&b));
    printf("Retur of printf: %d\n",printf("%d,%d\n",a,b));
}
