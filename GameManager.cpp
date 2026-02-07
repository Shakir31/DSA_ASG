#include "GameManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// trim is helper ONLY for this file
static string trim(string str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

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

        // Support 2 formats:
        // Old: title,minP,maxP,maxTime,minTime,year
        // New: gameID,title,minP,maxP,maxTime,minTime,year
        string c1, c2, c3, c4, c5, c6, c7;

        getline(ss, c1, ',');
        getline(ss, c2, ',');
        getline(ss, c3, ',');
        getline(ss, c4, ',');
        getline(ss, c5, ',');
        getline(ss, c6, ',');
        getline(ss, c7, ','); // empty if old format

        c1 = trim(c1); c2 = trim(c2); c3 = trim(c3);
        c4 = trim(c4); c5 = trim(c5); c6 = trim(c6); c7 = trim(c7);

        bool isNewFormat = !c7.empty();

        string gameID, title, minP, maxP, maxTime, minTime, year;

        if (isNewFormat) {
            // new format
            gameID = c1;
            title = c2;
            minP = c3;
            maxP = c4;
            maxTime = c5;
            minTime = c6;
            year = c7;
        }
        else {
            // old format
            title = c1;
            minP = c2;
            maxP = c3;
            maxTime = c4;
            minTime = c5;
            year = c6;

            // auto-generate ID
            gameID = "G";
            if (autoIdCounter < 10) gameID += "00";
            else if (autoIdCounter < 100) gameID += "0";
            gameID += to_string(autoIdCounter);
            autoIdCounter++;
        }

        // Clean title quotes if any
        title = trim(title);
        if (!title.empty() && title.front() == '"' && title.back() == '"' && title.length() >= 2) {
            title = title.substr(1, title.length() - 2);
        }
        if (title.empty()) continue;

        if (minP.empty() || maxP.empty() || year.empty()) continue;

        int minPlayers = 0, maxPlayers = 0, minPlaytime = 0, maxPlaytimeV = 0, yearPub = 0;

        try {
            minPlayers = stoi(minP);
            maxPlayers = stoi(maxP);
            minPlaytime = (minTime.empty() ? 0 : stoi(minTime));
            maxPlaytimeV = (maxTime.empty() ? 0 : stoi(maxTime));
            yearPub = stoi(year);
        }
        catch (...) {
            continue;
        }

        games[count] = Game(gameID, title, minPlayers, maxPlayers, minPlaytime, maxPlaytimeV, yearPub);
        count++;
    }

    file.close();
    cout << "\n*** Loaded " << count << " games successfully! ***\n" << endl;
    return count;
}

void buildHashTable(Game games[], int gameCount, HashTable& hashTable) {
    for (int i = 0; i < gameCount; i++) {
        hashTable.insert(games[i].getGameID(), i);
    }
    cout << "Hash table built with " << gameCount << " games." << endl;
}

bool saveGamesToCSV(string filename, Game games[], int gameCount) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "ERROR: Cannot write to " << filename << endl;
        return false;
    }

    // Save in new format WITH GameID
    file << "gameID,title,minPlayers,maxPlayers,maxPlaytime,minPlaytime,yearPublished\n";

    for (int i = 0; i < gameCount; i++) {
        file << games[i].getGameID() << ","
            << "\"" << games[i].getTitle() << "\"" << ","
            << games[i].getMinPlayers() << ","
            << games[i].getMaxPlayers() << ","
            << games[i].getMaxPlaytime() << ","
            << games[i].getMinPlaytime() << ","
            << games[i].getYear()
            << "\n";
    }

    file.close();
    cout << "? Saved " << gameCount << " games to " << filename << endl;
    return true;
}
