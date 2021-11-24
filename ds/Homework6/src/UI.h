//
// Created by AoK_Blast on 2021/11/20.
//

#ifndef HOMEWORK6_UI_H
#define HOMEWORK6_UI_H
#include "curses.h"
#include "Card.h"
#include <string>
#include <vector>


class UI {


public:

    struct Menu{
        std::string msg;
        int x;
        int y;
        Menu(const std::string &msg, int x, int y) : x(x), y(y), msg(msg){}
    };

    enum mode{
        LIFO_ORDER,
        FIFO_ORDER,
        SIZE_ORDER,
    };


    UI();
    int getInt(const std::string &msg = "Input a integer:");
    char getChar(const std::string &msg = "Input a char:");
    int getChoose(const std::vector<Menu> &chooses);
    void printCards(const hands &h, mode m, char color);
    void printCard(const Card &c);
    void printMsg(const std::string& msg);
    ~UI();

private:
    WINDOW * drawGetFunc(const std::string &msg);

};


#endif //HOMEWORK6_UI_H
