#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char g[100005] = {0};
char d[100005] = {0};
char result[200010] = {0};
char mid[200010] = {0};
int glength = 0;
int dlength = 0;
int tabled[52];
int tableg[52];

void solve(){
    char *left_ptr = d;
    char *right_ptr = d;
    char *left_tmp_ptr = d;
    char *right_tmp_ptr = d;
    int left_hash = 0, right_hash = 0, index = 0, nonzero = 0, nonzeroind = 0;
    glength = strlen(g);
    dlength = strlen(d);
    size_t minsize = 2147483646;
    for(int  i = 0; i < glength; ++i){
        index = ((isupper(g[i]) ? (26 + g[i] - 'A') : (g[i] - 'a')));
        //printf("%c %d\n", g[i], tableg[index]);
        if(tableg[index] == 0) ++nonzero;
        ++tableg[index];
    }
    
    while(*right_tmp_ptr != '\0') {
        while(*right_tmp_ptr != '\0') {
            index = (isupper(*right_tmp_ptr) ? (26 + (*right_tmp_ptr) - 'A') : ((*right_tmp_ptr) - 'a'));
            ++tabled[index];
            
            if(tableg[index] == tabled[index]) {
                ++nonzeroind;
            }
            ++right_tmp_ptr;
            if(nonzeroind == nonzero){
                break;
            }
            
        }
        
        while((right_tmp_ptr - left_tmp_ptr) >= glength - 1) {
            index = (isupper(*left_tmp_ptr) ? (26 + (*left_tmp_ptr) - 'A') : ((*left_tmp_ptr) - 'a'));
            if(tableg[index] != 0 && tabled[index] == tableg[index]) break;
            --tabled[index];
            ++left_tmp_ptr;
        }
        
        if((right_tmp_ptr - left_tmp_ptr) >= glength && *left_tmp_ptr != '\0' && nonzero == nonzeroind) {
            if((right_tmp_ptr - left_tmp_ptr + 1) < minsize ){
                minsize = right_tmp_ptr - left_tmp_ptr + 1;
                left_ptr = left_tmp_ptr;
                right_ptr = right_tmp_ptr;
                --nonzeroind;
                index = isupper(*left_tmp_ptr) ? 26 + (*left_tmp_ptr) - 'A' : (*left_tmp_ptr) - 'a';
                --tabled[index];
                ++left_tmp_ptr;
            }
        }else if(minsize == 2147483646){
            left_ptr = right_ptr = right_tmp_ptr;
        }
    }

    
    //printf("%s\n", left_ptr);
    
    strncpy(mid, d, left_ptr - d);
    strcpy(&mid[strlen(mid)], right_ptr);
    //printf("%s\n", mid);
    //printf("%ld\n", strlen(mid));
    left_ptr = mid;
    right_ptr = &mid[strlen(mid) - 1];
    char *prev_left = mid;
    char *left_result = result;
    char *right_result = &result[200010 - 1];
    int tmp = 1;
    while(left_ptr <= right_ptr){
        int left_num = (isupper(*left_ptr) ? (26 + (*left_ptr - 'A')) : (*left_ptr - 'a'));
        int right_num = (isupper(*right_ptr) ? (26 + (*right_ptr - 'A')) : (*right_ptr - 'a'));
    
        left_hash = (((left_hash * 52) % 21474836) + left_num) % 21474836;
        right_hash = (right_hash + ((right_num * tmp) % 21474836)) % 21474836;
        //printf("%d %d\n", left_hash, right_hash);
        if(left_hash == right_hash && strncmp(prev_left, right_ptr, left_ptr - prev_left + 1) == 0 && left_ptr != right_ptr){
            left_hash = 0;
            right_hash = 0;
            tmp = 1;
            *left_result = *left_ptr;
            *right_result = *right_ptr;
            ++left_result;
            --right_result;
            if((left_ptr + 1)  == right_ptr){
                *left_result = '|';
                ++left_result;
            }else{
                *left_result = '|';
                ++left_result;
                *right_result = '|';
                --right_result;
            }
            prev_left = left_ptr + 1;
            *right_ptr = '\0';
        }else{
            if(left_ptr == right_ptr){
                *left_result = *left_ptr;
                ++left_result;
            }else{
                *left_result = *left_ptr;
                *right_result = *right_ptr;
                ++left_result;
                --right_result;
            }
            tmp = (tmp * 52) % 21474836;
        }
        ++left_ptr;
        --right_ptr;
        
    }
    
}


int main(){
    int testCase = 0;
    scanf(" %d", &testCase);
    while(testCase--) {
        memset(tabled, 0, 52 * 4);
        memset(tableg, 0, 52 * 4);
        memset(result, 0, 200010);
        memset(mid, 0, 100005);
        scanf(" %s %s", d, g);
        solve();
        for(int i = 0; i < 200010; ++i){
            if(result[i]) fputc(result[i], stdout);
        }
        //printf("%lld\n", ans);
        fputc('\n', stdout);
    }
    
}
