#include <stdio.h>

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void merge_sort(int *arr, int front, int end) {
    if(end - front > 1){
        merge_sort(arr, front, (front + end) / 2);
        merge_sort(arr, (front + end) / 2, end);
        int mid = (front + end) / 2;

        for(; mid < end; ++mid) {
            for(int i = mid; i > front; --i) {
                if(arr[i] < arr[i - 1]) swap(&arr[i], &arr[i - 1]);
            }
        }
    }
}

int main(){
	int arr[10] = {5, 7, 4, 3, 6, 10, 30, 15, 40, 50};
	merge_sort(arr, 0, 10);
	for(int i = 0; i < 10; ++i) {
		printf("%d ", arr[i]);
	}
}
