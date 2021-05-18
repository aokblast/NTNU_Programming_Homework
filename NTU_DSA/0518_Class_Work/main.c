#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct DisjointSet{
    int parent;
    int rank;
} disjoint_set;

disjoint_set ds[1 << 24];
bool set[1 << 24] = {};

int c2i(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    else if ('a' <= c && c <= 'z') return c - 'a' + 10;
    else if ('A' <= c && c <= 'Z') return c - 'A' + 36;
    return -1;
}

int hash(const char* s) {
    int ret = 0;
    for (int i = 0; i < 4; ++i)
        ret = (ret << 6) | c2i(s[i]);
    return ret;
}

void makeset(const char* s){
    int i = hash(s);
    ds[i].rank = 1;
    ds[i].parent = i;
}

inline void static init(const char* s) {
    int i = hash(s);
    if (!set[i]) {
        makeset(s);
        set[i] = 1;
    }
}

int find_set_with_val(int set) {
    if(ds[set].parent != set) {
        ds[set].parent = find_set_with_val(ds[set].parent);
         
    }
    return ds[set].parent;
}

int find_set(const char* s) {
    init(s);
    int i = hash(s);
    return find_set_with_val(i);
}


void link(const char *ra, const char *rb) {
    int a = find_set(ra), b = find_set(rb);
    if(ds[a].rank > ds[b].rank) ds[a].parent = b;
    else {
        ds[b].parent = a;
        if(ds[a].rank == ds[b].rank) {
            ds[a].rank += 1;
        }
    }
}

bool same_set(const char *a, const char *b) {
    return (find_set(a) == find_set(b));
}

bool stringcompare(const char *a, const char *b) {
    for(int i =  0; a[i] != 0; i += 4) {
        if(!same_set(&a[i], &b[i])) return false;
    }
    return true;
}

int main() {
    int n;
    scanf("%d", &n);
    char cmd[16], a[512], b[512];
    for (int i = 0; i < n; ++i) {
        scanf("%s %s %s", cmd, a, b);
        if (!strcmp(cmd, "union")) {
            init(a);
            init(b);
            link(a, b);
        } else {
            bool same = stringcompare(a, b);
            if(same) puts("True");
            else puts("False");
        }
    }
}
