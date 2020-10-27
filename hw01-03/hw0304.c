#include <stdio.h>
#include <stdlib.h>


double *input(const char *prt,int n){
	double *num_arr=malloc(sizeof(double)*n);
	printf("%s",prt);
	for(int i=0;i<n;i++){
		if(scanf("%lf,",&num_arr[i])!=1){
			printf("Wrong input.");
			exit(0);
		}
	}
	return num_arr;
	
}


int main(){
	double *poly,*inter;
	poly=input("Please enter a quadratic polynomial(ax^2+bx+c):",3);
	int cnt=2;
	for(int i=0;i<=2;i++){
		if(poly[i]==0){
			printf("");
		}else if (cnt==0){
			printf("%lg",poly[i]);
		}
		else{
			printf("%lgx^%d",poly[i],cnt);
		}

		cnt--;
	
	}
	printf("\n");
	inter=input("Plase enter the interval[s,t]:",2);

}
