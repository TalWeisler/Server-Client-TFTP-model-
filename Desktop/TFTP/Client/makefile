CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system
CFLAGS += -pthread
LDFLAGS += -pthread


all:Boost_Echo_Client
	g++ -o bin/StompBookClubClient bin/Book.o bin/Client.o bin/ConnectionHandler.o bin/echoClient.o bin/InputKB.o bin/Messages.o bin/Receipt.o $(LDFLAGS)

Boost_Echo_Client: bin/Book.o bin/Client.o bin/ConnectionHandler.o bin/echoClient.o bin/InputKB.o bin/Messages.o bin/Receipt.o

bin/Book.o: src/Book.cpp include/Book.h
	g++ $(CFLAGS) -o bin/Book.o src/Book.cpp

bin/Client.o: src/Client.cpp include/Client.h
	g++ $(CFLAGS) -o bin/Client.o src/Client.cpp
	
bin/ConnectionHandler.o: src/ConnectionHandler.cpp include/ConnectionHandler.h
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/echoClient.o:  src/echoClient.cpp 
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

bin/InputKB.o: src/InputKB.cpp include/InputKB.h
	g++ $(CFLAGS) -o bin/InputKB.o src/InputKB.cpp

bin/Messages.o:  src/Messages.cpp include/Messages.h
	g++ $(CFLAGS) -o bin/Messages.o src/Messages.cpp

bin/Receipt.o : src/Receipt.cpp include/Receipt.h
	g++ $(CFLAGS) -o bin/Receipt.o src/Receipt.cpp

.PHONY: clean
clean:
	rm -f bin/*
