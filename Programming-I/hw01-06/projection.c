#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define i32 int32_t

void showplane(i32 a,i32 b,i32 c,i32 d){
    printf("The plane is ");
    i32 cnt=0; 
    if(a!=0){
        if(a==1) printf("x");
        else if(a==-1) printf("-x");
        else printf("%dx",a);
        cnt=1;
    }
    if(b!=0){
        if(cnt==1 && b>0) {
            printf("+");
            cnt=0;
        }
        if(b==1) printf("y");
        else if(b==-1) printf("-y");
        else printf("%dy",b);
        cnt=1;
    }
    if(c!=0){
        if(cnt==1 && c>0){
            printf("+");
            cnt=0;
        }
        if(c==1) printf("z");
        else if(c==-1) printf("-z");
        else printf("%dz",c);
        cnt=1;
    }
    if(d!=0){
        if(cnt==1 && d>0) printf("+");
        printf("%d",d);
    }
    printf("=0\n");
}

void project(double *x,double *y,double *z,i32 a,i32 b,i32 c,i32 d){
    double tmp_x=*x,tmp_y=*y,tmp_z=*z;
    i32 denomitor=a*a+b*b+c*c;
    *x=((b*b+c*c)*tmp_x-a*(b*tmp_y+c*tmp_z+d))/denomitor;
    *y=((a*a+c*c)*tmp_y-b*(a*tmp_x+c*tmp_z+d))/denomitor;
    *z=((a*a+b*b)*tmp_z-c*(a*tmp_x+b*tmp_y+d))/denomitor;
}
