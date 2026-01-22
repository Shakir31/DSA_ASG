#include "Game.h"
#include <iostream>
#include <sstream>
using namespace std;

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
string Game::getGameID() { return gameID; }
string Game::getTitle() { return title; }
int Game::getMinPlayers() { return minPlayers; }
int Game::getMaxPlayers() { return maxPlayers; }
int Game::getMinPlaytime() { return minPlaytime; }
int Game::getMaxPlaytime() { return maxPlaytime; }
int Game::getYear() { return yearPublished; }
string Game::getStatus() { return status; }
string Game::getBorrowedBy() { return borrowedBy; }
int Game::getBorrowCount() { return borrowCount; }

// Setters
void Game::setStatus(string s) { status = s; }
void Game::setBorrowedBy(string memberID) { borrowedBy = memberID; }
void Game::incrementBorrowCount() { borrowCount++; }

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

string Game::toString() {
    return gameID + "|" + title + "|" + to_string(yearPublished) +
        "|" + to_string(minPlayers) + "-" + to_string(maxPlayers);
}