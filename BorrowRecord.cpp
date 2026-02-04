#include "BorrowRecord.h"
#include <iostream>
using namespace std;

/*
============================================================
Function    : BorrowRecord (Default Constructor)
Description : Initializes a BorrowRecord object with default
              empty values and isReturned set to false.
Input       : None
Return      : None
============================================================
*/
BorrowRecord::BorrowRecord() {
    gameID = "";
    memberID = "";
    borrowDate = "";
    returnDate = "";
    isReturned = false;
}

/*
============================================================
Function    : BorrowRecord (Parameterized Constructor)
Description : Initializes a BorrowRecord object with the
              provided game ID, member ID, and borrow date.
Input       : gID - Game ID (string)
              mID - Member ID (string)
              bDate - Borrow date (string)
Return      : None
============================================================
*/
BorrowRecord::BorrowRecord(string gID, string mID, string bDate) {
    gameID = gID;
    memberID = mID;
    borrowDate = bDate;
    returnDate = "";
    isReturned = false;
}

/*
============================================================
Function    : getGameID
Description : Returns the game ID of this borrow record.
Input       : None
Return      : Game ID (string)
============================================================
*/
string BorrowRecord::getGameID() { return gameID; }

/*
============================================================
Function    : getMemberID
Description : Returns the member ID of this borrow record.
Input       : None
Return      : Member ID (string)
============================================================
*/
string BorrowRecord::getMemberID() { return memberID; }

/*
============================================================
Function    : getBorrowDate
Description : Returns the borrow date of this record.
Input       : None
Return      : Borrow date (string)
============================================================
*/
string BorrowRecord::getBorrowDate() { return borrowDate; }

/*
============================================================
Function    : getReturnDate
Description : Returns the return date of this record.
Input       : None
Return      : Return date (string, empty if not returned)
============================================================
*/
string BorrowRecord::getReturnDate() { return returnDate; }

/*
============================================================
Function    : getIsReturned
Description : Returns whether the game has been returned.
Input       : None
Return      : True if returned, false otherwise (bool)
============================================================
*/
bool BorrowRecord::getIsReturned() { return isReturned; }

/*
============================================================
Function    : setReturnDate
Description : Sets the return date for this borrow record.
Input       : rDate - Return date (string)
Return      : None
============================================================
*/
void BorrowRecord::setReturnDate(string rDate) {
    returnDate = rDate;
}

/*
============================================================
Function    : markAsReturned
Description : Marks this borrow record as returned by setting
              the isReturned flag to true.
Input       : None
Return      : None
============================================================
*/
void BorrowRecord::markAsReturned() {
    isReturned = true;
}

/*
============================================================
Function    : display
Description : Displays the borrow record information including
              game ID, member ID, borrow date, and return
              status to the console.
Input       : None
Return      : None
============================================================
*/
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

/*
============================================================
Function    : toString
Description : Converts the borrow record to a pipe-delimited
              string format.
Input       : None
Return      : Formatted string (string)
============================================================
*/
string BorrowRecord::toString() {
    return gameID + "|" + memberID + "|" + borrowDate + "|" + returnDate;
}