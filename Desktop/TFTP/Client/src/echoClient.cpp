//
// Created by tomroz on 13.1.2020.
//
#include <stdlib.h>
#include "../include/ConnectionHandler.h"
#include "../include/InputKB.h"
#include <thread>

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {
    /*  if (argc < 3) {
          std::cerr << "Usage: " << "tal" << " host port" << std::endl << std::endl;
          return -1;
      }*/
        std::string key;
        getline(std::cin, key);
        while (key.length() < 5 || key.substr(0, 5) != "login") {
            std::cout << "User isn't logged in" << std::endl;
            getline(std::cin, key);
        }
        std::string h = key.substr(6);
        std::string host = h.substr(0, h.find(':'));
        std::string p = h.substr(h.find(':') + 1);
        p = p.substr(0, p.find(' '));
        short port = atoi(p.c_str());

        ConnectionHandler connectionHandler(host, (short)port);
        if (!connectionHandler.connect()) {
            std::cout << " Could not connect to server" << std::endl;
            return 1;
        }
        Client *client = new  Client();
        InputKB input = InputKB(connectionHandler, *client, key);
        std::thread Thread1(&InputKB::process, &input);

        //From here we will see the rest of the ehco client implementation:
        while (1) {

            std::string line;
            if (!connectionHandler.getLine(line)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
            int len = line.length();
            //A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
            // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
            line.resize(len - 1);
            std::cout << "Reply: " << line << std::endl<< std::endl;
            std::string topic = line.substr(0, line.find('\n'));
            line = line.substr(line.find('\n') + 1);
            std::string request = (*client).ProcessByService(topic, line);
            if (request == "LOGOUT"||topic == "ERROR") {
                input.interrupt(true);

                std::cout << "Exiting...\n" << std::endl;
                break;
            } else if (request != "") {
                if (!connectionHandler.sendLine(request)) {
                    std::cout << "Disconnected. Exiting...\n" << std::endl;
                    break;
                }
            }
        }
        Thread1.join();
        connectionHandler.close();
        delete client;
        return 0;
}
