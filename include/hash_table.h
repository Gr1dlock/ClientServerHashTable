#pragma once

#include <shared_mutex>

struct Node
{
    std::shared_mutex mutex;
    int key;
    void *value;
    Node *next;
};

class HashTable
{
public:
    HashTable(int size);
    void insert(int key, void *value);
    void *read(int key);
    void remove(int key);
    ~HashTable();

private:
    Node **_nodes;
    int _size;
    int hash(int key);
};