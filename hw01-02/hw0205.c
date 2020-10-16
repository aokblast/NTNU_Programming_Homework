#include <stdio.h>
#include <stdlib.h>

#define size_of_array 5

int col_judge(int *a){
	int color=a[0]/13;
	for(int i=1;i<size_of_array;i++){
		if((a[i]/13)!=color){
			return 0;
		}
	}
	return 1;	
}

int num_judge(int *a,int num){
	int num_buck[13]={0},cnt=0;
	for(int i=0;i<size_of_array;i++){
		num_buck[a[i]%13]++;
	}
	
	for(int i=0;i<13;i++){
		if(num_buck[i]==num){
			cnt++;
		}
	}
	
	return cnt;
}


int contin_judge(int *a){
	int tmp;
	for(int i=0;i<size_of_array;i++){
		a[i]%=13;
	}
	//qsort
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
		if((a[i])!=contin+1){
			retrn 0;
		}
		else{
			contin++;
		}
	}
	
	return 1;

} 

int norm_judge(int *a){
	if(col_judge(a) && contin_judge(a)){
		return 0;
	}else if(num_judge(a,4)){
		return 1;
	}else if(num_judge(a,3) && num_judge(a,2)){
		return 2;
	}else if(col_judge(a)){
		return 3;
	}else if(contin_judge(a)){
		return 4;
	}else if(num_judge(a,3)){
		return 5;
	}else if(num_judge(a,2)==2){
		return 6;	
	}else if(num_judge(a,2)){
		return 7;
	}else{
		return 8;
	}
}

void input_judge(int *a){
	for(int i=0;i<size_of_array;i++){
		if(a[i]>=52 || a[i]<=0){
			printf("Wrong input.\n");
			exit(0);
		}
		for(int j=i+1;j<size_of_array;j++){
			if(a[i]==a[j]){
				printf("Wrong input.\n");
				exit(0);
			}
		}
	}
}


int main(){
	int num[5]={0};
	char deck[50][50]={"Straight Flush.","Four of a kind.","Full house.","Flush.","Straight.","Three of a kind.","Two pair.","One pair.","High card."};
	
	printf("Please enter 5 cards.");

	while(scanf("%d %d %d %d %d",&num[0],&num[1],&num[2],&num[3],&num[4])!=5){
		printf("Wrong input.\n");
		exit(0);
	}
	input_judge(num);

	for(int i=0;i<size_of_array;i++){
		num[i]--;
	}
	//printf("%d",contin_judge(num));
	printf("%s\n",deck[norm_judge(num)]);
	


}
