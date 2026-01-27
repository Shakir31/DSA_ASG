#include <iostream>
#include <string>
#include <limits>
#include "Game.h"
#include "Member.h"
#include "GameManager.h"
#include "HashTable.h"
#include "BorrowRecord.h"
#include "Review.h"
using namespace std;

//global storage
const int MAX_GAMES = 1000;
const int MAX_MEMBERS = 100;
const int MAX_RECORDS = 1000;
const int MAX_REVIEWS = 1000;
Game games[MAX_GAMES];
Member members[MAX_MEMBERS];
BorrowRecord records[MAX_RECORDS];
Review reviews[MAX_REVIEWS];
int gameCount = 0;
int memberCount = 0;
int recordCount = 0;
int reviewCount = 0;
HashTable gameHash;

//HELPER FUNCTIONS

//helper function to get current date as string
string getCurrentDate() {
    return "2026-01-18";
}

//helper function to clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//helper function to pause screen
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//function to find member by ID
int findMember(string memberID) {
    for (int i = 0; i < memberCount; i++) {
        if (members[i].getMemberID() == memberID) {
            return i;
        }
    }
    return -1;
}

//BORROW AND RETURN FUNCTIONS

bool borrowGame(string memberID, string gameID) {
    int memberIndex = findMember(memberID);
    if (memberIndex == -1) {
        cout << "ERROR: Member " << memberID << " not found!" << endl;
        return false;
    }

    int gameIndex = gameHash.search(gameID);
    if (gameIndex == -1) {
        cout << "ERROR: Game " << gameID << " not found!" << endl;
        return false;
    }

    if (games[gameIndex].getStatus() != "Available") {
        cout << "ERROR: Game is already borrowed by " << games[gameIndex].getBorrowedBy() << endl;
        return false;
    }

    games[gameIndex].setStatus("Borrowed");
    games[gameIndex].setBorrowedBy(memberID);
    games[gameIndex].incrementBorrowCount();

    members[memberIndex].addBorrowedGame(gameID);

    records[recordCount] = BorrowRecord(gameID, memberID, getCurrentDate());
    recordCount++;

    cout << "\nSUCCESS: " << members[memberIndex].getName() << " borrowed \""
        << games[gameIndex].getTitle() << "\"" << endl;

    return true;
}

bool returnGame(string gameID) {
    int gameIndex = gameHash.search(gameID);
    if (gameIndex == -1) {
        cout << "ERROR: Game " << gameID << " not found!" << endl;
        return false;
    }

    if (games[gameIndex].getStatus() == "Available") {
        cout << "ERROR: Game is not currently borrowed!" << endl;
        return false;
    }

    string memberID = games[gameIndex].getBorrowedBy();

    int memberIndex = findMember(memberID);
    if (memberIndex == -1) {
        cout << "ERROR: Member not found!" << endl;
        return false;
    }

    games[gameIndex].setStatus("Available");
    games[gameIndex].setBorrowedBy("");

    members[memberIndex].removeBorrowedGame(gameID);

    for (int i = recordCount - 1; i >= 0; i--) {
        if (records[i].getGameID() == gameID &&
            records[i].getMemberID() == memberID &&
            !records[i].getIsReturned()) {
            records[i].setReturnDate(getCurrentDate());
            records[i].markAsReturned();
            break;
        }
    }

    cout << "\nSUCCESS: " << members[memberIndex].getName() << " returned \""
        << games[gameIndex].getTitle() << "\"" << endl;

    return true;
}

int main()
{
    cout << "Hello World!\n";
}
