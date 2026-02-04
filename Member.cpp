#include "Member.h"
#include <iostream>
using namespace std;

/*
============================================================
Function    : Member (Default Constructor)
Description : Initializes a Member object with empty values.
Input       : None
Return      : None
============================================================
*/
Member::Member() {
    memberID = "";
    name = "";
    email = "";
}

/*
============================================================
Function    : Member (Parameterized Constructor)
Description : Initializes a Member object with the provided
              member details.
Input       : id - Member ID (string)
              n - Member name (string)
              e - Member email (string)
Return      : None
============================================================
*/
Member::Member(string id, string n, string e) {
    memberID = id;
    name = n;
    email = e;
}

/*
============================================================
Function    : getMemberID
Description : Returns the member ID.
Input       : None
Return      : Member ID (string)
============================================================
*/
string Member::getMemberID() { return memberID; }

/*
============================================================
Function    : getName
Description : Returns the member's name.
Input       : None
Return      : Member name (string)
============================================================
*/
string Member::getName() { return name; }

/*
============================================================
Function    : getEmail
Description : Returns the member's email address.
Input       : None
Return      : Email address (string)
============================================================
*/
string Member::getEmail() { return email; }

/*
============================================================
Function    : addBorrowedGame
Description : Adds a game ID to the member's list of
              currently borrowed games.
Input       : gameID - Game identifier (string)
Return      : None
============================================================
*/
void Member::addBorrowedGame(string gameID) {
    borrowedGames.add(gameID);
}

/*
============================================================
Function    : removeBorrowedGame
Description : Removes a game ID from the member's list of
              currently borrowed games by searching for it
              and deleting the matching entry.
Input       : gameID - Game identifier to remove (string)
Return      : None
============================================================
*/
void Member::removeBorrowedGame(string gameID) {
    for (int i = 0; i < borrowedGames.getLength(); i++) {
        if (borrowedGames.get(i) == gameID) {
            borrowedGames.remove(i);
            break;
        }
    }
}

/*
============================================================
Function    : getBorrowedCount
Description : Returns the number of games currently borrowed
              by this member.
Input       : None
Return      : Number of borrowed games (int)
============================================================
*/
int Member::getBorrowedCount() {
    return borrowedGames.getLength();
}

/*
============================================================
Function    : displayBorrowedGames
Description : Displays all game IDs currently borrowed by
              this member to the console.
Input       : None
Return      : None
============================================================
*/
void Member::displayBorrowedGames() {
    if (borrowedGames.isEmpty()) {
        cout << "No games borrowed." << endl;
    }
    else {
        cout << "Borrowed games: ";
        borrowedGames.print();
    }
}

/*
============================================================
Function    : hasBorrowed
Description : Checks if the member has currently borrowed
              a specific game.
Input       : gameID - Game identifier to check (string)
Return      : True if member has borrowed the game, false
              otherwise (bool)
============================================================
*/
bool Member::hasBorrowed(string gameID) {
    for (int i = 0; i < borrowedGames.getLength(); i++) {
        if (borrowedGames.get(i) == gameID) {
            return true;
        }
    }
    return false;
}

/*
============================================================
Function    : display
Description : Displays all member information including ID,
              name, email, and number of borrowed games to
              the console in a formatted layout.
Input       : None
Return      : None
============================================================
*/
void Member::display() {
    cout << "==================================" << endl;
    cout << "Member ID: " << memberID << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Games borrowed: " << getBorrowedCount() << endl;
    cout << "==================================" << endl;
}