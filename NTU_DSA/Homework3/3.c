#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int maxEdge = 0;


typedef struct _listNode{
    int val;
    struct _listNode *next;
} listNode;



typedef struct _list{
    listNode *first;
    listNode *end;
} list;

typedef struct _queue{
   listNode *front;
   listNode *end; 
} queue;

struct _edge{
   int from;
   int to; 
} edges[500000];

list adjs[100000] = {{NULL, NULL}};

void list_pushFront(list *adjList,int val) {
    listNode *newNode = calloc(1, sizeof(listNode));
    newNode->val = val;
    newNode->next = NULL; 
    if(adjList->end == NULL) {
        adjList->end = adjList->first = newNode;
        return ; 
    }

    adjList->end->next = newNode;
    adjList->end = newNode;
}

void list_popBack(list *adjList){
    if(adjList->first == NULL) {
        return;
    }
    listNode *deleteNode = adjList->first;
    adjList->first =  adjList->first->next;
    if(deleteNode == adjList->end) adjList->end = NULL;
    free(deleteNode);
    return;
}

static inline int list_peekBack(list *adjList){
    if(adjList->first == NULL) return -1;
    return adjList->first->val;
}

static inline listNode *getNext(listNode *now, listNode *prev) {
    return (listNode *)(((uint64_t) now) ^ ((uint64_t) prev));
}


void queue_push(queue *que, int val){
    listNode *newNode = calloc(1, sizeof(listNode));
    newNode->val = val;
    if(que->end == NULL) {
        que->front = que->end = newNode;
        newNode->next = getNext(NULL, NULL);
        return;
    }
    newNode->next = getNext(NULL, que->end);
    que->end->next = getNext(newNode, getNext(NULL, que->end->next));
    que->end = newNode;
    return; 
}

static inline int queue_peek(queue *);


int queue_pop(queue *que) {
    listNode *deleteNode = que->front;
    int result = queue_peek(que);
    //printf("%d\n", result);
    if(deleteNode == NULL) return -1;
    else if(que->front == que->end){
        free(que->front);
        que->front = que->end = NULL;
        return result;
    }
    que->front = getNext(NULL, que->front->next); 
    que->front->next = getNext(getNext(que->front->next, deleteNode), NULL);
    free(deleteNode);
    return result;
}


static inline int queue_peek(queue *que) {
    if(que->front == NULL) return -1;
    else return que->front->val;
}




int main(){
    int totalEdge = 0,  totalAdj = 0, adj = 0, from = 0, to = 0;
    queue processQue = {NULL, NULL};
    bool has = true;

    scanf(" %d", &totalEdge); 

    for(int i = 0; i < totalEdge; ++i) {
        scanf(" %d", &totalAdj);
        for(int j = 0; j < totalAdj; ++j) {
            scanf(" %d", &adj);
            list_pushFront(&adjs[i], adj);
        } 
    }

    for(int i = 0; i < totalEdge; ++i){
        to = list_peekBack(&adjs[i]);
        if(to == -1)continue;
        from = list_peekBack(&adjs[to - 1]);
        if(from == i + 1){
            list_popBack(&adjs[to - 1]);
            list_popBack(&adjs[from - 1]);
            edges[maxEdge].from = from;
            edges[maxEdge++].to = to;
            queue_push(&processQue, from);
            queue_push(&processQue, to);
            while((from = queue_pop(&processQue)) != -1){
                to = list_peekBack(&adjs[from - 1]);
                //printf("%d %d\n", from, to);
                if(to != -1 && from == list_peekBack(&adjs[to - 1])){
                    list_popBack(&adjs[to - 1]);
                    list_popBack(&adjs[from - 1]);
                    edges[maxEdge].from = from;
                    edges[maxEdge++].to = to;
                    queue_push(&processQue, from);
                    queue_push(&processQue, to);
                }
                //printQue(&processQue);
            
            }


        } 
    }

    for(int i = 0; i < totalEdge; ++i){
        if(list_peekBack(&adjs[i]) != -1){
            has = false;
            break;
        }
    }

    if(!has){
        printf("No\n");
    }else{
        printf("Yes\n");
        for(int i = 0; i < maxEdge; ++i){
            printf("%d %d\n", edges[i].from, edges[i].to);
        }
    }


}
