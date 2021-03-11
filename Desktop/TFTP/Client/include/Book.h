//
// Created by tomroz on 13.1.2020.
//

#ifndef BOOST_ECHO_CLIENT_BOOK_H
#define BOOST_ECHO_CLIENT_BOOK_H
#include <string>

class Book {

private:
    std::string BookName;
    std::string genre;
    std::string Lender;
    bool Availabe;

public:
    Book(std::string name, std::string genre);
    void setLender(std::string lender);
    std::string getLender();
    std::string getBookName();
    std::string getGenre();
    bool isAvailable();
    void setAvailabe(bool status);

};

#endif //BOOST_ECHO_CLIENT_BOOK_H
