#include "Game.h"
#include <iostream>
#include <sstream>
using namespace std;

/*
============================================================
Function    : Game (Default Constructor)
Description : Initializes a Game object with default empty
              values and status set to "Available".
Input       : None
Return      : None
============================================================
*/
Game::Game() {
    gameID = "";
    title = "";
    minPlayers = 0;
    maxPlayers = 0;
    minPlaytime = 0;
    maxPlaytime = 0;
    yearPublished = 0;
    status = "Available";
    borrowedBy = "";
    borrowCount = 0;
}

/*
============================================================
Function    : Game (Parameterized Constructor)
Description : Initializes a Game object with the provided
              game details and sets status to "Available".
Input       : id - Game ID (string)
              name - Game title (string)
              minP - Minimum players (int)
              maxP - Maximum players (int)
              minTime - Minimum playtime in minutes (int)
              maxTime - Maximum playtime in minutes (int)
              year - Year published (int)
Return      : None
============================================================
*/
Game::Game(string id, string name, int minP, int maxP, int minTime, int maxTime, int year) {
    gameID = id;
    title = name;
    minPlayers = minP;
    maxPlayers = maxP;
    minPlaytime = minTime;
    maxPlaytime = maxTime;
    yearPublished = year;
    status = "Available";
    borrowedBy = "";
    borrowCount = 0;
}

// Getters

/*
============================================================
Function    : getGameID
Description : Returns the game ID.
Input       : None
Return      : Game ID (string)
============================================================
*/
string Game::getGameID() { return gameID; }

/*
============================================================
Function    : getTitle
Description : Returns the game title.
Input       : None
Return      : Game title (string)
============================================================
*/
string Game::getTitle() { return title; }

/*
============================================================
Function    : getMinPlayers
Description : Returns the minimum number of players.
Input       : None
Return      : Minimum players (int)
============================================================
*/
int Game::getMinPlayers() { return minPlayers; }

/*
============================================================
Function    : getMaxPlayers
Description : Returns the maximum number of players.
Input       : None
Return      : Maximum players (int)
============================================================
*/
int Game::getMaxPlayers() { return maxPlayers; }

/*
============================================================
Function    : getMinPlaytime
Description : Returns the minimum playtime in minutes.
Input       : None
Return      : Minimum playtime (int)
============================================================
*/
int Game::getMinPlaytime() { return minPlaytime; }

/*
============================================================
Function    : getMaxPlaytime
Description : Returns the maximum playtime in minutes.
Input       : None
Return      : Maximum playtime (int)
============================================================
*/
int Game::getMaxPlaytime() { return maxPlaytime; }

/*
============================================================
Function    : getYear
Description : Returns the year the game was published.
Input       : None
Return      : Year published (int)
============================================================
*/
int Game::getYear() { return yearPublished; }

/*
============================================================
Function    : getStatus
Description : Returns the current status of the game.
Input       : None
Return      : Status - "Available" or "Borrowed" (string)
============================================================
*/
string Game::getStatus() { return status; }

/*
============================================================
Function    : getBorrowedBy
Description : Returns the member ID of who borrowed the game.
Input       : None
Return      : Member ID (string, empty if available)
============================================================
*/
string Game::getBorrowedBy() { return borrowedBy; }

/*
============================================================
Function    : getBorrowCount
Description : Returns the total number of times this game
              has been borrowed.
Input       : None
Return      : Borrow count (int)
============================================================
*/
int Game::getBorrowCount() { return borrowCount; }

// Setters

/*
============================================================
Function    : setStatus
Description : Sets the status of the game.
Input       : s - New status (string)
Return      : None
============================================================
*/
void Game::setStatus(string s) { status = s; }

/*
============================================================
Function    : setBorrowedBy
Description : Sets the member ID of who is borrowing the game.
Input       : memberID - Member ID (string)
Return      : None
============================================================
*/
void Game::setBorrowedBy(string memberID) { borrowedBy = memberID; }

/*
============================================================
Function    : incrementBorrowCount
Description : Increases the borrow count by 1.
Input       : None
Return      : None
============================================================
*/
void Game::incrementBorrowCount() { borrowCount++; }

/*
============================================================
Function    : display
Description : Displays all game information to the console
              in a formatted layout.
Input       : None
Return      : None
============================================================
*/
void Game::display() {
    cout << "==================================" << endl;
    cout << "Game ID: " << gameID << endl;
    cout << "Title: " << title << endl;
    cout << "Year: " << yearPublished << endl;
    cout << "Players: " << minPlayers << "-" << maxPlayers << endl;
    cout << "Playtime: " << minPlaytime << "-" << maxPlaytime << " mins" << endl;
    cout << "Status: " << status << endl;
    if (status == "Borrowed") {
        cout << "Borrowed by: " << borrowedBy << endl;
    }
    cout << "Times borrowed: " << borrowCount << endl;
    cout << "==================================" << endl;
}

/*
============================================================
Function    : toString
Description : Converts the game information to a pipe-
              delimited string format.
Input       : None
Return      : Formatted string (string)
============================================================
*/
string Game::toString() {
    return gameID + "|" + title + "|" + to_string(yearPublished) +
        "|" + to_string(minPlayers) + "-" + to_string(maxPlayers);
}