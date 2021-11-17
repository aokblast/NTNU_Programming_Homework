//
// Created by blast on 11/16/21.
//

#ifndef HOMEWORK6_LINKEDLIST_H
#define HOMEWORK6_LINKEDLIST_H
#include <iostream>


template<typename NodeType> class LinkedList {
public:
    LinkedList(): dummyHead(new Node), dummyEnd(new Node){
        dummyHead->next = dummyEnd;
        dummyEnd->prev = dummyHead;
        dummyHead->prev = dummyEnd->next = nullptr;
    }

    LinkedList(const LinkedList<NodeType> &list) : LinkedList(){
        for(const NodeType &node : list){
            push_back(node);
        }
    }

    void push_back(const NodeType &_val){
        auto newNode = new Node(_val);
        dummyEnd->prev->next = newNode;
        newNode->prev = dummyEnd->prev;
        dummyEnd->prev = newNode;
        newNode->next = dummyEnd;
    }

    void emplace_back(NodeType &&_val){
        auto newNode = new Node(_val);
        dummyEnd->prev->next = newNode;
        newNode->prev = dummyEnd->prev;
        dummyEnd->prev = newNode;
        newNode->next = dummyEnd;
    }

    NodeType &back(){
        return dummyEnd->prev->val;
    }

    void pop_back(){
        if(empty())return;
        Node *tmp = dummyEnd->prev;
        dummyEnd->prev = dummyEnd->prev->prev;
        dummyEnd->prev->next = dummyEnd;
        delete tmp;
    }

    void push_front(const NodeType &_val){
        auto newNode = new Node(_val);
        dummyHead->next->prev=  newNode;
        newNode->next = dummyHead->next;
        dummyHead->next = newNode;
        newNode->prev = dummyHead;
    }

    void emplace_front(NodeType &&_val){
        auto newNode = new Node(_val);
        dummyHead->next->prev=  newNode;
        newNode->next = dummyHead->next;
        dummyHead->next = newNode;
        newNode->prev = dummyHead;
    }

    NodeType &front(){
        return dummyHead->next->val;
    }

    void pop_front(){
        if(empty())return;
        Node *tmp = dummyHead->next;
        dummyHead->next = dummyHead->next->next;
        dummyHead->next->prev = dummyHead;
        delete tmp;
    }

    bool empty(){
        return dummyHead->next == dummyEnd;
    }

    ~LinkedList(){
        for(Node *node = dummyHead->next; node != dummyEnd; node = node->next){
            delete node->prev;
        }
        delete dummyEnd;
    }



private:
    struct Node{
        NodeType val;
        Node *next = nullptr;
        Node *prev = nullptr;
        Node(const NodeType &_val){
            val = _val;
        }
        Node(NodeType &&_val){
            val = std::move(_val);
        }
        Node(){}
    };


    Node *dummyHead;
    Node *dummyEnd;


public:
    class __iterator{
    public:
        __iterator operator++(){
            iterator tmp = *this;
            cur = cur->next;
            return tmp;
        }

        __iterator operator--(){
            iterator tmp = *this;
            cur = cur->prev;
            return tmp;
        }

        bool operator==(const __iterator &iter) const{
            return cur == iter.cur;
        }

        bool operator!=(const __iterator &iter) const {
            return cur != iter.cur;
        }

        NodeType &operator*(){
            return cur->val;
        }
        __iterator(Node *_node){
            cur = _node;
        }

        __iterator(){}
    private:
        Node* cur;

    };
    using iterator = __iterator;
    iterator begin() const{
        return iterator(dummyHead->next);
    }

    iterator end() const {
        return iterator(dummyEnd);
    }

    void insert(iterator iter, const NodeType &val) {
        Node *newNode = new Node(val);
        iter->prev->next = newNode;
        newNode->prev = iter->prev;
        iter->prev = newNode;
        newNode->next = iter;
    }
    void erase(iterator node){
        if(!(isErasableNode(node)))return;
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
    }

private:
    inline bool isErasableNode(iterator iter){
        return (iter != dummyHead) && (iter != dummyEnd);
    }
};

template<typename NodeType> std::ostream &operator<<(std::ostream &os, const LinkedList<NodeType> &list){
    for(const NodeType &node : list){
        os << node << " ";
    }
    return os;
}

#endif //HOMEWORK6_LINKEDLIST_H
