#include "hw06.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    double theta,ptr[2]={0};
    printf("Please input a point:");
    if(scanf(" %lf %lf",&ptr[0],&ptr[1])!=2){
        printf("Wrong input.\n");
        exit(0);
    }
    printf("Please enter theta (0-360): ");
    if(scanf(" %lf",&theta)!=1 || theta>360 || theta<0){
        printf("Wrong input.\n");
        exit(0);
    }
    rotate(&ptr[0],&ptr[1],theta);
    printf("%lf %lf\n",ptr[0],ptr[1]);
    
}
