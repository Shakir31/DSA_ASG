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

int main()
{
    cout << "Hello World!\n";
}
