//
// Created by tomroz on 13.1.2020.
//

#ifndef BOOST_ECHO_CLIENT_INPUTKB_H
#define BOOST_ECHO_CLIENT_INPUTKB_H
#include <string>
#include "../include/Client.h"
#include "../include/ConnectionHandler.h"

class InputKB {
private:
    ConnectionHandler & connectionHandler;
    bool interrupted;
    Client & client;
    std::string login;
public:
    InputKB(ConnectionHandler &_connectionHandler,Client &_client,std::string login);
    void process ();
    void interrupt(bool x);
};

#endif //BOOST_ECHO_CLIENT_INPUTKB_H
