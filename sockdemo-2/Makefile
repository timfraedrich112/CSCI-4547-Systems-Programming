# Makefile for socket demo collection
# March 2010, by Michael J. Fischer and Alice E. Fischer

CXX = clang++
CXXFLAGS = -O1 -Wall -std=c++17
OBJECTS = server2.o clientPat.o clientJohn.o clientMary.o Socket.o tools.o 

all: server2 clientPat

# targets 
# clients --------------------------------------------------------
# All clients work with all servers; they send different messages. 
clientPat: clientPat.o tools.o
	$(CXX) $(CxXFLAGS) -o client clientPat.o tools.o

clientJohn: clientJohn.o tools.o
	$(CXX) $(CXXFLAGS) -o clientJohn clientJohn.o tools.o

clientMary: clientMary.o tools.o
	$(CXX) $(CXXFLAGS) -o clientMary clientMary.o tools.o

# servers --------------------------------------------------------
# socket example: multiple clients, fixed port 1099
server2: server2.o  Socket.o tools.o
	$(CXX) $(CXXFLAGS) -o server2 server2.o Socket.o tools.o

# cleanup --------------------------------------------------------
clean:
	rm -f server2 clientPat clientJohn clientMary $(OBJECTS)

# dependencies ---------------------------------------------------
clientPat.o: clientPat.cpp tools.hpp
clientJohn.o: clientJohn.cpp tools.hpp
clientMary.o: clientMary.cpp tools.hpp
server2.o: server2.cpp tools.hpp Socket.hpp
Socket.o: Socket.cpp Socket.hpp tools.hpp
tools.o: tools.cpp tools.hpp
