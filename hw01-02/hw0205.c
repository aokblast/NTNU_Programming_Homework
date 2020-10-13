#include <stdio.h>
#include <stdlib.h>





int main(){
	int num[5]={0};
	
	printf("Please enter 5 cards.");

	while(scanf("%d %d %d %d %d",&num[0],&num[1],&num[2],&num[3],&num[4])!=5){
		printf("Wrong input.");
		exit(0);
	}
	
	for(int i=0;i<5;i++){
		if(num[i]>52 || num[i]<0){
			printf("Wrong input.");
			exit(0);
	
		}
	}
	







}
