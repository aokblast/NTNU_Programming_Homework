#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"

#define ll long long

int ans , n , *p, *q, *r, tmp_p[100005], tmp_q[100005], tmp_r[100005];


void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int p_merge(int a, int b) {
    return  p[a] < p[b];
}


void merge_sort(int front, int end, int (*cmp_func)(int a, int b)) {
    if(end - front > 1){
        int mid = (front + end) / 2;
        merge_sort(front, mid, cmp_func);
        merge_sort(mid, end, cmp_func);
        int left_index = front;
        int right_index = mid;
        int exc_index = 0;
        while(left_index < mid && right_index < end) {
            if(cmp_func(left_index, right_index)) {
                tmp_q[exc_index] = q[left_index];
                tmp_r[exc_index] = r[left_index];
                tmp_p[exc_index++] = p[left_index++];
            }else {
                tmp_q[exc_index] = q[right_index];
                tmp_r[exc_index] = r[right_index];
                tmp_p[exc_index++] = p[right_index++];
            }
        }
        
        for(; left_index < mid;  ++left_index) {
                tmp_q[exc_index] = q[left_index];
                tmp_r[exc_index] = r[left_index];
                tmp_p[exc_index++] = p[left_index];            
        }
        for(; right_index < end;  ++right_index) {
                tmp_q[exc_index] = q[right_index];
                tmp_r[exc_index] = r[right_index];
                tmp_p[exc_index++] = p[right_index];            
        }
        for(int i = 0; i < exc_index; ++i) {
            p[i+front] = tmp_p[i];
            q[i+front] = tmp_q[i];
            r[i+front] = tmp_r[i];
        }
    }
}



void normalize() {
    for(int i = 0; i < n; ++i) {
        if(q[i] < r[i]) {
            swap(&q[i], &r[i]);
        }
    }
}


void solve(){
    ans = 0;
    normalize();
    merge_sort(0, n, p_merge);
    ans = 0;
    merge_sort(0, n, qr_merge);
}



int main () {
    generator.init();
    int t = generator.getT();
    while(t--) {
        generator.getData(&n, &p, &q, &r);
        solve();
        printf("%d\n", ans);
        
    }
}
