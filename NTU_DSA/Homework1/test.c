#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void reversearrSeg(int *arr1, int *arr2, int start1, int end1, int start2, int end2) {
    int total = (end1 + end2 - start1 - start2) / 2;
    for(int i = 0; i <= total; ++i) {
        if(((i + start1) <= end1) && ((end2 - i) >= start2)) {
            swap(&arr1[i + start1], &arr2[end2 - i]);
        }else if(((i + start1) > end1) && ((end2 - i) >= start2)) {
            swap(&arr2[i + start2 - (end1 - start1 + 1)], & arr2[end2 - i]);
        }else if(((i + start1) <= end1) && ((end2 - i) < start2))  {
            swap(&arr1[i + start1], &arr1[end1 - i - (end2 - start2 + 1)]);
        }
    }
}

void printArr(int *arr, int len) {
    for(int i = 0; i < len; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(){
    int arr1[] = {1,2,3,4,5}, arr2[] = {6,7,8,9,10};
    reversearrSeg(arr1, arr2, 1, 4, 0, 2);
    printArr(arr1, sizeof(arr1) / sizeof(int));
    printArr(arr2, sizeof(arr2) / sizeof(int));

}
