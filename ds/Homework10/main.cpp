#include <string>
#include <iostream>
#include "ExpressionTree.h"

int main() {
    std::string exp;
    std::cout << "Please input your infix expression: ";
    std::getline(std::cin, exp);


    ExpressionTree tree(exp);

    std::cout << "The postfix expression: " <<  tree.str(tree.POSTFIX_ORDER) << '\n';
    std::cout << "The prefix expression: " <<  tree.str(tree.PREFIX_ORDER) << '\n';
    std::cout << "The level-order of expression tree: " << tree.str(tree.LEVEL_ORDER) << '\n';

    char var;
    std::string num;
    std::cout << "Calculator(All variable are default zero):\n";
    while(true){
        std::cout << "Please input your variable: ";
        std::cin >> var;
        if(var == 27){
            break;
        }
        std::cout << "Please input the value: ";
        std::cin >> num;

        if(num == std::string() + char(27)){
            break;
        }
        tree.setVar(var, std::stoi(num));
        std::cout << "Answer: " << tree.calculate() << '\n';

    }
}
