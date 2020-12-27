#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define i32 int32_t

void rotate(double *x,double *y,double theta){
    double radian=theta/180*3.14159265358979;
    double tmp_x=*x,tmp_y=*y;
    *x=tmp_x*cos(radian)+tmp_y*sin(radian);
    *y=-tmp_x*sin(radian)+tmp_y*cos(radian);
}
