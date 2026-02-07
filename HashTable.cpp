#include "HashTable.h"
#include <iostream>
using namespace std;

/*
============================================================
Function    : HashTable (Constructor)
Description : Initializes the hash table by setting all
              buckets to nullptr.
Input       : None
Return      : None
============================================================
*/
HashTable::HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

/*
============================================================
Function    : ~HashTable (Destructor)
Description : Deallocates all dynamically allocated nodes
              in the hash table to prevent memory leaks.
Input       : None
Return      : None
============================================================
*/
HashTable::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

/*
============================================================
Function    : hashFunction
Description : Computes the hash index for a given key by
              summing the ASCII values of all characters
              and taking modulo TABLE_SIZE.
Input       : key - String to hash (string)
Return      : Hash index (int)
============================================================
*/
int HashTable::hashFunction(string key) {
    int sum = 0;
    for (int i = 0; i < key.length(); i++) {
        sum += (int)key[i];
    }
    return sum % TABLE_SIZE;
}

/*
============================================================
Function    : insert
Description : Inserts a new game ID and array index into
              the hash table using chaining for collision
              resolution.
Input       : gameID - Game identifier (string)
              arrayIndex - Index in games array (int)
Return      : None
============================================================
*/
void HashTable::insert(string gameID, int arrayIndex) {
    int index = hashFunction(gameID);
    HashNode* newNode = new HashNode;
    newNode->gameID = gameID;
    newNode->arrayIndex = arrayIndex;
    newNode->next = nullptr;
    if (table[index] == nullptr) {
        table[index] = newNode;
    }
    else {
        newNode->next = table[index];
        table[index] = newNode;
    }
}

/*
============================================================
Function    : search
Description : Searches for a game ID in the hash table and
              returns its array index if found.
Input       : gameID - Game identifier to search for (string)
Return      : Array index if found, -1 if not found (int)
============================================================
*/
int HashTable::search(string gameID) {
    int index = hashFunction(gameID);
    HashNode* current = table[index];
    while (current != nullptr) {
        if (current->gameID == gameID) {
            return current->arrayIndex;
        }
        current = current->next;
    }
    return -1;
}

/*
============================================================
Function    : remove
Description : Removes a game ID from the hash table and
              deallocates the corresponding node.
Input       : gameID - Game identifier to remove (string)
Return      : True if removed successfully, false if not
              found (bool)
============================================================
*/
bool HashTable::remove(string gameID) {
    int index = hashFunction(gameID);
    HashNode* current = table[index];
    HashNode* prev = nullptr;
    while (current != nullptr) {
        if (current->gameID == gameID) {
            if (prev == nullptr) {
                table[index] = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

/*
============================================================
Function    : display
Description : Displays the contents of the hash table,
              showing each bucket and its linked list of
              game IDs and array indices.
Input       : None
Return      : None
============================================================
*/
void HashTable::display() {
    cout << "=== Hash Table Contents ===" << endl;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i] != nullptr) {
            cout << "Bucket " << i << ": ";
            HashNode* current = table[i];
            while (current != nullptr) {
                cout << current->gameID << "(" << current->arrayIndex << ") ";
                current = current->next;
            }
            cout << endl;
        }
    }
}

/*
============================================================
Function    : clear
Description : Clears all entries from the hash table by
              deleting all nodes and resetting buckets to
              nullptr. Used when rebuilding the hash table.
Input       : None
Return      : None
============================================================
*/
void HashTable::clear() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
        table[i] = nullptr;
    }
}