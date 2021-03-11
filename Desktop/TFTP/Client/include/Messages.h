//
// Created by tomroz on 13.1.2020.
//

#ifndef BOOST_ECHO_CLIENT_MESSAGES_H
#define BOOST_ECHO_CLIENT_MESSAGES_H

#include <string>

class Messages {
public:
   // Messages();
    std::string connect(std::string host,std::string UN,std::string pass);
    std::string send(std::string genre,std::string text);
    std::string subscribe(std::string genre,int SubID, int RecID);
    std::string unsubscribe(int id, int RecID);
    std::string disconnect(int id);
};

#endif //BOOST_ECHO_CLIENT_MESSAGES_H
