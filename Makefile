TARGET=client server 
CXX=g++
RM=rm -f
IDIR =../include
CXXFLAGS=-std=c++17 -Wall -pedantic -I$(IDIR)

all: $(TARGET)

client: client.cpp shared_memory.o
	$(CXX) $(CXXFLAGS) -o client client.cpp shared_memory.o

server: server.cpp shared_memory.o hash_table.o
	$(CXX) $(CXXFLAGS) -o server server.cpp  shared_memory.o hash_table.o 

shared_memory.o: src/shared_memory.cpp include/shared_memory.h
	$(CXX) $(CXXFLAGS) -c src/shared_memory.cpp -o shared_memory.o

hash_table.o: src/hash_table.cpp include/hash_table.h
	$(CXX) $(CXXFLAGS) -c src/hash_table.cpp -o hash_table.o

clean:
	$(RM) $(TARGET) *.o