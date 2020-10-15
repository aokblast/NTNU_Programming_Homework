#include <stdio.h>
#include <stdlib.h>


int col_judge(int *a,int size_of_array){
	int color=a[0]/13;
	for(int i=1;i<size_of_array;i++){
		if((a[i]/13)!=color){
			return 0;
		}
	}
	return 1;	
}

int num_judge(int *a,int size_of_array,int num){
	int num_arr[13]={0};
	for(int i=0;i<size_of_array;i++){
		num_arr[a[i]%13]++;
	}
	
	for(int i=0;i<13;i++){
		if(num_arr[i]==num){
			return 1;
		}
	}
	
	return 0;
}

int contin_judge(int *a,int size_of_array){
	int tmp;
	for(int i=0;i<size_of_array;i++){
		for(int j=i+1;j<size_of_array;j++){
			if(a[i]>a[j]){
				tmp=a[i];
				a[i]=a[j];
				a[j]=tmp;
			}
		}
	}
	int contin=a[0];

	for(int i=1;i<size_of_array;i++){
		if(a[i]!=contin+1){
			return 0;
		}
		else{
			contin++;
		}
	}
	return 1;

}

int norm_judge(int *a,int size_of_array){
	if(col_judge(a) && contin_judge(a)){
		return "Straight Flush";
	}	
}


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
