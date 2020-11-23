#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#define i32 int32_t

i32 cnt=1;
i32 input(const char *);
i32 range_judge(i32);
i32 judge(i32,i32);
i32 rand_generator();
i32 num_judge(i32);
void compare(i32,i32);

void bac(i32 ans){
    printf("Round %d >>>\n",cnt++);
    i32 num=input("Your Guess: ");
    printf("Response: ");
    if(num==ans){
        printf("\033[1;31mB\033[0;32mi\033[1;33mn\033[1;34mg\033[1;35mo\033[1;36m! \033[1;35mC\033[1;34mo\033[1;33mn\033[1;32mg\033[1;31mr\033[1;32ma\033[1;33mt\033[1;34mu\033[1;35ml\033[1;36ma\033[1;35mt\033[1;34mi\033[1;33mo\033[1;32mn\033[1;31ms\033[1;32m.\n");
        exit(0);
    }else if(num_judge(num) || !range_judge(num)){
        printf("Invalid Guess.\n");
        bac(ans);
    }else{
        compare(num,ans);
        bac(ans);
    }
}

void bac_init(){
    i32 ans=rand_generator();
    //i32 ans=1234;
    bac(ans);
}

i32 input(const char *p){
    printf("%s",p);
    i32 num=0;
    scanf("%d",&num);
    return num;  
}

i32 range_judge(i32 num){
    if(num>=1000 && num<=9999){
        return 1;
    }else{
        return 0;
    }
}

i32 rand_generator(){
    srand(time(NULL));
    i32 num=0;
    do{
        num=rand()%8999+1000;
    }while(num_judge(num));
    return num;
}

i32 num_judge(i32 num){
    i32 num_arr[10]={0};
    while(num>0){
        num_arr[num%10]++;
        num/=10;
    }
    for(i32 i=0;i<=9;i++){
        if(num_arr[i]>=2){
            return 1;
        }
    }
    return 0;
}

void compare(i32 num,i32 ans){
    i32 num_table[10]={0};
    i32 a=0,b=0;
    while(num>0 || ans>0){
        if(num%10==ans%10){
            a++;
        }
        num_table[num%10]++;
        num_table[ans%10]++;
        num/=10;
        ans/=10;
    }
    for(size_t i=0;i<=9;i++){
        if(num_table[i]>=2){
            b++;
        }
    }
    printf(" %d A %d B\n",a,b-a);
}
