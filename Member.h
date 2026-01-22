#pragma once
#include <string>
#include "List.h"
using namespace std;

class Member {
private:
    string memberID;
    string name;
    string email;
    List borrowedGames; // List of game IDs this member has borrowed

public:
    // Constructor
    Member();
    Member(string id, string n, string e);

    // Getters
    string getMemberID();
    string getName();
    string getEmail();

    // Borrow management
    void addBorrowedGame(string gameID);
    void removeBorrowedGame(string gameID);
    int getBorrowedCount();
    void displayBorrowedGames();
    bool hasBorrowed(string gameID);

    // Display
    void display();
};