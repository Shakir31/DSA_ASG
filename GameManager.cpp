#include "GameManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

/*
============================================================
Function    : trim
Description : Removes leading and trailing whitespace
              (spaces, tabs, newlines) from a string.
Input       : str - String to trim (string)
Return      : Trimmed string (string)
============================================================
*/
string trim(string str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/*
============================================================
Function    : loadGamesFromCSV
Description : Reads a CSV file and loads game data into the
              games array. Parses each line, validates data,
              generates Game IDs, and creates Game objects.
              Handles quoted game names and empty fields.
Input       : filename - Path to CSV file (string)
              games[] - Array to store Game objects
              maxSize - Maximum capacity of games array (int)
Return      : Number of games successfully loaded (int)
============================================================
*/
int loadGamesFromCSV(string filename, Game games[], int maxSize) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "ERROR: Cannot open " << filename << endl;
        return 0;
    }

    string line;
    getline(file, line); // header

    int count = 0;
    int autoIdCounter = 1;

    while (getline(file, line) && count < maxSize) {
        line = trim(line);
        if (line.empty()) continue;

        stringstream ss(line);
        string c1, c2, c3, c4, c5, c6, c7;

        getline(ss, c1, ','); // gameID OR title
        getline(ss, c2, ',');
        getline(ss, c3, ',');
        getline(ss, c4, ',');
        getline(ss, c5, ',');
        getline(ss, c6, ',');
        getline(ss, c7, ',');

        c1 = trim(c1); c2 = trim(c2); c3 = trim(c3);
        c4 = trim(c4); c5 = trim(c5); c6 = trim(c6); c7 = trim(c7);

        bool newFormat = !c7.empty();

        string gameID, title, minP, maxP, maxTime, minTime, year;

        if (newFormat) {
            gameID = c1;
            title = c2;
            minP = c3;
            maxP = c4;
            maxTime = c5;
            minTime = c6;
            year = c7;
        }
        else {
            title = c1;
            minP = c2;
            maxP = c3;
            maxTime = c4;
            minTime = c5;
            year = c6;

            gameID = "G";
            if (autoIdCounter < 10) gameID += "00";
            else if (autoIdCounter < 100) gameID += "0";
            gameID += to_string(autoIdCounter++);
        }

        if (!title.empty() && title.front() == '"' && title.back() == '"') {
            title = title.substr(1, title.length() - 2);
        }

        try {
            games[count++] = Game(
                gameID,
                title,
                stoi(minP),
                stoi(maxP),
                stoi(minTime),
                stoi(maxTime),
                stoi(year)
            );
        }
        catch (...) {
            continue;
        }
    }

    cout << "\n*** Loaded " << count << " games successfully! ***\n";
    return count;
}

/*
============================================================
Function    : buildHashTable
Description : Populates the hash table with game IDs and
              their corresponding array indices for fast
              O(1) game lookups.
Input       : games[] - Array of Game objects
              gameCount - Number of games in array (int)
              hashTable - HashTable object (passed by reference)
Return      : None
============================================================
*/
void buildHashTable(Game games[], int gameCount, HashTable& hashTable) {
    for (int i = 0; i < gameCount; i++) {
        hashTable.insert(games[i].getGameID(), i);
    }
    cout << "Hash table built with " << gameCount << " games." << endl;
}