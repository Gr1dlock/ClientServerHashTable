#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <chrono>
#include <thread>
#include <stdio.h>
#include "include/shared_memory.h"
#include "include/hash_table.h"

void processCommand(Command command, HashTable *hashTable)
{
    switch (command.action)
    {
    case INSERT:
        hashTable->insert(command.key, command.value);
        break;
    case READ:
        hashTable->read(command.key);
        break;
    case REMOVE:
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

    HashTable *hashTable = new HashTable(atoi(argv[1]));

    int shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        std::cerr << "Shared memory creation failed" << std::endl;
        return 1;
    }

    SharedMemory *sharedMemory = reinterpret_cast<SharedMemory *>(shmat(shmid, NULL, 0));
    if (sharedMemory == (void *)-1)
    {
        std::cerr << "Shared memory attach failed" << std::endl;
        return 1;
    }

    while (true)
    {
        if (!sharedMemory->isEmpty())
        {
            Command command = sharedMemory->read();
            processCommand(command, hashTable);
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

    return 0;
}