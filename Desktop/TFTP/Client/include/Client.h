//
// Created by tomroz on 13.1.2020.
//

#ifndef BOOST_ECHO_CLIENT_CLIENT_H
#define BOOST_ECHO_CLIENT_CLIENT_H
#include <unordered_map>
#include <vector>
#include "../include/Messages.h"
#include "../include/Receipt.h"
#include "../include/Book.h"
#include <mutex>


class Client {

private:
    std::string UserName;
    std::string Password;
    std::unordered_map<std::string ,int> TopicsMap;
    std::unordered_map<std::string,std::vector<Book*>> Inventory;
    std::unordered_map<int,Receipt*> WaitingReceipts;
    std::vector<Book*> BorrowList;
    Messages message;
    int SubscriptionID;
    int ReceiptID;
    std::mutex _mtxAddBook;
    std::mutex _mtxGenre;
    std::mutex _mtxBook;


public:
    Client();
    ~Client();
    std::string login (std::string m);
    std::string JoinClub(std::string m);
    std::string ExitClub(std::string m);
    std::string addBook(std::string m);
    std::string BorrowBook(std::string m);
    std::string ReturnBook(std::string m);
    std::string getBookLender(std::string book,std::string genre);
    void eraseBook(std::string book,std::string genre);
    std::string bookStatus(std::string m);
    std::string logout();
    std::string gotReceipt(int r);
    std::string gotMessage(std::string genre,std::string m);
    std::string gotError(std::string message);
    bool gotBook(std::string book,std::string genre);
    bool WishtoBorrow(std::string book);
    std::string ProcessFromKB(std::string message);
    std::string ProcessByService(std::string topic,std::string message);
    void ChangeTrueBook(std::string book, std::string genre);
    bool BorrowBookAgain(std::string book, std::string genre,std::string lender);

};

#endif //BOOST_ECHO_CLIENT_CLIENT_H
