#pragma once
#include <string>
using namespace std;

struct HashNode {
    string gameID;
    int arrayIndex;
    HashNode* next;
};

class HashTable {
private:
    static const int TABLE_SIZE = 101;
    HashNode* table[TABLE_SIZE];

    int hashFunction(string key);

public:
    HashTable();
    ~HashTable();

    void insert(string gameID, int arrayIndex);
    int search(string gameID);
    bool remove(string gameID);
    void display();
};