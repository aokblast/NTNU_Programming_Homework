#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define i32 int32_t

double eqresis(i32 resis,i32 n){
    if(n==1) return resis*2;
    else{
        double k=eqresis(resis,n-1);
        return k*resis/(k+resis)+resis;
    }    
}

int main(){
    i32 resis=0,n=0;
    printf("Please enter the resistance (1-100):");
    if(scanf("%d",&resis)!=1 || resis>100 || resis<1){
        printf("Wrong input.\n");
        exit(0);
    }
    printf("Please enter n (1-100):");
    if(scanf("%d",&n)!=1 || n>100 || n<1){
        printf("Wrong input.\n");
        exit(0);
    }
    printf("%lf\n",eqresis(resis,n));

}