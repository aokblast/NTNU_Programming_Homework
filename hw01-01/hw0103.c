#include <stdio.h>
#include <math.h>

int main(){
	double_t grav,t;
	printf("Please enter the acceleration due to gravity:");
	scanf("%lf",&grav);
	printf("Please enter the time(s):");
	scanf("%lf",&t);
	printf("Final velocity: %.3lf\nThe altitude: %.3lf",grav*t,grav*t*t/2.0);

}
