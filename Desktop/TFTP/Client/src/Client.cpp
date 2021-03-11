//
// Created by tomroz on 13.1.2020.
//

#include "../include/Client.h"
#include "../include/Book.h"
#include <iostream>

Client::Client():UserName(""),Password(""),TopicsMap(),Inventory(),WaitingReceipts(),BorrowList(),message(),SubscriptionID(1),ReceiptID(1),_mtxAddBook(),_mtxGenre(),_mtxBook(){}

Client::~Client() {
    for (std::unordered_map<std::string,std::vector<Book*>>::iterator it1=Inventory.begin(); it1 != Inventory.end(); ++it1)
    {
        for (std::vector<Book*>::iterator it2=Inventory.at(it1->first).begin(); it2 != Inventory.at(it1->first).end(); ++it2){
            delete (*it2);
        }
        Inventory.at(it1->first).clear();
    }
    Inventory.clear();
    TopicsMap.clear();

    for (std::vector<Book*>::iterator it=BorrowList.begin(); it != BorrowList.end(); ++it) {
        delete (*it);
    }
    BorrowList.clear();

    for (std::unordered_map<int,Receipt*>::iterator it=WaitingReceipts.begin(); it != WaitingReceipts.end(); ++it)
    {
        delete (*it).second;
    }
    WaitingReceipts.clear();
}
//***functions handle with KB:***

std::string Client::getBookLender(std::string book, std::string genre) {
    if (Inventory.find(genre) != Inventory.end()) {
        for (std::vector<Book*>::iterator it = Inventory.at(genre).begin(); it != Inventory.at(genre).end(); ++it) {
            if ((*it)->getBookName() == book)
                return (*it)->getLender();
        }
    }
    return "";
}

void Client::eraseBook(std::string book, std::string genre) {
    for (std::vector<Book*>::iterator it=Inventory.at(genre).begin(); it != Inventory.at(genre).end(); ++it)
    {
        if((*it)->getBookName()==book) {
            std::lock_guard<std::mutex>lock(_mtxBook);
            (*it)->setAvailabe(false);
            return;
        }
    }
}

bool Client::gotBook(std::string book, std::string genre) {
    for (std::vector<Book*>::iterator it=Inventory.at(genre).begin(); it != Inventory.at(genre).end(); ++it)
    {
        if((*it)->getBookName()==book)
        {
            std::lock_guard<std::mutex>lock(_mtxBook);
            if ((*it)->isAvailable())
            return true;
        }
    }
    return false;
}

bool Client::BorrowBookAgain(std::string book, std::string genre,std::string lender) {
    if(Inventory.find(genre)!=Inventory.end()) {
        for (std::vector<Book *>::iterator it = Inventory.at(genre).begin(); it != Inventory.at(genre).end(); ++it) {
            if ((*it)->getBookName() == book) {
                std::lock_guard<std::mutex> lock(_mtxBook);
                (*it)->setAvailabe(true);
                (*it)->setLender(lender);
                return true;
            }
        }
    }
    return false;
}

std::string Client::login(std::string m) {
    int place1 = m.find(' ');
    int place2 = m.find(' ', place1 + 1);
    std::string host = m.substr(0, place1);
    std::string UN = m.substr(place1 + 1, place2 - place1 - 1);
    std::string pass = m.substr(place2 + 1);
    UserName = UN;
    Password = pass;
    std::string request=message.connect(host,UN,pass);
    return request;
}

std::string Client::JoinClub(std::string m) {
    std::string request=message.subscribe(m,SubscriptionID,ReceiptID);
    Receipt *r= new Receipt(ReceiptID,1,m,SubscriptionID);
    WaitingReceipts.insert(std::pair<int,Receipt*>(ReceiptID,r));
    SubscriptionID=SubscriptionID+1;
    ReceiptID=ReceiptID+1;
    return request;
}

std::string Client::ExitClub(std::string m) {
    std::string request=message.unsubscribe(TopicsMap.at(m),ReceiptID);
    Receipt *r= new Receipt(ReceiptID,2,m,0);
    WaitingReceipts.insert(std::pair<int,Receipt*>(ReceiptID,r));
    ReceiptID=ReceiptID+1;
    return request;
}

std::string Client::addBook(std::string m) {
    std::string genre=m.substr(0,m.find(' '));
    std::string bookName=m.substr(m.find(' ')+1);
    Book *b= new Book(bookName,genre);
    std::lock_guard<std::mutex>lock(_mtxAddBook);
    if(Inventory.find(genre)==Inventory.end())
    {
        Inventory.insert(std::pair<std::string,std::vector<Book*>>(genre, std::vector<Book*>()));
    }
    Inventory.at(genre).push_back(b);
    if(TopicsMap.find(genre)!=TopicsMap.end()) {
        std::string text = UserName + " has added the book " + bookName;
        std::string request = message.send(genre, text);
        return request;
    }
    else
        return "the book was added to the invntory but not send to the client";
}

std::string Client::BorrowBook(std::string m) {
    std::string genre=m.substr(0,m.find(' '));
    std::string bookName=m.substr(m.find(' ')+1);
    if(TopicsMap.find(genre)!=TopicsMap.end()){
        Book *b=new Book(bookName,genre);
        BorrowList.push_back(b);
        std::string text=UserName+" wish to borrow "+bookName;
        std::string request=message.send(genre,text);
        return request;

    } else
        return "You are not subscribe to this genre";
}

std::string Client::ReturnBook(std::string m) {
    std::string genre=m.substr(0,m.find(' '));
    std::string bookName=m.substr(m.find(' ')+1);
    if (Inventory.find(genre)!=Inventory.end() && gotBook(bookName,genre)) {
        std::string lender = getBookLender(bookName, genre);
        eraseBook(bookName, genre);
        if(TopicsMap.find(genre)!=TopicsMap.end()) {
            if (lender == "")
                return "Cannot find the book";
            else if (lender == "notBorrow")
                return "You own the book";
            else {
                std::string text = "Returning " + bookName + " to " + lender;
                std::string request = message.send(genre, text);
                return request;
            }
        } else
            return "You are not subscribe to this genre";
    }
    else
        return "Cannot find the book";
}

void Client::ChangeTrueBook(std::string book, std::string genre) {
    for (std::vector<Book*>::iterator it=Inventory.at(genre).begin(); it != Inventory.at(genre).end(); ++it) {
        if ((*it)->getBookName() == book) {
            std::lock_guard<std::mutex>lock(_mtxBook);
            (*it)->setAvailabe(true);
            return;

        }
    }
}

std::string Client::bookStatus(std::string m) {
    if(TopicsMap.find(m)!=TopicsMap.end())
    {
        return message.send(m,"book status");
    }
    else
        return "You are not subscribe to this genre";
}

std::string Client::logout() {
    std::string request=message.disconnect(ReceiptID);
    Receipt *r= new Receipt(ReceiptID,3,"",0);
    WaitingReceipts.insert(std::pair<int,Receipt*>(ReceiptID,r));
    ReceiptID=ReceiptID+1;
    return request;
}

std::string Client::ProcessFromKB(std::string message) {
    std::string request="";
    std::string Action=message.substr(0,message.find(' '));
    message=message.substr(message.find(' ')+1);
    message=message.substr(0,message.length());
    if(Action=="login")
        request = login(message);
    else if(Action=="join")
        request=JoinClub(message);
    else if(Action=="exit")
        request=ExitClub(message);
    else if(Action=="add") {
        request = addBook(message);
        if(request.substr(0,request.find('\n'))!="SEND")
        {
            std::cout<<request<<std::endl;
            request="";
        }
    }
    else if(Action=="borrow") {
        request = BorrowBook(message);
        if(request.substr(0,request.find('\n'))!="SEND")
        {
            std::cout<<request<<std::endl;
            request="";
        }
    }
    else if(Action=="return") {
        request = ReturnBook(message);
        if(request.substr(0,request.find('\n'))!="SEND")
        {
            std::cout<<request<<std::endl;
            request="";
        }
    }
    else if(Action=="status") {
        request = bookStatus(message);
        if (request.substr(0, request.find('\n')) != "SEND") {
            std::cout << request << std::endl;
            request = "";
        }
    }
    else if(Action=="logout") {
        request = logout();
    }
    return request;
}


//***functions handle with Server:***

std::string Client::gotReceipt(int r) {
    std::string request="";
    if(WaitingReceipts.find(r)!=WaitingReceipts.end())
    {
        int Action=(*WaitingReceipts.at(r)).getAction();
        switch (Action)
        {
            case 1:
            {
                std::lock_guard<std::mutex>lock(_mtxGenre);
                TopicsMap.insert(std::pair<std::string,int>((*WaitingReceipts.at(r)).getGenre(),(*WaitingReceipts.at(r)).getValue()));
                std::cout<<"Joined club "+(*WaitingReceipts.at(r)).getGenre()<<std::endl;
                delete WaitingReceipts.at(r);
                WaitingReceipts.erase(r);
                break;
            }
            case 2:
            {
                std::lock_guard<std::mutex>lock(_mtxGenre);
                if(TopicsMap.find((*WaitingReceipts.at(r)).getGenre())!=TopicsMap.end())
                    TopicsMap.erase((*WaitingReceipts.at(r)).getGenre());
                std::cout<<"Exited club "+(*WaitingReceipts.at(r)).getGenre()<<std::endl;
                delete WaitingReceipts.at(r);
                WaitingReceipts.erase(r);
                break;
            }
            case 3: {
                request="LOGOUT";
                delete WaitingReceipts.at(r);
                WaitingReceipts.erase(r);
                break;
            }
        }
    }
    return request;
}

bool Client::WishtoBorrow(std::string book) {
    for (std::vector<Book*>::iterator it=BorrowList.begin(); it != BorrowList.end(); ++it)
    {
        if((*it)->getBookName()==book)
            return true;
    }
    return false;
}

std::string Client::gotMessage(std::string genre, std::string m) {
    if(m.find(" borrow ")!=std::string::npos)//Bob wish to borrow Dune
    {
        if(Inventory.find(genre)!=Inventory.end())
        {
            std::string bookName=m.substr(m.find(" borrow ")+8);
            if(gotBook(bookName,genre) && m.substr(0,m.find(' '))!=this->UserName)
            {
                std::string text= UserName+" has "+bookName;
                std::string request=message.send(genre,text);
                return request;
            }
        }
    }
    else if (m.find(" has ")!=std::string::npos && m.find(" added ")==std::string::npos )//john has Dune
    {
        std::string bookName=m.substr(m.find(" has ")+5);
        if(WishtoBorrow(bookName))
        {
            for (std::vector<Book*>::iterator it=BorrowList.begin(); it != BorrowList.end(); ++it)
            {
                if((*it)->getBookName()==bookName)
                {
                    std::string lender= m.substr(0,m.find(' '));
                    if(!BorrowBookAgain(bookName,genre,lender)) {
                        (*it)->setLender(lender);
                        std::string genre = (*it)->getGenre();
                        if (Inventory.find(genre) == Inventory.end()) {
                            Inventory.insert(std::pair<std::string, std::vector<Book *>>(genre, std::vector<Book *>()));
                        }
                        Inventory.at(genre).push_back(*it);
                    }
                    BorrowList.erase(it);
                    std::string text= "Taking "+bookName+" from "+lender;
                    std::string request=message.send(genre,text);
                    return request;

                }
            }
        }
    }
    else if (m.find("Taking ")!=std::string::npos){//Taking Dune from john
        if((m.substr(m.find(" from ")+6))==UserName)
        {
            std::string bookName=m.substr(7,m.find(" from ")-7);
            eraseBook(bookName,genre);
        }
    }
    else if(m.find("Returning ")!=std::string::npos){//Returning Dune to john
        if((m.substr(m.find(" to ")+4))==UserName)
        {
            std::string bookName=m.substr(10,m.find(" to ")-10);
            ChangeTrueBook(bookName,genre);
        }
    }
    else if(m.find("book status")!=std::string::npos){
        std::string text=UserName+":";
        for (std::vector<Book*>::iterator it=Inventory.at(genre).begin(); it != Inventory.at(genre).end(); ++it)
        {
            if ((*it)->isAvailable())
                text = text + (*it)->getBookName() + ",";
        }
        text=text.substr(0,text.length()-1);
        std::string request=message.send(genre,text);
        return request;
    }
    return "";

}

std::string Client::ProcessByService(std::string topic,std::string message) {
    std::string request="";
    if(topic=="RECEIPT")
    {
        std::string SID=message.substr(message.find("receipt-id:")+11);
        SID=SID.substr(0,SID.find('\n'));
        request=gotReceipt(std::stoi(SID));
    }
    else if (topic=="MESSAGE")
    {
        std::string genre=message.substr(message.find("destination:")+12);
        genre=genre.substr(0,genre.find('\n'));
        std::string m=message.substr(message.find("\n\n")+2);
        m=m.substr(0,genre.find('\n'));
        request=gotMessage(genre,m);
    }
    else if(topic=="ERROR")
    {
        request=gotError(message);
    }
    else
    {
        std::cout<<"Login successful"<<std::endl;
    }
    return request;
}

std::string Client::gotError(std::string message) {
    return "ERROR";
}



