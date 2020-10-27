#include <stdio.h>
#include <stdlib.h>
int s1();
int s2();
int s3();
int s4();
int s5();
int s6();
int start();
int final();


int input(){
	int num=0;
	printf("Please input an integer: ");
	if(scanf(" %d",&num)==0){
		printf("Wrong input.\n");
		exit(0);
	}
	return num;

}

int start(){
	printf("Start\n");
	int num=input();
	if(num==10 || num==35){
		return s1();
	}else if(num==11){
		return s3();
	}else if(num==20 || num==78){
		return s5();
	}else{
		return start();
	}

}


int s1(){
	printf("s1\n");
	int num=input();
	if(num==19){
		return s2();
	}else if(num==12 || num==36){
		return s6();
	}else{
		return s1();
	}
}

int s2(){
	printf("s2\n");
	int num=input();
	if(num==43){
		return s2();
	}else if(num==99){
		return final();
	}else{
		return start();
	}

}

int s3(){
	printf("s3\n");
	int num=input();
	return s4();
}
int s4(){
	printf("s4\n");
	int num=input();
	return s6();
}

int s5(){
	printf("s5\n");
	int num=input();
	if(num==1){
		return s4();
	}else if(num==2){
		return s6();
	}else{
		return start();
	}
}
int s6(){
	printf("s6\n");
	int num=input();
	if(num==108){
		return final();
	}else{
		return s5();
	}
}

int final(){
	printf("final\n");
	int num=input();
	return final();


}
int main(){

	start();

}
