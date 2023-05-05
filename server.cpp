#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <chrono>
#include <thread>
#include <stdio.h>
#include "shared_memory.h"
#include "hash_table.h"
#include "thread_pool.h"

void processCommand(Command command, HashTable *hashTable)
{
    switch (command.action)
    {
    case INSERT:
        std::cout << "Inserting " << command.key << std::endl;
        hashTable->insert(command.key, command.value);
        break;
    case READ:
        std::cout << "Reading " << command.key << std::endl;
        hashTable->read(command.key);
        break;
    case REMOVE:
        std::cout << "Removing " << command.key << std::endl;
        hashTable->remove(command.key);
        break;
    default:
        std::cerr << "Unknown command action" << std::endl;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " [hash table size]" << std::endl;
        return 1;
    }

    int shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        std::cerr << "Shared memory creation failed" << std::endl;
        return 1;
    }

    void *shmAddress = shmat(shmid, NULL, 0);
    if (shmAddress == (void *)-1)
    {
        std::cerr << "Shared memory attach failed" << std::endl;
        return 1;
    }

    SharedMemory *sharedMemory = new (shmAddress) SharedMemory();
    HashTable *hashTable = new HashTable(atoi(argv[1]));
    ThreadPool threadPool(std::thread::hardware_concurrency());

    while (true)
    {
        if (!sharedMemory->isEmpty())
        {
            Command command = sharedMemory->read();
            threadPool.enqueue(processCommand, command, hashTable);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (shmdt(sharedMemory) == -1)
    {
        std::cerr << "Shared memory detach failed" << std::endl;
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1)
    {
        std::cerr << "Shared memory deletion failed" << std::endl;
        return 1;
    }

    delete hashTable;

    return 0;
}