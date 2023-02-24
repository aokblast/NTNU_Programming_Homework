//
// Created by AoK_Blast on 2021/11/19.
//
#include "UI.h"
#include <vector>
#include <algorithm>


static std::vector<Card> deck;


template<typename eleType> void shuffle(std::vector<eleType> &vec){
    for(int i = 0; i < vec.size(); ++i){
        int j = rand() % vec.size();
        std::swap(vec[j], vec[i]);
    }
}

Card getRandCard(hands &h){
    int idx = rand() % h.size();
    auto iter = h.begin();
    while(idx--)++iter;
    Card res = *iter;
    h.erase(iter);
    return res;
}

std::vector<LinkedList<Card>::iterator> getSameColor(hands &h, int color){
    auto iter = h.begin(), ed = h.end();
    std::vector<LinkedList<Card>::iterator> res;
    while(iter != ed){
        if((*iter).color == color)res.push_back(iter);
        ++iter;
    }
    std::sort(res.begin(), res.end(), [](const LinkedList<Card>::iterator &i1, const LinkedList<Card>::iterator &i2){
        return  (*i1).num < (*i2).num;
    });
    return res;
}

void cardDeletionHandler(UI &u, hands h[2]){
    int cur = 0;
    Card c;
    std::vector<decltype(h[0].begin())> hsort[2];
    while(h[0].size() && h[1].size()){
        c = getRandCard(h[cur]);

        deck.push_back(c);
        hsort[0] = getSameColor(h[0], c.color);
        hsort[1] = getSameColor(h[1], c.color);
        int ccur = 1 - cur;

        u.printMsg("Player " + std::to_string(cur + 1) + " choose " + c.getColorString() + " " + std::to_string(c.num));

        u.printCards(h[0], UI::ALL_ORDER, 0, "Player 1(Before):");
        u.printCards(h[1], UI::ALL_ORDER, 0, "Player 2(Before):");

        while(hsort[ccur].size()){
            auto del = std::find_if(hsort[ccur].begin(), hsort[ccur].end(), [&c](const LinkedList<Card>::iterator &iter){
                    if((*iter).num > c.num)return true;
                    return false;
            });
            if(del == hsort[ccur].end()){
                del = hsort[ccur].begin();
            }

            u.printMsg("Player " + std::to_string(ccur + 1) + " delete " + (*(*del)).getColorString() + " " + std::to_string((*(*del)).num));

            //before erase
            deck.push_back(**del);

            h[ccur].erase(*del);
            hsort[ccur].erase(del);



            ccur = 1 - ccur;
        }
        cur = 1 - cur;
        shuffle(deck);

        u.printCards(h[0], UI::ALL_ORDER, 0, "Player 1(After):");
        u.printCards(h[1], UI::ALL_ORDER, 0, "Player 2(After):");
        if(h[ccur].size()) h[ccur].push_back(deck.back()), deck.pop_back();
    }
    u.printMsg("Winner is player " + std::to_string(h[0].size() ? 2 : 1));
}

int idxtoColor(int i){
    switch(i){
        case 0:
            return 'S';
        case 1:
            return 'H';
        case 2:
            return 'D';
        case 3:
            return 'C';
        default :
            return -1;
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
    LinkedList<Card> arr[2][4];


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
                    arr[cur][h[cur].back().color == 'C' ? 0 : h[cur].back().color == 'D' ?  1 : h[cur].back().color == 'H' ? 2 : 3].push_back(h[cur].back());
                    deck.pop_back();
                }
            }
                break;
            case 1: {
                int i = u.getChoose(colorMenu, "Please choose the color you want: ");
                i = idxtoColor(i);
                if(i == -1)break;
                char c = i;
                u.printCards(h[cur], UI::LIFO_ORDER, c);
            }
                break;
            case 2: {
                int i = u.getChoose(colorMenu, "Please choose the color you want: ");
                i = idxtoColor(i);
                if(i == -1)break;
                char c = i;
                u.printCards(h[cur], UI::FIFO_ORDER, c);
            }
                break;
            case 3: {
                int i = u.getChoose(colorMenu, "Please choose the color you want: ");
                i = idxtoColor(i);
                if(i == -1)break;
                char c = i;
                u.printCards(h[cur], UI::SIZE_ORDER, c);
            }
                break;
            case 4:{
                int i = u.getChoose(colorMenu, "Please choose the color you want: ");
                i = idxtoColor(i);
                if(i == -1)break;
                char c = i;
                int num = u.getInt("Please enter the number you want to search: ");
                Card ca = {c, num};
                auto tmp = h[cur];
                auto dele = tmp.upper_bound(ca);
                if(dele == tmp.end()){
                    auto f = tmp.lower_bound({c, 2});
                    if(f != tmp.end()){
                        u.printCard(*f, "No card, the minimal value is: ");
                    }else{
                        u.printMsg("Card not found!");
                    }
                }else{
                    deck.push_back(*dele);
                    auto del = h[cur].lower_bound(*dele);
                    u.printCard(*del, std::string("Delete Card") + " Color " + (*del).getColorString());
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
                cardDeletionHandler(u, h);
                end = true;
                break;

        }
    }


}
