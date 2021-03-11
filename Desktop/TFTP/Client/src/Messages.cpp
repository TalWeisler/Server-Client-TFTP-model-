//
// Created by tomroz on 8.1.2020.
//

#include "../include/Messages.h"

//Messages::Messages(){};

std::string Messages::connect(std::string host,std::string UN,std::string pass) {
    std::string ans="CONNECT\naccept-version:1.2\nhost:"+host+"\nlogin:"+UN+"\npasscode:"+pass+"\n\n";
    return ans;
}

std::string Messages::subscribe(std::string genre, int SubID, int RecID) {
    std::string ans="SUBSCRIBE\ndestination:"+genre+"\nid:"+std::to_string(SubID)+"\nreceipt:"+std::to_string(RecID)+"\n\n";
    return ans;
}

std::string Messages::unsubscribe(int id, int RecID) {
    std::string ans="UNSUBSCRIBE\nid:"+std::to_string(id)+"\nreceipt:"+std::to_string(RecID)+"\n\n";
    return ans;
}

std::string Messages::send(std::string genre, std::string text) {
    std::string ans="SEND\ndestination:"+genre+"\n\n";
    if(text.size()!=0)
        ans=ans+text+"\n";
    return ans;
}

std::string Messages::disconnect(int id) {
    std::string ans="DISCONNECT\nreceipt:"+std::to_string(id)+"\n\n";
    return ans;
}//
// Created by tomroz on 13.1.2020.
//

