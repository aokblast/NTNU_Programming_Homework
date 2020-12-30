#include <stdint.h>
#include <stdio.h>
#include "hw06.h"
#define i32 int32_t

int main(){
    double x=0,y=0,z=0,a=0,b=0,c=0,d=0;
    printf("Please enter the plane:");
    if(scanf(" %lf %lf %lf %lf",&a,&b,&c,&d)!=4 && a!=(i32)a && b!=(i32)b && c!=(i32)c && d!=(i32)d) printf("Wrong input\n");
    showplane((i32)a,(i32)b,(i32)c,(i32)d);
    printf("Please enter the point:");
    if(scanf(" %lf %lf %lf",&x,&y,&z)!=3) printf("Wrong input.\n"); 
    project(&x,&y,&z,(i32)a,(i32)b,(i32)c,(i32)d);
    printf("The projection is (%lf %lf %lf)",x,y,z);
}
