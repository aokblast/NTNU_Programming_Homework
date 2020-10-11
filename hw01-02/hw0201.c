#include <stdio.h>
#include <math.h>

int main(){
	double_t a=0,b=0,c=0,d=0;
	printf("Please enter a quadratic polynomial (a,b,c):");
	if(scanf("%lf,%lf,%lf",&a,&b,&c)==3){
		d=b*b-4*a*c;
		if(d>0){
			printf("Two distinct real roots.\n");
		}	
		else if(d<0){
			printf("No real roots.\n");
		}
		else{
			printf("One real root.\n");
		}
	}
	else{
		printf("Wrong input.");
	}
}
