#pragma once
#include "Game.h"
#include "HashTable.h"
#include <string>
using namespace std;

// Load games from CSV into array, return number loaded
int loadGamesFromCSV(string filename, Game games[], int maxSize);

// Build hash table index for fast search by GameID
void buildHashTable(Game games[], int gameCount, HashTable& hashTable);

// Save games back to CSV (new format includes GameID so IDs persist)
bool saveGamesToCSV(string filename, Game games[], int gameCount);
