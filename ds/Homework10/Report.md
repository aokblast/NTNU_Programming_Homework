# Expression Tree Class

# Inner Class

The class of Tree Node

```c++
    class Node{
        Node *left;
        Node *right;
    public:
        char val;
        Node() = default;
        explicit Node(char val);
        friend class ExpressionTree;
    };
```

## Enum

```c++
enum TraversalMode{
    PREFIX_ORDER,
    POSTFIX_ORDER,
    INFIX_ORDER,
    LEVEL_ORDER
};
```
define how tree was traversed

## Constructor

```c++
explicit ExpressionTree(std::string exp); // use given infix expression to construct expression tree
```

## General Method

```c++
std::string str(TraversalMode ts); // return the string of the given traversal mode
int calculate(); // calculate the value of expression by given value
void setVar(char var, int val); // set the value of a given variable(var) with val
```

## Private Method

```c++
int dfs(Node *root); // the inner implementation of int calcuate();
void prefixTraverse(Node *root, std::string &res); // the inner implementation of int str() with PREFIX_ORDER mode
void postfixTraverse(Node *root, std::string &res); // the inner implementation of int str() with POSTFIX_ORDER mode
void infixTraverse(Node *root, std::string &res); // the inner implementation of int str() with INFIX_ORDER mode
void levelTraverse(Node *root, std::string &res); // the inner implementation of int str() with LEVEL_ORDER mode
```

## Static Method

```c++
static std::string infixToPostFix(const std::string &exp);  //turn the infix expression to postfix expression
static void checkExpression(const std::string &exp); //check the validation of infix expression
```

