#include <iostream>
#include <chrono>
#include <thread>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "include/shared_memory.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " [hash table size] [commands count]" << std::endl;
        return 1;
    }

    srand((unsigned)time(NULL));

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

    for (int i = 0; i < atoi(argv[2]); i++)
    {
        Command command = {static_cast<Action>(rand() % 3), rand() % atoi(argv[2]), NULL};
        sharedMemory->insert(command);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (shmdt(sharedMemory) == -1)
    {
        std::cerr << "Shared memory detach failed" << std::endl;
        return 1;
    }

    return 0;
}