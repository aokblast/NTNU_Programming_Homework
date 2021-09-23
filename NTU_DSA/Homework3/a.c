#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int shelf[1000005] = {0};

int main(){
    int n, q, p, l,r, m, k, tmp, tmp2, max_index = 0, i, max;
    scanf(" %d %d", &n, &q);
    for(i = 0; i < n; ++i){
        scanf(" %d", &p);
        shelf[i] = p;
    }

    while(q--){
        scanf(" %d", &m);
        switch(m){
            case 1:
                scanf(" %d %d", &p, &k);
                tmp = shelf[k];
                shelf[k] = p;
                for(i = k + 1; i <= n; ++i){
                    tmp2 = shelf[i];
                    shelf[i] = tmp;
                    tmp = tmp2;
                }
                ++n;
                break;
            case 2:
                scanf(" %d", &k);
                for(i = k - 1; i < n - 1; ++i){
                    shelf[i] = shelf[i + 1];
                }
                --n;
                break;
            case 3:
                scanf(" %d %d %d", &l, &r, &p);
                for(i = l - 1; i < r; ++i){
                    shelf[i] += p;
                }
                break;
            case 4:
                scanf("%d %d", &l, &r);
                max = shelf[l - 1];
                for(i = l; i < r; ++i){
                    if(shelf[i] > max) max = shelf[i];
                }
                printf("%d\n", max );
                break;
            case 5:
                scanf("%d %d", &l, &r);
                --l, --r;
                for(i = 0; i < (r - l) / 2 + 1; ++i){
                    tmp = shelf[i + l];
                    shelf[i + l] = shelf[r - i];
                    shelf[r - i] = tmp;
                }
                break;
            case 6:
                max_index = 0;
                max = shelf[0];
                if(n == 0) break;
                for(i = 1; i < n; ++i){
                    if(max < shelf[i]) max_index = i, max = shelf[i];
                }
                for(i = max_index; i < n - 1; ++i){
                    shelf[i] = shelf[i + 1];
                }
                --n;
                break;            
        }

    }
}
