#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct _trieNode{
    struct _trieNode *next[36];
    struct _trieNode *fail;
    int num;
} trieNode;

typedef struct _trie{
    trieNode *root;
} trie;

typedef struct _queueNode{
    trieNode *val;
    struct _queueNode *next;
} queueNode;

typedef struct _trieQueue{
    queueNode *front;
    queueNode *back;
}trieQueue;

trieQueue *init_queue(){
    trieQueue *result = calloc(1, sizeof(trieQueue));
    return result;
}

static inline queueNode * getNext(queueNode *prev, queueNode *next){
    return (queueNode *)(((uint64_t)prev) ^ ((uint64_t)next));
}

static inline bool empty_que(trieQueue *que){
    return (que->front == NULL);
}

void push(trieQueue *que, trieNode *node){
    queueNode *newNode = calloc(1, sizeof(queueNode));
    newNode->val = node;
    if(que->back == NULL){
        newNode->next = getNext(NULL, NULL);
        que->front = que->back = newNode;
        return;
    }

    newNode->next = getNext(NULL, que->back);
    que->back->next = getNext(newNode, getNext(NULL, que->back->next));
    que->back = newNode;
}

trieNode *pop(trieQueue *que){
    trieNode *result = que->front->val;
    if(que->back == que->front){
        que->front = que->back = NULL;
        return result;
    }
    queueNode *prev = getNext(que->front->next, NULL);
    prev->next = getNext(getNext(que->front, prev->next), NULL);
    que->front = prev;

    return result;
}

trieNode *newNode(){
    trieNode *result = calloc(1, sizeof(trieNode));
    return result;
}

trie* initTrie(){
    trie *result = calloc(1, sizeof(trie));
    result->root = newNode();
    return result;
} 

void add_pattern(trieNode * trie, char *str){
    if(*str == '\0'){
        return ;
    }else{
        int nodeNum = (isdigit(*str) ? (*str - '0') : (tolower(*str) - 'a' + 10));
        if(trie->next[nodeNum] == NULL){
            trie->next[nodeNum] = newNode();
        }
        if((*(str + 1)) == '\0') {
            ++trie->next[nodeNum]->num;
            return;
        }
        add_pattern(trie->next[nodeNum], str + 1);
    }
}

int find_pattern(trieNode * trie, char *str){ // for trie test
    int nodeNum = (isdigit(*str) ? (*str - '0') : (tolower(*str) - 'a' + 10));
    if(trie->next[nodeNum] == NULL) return 0;
    if(*(str + 1) == '\0') return trie->next[nodeNum]->num;
    return find_pattern(trie->next[nodeNum], str + 1);
}

void build_failure(trie *trie){
    trieQueue *que = init_queue();

    for(int i = 0; i < 36; ++i){
        if(trie->root->next[i] != NULL) {
            push(que, trie->root->next[i]);
            trie->root->next[i]->fail = trie->root;
        }
        
    }

    while(!empty_que(que)){
        trieNode *front = pop(que);
        for(int i = 0; i < 36; ++i){ 
        
            if(front->next[i] != NULL){
                trieNode *fail = front->fail;
                while(fail != trie->root){
                    if(fail->next[i] != NULL){
                        front->next[i]->fail = fail->next[i];
                        break;
                    }
                    fail = fail->fail;
                }
                if(fail == trie->root){
                    front->next[i]->fail = trie->root;
                }
                push(que, front->next[i]);
                
            }
            
        }

    }
}


int main(){
    trie* trie = initTrie();
    add_pattern(trie->root, "abc123");
    add_pattern(trie->root, "abc123");
    add_pattern(trie->root, "acd");
    add_pattern(trie->root, "ace");
    build_failure(trie);
}