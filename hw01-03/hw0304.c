#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double *input(const char *prt,int n){
	double *num_arr=malloc(sizeof(double)*n);
	printf("%s",prt);
	for(int i=0;i<n;i++){
		if(scanf("%lf,",&num_arr[i])!=1){
			printf("Wrong input.\n");
			exit(0);
		}
	}
	return num_arr;
	
}


void print(double *arr){
    printf("%lgx^2",arr[0]);
    int cnt=1;
    for(int i=1;i<=2;i++){
        if(arr[i]==0){
            printf("");
        }else if(cnt==0){
            printf("+%lg",arr[i]);
        }else{
            printf("+%lgx^%d",arr[i],cnt);
        }
        cnt--;
    }
    printf("\n");
}

double integral(double *arr,double *interval){
    double sum=0;    

    for(int i=3;i>=1;i--){
        sum+=((1/(double)i)*arr[3-i]*pow(interval[1],i));
    }
    //printf("%lf",sum);
    for(int i=3;i>=1;i--){
        sum-=((1/(double)i)*arr[3-i]*pow(interval[0],i));
    } 
    return sum; 
}
double target_poly(double*,double);
double riemann(double *arr,double *interval,int num){
    double sum=0;
    for(int i=1;i<=num;i++){
        sum+=target_poly(arr,interval[0]+((interval[1]-interval[0])/(double)num*i))*(interval[1]-interval[0])/(double)num;
    }
    return sum;
}

double target_poly(double *arr,double num){
    return arr[0]*pow(num,2)+arr[1]*pow(num,1)+arr[2];
}

int main(){
	double *poly,*inter;
	poly=input("Please enter a quadratic polynomial(ax^2+bx+c):",3);
	print(poly);
    inter=input("Plase enter the interval[s,t]:",2);
    printf("The integral: %lg\n",integral(poly,inter));
    for(int i=2;i<=65536;i*=2){
        printf("The Riemann sum of n=%d:%lg\n",i,riemann(poly,inter,i));
    }

}
