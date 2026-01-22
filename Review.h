#pragma once
#include <string>
using namespace std;

class Review {
private:
    string gameID;
    string memberID;
    string memberName;
    int rating;        // 1-10
    string reviewText;
    string date;

public:
    // Constructor
    Review();
    Review(string gID, string mID, string mName, int r, string text, string d);

    // Getters
    string getGameID();
    string getMemberID();
    string getMemberName();
    int getRating();
    string getReviewText();
    string getDate();

    // Display
    void display();
};