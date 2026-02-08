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
Function    : parseCSVField
Description : Extracts the next field from a CSV line,
              respecting quotes. Handles commas inside quotes.
Input       : line - CSV line string (string&)
              pos - current position in line (size_t&)
Return      : string - the extracted field
============================================================
*/
string parseCSVField(const string& line, size_t& pos) {
    string field;
    bool inQuotes = false;

    while (pos < line.length()) {
        char c = line[pos];

        if (c == '"') {
            inQuotes = !inQuotes;
            pos++;
        }
        else if (c == ',' && !inQuotes) {
            pos++; // Skip the comma
            break;
        }
        else {
            field += c;
            pos++;
        }
    }

    return trim(field);
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
    int autoIdCounter = 1;

    while (getline(file, line) && count < maxSize) {
        line = trim(line);
        if (line.empty()) continue;

        // Parse CSV fields manually without STL
        size_t pos = 0;
        string name = parseCSVField(line, pos);
        string minP = parseCSVField(line, pos);
        string maxP = parseCSVField(line, pos);
        string maxTime = parseCSVField(line, pos);
        string minTime = parseCSVField(line, pos);
        string year = parseCSVField(line, pos);

        // Validate
        if (name.empty() || minP.empty() || maxP.empty() || year.empty()) {
            continue;
        }

        // Generate Game ID
        string gameID = "G";
        if (autoIdCounter < 10) gameID += "00";
        else if (autoIdCounter < 100) gameID += "0";
        gameID += to_string(autoIdCounter);

        // Convert to integers with error checking
        try {
            int minPlayers = stoi(minP);
            int maxPlayers = stoi(maxP);
            int minPlaytime = (minTime.empty() ? 0 : stoi(minTime));
            int maxPlaytime = (maxTime.empty() ? 0 : stoi(maxTime));
            int yearPub = stoi(year);

            games[count] = Game(gameID, name, minPlayers, maxPlayers,
                minPlaytime, maxPlaytime, yearPub);
            count++;
            autoIdCounter++;
        }
        catch (...) {
            // Skip this game if conversion fails
            continue;
        }
    }

    file.close();
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