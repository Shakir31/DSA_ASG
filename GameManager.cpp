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
    getline(file, line); // Skip header
    int count = 0;
    int gameCounter = 1;
    while (getline(file, line) && count < maxSize) {
        // Skip empty lines
        line = trim(line);
        if (line.empty() || line.length() < 5) {
            continue;
        }
        stringstream ss(line);
        string name, minP, maxP, maxTime, minTime, year;
        // Parse CSV
        getline(ss, name, ',');
        getline(ss, minP, ',');
        getline(ss, maxP, ',');
        getline(ss, maxTime, ',');
        getline(ss, minTime, ',');
        getline(ss, year, ',');
        // Clean name
        name = trim(name);
        if (name.empty()) continue;
        // Remove quotes
        if (name.length() > 0 && name[0] == '"') {
            name = name.substr(1, name.length() - 2);
        }
        // Validate we have all fields
        minP = trim(minP);
        maxP = trim(maxP);
        minTime = trim(minTime);
        maxTime = trim(maxTime);
        year = trim(year);
        if (minP.empty() || maxP.empty() || year.empty()) {
            continue; // Skip this line
        }
        // Generate Game ID
        string gameID = "G";
        if (gameCounter < 10) gameID += "00";
        else if (gameCounter < 100) gameID += "0";
        gameID += to_string(gameCounter);
        // Convert to integers with error checking
        int minPlayers = 0;
        int maxPlayers = 0;
        int minPlaytime = 0;
        int maxPlaytime = 0;
        int yearPub = 0;
        try {
            minPlayers = stoi(minP);
            maxPlayers = stoi(maxP);
            minPlaytime = (minTime.empty() ? 0 : stoi(minTime));
            maxPlaytime = (maxTime.empty() ? 0 : stoi(maxTime));
            yearPub = stoi(year);
        }
        catch (...) {
            // Skip this game if conversion fails
            continue;
        }
        // Create game object
        games[count] = Game(gameID, name, minPlayers, maxPlayers, minPlaytime, maxPlaytime, yearPub);
        count++;
        gameCounter++;
    }
    file.close();
    cout << "\n*** Loaded " << count << " games successfully! ***\n" << endl;
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