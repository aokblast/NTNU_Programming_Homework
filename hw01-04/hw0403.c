#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hw04.h"
#define i32 int32_t

i32 input(const char *p){
    double num;
    printf("%s",p);
    if(scanf("%lf",&num)==1 && num>=1 && num==(int)num){
        return (int)num;
    }else{
        printf("Wrong input.\n");
        exit(0);
    }

}


int main(){
    i32 num=0;
    printf("k-th order Taylor polynomial for e\n");
    num=input("Please enter k: ");
    printf("%.50lf\n",natural_log(num));
    //printf("%.5lf\n",exp(num));
}
