//
// Created by tomroz on 8.1.2020.
//

#include "../include/InputKB.h"
#include <stdlib.h>
#include <iostream>
#include <string>


InputKB::InputKB(ConnectionHandler &_connectionHandler,Client &_client,std::string login):connectionHandler(_connectionHandler),interrupted(false),client(_client),login(login) {}

void InputKB::process() {
    while (interrupted == false) {
        std::string request;
        if(login!="")
        {
            request = client.ProcessFromKB(login);
            login="";
        }
        else {
            const short bufsize = 1024;
            char buf[bufsize];
            std::cin.getline(buf, bufsize);
            std::string line(buf);
            request = client.ProcessFromKB(line);
            if (request.substr(0, 10) == "DISCONNECT")
                interrupted = true;

        }
        if (request != "") {
            if (!connectionHandler.sendLine(request)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                return;
            }
        }
    }
}

void InputKB::interrupt(bool x) {interrupted=x;}
