#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"

#define ll long long

int  n , *p, *q, *r, tmp_p[100005], tmp_q[100005], tmp_r[100005];
ll ans;

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int p_merge(int a, int b) {
    if(p[a] != p[b]) return p[a] < p[b];
    else if(q[a] != q[b]) {
        if((q[a] >= q[b] && r[b] >= r[a] ) || (r[a] >= q[b]) || (q[b] >= q[a] && r[a] >= r[b] ) || (r[b] >= q[a])) ++ans;
        return q[a] < q[b];
    }
    else if(r[a] != r[b]){
        if((q[a] >= q[b] && r[b] >= r[a] ) || (r[a] >= q[b]) || (q[b] >= q[a] && r[a] >= r[b] ) || (r[b] >= q[a]) ) ++ans;
        return r[a] < r[b];
    }
    else return 0;
}

int q_merge(int a, int b) {
    return q[a] < q[b];
}

int r_merge(int a, int b) {
    return r[a] < r[b];
}

void merge_sort_1(int front, int end) {
    if((end - front) > 1){
        int mid = (front + end) / 2;
        merge_sort_1(front, mid);
        merge_sort_1(mid, end);
        int left_index = front;
        int right_index = mid;
        int exc_index = 0;
        while(left_index < mid && right_index < end) {
            if(p_merge(left_index, right_index)) {
                tmp_q[exc_index] = q[left_index];
                tmp_r[exc_index] = r[left_index];
                tmp_p[exc_index++] = p[left_index++];
            }else {
                tmp_q[exc_index] = q[right_index];
                tmp_r[exc_index] = r[right_index];
                tmp_p[exc_index++] = p[right_index++];
            }
        }
        
        for(; left_index < mid;  ++left_index, ++exc_index) {
                tmp_q[exc_index] = q[left_index];
                tmp_r[exc_index] = r[left_index];
                tmp_p[exc_index] = p[left_index];            
        }
        for(; right_index < end;  ++right_index, ++exc_index) {
                tmp_q[exc_index] = q[right_index];
                tmp_r[exc_index] = r[right_index];
                tmp_p[exc_index] = p[right_index];            
        }
        for(int i = 0; i < exc_index; ++i) {
            p[i+front] = tmp_p[i];
            q[i+front] = tmp_q[i];
            r[i+front] = tmp_r[i];
        }
    }
}

void merge_sort_2(int front, int end) {
    if((end - front) > 1){
        int mid = (front + end) / 2;
        merge_sort_2(front, mid);
        merge_sort_2(mid, end);


        for(int i = front, j = mid; i < mid; ++i){
            while(q[i] >= r[j] && j < end) ++j;
            ans += j - mid;
        }

    
        int left_q_index = front;
        int right_q_index = mid;
        int left_r_index = front;
        int right_r_index = mid;
        int exc_q_index = 0;
        int exc_r_index = 0;

        while(left_q_index < mid && right_q_index < end) {
            if(q_merge(left_q_index, right_q_index)) {
                tmp_q[exc_q_index++] = q[left_q_index++];
            }else {
                tmp_q[exc_q_index++] = q[right_q_index++];
            }
        }

        while(left_r_index < mid && right_r_index < end) {
            if(r_merge(left_r_index, right_r_index)) {
                tmp_r[exc_r_index++] = r[left_r_index++];
            }else {
                tmp_r[exc_r_index++] = r[right_r_index++];
            }
        }
        


        for(; left_q_index < mid;  ++left_q_index, ++exc_q_index) {
                tmp_q[exc_q_index] = q[left_q_index];        
        }
        for(; right_q_index < end;  ++right_q_index, ++exc_q_index) {
                tmp_q[exc_q_index] = q[right_q_index];        
        }

        for(int i = 0; i < exc_q_index; ++i) {
            q[i+front] = tmp_q[i];
        }

        for(; left_r_index < mid;  ++left_r_index, ++exc_r_index) {
            tmp_r[exc_r_index] = r[left_r_index];        
        }
        for(; right_r_index < end;  ++right_r_index, ++exc_r_index) {
            tmp_r[exc_r_index] = r[right_r_index];        
        }

        for(int i = 0; i < exc_r_index; ++i) {
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
    
    normalize();
    ans = 0;
    merge_sort_1(0, n);
    
    merge_sort_2(0, n);
    /*
    for(int i = 0; i < n; ++i) printf("%d ", q[i]);
    printf("\n");
    for(int i = 0; i < n; ++i) printf("%d ", r[i]);
    printf("\n");
    */
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
