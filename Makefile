TARGET=client server 
CC=gcc
CXXFLAGS=-g -std=c++11 -Wall -pedantic

all: $(TARGET)

client: client.cpp shared_memory.o
	$(CC) $(CXXFLAGS) -o client client.cpp shared_memory.o

server: server.cpp shared_memory.o hash_table.o
	$(CC) $(CXXFLAGS) -o server server.cpp  shared_memory.o hash_table.o 

shared_memory.o: src/shared_memory.cpp include/shared_memory.h
	$(CC) $(CXXFLAGS) -I include -c src/shared_memory.cpp -o shared_memory.o

hash_table.o: src/hash_table.cpp include/hash_table.h
	$(CC) $(CXXFLAGS) -I include -c src/hash_table.cpp -o hash_table.o

clean:
	$(RM) $(TARGET)