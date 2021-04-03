// XOR Linked List reference:http://www.tastones.com/zh-tw/stackoverflow/data-structures/linked-list/xor_linked_list/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef struct ListNode{
    int val;
    struct ListNode *magic;
    struct ListNode *last;
} node;

node *rails[1000000] = {NULL};

node *getNext(node *addr1, node*addr2) {
    return (node*) ((uint64_t) addr1 ^ (uint64_t)addr2);
} 

void push(int railNum, int num) {
    if(rails[railNum] == NULL) {
        rails[railNum] = calloc(1, sizeof(node));
        rails[railNum]->val = num;
        rails[railNum]->magic = getNext(NULL, NULL);
        rails[railNum]->last = rails[railNum];
        return;
    }
    node *trav = rails[railNum]->last, *prev = getNext(trav->magic, NULL);
    node *newNode = calloc(1, sizeof(node));
    newNode->val = num;
    trav->magic = getNext(prev, newNode);
    newNode->magic = getNext(trav, NULL);
    rails[railNum]->last = newNode;
}

int pop(int railNum) {
    if(rails[railNum] == NULL) return 0;
    else if(getNext(rails[railNum]->magic, NULL) == NULL) {
        node *tmp = rails[railNum];
        int result = tmp->val;
        free(tmp);
        rails[railNum] = NULL;
        return result;
    }
    node *trav = rails[railNum]->last, *prev = getNext(trav->magic, NULL);
    prev->magic = getNext(getNext(prev->magic, trav), NULL);
    int result = trav->val;
    free(trav);
    rails[railNum]->last = prev;
    return result;
}

void printList(int railNum) {
    node *trav = rails[railNum], *prev = NULL, *tmp;
    while(trav != NULL) {
        printf("%d ", trav->val);
        tmp = getNext(trav->magic, prev);
        prev = trav;
        trav = tmp;
    }
}

void reverse(int railNum) {
    if(rails[railNum] == NULL || getNext(rails[railNum]->magic, NULL) == NULL) return;
    node *trav = rails[railNum]->last, *prev = getNext(trav->magic, NULL);
    node *tmp = rails[railNum];
    rails[railNum] = trav;
    rails[railNum]->last = tmp;
}

void migrate(int from, int to) {
    if(rails[from] == NULL) return;
    reverse(from);
    if(rails[to] == NULL) {
        rails[to] = rails[from];
        rails[from] = NULL;
        return;
    }
    node *trav = rails[to]->last, *prev = getNext(trav->magic, NULL);
    rails[from]->magic = getNext(getNext(NULL, rails[from]->magic), trav);
    trav->magic = getNext(prev, rails[from]);
    rails[to]->last = rails[from]->last;
    rails[from] = NULL;
}

int main(){
    int label = 0, railNum = 0, railNum2 = 0 , total = 0, totalOper= 0;
    scanf(" %d %d", &total, &totalOper);
    char command[100];
    while(totalOper--) {
        scanf(" %s", command);
        if(strcmp(command, "enter") == 0) {
            scanf(" %d %d", &railNum, &label);
            push(railNum, label);
        }else if(strcmp(command, "leave") == 0) {
            scanf(" %d", &railNum);
            pop(railNum);
        }else {
            scanf(" %d %d", &railNum, &railNum2);

            migrate(railNum, railNum2);
        }  

    }
        for(int i = 0; i < total; ++i) {
            printList(i);
            printf("\n");
        }
    
}


/*

// XOR Linked List reference:http://www.tastones.com/zh-tw/stackoverflow/data-structures/linked-list/xor_linked_list/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef struct ListNode{
    int val;
    struct ListNode *magic;
} node;

node *rails[1000000] = {NULL};

node *getNext(node *addr1, node*addr2) {
    return (node*) ((uint64_t) addr1 ^ (uint64_t)addr2);
} 

void push(int nodeNum, int num) {
    if(rails[nodeNum] == NULL) {
        rails[nodeNum] = calloc(1, sizeof(node));
        rails[nodeNum]->val = num;
        rails[nodeNum]->magic = (node *)((uint64_t)NULL ^ (uint64_t) NULL);
        return;
    }
    node *trav = rails[nodeNum], *prev = NULL, *tmp;
    while(getNext(trav->magic, prev) != NULL) {
        tmp = getNext(trav->magic, prev);
        prev = trav;
        trav = tmp;
    }
    node *newNode = calloc(1, sizeof(node));
    newNode->val = num;
    trav->magic = getNext(prev, newNode);
    newNode->magic = getNext(trav, NULL);
}

int pop(int railNum) {
    if(rails[railNum] == NULL) return 0;
    else if(getNext(rails[railNum]->magic, NULL) == NULL) {
        node *tmp = rails[railNum];
        int result = tmp->val;
        free(tmp);
        rails[railNum] = NULL;
        return result;
    }
    node *trav = rails[railNum], *prev = NULL, *tmp;
    while(getNext(trav->magic, prev) != NULL) {
        tmp = getNext(trav->magic, prev);
        prev = trav;
        trav = tmp;
    }
    prev->magic = getNext(getNext(prev->magic, trav), NULL);
    int result = trav->val;
    free(trav);
    return result;
}

void printList(int railNum) {
    node *trav = rails[railNum], *prev = NULL, *tmp;
    while(trav != NULL) {
        printf("%d ", trav->val);
        tmp = getNext(trav->magic, prev);
        prev = trav;
        trav = tmp;
    }
}

void reverse(int railNum) {
    if(rails[railNum] == NULL || getNext(rails[railNum]->magic, NULL) == NULL) return;
    node *trav = rails[railNum], *prev = NULL, *tmp;
    while(getNext(trav->magic, prev) != NULL) {
        tmp = getNext(trav->magic, prev);
        prev = trav;
        trav = tmp;
    }
    rails[railNum] = trav;
}

void migrate(int from, int to) {
    if(rails[from] == NULL) return;
    if(rails[to] == NULL) {
        rails[to] = rails[from];
        rails[from] = NULL;
        return;
    }
    node *trav = rails[to], *prev = NULL, *tmp;
    while(getNext(trav->magic, prev) != NULL) {
        tmp = getNext(trav->magic, prev);
        prev = trav;
        trav = tmp;
    }
    rails[from]->magic = getNext(getNext(NULL, rails[from]->magic), trav);
    trav->magic = getNext(prev, rails[from]);
    rails[from] = NULL;
}

int main(){
    int label = 0, railNum = 0, railNum2 = 0 , total = 0, totalOper= 0;
    scanf(" %d %d", &total, &totalOper);
    char command[100];
    while(totalOper--) {
        scanf(" %s", command);
        if(strcmp(command, "enter") == 0) {
            scanf(" %d %d", &railNum, &label);
            push(railNum, label);
        }else if(strcmp(command, "leave") == 0) {
            scanf(" %d", &railNum);
            pop(railNum);
        }else {
            scanf(" %d %d", &railNum, &railNum2);
            reverse(railNum);
            migrate(railNum, railNum2);
        }
    }
    for(int i = 0; i < total; ++i) {
        printList(i);
        printf("\n");
    }
}

*/
