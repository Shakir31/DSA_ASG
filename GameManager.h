#pragma once
#include "Game.h"
#include "HashTable.h"
#include <string>
using namespace std;

int loadGamesFromCSV(string filename, Game games[], int maxSize);
void buildHashTable(Game games[], int gameCount, HashTable& hashTable);
string trim(string str);