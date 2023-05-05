# Client/Server Hash Table

 Client/server application that manages the hash table by communicating via shared memory

***

## Code Structure

* include/ - Headers
    * hash_table.h - Header file for the `HashTable` class
    * shared_memory.h - Header file for the `Command` and `SharedMemory` classes
    * thread_pool.h - Header file for the generic `ThreadPool` class with its inline implementation
* src/ - Source files
    * hash_table.cpp - Implementation of the `HashTable` class
    * shared_memory.h - Implementation of the `Command` and `SharedMemory` classes
* client.cpp - Source code of the client
* server.cpp - Source code of the server

***

## Business Logic

### Server

The server allocates and attaches a segment of shared memory, then initializes an instance of the `SharedMemory` class within it, and also creates instances of the `HashTable` and `ThreadPool` classes. After that, it starts infinitely checking for a new command in shared memory. When the command is read, it queues it up in the thread pool, creating a separate thread to process it. At the end, the server detaches the shared memory and destroys it.

### Client

The client allocates and attaches a segment of shared memory, then maps it to an instance of the `SharedMemory` class. It then executes a specified number of random commands at 100ms intervals. At the end the client detaches the shared memory.

### Shared Memory

This class provides functionality to store commands with a specified action, key and value in a queue fashion, meaning that readers can read commands from the head and writers can write them to the tail. It has limited capacity, so the implementation is round/robbin style with two counters - for read and write commands, which are mutex protected.

### Hash Table

This class provides functionality for inserting, reading and deleting elements by their keys. It uses the modulo operator as a hash function. To handle hash collisions, each bucket maintains a linked list of nodes, new nodes are added to the end of this list. For thread safety, each node has its own mutex, which allows granular access to a specific element without locking the whole table, and also has a "multiple reader/one writer" access type.

### Thread Pool

This class provides functionality for scheduling work by maintaining a task queue and a pool of workers. Each worker has a separate thread, they continuously check the queue head for new tasks and execute them when detected.

***

## Build & Run

To build a server and a client, open a terminal and run the command:
```
make
```

To start the server, open a terminal and run the command:
```
./server [hash table size]
```

To start the client, open a terminal and run the command:
```
./client [command count]
```

To clean up, open a terminal and run the command:
```
make clean
```