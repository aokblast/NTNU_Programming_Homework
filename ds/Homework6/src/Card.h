//
// Created by AoK_Blast on 2021/11/22.
//

#ifndef HOMEWORK6_CARD_H
#define HOMEWORK6_CARD_H
#include <unordered_map>
#include "LinkedList.h"



struct Card {

    char color;
    int num;

    bool operator<(const Card &c) const;
    bool operator<=(const Card &c) const;

private:
    static std::unordered_map<char, int> colorc;
    static std::unordered_map<int, int> numc;
};

using hands = LinkedList<Card>;

#endif //HOMEWORK6_CARD_H
