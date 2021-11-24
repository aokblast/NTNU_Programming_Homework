//
// Created by AoK_Blast on 2021/11/20.
//

#include <algorithm>
#include "UI.h"


UI::UI() {
    initscr();
    cbreak();
    box(stdscr, '|', '-');
    refresh();
}

int UI::getInt(const std::string &msg) {
    int res = 0;
    clear();
    WINDOW *newWind = drawGetFunc(msg);

    keypad(newWind, true);

    wrefresh(newWind);

    mvwscanw(newWind, 3, 3 + msg.size() + 1, " %d", &res);

    delwin(newWind);
    clear();
    box(stdscr, '|', '-');
    refresh();
    return res;
}


char UI::getChar(const std::string &msg) {
    char res = 0;
    WINDOW *newWind = drawGetFunc(msg);

    keypad(newWind, true);
    mvwscanw(newWind, 3, 3 + msg.size() + 1, " %c", &res);
    wrefresh(newWind);

    delwin(newWind);
    clear();
    box(stdscr, '|', '-');
    refresh();
    return res;
}

WINDOW * UI::drawGetFunc(const std::string &msg) {
    WINDOW  *newWind = newwin(20, 80, 5, 5);

    box(newWind, '|', '-');
    mvwaddstr(newWind, 3, 3, msg.c_str());
    return newWind;
}


int UI::getChoose(const std::vector<Menu> &chooses) {
    WINDOW *newWind = newwin(LINES, COLS, 0, 0);
    keypad(newWind, true);
    int cur = 0;
    int ch;
    do{
        for(const auto &c : chooses){
            mvwaddstr(newWind, c.x, c.y, c.msg.c_str());
        }

        wattron(newWind, A_REVERSE);
        mvwaddstr(newWind, chooses[cur].x, chooses[cur].y, chooses[cur].msg.c_str());
        wattroff(newWind, A_REVERSE);

        move(chooses[cur].x, chooses[cur].y);

        wrefresh(newWind);

        ch = wgetch(newWind);

        switch(ch){
            case KEY_UP:
                if(cur > 0) --cur;
                break;
            case KEY_DOWN:
                if(cur < chooses.size() - 1) ++cur;
                break;
            case 10:
                delwin(newWind);
                return cur;
        }

    }while(true);
}

void UI::printCards(const hands& h, mode m, char color) {
    hands res;


    std::for_each(h.begin(), h.end(), [&res, &color](const Card &c){
        if(c.color == color) { res.push_back(c);}
    });

    std::vector<int> printVal;


    switch(m){
        case LIFO_ORDER:{
            std::for_each(res.begin(), res.end(), [&printVal](const Card &c){printVal.push_back(c.num);});
        }
            break;
        case FIFO_ORDER:{
            std::for_each(res.rbegin(), res.rend(), [&printVal](const Card &c){printVal.push_back(c.num);});
        }
            break;
        case SIZE_ORDER:{
            res.sort();
            std::for_each(res.rbegin(), res.rend(), [&printVal](const Card &c){printVal.push_back(c.num);});
        }
            break;
    }

    clear();
    box(stdscr, '|', '-');
    WINDOW *newWind = newwin(20, 50, 5, COLS / 3);
    keypad(newWind, true);
    if(printVal.size()) mvwprintw(newWind, 5, 0, "%d", printVal[0]);
    for(int i = 1; i < printVal.size(); ++i){
        mvwprintw(newWind, 5, 6 * i, "->%d", printVal[i]);
    }
    mvwprintw(newWind, 7, 0, "Press enter to go back:");

    int ch;
    wrefresh(newWind);
    while(true){
        ch = mvwgetch(newWind, 7, 23);
        if(ch == 10){
            break;
        }
    }

}

void UI::printCard(const Card &c) {
    clear();
    box(stdscr, '|', '-');
    mvprintw(5, 0, "%d", c.num);
    mvprintw(7, 0, "Press enter to go back:");
    refresh();
    int ch;
    while(true){
        ch = mvgetch( 7, 23);
        if(ch == '\r'){
            break;
        }
    }
}

void UI::printMsg(const std::string &msg) {
    clear();
    box(stdscr, '|', '-');
    mvprintw(5, COLS / 2.5, "%s", msg.c_str());
    mvprintw(7, COLS / 2.5, "Press enter to go back:");
    refresh();
    int ch;
    keypad(stdscr, true);
    while(true){
        ch = mvgetch(7, COLS / 2.5 + 23);
        if(ch == 10){
            break;
        }
    }
}

UI::~UI(){
    endwin();
}

