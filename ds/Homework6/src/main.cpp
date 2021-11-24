//
// Created by AoK_Blast on 2021/11/19.
//
#include "UI.h"
#include <vector>





template<typename eleType> void shuffle(std::vector<eleType> &vec){
    for(int i = 0; i < vec.size(); ++i){
        int j = rand() % vec.size();
        std::swap(vec[j], vec[i]);
    }

}

int main(){



    UI u;
    std::vector<UI::Menu> m = {UI::Menu("Get Cards", 5), UI::Menu("Show order", 7),
                               UI::Menu("Show reverse order", 9), UI::Menu("Show by size", 11), UI::Menu("Delete a card", 13),
                               UI::Menu("Abort", 15), UI::Menu("Add another player", 17)};
    std::vector<UI::Menu> colorMenu = {UI::Menu("Spade", 5), UI::Menu("Heart", 7),
                                       UI::Menu("Diamond", 9), UI::Menu("Club", 11), UI::Menu("Go back", 13)};

    bool end = false;
    hands h[2];
    std::vector<Card> deck;
    int cur = 0;

    for(int i = 1; i <= 4; ++i){
        for(int j = 1; j <= 13; ++j){
            deck.push_back({i == 1 ? 'C' : i == 2 ? 'D' : i == 3 ? 'H' : 'S', j});
        }
    }

    std::string title = "Player 1";


    while(!end){
        title[7] = '1' + cur;
        int i = u.getChoose(m, title);
        switch(i){
            case 0: {
                while(h[cur].size())deck.push_back(h[cur].back()), h[cur].pop_back();
                int n = u.getInt("Please enter the number of card you want to add: ");

                if(n == -1)break;

                shuffle(deck);
                while(n--){
                    h[cur].push_back(deck.back());
                    deck.pop_back();

                }
            }
                break;
            case 1: {
                int i = u.getChoose(colorMenu, "Please choose the color you want: ");
                char c;
                bool choose = true;
                switch(i){
                    case 0:
                        c = 'S';
                        break;
                    case 1:
                        c = 'H';
                        break;
                    case 2:
                        c = 'D';
                        break;
                    case 3:
                        c = 'C';
                        break;
                    default:
                        choose = false;
                        break;
                }
                if(choose)u.printCards(h[cur], UI::LIFO_ORDER, c);
            }
                break;
            case 2: {
                int i = u.getChoose(colorMenu, "Please choose the color you want: ");
                char c;
                bool choose = true;
                switch(i){
                    case 0:
                        c = 'S';
                        break;
                    case 1:
                        c = 'H';
                        break;
                    case 2:
                        c = 'D';
                        break;
                    case 3:
                        c = 'C';
                        break;
                    default:
                        choose = false;
                        break;
                }
                u.printCards(h[cur], UI::FIFO_ORDER, c);
            }
                break;
            case 3: {
                int i = u.getChoose(colorMenu, "Please choose the color you want: ");
                char c;
                bool choose = true;
                switch(i){
                    case 0:
                        c = 'S';
                        break;
                    case 1:
                        c = 'H';
                        break;
                    case 2:
                        c = 'D';
                        break;
                    case 3:
                        c = 'C';
                        break;
                    default:
                        choose = false;
                        break;
                }
                u.printCards(h[cur], UI::SIZE_ORDER, c);
            }
                break;
            case 4:{
                int i = u.getChoose(colorMenu, "Please choose the color you want: ");
                char c;
                bool choose = true;
                switch(i){
                    case 0:
                        c = 'S';
                        break;
                    case 1:
                        c = 'H';
                        break;
                    case 2:
                        c = 'D';
                        break;
                    case 3:
                        c = 'C';
                        break;
                    default:
                        choose = false;
                        break;
                }
                if(!choose)break;
                int num = u.getInt("Please enter the number you want to search: ");
                Card ca = {c, num};
                auto tmp = h[cur];
                tmp.sort();
                auto dele = tmp.upper_bound(ca);
                if(dele == tmp.end()){
                    auto f = tmp.lower_bound({c, - 1});
                    if(f != tmp.end()){
                        u.printCard(*f);
                    }else{
                        u.printMsg("Card not found!");
                    }
                }else{
                    deck.push_back(*dele);
                    auto del = h[cur].lower_bound(*dele);
                    h[cur].erase(del);
                }
            }
                break;
            case 5:
                end = true;
                break;
            case 6:
                if(m[6].msg[0] == 'A'){
                    m[6] = UI::Menu("Switch player", 17);
                    m.push_back({"Compare card with two player", 19});
                    cur = 1 - cur;
                }else{
                    cur = 1 - cur;
                }
                break;
            case 7:
                break;

        }
    }


}
