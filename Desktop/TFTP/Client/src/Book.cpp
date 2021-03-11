//
// Created by tomroz on 13.1.2020.
//

#include "../include/Book.h"

Book::Book(std::string name, std::string genre):BookName(name),genre(genre),Lender("notBorrow"),Availabe(true) {}

void Book::setLender(std::string lender) {Lender=lender;}

std::string Book::getLender() { return Lender;}

std::string Book::getBookName() { return BookName;}

std::string Book::getGenre() {return genre;}

bool Book::isAvailable() { return Availabe;}

void Book::setAvailabe(bool status) {Availabe=status;}