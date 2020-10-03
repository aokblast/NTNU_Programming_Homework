#include <stdio.h>
#include <stdint.h>
int main(){
	int8_t num1;
	printf("Please enter an integer: ");
	scanf("%hhd",&num1);
	printf("%hhd: %hhx",num1,num1);
}
