//
// Created by tomroz on 13.1.2020.
//

#ifndef BOOST_ECHO_CLIENT_RECEIPT_H
#define BOOST_ECHO_CLIENT_RECEIPT_H

#include <string>

class Receipt {
private:
    int ID;
    int Action;
    std::string Genre;
    int Value;

public:
    Receipt(int id, int act, std::string genre, int val);
    int getID();
    int getAction();
    std::string getGenre();
    int getValue();
};

#endif //BOOST_ECHO_CLIENT_RECEIPT_H
