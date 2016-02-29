#!/bin/make

# MINGW ONLY..
CXXFLAGS=-std=c++11 -DWINDOWS 
# OTHER CXX Compiler.
#CXXFLAGS=-std=c++11

# MINGW ONLY..
LDFLAGS=-lboost_system -lWS2_32 -lmswsock
# OTHER CXX Compiler.
#LDFLAGS=-lboost_system

PROJS=echo-server \
    echo-client \
    async-echo-server \
    async-echo-client

.PHONY: all clean

all: $(PROJS)

clean:
	rm -fr *.o *.exe

echo-server: echo-server.o
	$(CXX) -o $@ $^ $(LDFLAGS)

echo-client: echo-client.o
	$(CXX) -o $@ $^ $(LDFLAGS)

async-echo-server: async-echo-server.o
	$(CXX) -o $@ $^ $(LDFLAGS)

async-echo-client: async-echo-client.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS)
