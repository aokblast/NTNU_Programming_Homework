#include <stdio.h>
#include <stdlib.h>

#define i32 int32_t

struct node{
    i32 value;
    struct node *prev,*next;
};

typedef struct node Node;

Node *createList(i32 *arr,i32 len){
    Node *first,*current,*previous;
    for(int i=0;i<len;i++){
        current=(Node *) malloc(sizeof(Node));
        current->value=arr[i];
        current->prev=NULL;
        if(i==0){
            first=current;
        }else{
            previous->next=current;
            current->prev=previous;
        }
        current->next=NULL;
        previous=current;
    }
    return first;

}
Node *reverseList(Node *first){
    Node *tmp=first;
    first=first->next;
    tmp->next=NULL;
    tmp->prev=first;
    while(first->next!=NULL){
        tmp=first;
        first=first->next;
        tmp->next=tmp->prev;
        first->prev=tmp;
    }
    first->next=tmp;
    first->prev=NULL;
    return first;
}
Node *push_frontList(Node *first,i32 num){
    Node *newfirst=(Node *) malloc(sizeof(Node));
    first->prev=newfirst;
    newfirst->value=num;
    newfirst->prev=NULL;
    newfirst->next=first;
    return newfirst;
}

void push_backList(Node *first,i32 num){
    Node *newlast=(Node *) malloc(sizeof(Node));
    Node *trav=first;
    while(trav->next!=NULL){
        trav=trav->next;
    }
    newlast->value=num;
    newlast->next=NULL;
    newlast->prev=trav;
    trav->next=newlast; 
}

void deleteNode(Node *first,i32 num){
    Node *trav=first;
    Node *tmp;
    while(trav!=NULL){
        if(trav->value==num){
            tmp=trav->next;
            tmp->prev=trav->prev;
            tmp=trav->prev;
            tmp->next=trav->next;
            tmp=trav;
            trav=trav->next;
            free(tmp);  
        }else{
            trav=trav->next;
        
        }
    }
}

void printList(Node *first){
    Node *trav=first;
    if(trav==NULL){
        printf("Empty list!");
    }else{
        while(trav!=NULL){
            printf("%d ",trav->value);
            trav=trav->next; 
        }
        printf("\n");
    }
}
void deleteList(Node *first){
    Node *current,*tmp;
    current=first;
    while(current!=NULL){
        tmp=current;
        current=current->next;
        free(tmp);
    }
}

int main(){
    i32 arr[]={5,1,2,5};
    Node *first=createList(arr,sizeof(arr)/sizeof(int));
    printList(first);
    first=push_frontList(first,15);
    printList(first);
    push_backList(first,20);
    printList(first);
    //printf("%d",first->value);
    deleteNode(first,5);
    printList(first);
    first=reverseList(first);
    printList(first);
    deleteList(first);
}
