#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
int main(){
	int32_t cnt=0,tmp=0,start=0,end=0;
    double num;
	printf("Please enter a number:");
	if(scanf(" %lf",&num)==0 || num<0 || num!=(int)num){
		printf("Wrong input.\n");
		exit(0);
	}
	tmp=num;
	end=tmp%10;
	num-=end;	
	while(tmp>10){
		tmp/=10;
		cnt++;
	}
	start=tmp;
	num-=(start*pow(10,cnt));
	num+=(end*pow(10,cnt));
	num+=start;
    printf("%d\n",(int)num);
}
