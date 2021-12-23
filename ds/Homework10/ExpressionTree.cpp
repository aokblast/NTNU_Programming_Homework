//
// Created by aokblast on 2021/12/16.
//

#include "ExpressionTree.h"
#include "LinkedList.h"
#include <iostream>


int level(char op){
    switch(op){
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            break;
    }
    return 0;
}

static std::string infixToPostFix(const std::string &exp) {
    LinkedList<char> opers;
    std::string res;

    for(const char &c : exp){
        switch (c) {
            case '+':
            case '-':
            case '*':
            case '/':
                while(!opers.empty() && opers.front() != '(' && level(opers.front()) >= level(c)){
                    res += opers.front();
                    opers.pop_front();
                }
                opers.push_front(c);
                break;
            case ')':
                while(opers.front() != '('){
                    res += opers.front();
                    opers.pop_front();
                }
                opers.pop_front();
                break;
            case '(':
                opers.push_front('(');
                break;
            default:
                res += c;
                break;
        }
    }
    while(!opers.empty()){
        res += opers.front();
        opers.pop_front();
    }

    return res;

}

static void checkExpression(const std::string &exp){
    int left = 0, right = 0, opers = 0, vars = 0;
    for(const char &c : exp){
        switch(c){
            case '(':
                ++left;
                break;
            case ')':
                ++right;
                if(right > left){
                    throw "Parantheses is not paired";
                }
                break;
            case '+':
            case '-':
            case '*':
            case '/':
                ++opers;
                break;
            default:
                break;
        }
        if(isupper(c)){
            ++vars;
        }
    }
    if(opers + 1 != vars){
        if(opers + 1 > vars ){
            throw "Too much operators";
        }else{
            throw "Too much variable";
        }
    }
    if(left != right){
        throw "Parantheses is not paired";
    }

}

ExpressionTree::Node::Node(char val) : val(val){
    left = right = nullptr;
}

ExpressionTree::ExpressionTree(const std::string &infix) {

    try{
        checkExpression(infix);
    }catch(const char *err){
        std::cout << err << '\n';
        exit(0);
    }

    std::string exp = infixToPostFix(infix);
    LinkedList<Node*> stk;
    for(const char &c : exp){
        if(level(c) != 0){
            root = new Node(c);
            root->right = stk.front(); stk.pop_front();
            root->left = stk.front(); stk.pop_front();
            stk.push_front(root);
        }else{
            root = new Node(c);
            vals[c] = 0;
            stk.push_front(root);
        }
    }
}

void ExpressionTree::setVar(char var, int val) {
    vals[var] = val;
}

int ExpressionTree::calculate() {
    return dfs(root);
}

int ExpressionTree::dfs(Node *root) {
    if(!root)return 0;
    int l = dfs(root->left);
    int r = dfs(root->right);
    if(level(root->val) != 0){
        switch(root->val){
            case '+':
                return l + r;
            case '-':
                return l - r;
            case '*':
                return l * r;
            case '/':
                return l / r;
        }
    }else{
        return vals[root->val];
    }
    return INT_MIN;
}

std::string ExpressionTree::str(TraversalMode ts) {
    std::string res;
    switch(ts){
        case PREFIX_ORDER:
            prefixTraverse(root, res);
            break;
        case POSTFIX_ORDER:
            postfixTraverse(root, res);
            break;
        case INFIX_ORDER:
            infixTraverse(root, res);
            break;
        case LEVEL_ORDER:
            levelTraverse(root, res);
            break;
    }
    return res;
}

void ExpressionTree::prefixTraverse(Node *root, std::string &res) {
    if(!root)return;
    res += root->val;
    prefixTraverse(root->left, res);
    prefixTraverse(root->right, res);
}

void ExpressionTree::postfixTraverse(Node *root, std::string &res) {
    if(!root)return;
    postfixTraverse(root->left, res);
    postfixTraverse(root->right, res);
    res += root->val;
}

void ExpressionTree::infixTraverse(Node *root, std::string &res) {
    if(!root)return;
    infixTraverse(root->left, res);
    res += root->val;
    infixTraverse(root->right, res);
}


void ExpressionTree::levelTraverse(Node *root, std::string &res) {
    LinkedList<Node *> q;
    if(root != nullptr)q.push_front(root);
    while(!q.empty()){
        Node *top = q.back(); q.pop_back();
        res += top->val;
        if(top->left) q.push_front(top->left);
        if(top->right) q.push_front(top->right);
    }
}