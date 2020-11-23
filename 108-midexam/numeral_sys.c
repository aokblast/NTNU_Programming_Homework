#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


char* baseN(uint32_t num,uint32_t N){
	char num_table[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	int64_t cnt,index=0,num_arr[1000]={0};
	while(num>=N){
		num_arr[index]=num%N;
		num/=N;
		index++;
	}
	
	num_arr[index]=num;
	/*
	
	for(int i=0;i<=index;i++){
	
		printf("%ld ",num_arr[i]);
	}
	*/
	char *ans=malloc(sizeof(char)*index);
	cnt=0;
	for(int i=index;i>=0;i--){
		ans[cnt]=num_table[num_arr[i]];		
		cnt++;	
	
	}


	return ans;
	
}
