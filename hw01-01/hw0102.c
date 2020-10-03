#include <stdio.h>
#include <stdint.h>
int main(){
	int64_t	num=0,prod=1;
	printf("Please enter a 5-digits integer:");
	scanf("%ld",&num);
	for(int i=0;i<5;i++){
		prod*=(num%10);
		num/=10;
	}
	printf("Result:%ld",prod);
}
