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

// Global storage
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

#include <iostream>

int main()
{
    cout << "Hello World!\n";
}
