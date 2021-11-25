//
// Created by AoK_Blast on 2021/11/22.
//

#include "Card.h"

bool Card::operator<(const Card &c) const {
    return colorc[color] == colorc[c.color] ? numc[num] < numc[c.num] : colorc[color] < numc[c.color];
}

bool Card::operator<=(const Card &c) const {
    return colorc[color] == colorc[c.color] ? numc[num] <= numc[c.num] : colorc[color] <= numc[c.color];
}

std::string Card::getColorString() {
    return color == 'C' ? "Club" : color == 'D' ? "Diamond" : color == 'H' ? "Heart" : "Spade";
}

std::unordered_map<char, int> Card::colorc = {{'C', 1}, {'D', 2}, {'H', 3}, {'S', 4}};
std::unordered_map<int, int> Card::numc = {{2, 1}, {3, 2}, {4, 3}, {5, 4}, {6, 5}, {7, 6},
                                           {8, 7}, {9, 8}, {10, 9}, {11, 10}, {12, 11}, {13, 12}, {1, 13}};