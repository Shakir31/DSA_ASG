#include "HashTable.h"
#include <iostream>
using namespace std;

HashTable::HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

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

int HashTable::hashFunction(string key) {
    int sum = 0;
    for (int i = 0; i < key.length(); i++) {
        sum += (int)key[i];
    }
    return sum % TABLE_SIZE;
}

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