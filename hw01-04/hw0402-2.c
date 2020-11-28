#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hw04.h"
#define i32 int32_t

i32 input(const char *p){
    double num=0;
    printf("%s",p);
    if(scanf("%lf",&num)==1 && num>=2 && num<=20 && num==(int)num){
        return (int)num;
    }else{
        printf("Wrong input.\n");
        exit(0);
    }
}

int main(){
    i32 num=input("Please enter the disk number (2-20): "); 
    hanoi_loop(num,1,2,3);

}
