EXE = peer
CFLAGS = -Wall
CXXFLAGS = -Wall
LDLIBS = 
CC = gcc
CXX = g++

.PHONY: all
all: $(EXE)

peer: main.cpp
	$(CXX) $(CFLAGS) main.cpp p2p.cpp p2p.h lookup-and-connect.cpp lookup-and-connect.h  -o peer

.PHONY: clean
clean:
	rm -f $(EXE)
