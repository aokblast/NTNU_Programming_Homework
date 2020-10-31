#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double *poly;

double *input(const char *prt,int n){
	double *num_arr=malloc(sizeof(double)*n);
	printf("%s",prt);
	for(int i=0;i<n;i++){
		if(scanf(" %lf,",&num_arr[i])!=1){
			printf("Wrong input.\n");
			exit(0);
		}
	}
	return num_arr;
	
}


void print(){
    if(poly[0]==1){
        printf("x^2");
    }else if(poly[0]==0){
        printf("Wrong input.\n");
    }else{
        printf("%lgx^2",poly[0]);
    }
    if(poly[1]==1){
        printf("+x^1");
    }else if(poly[1]!=0){
        printf("+%lgx^1",poly[1]);
    }
    if(poly[2]!=0){
        printf("+%lg",poly[2]);
    }
    printf("\n");
}

double integral(double *interval){
    double sum=0;    

    for(int i=3;i>=1;i--){
        sum+=((1/(double)i)*poly[3-i]*pow(interval[1],i));
    }
    //printf("%lf",sum);
    for(int i=3;i>=1;i--){
        sum-=((1/(double)i)*poly[3-i]*pow(interval[0],i));
    } 
    return sum; 
}
double target_poly(double);
double riemann(double *interval,int num){
    double sum=0;
    for(int i=0;i<=num-1;i++){
        sum+=target_poly(interval[0]+((interval[1]-interval[0])/(double)num*i))*(interval[1]-interval[0])/(double)num;
    }
    return sum;
}

double target_poly(double num){
    return poly[0]*pow(num,2)+poly[1]*pow(num,1)+poly[2];
}

int main(){
	double *inter;
	poly=input("Please enter a quadratic polynomial(ax^2+bx+c):",3);
	print();
    inter=input("Plase enter the interval[s,t]:",2);
    printf("The integral: %lg\n",integral(inter));
    for(int i=2;i<=65536;i*=2){
        printf("The Riemann sum of n=%d:%lg\n",i,riemann(inter,i));
    }

}
