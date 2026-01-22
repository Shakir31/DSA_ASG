#pragma once
#include <string>
using namespace std;

class BorrowRecord {
private:
    string gameID;
    string memberID;
    string borrowDate;
    string returnDate;
    bool isReturned;

public:
    // Constructor
    BorrowRecord();
    BorrowRecord(string gID, string mID, string bDate);

    // Getters
    string getGameID();
    string getMemberID();
    string getBorrowDate();
    string getReturnDate();
    bool getIsReturned();

    // Setters
    void setReturnDate(string rDate);
    void markAsReturned();

    // Display
    void display();
    string toString();
};