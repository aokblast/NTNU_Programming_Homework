//
// Created by AoK_Blast on 2021/11/20.
//

#include <algorithm>
#include <sstream>
#include "UI.h"


UI::UI() {
    initscr();
    cbreak();
    box(stdscr, '|', '-');
    refresh();
}

int UI::getInt(const std::string &msg) {
    int res = -1;
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



WINDOW * UI::drawGetFunc(const std::string &msg) {
    WINDOW  *newWind = newwin(LINES / 2, COLS / 2, LINES / 4, COLS / 4);

    box(newWind, '|', '-');
    mvwaddstr(newWind, 3, 3, msg.c_str());
    return newWind;
}


int UI::getChoose(const std::vector<Menu> &chooses,  const std::string &msg) {
    WINDOW *newWind = newwin(LINES, COLS, 0, 0);
    keypad(newWind, true);
    int cur = 0;
    int ch;
    clear();
    mvwaddstr(newWind, 3, (COLS - msg.size()) / 2, msg.c_str());

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

void UI::printCards(const hands& h, mode m, char color, const std::string &msg) {
    hands res;

    clear();
    std::for_each(h.begin(), h.end(), [&res, &color](const Card &c){
        if(c.color == color) { res.push_back(c);}
    });

    if(m == ALL_ORDER){

        WINDOW *newWind = newwin(LINES, COLS, 0, 0);

        mvwprintw(newWind, 3, (COLS - msg.size()) / 2, msg.c_str());



        keypad(newWind, true);
        std::stringstream ss;
        auto iter = h.begin(), ed = h.end();
        if(h.size()) ss << (*iter).getColorString() << " " << (*iter).num, ++iter;
        while(iter != ed){
            ss << " -> " << (*iter).getColorString() << " " << (*iter).num, ++iter;
        }
        std::string line = ss.str();


        if(line.size() >= COLS){
            std::string prev(line.begin(), line.begin() + (line.size() / 2));
            mvwprintw(newWind, 5, (COLS - prev.size()) / 2, prev.c_str());
            mvwprintw(newWind, 7, (COLS - line.size() / 2) / 2, line.c_str() + (line.size() / 2));
        }else{
            mvwprintw(newWind, 5, (COLS - line.size()) / 2, line.c_str());
        }


        mvwprintw(newWind, 9, (COLS - 23) / 2, "Press enter to go back:");

        int ch;
        wrefresh(newWind);
        while(true){
            ch = mvwgetch(newWind, 9, (COLS - 23) / 2 + 23);
            if(ch == 10){
                break;
            }
        }
        delwin(newWind);
        return ;
    }

    std::vector<std::pair< std::string, int> > printVal;


    switch(m){
        case LIFO_ORDER:{
            std::for_each(res.begin(), res.end(), [&printVal](const Card &c){printVal.push_back({c.getColorString(), c.num});});
        }
            break;
        case FIFO_ORDER:{
            std::for_each(res.rbegin(), res.rend(), [&printVal](const Card &c){printVal.push_back({c.getColorString(), c.num});});
        }
            break;
        case SIZE_ORDER:{
            res.sort();
            std::for_each(res.begin(), res.end(), [&printVal](const Card &c){printVal.push_back({c.getColorString(), c.num});});
        }
            break;
    }

    clear();
    box(stdscr, '|', '-');
    WINDOW *newWind = newwin(LINES, COLS, 0, 0);
    keypad(newWind, true);
    std::stringstream ss;

    if(printVal.size()) ss << printVal[0].first << " " << printVal[0].second;
    for(int i = 1; i < printVal.size(); ++i){
        ss << " -> " <<  printVal[i].first << " " << printVal[i].second;
    }
    std::string line = ss.str();

    mvwprintw(newWind, 3, (COLS - msg.size()) / 2, msg.c_str());

    mvwprintw(newWind, 5, (COLS - line.size()) / 2, line.c_str());

    mvwprintw(newWind, 7, (COLS - 23) / 2, "Press enter to go back:");

    int ch;
    wrefresh(newWind);
    while(true){
        ch = mvwgetch(newWind, 7, (COLS - 23) / 2 + 23);
        if(ch == 10){
            break;
        }
    }
    delwin(newWind);

}

void UI::printCard(const Card &c, const std::string &msg) {
    clear();
    box(stdscr, '|', '-');
    mvprintw(3, (COLS - msg.size()) / 2, "%s", msg.c_str());
    mvprintw(5, (COLS) / 2, "%d", c.num);
    mvprintw(7, (COLS - 23) / 2, "Press enter to go back:");
    refresh();
    keypad(stdscr, true);
    int ch;
    while(true){
        ch = mvgetch( 7, (COLS - 23) / 2 + 23);
        if(ch == 10){
            break;
        }
    }
}

void UI::printMsg(const std::string &msg) {
    clear();
    box(stdscr, '|', '-');
    mvprintw(5, (COLS - msg.size()) / 2, "%s", msg.c_str());
    mvprintw(7, (COLS - 23) / 2, "Press enter to go back:");
    refresh();
    int ch;
    keypad(stdscr, true);
    while(true){
        ch = mvgetch(7, (COLS - 23) / 2 + 23);
        if(ch == 10){
            break;
        }
    }
}

UI::~UI(){
    endwin();
}

