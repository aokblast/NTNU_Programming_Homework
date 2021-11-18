//
// Created by blast on 11/16/21.
//

#ifndef HOMEWORK6_LINKEDLIST_H
#define HOMEWORK6_LINKEDLIST_H
#include <iostream>


template<typename NodeType> class LinkedList {
public:
    using size_type = unsigned long;

    LinkedList(): dummyHead(new Node), dummyEnd(new Node){
        dummyHead->next = dummyEnd;
        dummyEnd->prev = dummyHead;
        dummyHead->prev = dummyEnd->next = nullptr;
    }

    const LinkedList &operator=(const LinkedList<NodeType> &list){
        auto lbeg = list.begin(), lend = list.end();
        for(lbeg; lbeg != lend; ++lbeg){
            push_back(*lbeg);
        }
        _size = list.size();
        return *this;
    }

    LinkedList(const LinkedList<NodeType> &list) : LinkedList(){
        for(const NodeType &node : list){
            push_back(node);
        }
        _size = list.size();
    }

    void push_back(const NodeType &_val){
        Node *newNode = new Node(_val);
        add_back(newNode);
    }

    void emplace_back(NodeType &&_val){
        Node *newNode = new Node(_val);
        add_back(newNode);
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
        --_size;
    }

    void push_front(const NodeType &_val){
        Node *newNode = new Node(_val);
        add_front(newNode);
    }

    void emplace_front(NodeType &&_val){
        Node *newNode = new Node(_val);
        add_front(newNode);
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
        --_size;
    }

    bool empty(){
        return dummyHead->next == dummyEnd;
    }

    size_type size() const{
        return _size;
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
        Node() = default;
    };

    void add_front(Node* newNode){
        dummyHead->next->prev =  newNode;
        newNode->next = dummyHead->next;
        dummyHead->next = newNode;
        newNode->prev = dummyHead;
        ++_size;
    }

    void add_back(Node *newNode){
        dummyEnd->prev->next = newNode;
        newNode->prev = dummyEnd->prev;
        dummyEnd->prev = newNode;
        newNode->next = dummyEnd;
        ++_size;
    }

    Node *dummyHead;
    Node *dummyEnd;
    size_type _size = 0;


public:
    class __iterator{
    public:
        const __iterator operator++(int){
            iterator tmp = *this;
            cur = cur->next;
            return tmp;
        }

        __iterator operator++(){
            cur = cur->next;
            return cur;
        }

        const __iterator operator--(int){
            iterator tmp = *this;
            cur = cur->next;
            return tmp;
        }

        __iterator operator--(){
            cur = cur->prev;
            return cur;
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
        friend class LinkedList<NodeType>;
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

    void insert(iterator _node, const NodeType &val) {
        Node *newNode = new Node(val);
        Node *node = _node.cur;
        node->prev->next = newNode;
        newNode->prev = node->prev;
        node->prev = newNode;
        newNode->next = node;
        ++_size;
    }

    iterator lower_bound(const NodeType &node){
        auto ed = end();
        for(auto iter = begin(); iter != ed; ++iter){
            if(*iter <= node){
                return iter;
            }
        }
        return end();
    }

    iterator upper_bound(const NodeType &node){
        auto ed = end();
        for(auto iter = begin(); iter != ed; ++iter){
            if(*iter > node){
                return iter;
            }
        }
        return end();
    }

    void erase(iterator _node){
        Node *node = _node.cur;
        if(!(isErasableNode(node)))return;
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
        --_size;
    }

    void sort(){
        LinkedList<NodeType> tmp = *this;
        __sort(tmp, 0, _size - 1);
    }

private:
    inline bool isErasableNode(Node* iter){
        return (iter != dummyHead) && (iter != dummyEnd);
    }

    void __sort(LinkedList<NodeType> &tmp, size_type left, size_type right){
        if((right - left) <= 1)return;
        size_type mid = left + ((right - left) >> 1);
        __sort(tmp, left, mid);
        __sort(tmp, mid + 1, right);

        size_type lb = left, le = mid, re = right;
        iterator liter = begin(), riter = begin();
        while(lb-- && le-- && re--){
            ++liter;
            ++riter;
        }
        while(le-- && re--){
            ++riter;
        }
        iterator lend = ++riter;
        iterator rend = lend;

        while(re--) ++rend;

        iterator titer = tmp.begin(), lc = liter, rc = rend, tc = tmp.begin();

        while(liter != lend && riter != rend){
            if(*liter < *riter)*titer = *liter, ++liter;
            else *titer = *riter, ++riter;
            ++titer;
        }

        while(liter != lend){
            *titer = *liter;
            ++liter;
            ++titer;
        }

        while(riter != rend){
            *titer = *riter;
            ++riter;
            ++titer;
        }

        while(lc != rc){
            *lc = *tc;
            ++tc;
            ++lc;
        }


    }
};

template<typename NodeType> std::ostream &operator<<(std::ostream &os, const LinkedList<NodeType> &list){
    for(const NodeType &node : list){
        os << node << " ";
    }
    return os;
}

#endif //HOMEWORK6_LINKEDLIST_H
