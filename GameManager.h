#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <string>
#include "Game.h"
#include "HashTable.h"

using namespace std;

// Helper function to trim whitespace
string trim(string str);

// Helper function to parse a single CSV field respecting quotes
string parseCSVField(const string& line, size_t& pos);

// Load games from CSV file
int loadGamesFromCSV(string filename, Game games[], int maxSize);

// Build hash table from loaded games
void buildHashTable(Game games[], int gameCount, HashTable& hashTable);

#endif