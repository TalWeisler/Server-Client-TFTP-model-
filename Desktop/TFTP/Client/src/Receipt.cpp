//
// Created by tomroz on 8.1.2020.
//

#include "../include/Receipt.h"

//actions:
//1-join
//2-exit
//3-logout

Receipt::Receipt(int id, int act, std::string genre, int val):ID(id),Action(act),Genre(genre),Value(val) {}

int Receipt::getID() { return ID;}

int Receipt::getAction() { return Action;}

std::string Receipt::getGenre() { return Genre;}

int Receipt::getValue() { return Value;}//
// Created by tomroz on 13.1.2020.
//

