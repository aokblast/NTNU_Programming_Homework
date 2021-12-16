//
// Created by aokblast on 2021/12/16.
//

#include "ExpressionTree.h"
#include <stack>
#include <queue>

static std::unordered_map<char, int> level = {{'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}};

static std::string infixToPostFix(const std::string &exp) {
    std::stack<char> opers;
    std::string res;

    for(const char &c : exp){
        switch (c) {
            case '+':
            case '-':
            case '*':
            case '/':
                while(!opers.empty() && opers.top() != '(' && level[opers.top()] >= level[c]){
                    res += opers.top();
                    opers.pop();
                }
                opers.push(c);
                break;
            case ')':
                while(opers.top() != '('){
                    res += opers.top();
                    opers.pop();
                }
                opers.pop();
                break;
            case '(':
                opers.push('(');
                break;
            default:
                res += c;
                break;
        }
    }
    while(!opers.empty()){
        res += opers.top();
        opers.pop();
    }

    return res;

}

ExpressionTree::Node::Node(char val) : val(val){
    left = right = nullptr;
}

ExpressionTree::ExpressionTree(std::string exp) {
    exp = infixToPostFix(exp);
    std::stack<Node*> stk;
    for(const char &c : exp){
        if(level.find(c) != level.end()){
            root = new Node(c);
            root->right = stk.top(); stk.pop();
            root->left = stk.top(); stk.pop();
            stk.push(root);
        }else{
            root = new Node(c);
            vals[c] = 0;
            stk.push(root);
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
    if(level.find(root->val) != level.end()){
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
    std::queue<Node *> q;
    if(root != nullptr)q.push(root);
    while(!q.empty()){
        Node *top = q.front(); q.pop();
        res += top->val;
        if(top->left) q.push(top->left);
        if(top->right) q.push(top->right);
    }
}