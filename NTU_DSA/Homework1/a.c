#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#define DEBUG 1
#define MAX 10

typedef struct ListNode{
    int val[MAX];
    int sorted[MAX];
    struct ListNode *magic;
    int top;        
    int order;
} node;

typedef struct {
    node *first;
    node *last;
    int length;
} list;

list seq = {NULL, NULL, 0};
int res, eq;

int cmp(const void *a,const  void *b) {
    int *a_ptr = (int *)a, *b_ptr = (int *)b;
    return *a_ptr > *b_ptr;
}

node *getNext(node *addr1, node*addr2) {
    return (node*) ((uint64_t) addr1 ^ (uint64_t)addr2);
} 

node *createNewNode(){
    node *result = calloc(1, sizeof(node));
    memset(result->sorted, 0, MAX);
    result->top = -1;
    result->order = 1;
    return result;
}

int nodeLength(node *trav) {
    return trav->top + 1;
}

void insertNode(node **prev, node **trav, node **next) {
        if((*prev) == NULL) {
            node *newNode = createNewNode();
            (*trav)->magic = getNext(newNode, getNext(NULL, (*trav)->magic));
             newNode->magic = getNext(NULL, *trav);
            if(seq.first == *trav) seq.first = newNode;
            (*prev) = NULL;
            (*next) = *trav;
            (*trav) = newNode;
            return;
        }
        node *newNode = createNewNode();
        (*prev)->magic = getNext(getNext(*trav, (*prev)->magic), newNode);
        newNode->magic = getNext(*prev, *trav);
        (*trav)->magic = getNext(newNode, *next);
        (*next) = *trav;
        (*trav) = newNode;
}

void push(int num){
    if(nodeLength(seq.last)  == MAX) {
        node *newNode = createNewNode();
        seq.last->magic = getNext(getNext(seq.last->magic, NULL), newNode);
        newNode->magic = getNext(seq.last, NULL);
        seq.last = newNode;
    }
    seq.last->val[++seq.last->top] = num;
    seq.last->sorted[seq.last->top] = num;
    //printf("Len:%d\n", nodeLength(seq.last));
    qsort(seq.last->sorted, nodeLength(seq.last), sizeof(int), cmp);
    ++seq.length;
}

int arrayInsert(int *arr,  int index, int len, int num) {
    int tmp =  arr[index];
    arr[index] = num;
    for(int i = index + 1; i <  len; ++i) {
        int dump = arr[i];
        arr[i] = tmp;
        tmp = dump;
    }
    return tmp;   
}

void arrayDelete(int *arr, int index, int len ) {
    //printf("Index %d Len: %d", index, len);
    for(int i = index; i < len - 1; ++i) {
        arr[i] = arr[i + 1];
    }
}

void nextNode(node **prev, node **trav, node **next) {
    node* tmp = getNext((*trav)->magic, *prev);
    *prev = *trav;
    *trav = tmp;
    *next = getNext((*trav)->magic, *prev);
}

void reverseElement(node *trav, int from, int to) {
    //printf("from: %d to: %d\n", from, to);
    for(int i = 0; i <= (to - from) / 2; ++i)  {
        //printf("a: %d b: %d", i + from, to - i);
        int tmp = trav->val[i + from];
        trav->val[i + from] = trav->val[to - i];
        trav->val[to - i] = tmp;
    }
}

void insert(int index, int num) {
    node *trav  =  seq.first;
    node *prev = NULL;
    node *next = NULL;
    while(getNext(prev, trav->magic) != NULL && (index - nodeLength(trav)) > 0) {
        index -= nodeLength(trav);
        nextNode(&prev, &trav, &next);
    }
    //printf("Index :%d\n", index);
    if(index == 0) {
        insertNode(&prev, &trav, &next);
        trav->val[++trav->top] = num;
        return;
    }
    if(trav->order == -1) reverseElement(trav, 0, nodeLength(trav) - 1), trav->order *= -1;
    int test = arrayInsert(trav->val, index, nodeLength(trav), num);
    if(nodeLength(trav) == MAX) {
        nextNode(&prev, &trav, &next);
        insertNode(&prev, &trav, &next);
    }
    trav->val[++trav->top] = test;
    trav->sorted[trav->top] = test;
    qsort(trav->sorted, nodeLength(trav), 4, cmp);
    ++seq.length;
}

void delete(int index) {
    node *trav  =  seq.first;
    node *prev = NULL;
    node *next = NULL;
    while(getNext(prev, trav->magic) != NULL && (index - nodeLength(trav)) > 0) {
        index -= nodeLength(trav);
        nextNode(&prev, &trav, &next);
    }
    if(trav->order == -1) reverseElement(trav, 0, nodeLength(trav) - 1), trav->order *= -1;
    arrayDelete(trav->val, index, nodeLength(trav));
    arrayDelete(trav->sorted, index, nodeLength(trav));
    --trav->top;
    --seq.length;
}


void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void printNode(node *trav) {
    int start = 0, end = 0;
    if(trav->order == 1) {
        for(int  i = 0; i < nodeLength(trav); ++i) printf("%d ", trav->val[i]);
    }else {
        for(int i = nodeLength(trav) - 1; i >= 0; --i) printf("%d ", trav->val[i]);
    }
    printf("\n");
}

void reverse(int from, int to) {
    node *trav  =  seq.first;
    node *prev = NULL;
    node *next = NULL;
    while(getNext(prev, trav->magic) != NULL && from - nodeLength(trav) >= 0) {
        from -= nodeLength(trav);
        to -= nodeLength(trav);
        nextNode(&prev, &trav, &next);
    }
    int totalEle = to - from + 1;
    int start = 0, over = -1;
    //printf("from %d to %d\n", from, to);
        if((trav->order) == 1) {
            start = from;
            over = (totalEle >= (nodeLength(trav) - from) ? nodeLength(trav) : totalEle + from ) - 1;
        }else if((trav->order) == -1) {
            start = nodeLength(trav) -  (totalEle >= (nodeLength(trav) - from) ? nodeLength(trav): totalEle + from);
            over = nodeLength(trav) - from - 1;
        }
         totalEle -= (over - start + 1);
    printf("start %d over %d\n", start, over);
    node *origtrav = trav, *origprev = prev, *orignext = next;
    if(getNext(trav->magic, prev) != NULL)nextNode(&prev, &trav, &next);
    while(getNext(trav->magic, prev) != NULL && totalEle - nodeLength(trav) > 0) {
        trav->order *= -1;
        totalEle -= nodeLength(trav);
        nextNode(&prev, &trav, &next);
    }
    int start2 = 0, over2 = -1;

        if((trav->order ) == 1) {
            start2 = 0;
            over2 = totalEle - 1;
        }else {
            start2 = nodeLength(trav) - totalEle;
            over2 = nodeLength(trav) - 1;
        }
    //printNode(origtrav);
    //printNode(trav);
    //reverseArrSeg(origtrav->val, trav->val, start, over, start2, over2);
    //printNode(origtrav);
    //printNode(trav);
    memcpy(origtrav->sorted, origtrav->val, nodeLength(origtrav) * sizeof(int));
    memcpy(trav->sorted, trav->val, nodeLength(trav) * sizeof(int));
    qsort(origtrav->sorted,nodeLength(origtrav) , sizeof(int), cmp);
    qsort(trav->sorted, nodeLength(trav), sizeof(int), cmp);
    if(trav != origtrav){
        trav->magic = getNext(orignext, next);
        prev->magic = getNext(getNext(prev->magic, trav), origtrav);
        origtrav->magic = getNext(prev, origprev);
        orignext->magic = getNext(getNext(orignext->magic, origtrav), trav);
    }
}

int binarysearchIndex(int *arr, int num, int len) {
    int low = 0, high = len;
    while(low != high) {
        int mid = (high - low) / 2;
        if(arr[mid] > num) high = mid;
        else if (arr[mid] < high) low = mid;
        else {
                if(res > arr[mid + 1]){
                    res = arr[mid + 1];
                }
                return mid;
            }
        }
    if(res > arr[high])res = arr[high];
    while(arr[high] == num) ++high, ++eq;
    return high;
}

int totalLess(node *prev, node *trav, node *next, int from, int to, int num, int re) {
    res = 21474837;
    int ans = 0, totalEle = to - from  + 1;
    eq = 0;
    //printf("Mid:%d\n", num);
    //printf("Total Ele: %d\n", totalEle);
    //printf("From: %d To: %d\n", from, to);
    if (from == 0 && totalEle >= nodeLength(trav)) {
        ans += binarysearchIndex(trav->sorted, num, nodeLength(trav));
        totalEle -= nodeLength(trav);
    }else {
        int start = 0, over = 0;
        if((trav->order) == 1) {
            start = from;
            over = (totalEle >= (nodeLength(trav) - from) ? nodeLength(trav): totalEle + from);
        }else if((trav->order) == -1) {
            start = nodeLength(trav) -  (totalEle >= (nodeLength(trav) - from) ? nodeLength(trav): totalEle + from);
            over = nodeLength(trav) - from;
        }
        //printf("Start:%d Over:%d\n", start, over);
        for(int i = start; i < over; ++i) {
            if(trav->val[i] < num)  {
                    ++ans;
                }else {
                    if(trav->val[i] == num) ++eq;
                    if(res >= (trav->val[i])) res =trav->val[i];
                }
            }
            //printf("Ans:%d\n", ans);
        totalEle -= nodeLength(trav) - from;
    }
    if(getNext(prev, trav->magic) != NULL)nextNode(&prev, &trav, &next );
    while(getNext(trav->magic, prev) != NULL && totalEle -nodeLength(trav) > 0) {
        ans += binarysearchIndex(trav->sorted, num, nodeLength(trav));
        totalEle -= nodeLength(trav);
        nextNode(&prev, &trav, &next );
    }
    if(totalEle > 0) for(int i  = 0; i < totalEle; ++i) {
        if(trav->val[i] < num) {
            ++ans;
        }else {
            if(trav->val[i] == num) ++eq;
            if(res >= (trav->val[i])) res =trav->val[i];
        }
    }
    if(ans > eq) return ans;
    if(ans + eq > ans) return re;
    return ans;
}

void query(int from, int to, int value){
    node *trav  =  seq.first;
    node *prev = NULL;
    node *next = NULL;
    //printf("Len:%d\n", nodeLength(trav));
    //printf("from - nodeLength(trav): %d\n", from - nodeLength(trav));
    while(getNext(trav->magic, prev) != NULL && from - nodeLength(trav) > 0) {
        from -= nodeLength(trav);
        to -= nodeLength(trav);
        nextNode(&prev, &trav, &next);
        //printf("From :%d To:%d", from, to);
    }
    //printf("From: %d To: %d\n", from, to);
    int low = -100000, high = 100000,mid = 0, ans  = -1;
    //printf("Less than: %d\n",value - 1 );
    while(1) {
        mid = (low + high) / 2;
        ans = totalLess(prev, trav, next, from, to, mid, value - 1);
        printf("Ans:%d Less_than:%d\n", ans, value - 1);
        if(ans > (value - 1)) {
            high= mid;
        }else if (ans < (value - 1)) {
             low = mid;
        }else break;
        //printf("Low:%d High %d Mid %d\n", low, high, mid);
    }
    printf("%d\n", res);
}

void print(){
    node *trav = seq.first, *prev = NULL, *next = NULL;
    if(!DEBUG) return;
    //printf("TotalLen:%d\n", seq.length);
    do{
        //printf("Top:%d \n",trav->top);
        if(trav->order == 1)for(int i = 0; i < nodeLength(trav); ++i) printf("%d ", trav->val[i]);
        else if(trav->order == -1) for(int i = nodeLength(trav) - 1; i >=0; --i) printf("%d ", trav->val[i]);
        nextNode(&prev, &trav,&next );
    }while(trav != NULL);
    printf("\n");
}

void printLast(){
    if(!DEBUG) return;
    //printf("%d\n", seq.last->val[0]);
    for(int i = 0; i < nodeLength(seq.last); ++i) printf("%d ", seq.last->val[i]);
    printf("\n");
}

void printFirst(){
    if(!DEBUG) return;
    for(int i = 0; i < nodeLength(seq.first); ++i) printf("%d ", seq.first->val[i]);
    printf("\n");
}

int main(){
    int length, testCase;
    char operation[10];
    scanf(" %d %d", &length, &testCase);
    seq.first = seq.last = createNewNode();
    seq.first->magic = getNext(NULL, NULL);
    for(int i = 0;i < length; ++i) {
        int tmp = 0;
        scanf(" %d", &tmp);
        push(tmp);
        //print();
    }
    while(testCase--){
        scanf(" %s", operation);
        if(strcmp(operation, "Insert") == 0){
            int index,num;
            scanf(" %d %d", &index, &num);    
            insert(index - 1, num);
        }
        else if(strcmp(operation, "Delete") == 0) {
            int index;
            scanf(" %d", &index);
            delete(index - 1);
        }
        else if(strcmp(operation, "Reverse" ) == 0) {
            int index_from, index_to;
            scanf(" %d %d", &index_from, &index_to);
            reverse(index_from - 1, index_to - 1);
        }else if(strcmp(operation, "Query") == 0) {
            int index_from, index_to, value;
            scanf(" %d %d %d", &index_from, &index_to, &value);
            query(index_from - 1, index_to - 1, value);
        }
        print();
    }
}


