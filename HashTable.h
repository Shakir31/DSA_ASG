#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
using namespace std;

const int TABLE_SIZE = 101;

struct HashNode {
    string gameID;
    int arrayIndex;
    HashNode* next;
};

class HashTable {
private:
    HashNode* table[TABLE_SIZE];
    int hashFunction(string key);

public:
    HashTable();
    ~HashTable();
    void insert(string gameID, int arrayIndex);
    int search(string gameID);
    bool remove(string gameID);
    void display();
    void clear();  // Add this method
};

#endif