#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define i32 int32_t

struct point{
    i32 x;
    i32 y;
};

double sigma(struct point *ptr,double avg_x,i32 n){
    if(n>=0){
        return ((ptr+n)->x-avg_x)*((ptr+n)->x-avg_x)+sigma(ptr,avg_x,n-1);
    }else{
        return 0;
    }
}
double omega(struct point *ptr,double avg_x,double avg_y,i32 n){
    if(n>=0){
        return ((ptr+n)->x-avg_x)*((ptr+n)->y-avg_y)+omega(ptr,avg_x,avg_y,n-1);
    }else{
        return 0;
    }
}

void regression(){
    i32 number=0,total_x=0,total_y=0;
    double avg_x=0,avg_y=0;
    printf("Please enter the point number:");
    scanf(" %d",&number);
    struct point *points=(struct point*)malloc(sizeof(struct point)*number);
    for(int i=0;i<number;i++){
        printf("Please enter Point %d:",i+1);
        scanf("%d %d",&(points+i)->x,&(points+i)->y);
        total_x+=(points+i)->x;
        total_y+=(points+i)->y;
    }
    avg_x=total_x*1.0/number;
    avg_y=total_y*1.0/number;
    double r=omega(points,avg_x,avg_y,number-1)/sigma(points,avg_x,number-1);
    printf("y=%lgx+%lg\n",r,-r*avg_x+avg_y);
}
