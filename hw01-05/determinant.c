#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#define i32 int32_t
void ptrarr(i32 *arr,i32 size){
    for(i32 i=0;i<size;i++){
        for(i32 j=0;j<size;j++){
            printf("%d",*(arr+i*size+j));
        }
        printf("\n");
    }
    printf("==========\n");
}

i32 deter(i32 *arr,i32 m){
    i32 flag=1,total=0,*next_arr=(i32 *)malloc(sizeof(i32)*(m-1)*(m-1));
    if(m==2){
        return (*arr)*(*(arr+3))-(*(arr+2))*(*(arr+1));  
    }else{ 
        for(i32 i=0;i<m;i++){
            i32 cnt=0;
            for(i32 j=1;j<m;j++){
                for(i32 k=0;k<m;k++){
                if(i!=k){
                        *(next_arr+cnt)=*(arr+m*j+k); 
                        cnt++;    
                    }
                }
            }
            total+= flag *(*(arr+i)*deter(next_arr,m-1));
            flag*=-1;
        }
    }
    return total;
} 
