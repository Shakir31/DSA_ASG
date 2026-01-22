#pragma once
#include <string>
using namespace std;

class Game {
private:
    string gameID;      // auto-generated
    string title;
    int minPlayers;
    int maxPlayers;
    int minPlaytime;
    int maxPlaytime;
    int yearPublished;
    string status;      // "Available" or "Borrowed"
    string borrowedBy;  // Member ID if borrowed
    int borrowCount;    // For statistics

public:
    // Constructors
    Game();
    Game(string id, string name, int minP, int maxP, int minTime, int maxTime, int year);

    // Getters
    string getGameID();
    string getTitle();
    int getMinPlayers();
    int getMaxPlayers();
    int getMinPlaytime();
    int getMaxPlaytime();
    int getYear();
    string getStatus();
    string getBorrowedBy();
    int getBorrowCount();

    // Setters
    void setStatus(string s);
    void setBorrowedBy(string memberID);
    void incrementBorrowCount();

    // Display
    void display();
    string toString();
};