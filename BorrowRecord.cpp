#include "BorrowRecord.h"
#include <iostream>
using namespace std;

BorrowRecord::BorrowRecord() {
    gameID = "";
    memberID = "";
    borrowDate = "";
    returnDate = "";
    isReturned = false;
}

BorrowRecord::BorrowRecord(string gID, string mID, string bDate) {
    gameID = gID;
    memberID = mID;
    borrowDate = bDate;
    returnDate = "";
    isReturned = false;
}

string BorrowRecord::getGameID() { return gameID; }
string BorrowRecord::getMemberID() { return memberID; }
string BorrowRecord::getBorrowDate() { return borrowDate; }
string BorrowRecord::getReturnDate() { return returnDate; }
bool BorrowRecord::getIsReturned() { return isReturned; }

void BorrowRecord::setReturnDate(string rDate) {
    returnDate = rDate;
}

void BorrowRecord::markAsReturned() {
    isReturned = true;
}

void BorrowRecord::display() {
    cout << "Game: " << gameID << " | Member: " << memberID;
    cout << " | Borrowed: " << borrowDate;
    if (isReturned) {
        cout << " | Returned: " << returnDate;
    }
    else {
        cout << " | Status: Currently Borrowed";
    }
    cout << endl;
}

string BorrowRecord::toString() {
    return gameID + "|" + memberID + "|" + borrowDate + "|" + returnDate;
}