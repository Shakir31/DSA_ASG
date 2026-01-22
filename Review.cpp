#include "Review.h"
#include <iostream>
using namespace std;

Review::Review() {
    gameID = "";
    memberID = "";
    memberName = "";
    rating = 0;
    reviewText = "";
    date = "";
}

Review::Review(string gID, string mID, string mName, int r, string text, string d) {
    gameID = gID;
    memberID = mID;
    memberName = mName;
    rating = r;
    reviewText = text;
    date = d;
}

string Review::getGameID() { return gameID; }
string Review::getMemberID() { return memberID; }
string Review::getMemberName() { return memberName; }
int Review::getRating() { return rating; }
string Review::getReviewText() { return reviewText; }
string Review::getDate() { return date; }

void Review::display() {
    cout << "----------------------------------------" << endl;
    cout << "Reviewer: " << memberName << " (" << memberID << ")" << endl;
    cout << "Rating: " << rating << "/10" << endl;
    cout << "Review: " << reviewText << endl;
    cout << "Date: " << date << endl;
    cout << "----------------------------------------" << endl;
}