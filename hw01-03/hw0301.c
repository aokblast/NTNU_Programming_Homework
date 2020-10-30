#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pas(const char *p){
	int num=0;
	printf("%s",p);
	if(scanf(" %d",&num)==1 && num>0){
		return num;
	
	}
	else{
		printf("Wrong input.");
		exit(0);		
	}
}





int main(){
	int total=0,x=0,y=0,num=1,direct[4][2]={{1,0},{0,1},{-1,0},{0,-1}},dircnt=0;
	int width=pas("Please enter the width:");
	int height=pas("Please enter the height:");
	int torna[width][height];
	total=0;
	memset(torna,0,sizeof(torna));
	
	while(num<=width*height){
		torna[x][y]=num;
		if(torna[x+direct[dircnt%4][0]][y+direct[dircnt%4][1]]!=0 || x+direct[dircnt%4][0]==width || y+direct[dircnt%4][1]==height){
			dircnt++;
		}		
		x+=direct[dircnt%4][0];
		y+=direct[dircnt%4][1];
		num++;	
		}

	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			printf("%4d ",torna[j][i]);
		
		}
		printf("\n");
	}

		

}
