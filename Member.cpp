#include "Member.h"
#include <iostream>
using namespace std;

Member::Member() {
    memberID = "";
    name = "";
    email = "";
}

Member::Member(string id, string n, string e) {
    memberID = id;
    name = n;
    email = e;
}

string Member::getMemberID() { return memberID; }
string Member::getName() { return name; }
string Member::getEmail() { return email; }

void Member::addBorrowedGame(string gameID) {
    borrowedGames.add(gameID);
}

void Member::removeBorrowedGame(string gameID) {
    for (int i = 0; i < borrowedGames.getLength(); i++) {
        if (borrowedGames.get(i) == gameID) {
            borrowedGames.remove(i);
            break;
        }
    }
}

int Member::getBorrowedCount() {
    return borrowedGames.getLength();
}

void Member::displayBorrowedGames() {
    if (borrowedGames.isEmpty()) {
        cout << "No games borrowed." << endl;
    }
    else {
        cout << "Borrowed games: ";
        borrowedGames.print();
    }
}

bool Member::hasBorrowed(string gameID) {
    for (int i = 0; i < borrowedGames.getLength(); i++) {
        if (borrowedGames.get(i) == gameID) {
            return true;
        }
    }
    return false;
}

void Member::display() {
    cout << "==================================" << endl;
    cout << "Member ID: " << memberID << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Games borrowed: " << getBorrowedCount() << endl;
    cout << "==================================" << endl;
}