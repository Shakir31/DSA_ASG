#include "Review.h"
#include <iostream>
using namespace std;

/*
============================================================
Function    : Review (Default Constructor)
Description : Initializes a Review object with empty values
              and rating set to 0.
Input       : None
Return      : None
============================================================
*/
Review::Review() {
    gameID = "";
    memberID = "";
    memberName = "";
    rating = 0;
    reviewText = "";
    date = "";
}

/*
============================================================
Function    : Review (Parameterized Constructor)
Description : Initializes a Review object with the provided
              review details.
Input       : gID - Game ID (string)
              mID - Member ID (string)
              mName - Member name (string)
              r - Rating from 1-10 (int)
              text - Review text (string)
              d - Date of review (string)
Return      : None
============================================================
*/
Review::Review(string gID, string mID, string mName, int r, string text, string d) {
    gameID = gID;
    memberID = mID;
    memberName = mName;
    rating = r;
    reviewText = text;
    date = d;
}

/*
============================================================
Function    : getGameID
Description : Returns the game ID of the reviewed game.
Input       : None
Return      : Game ID (string)
============================================================
*/
string Review::getGameID() { return gameID; }

/*
============================================================
Function    : getMemberID
Description : Returns the member ID of the reviewer.
Input       : None
Return      : Member ID (string)
============================================================
*/
string Review::getMemberID() { return memberID; }

/*
============================================================
Function    : getMemberName
Description : Returns the name of the reviewer.
Input       : None
Return      : Member name (string)
============================================================
*/
string Review::getMemberName() { return memberName; }

/*
============================================================
Function    : getRating
Description : Returns the rating given in the review.
Input       : None
Return      : Rating (1-10) (int)
============================================================
*/
int Review::getRating() { return rating; }

/*
============================================================
Function    : getReviewText
Description : Returns the text content of the review.
Input       : None
Return      : Review text (string)
============================================================
*/
string Review::getReviewText() { return reviewText; }

/*
============================================================
Function    : getDate
Description : Returns the date the review was written.
Input       : None
Return      : Review date (string)
============================================================
*/
string Review::getDate() { return date; }

/*
============================================================
Function    : display
Description : Displays the complete review information
              including reviewer name, rating, review text,
              and date to the console in a formatted layout.
Input       : None
Return      : None
============================================================
*/
void Review::display() {
    cout << "----------------------------------------" << endl;
    cout << "Reviewer: " << memberName << " (" << memberID << ")" << endl;
    cout << "Rating: " << rating << "/10" << endl;
    cout << "Review: " << reviewText << endl;
    cout << "Date: " << date << endl;
    cout << "----------------------------------------" << endl;
}