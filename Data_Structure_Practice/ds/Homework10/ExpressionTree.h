//
// Created by aokblast on 2021/12/16.
//

#ifndef HOMEWORK10_EXPRESSIONTREE_H
#define HOMEWORK10_EXPRESSIONTREE_H
#include <string>
#include <unordered_map>

class ExpressionTree {
public:
    enum TraversalMode{
        PREFIX_ORDER,
        POSTFIX_ORDER,
        INFIX_ORDER,
        LEVEL_ORDER
    };

    explicit ExpressionTree(const std::string &exp);
    std::string str(TraversalMode ts);
    int calculate();
    void setVar(char var, int val);


private:
    class Node{
        Node *left;
        Node *right;
    public:
        char val;
        Node() = default;
        explicit Node(char val);
        friend class ExpressionTree;
    };

    int dfs(Node *root);
    void prefixTraverse(Node *root, std::string &res);
    void postfixTraverse(Node *root, std::string &res);
    void infixTraverse(Node *root, std::string &res);
    void levelTraverse(Node *root, std::string &res);


    Node *root = nullptr;
    int vals[128] = {0};


};


#endif //HOMEWORK10_EXPRESSIONTREE_H
