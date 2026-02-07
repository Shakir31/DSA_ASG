/*
============================================================
NPTTGC Board Game Management System
============================================================
Team Members: Shakir, Ethan
Group:        Team 5
Student IDs:  S10269080A, S10266834
============================================================
Features Highlighted:
- Hash Table for O(1) game lookup by Game ID
- Linked List to store each member's borrowed games
- Merge Sort with user-selectable sort key (Year / Title)
- Game Reviews: write, display, average rating
- Full borrow/return summary for members and admin
- Advanced Feature (Ethan): Record game play session (players + winner)
============================================================
*/

#include <iostream>
#include <string>
#include <limits>
#include <ctime>
#include "Game.h"
#include "Member.h"
#include "GameManager.h"
#include "HashTable.h"
#include "BorrowRecord.h"
#include "Review.h"
#include "Admin.h"

using namespace std;

// ============= GLOBAL STORAGE =============
const int MAX_GAMES = 1000;
const int MAX_MEMBERS = 100;
const int MAX_RECORDS = 1000;
const int MAX_REVIEWS = 1000;

Game         games[MAX_GAMES];
Member       members[MAX_MEMBERS];
BorrowRecord records[MAX_RECORDS];
Review       reviews[MAX_REVIEWS];

int gameCount = 0;
int memberCount = 0;
int recordCount = 0;
int reviewCount = 0;

// ============= ADVANCED FEATURE: PLAY RECORDING (ARRAY) =============
const int MAX_PLAY_RECORDS = 2000;
const int MAX_SESSION_PLAYERS = 8;

struct PlayRecord {
    string gameID;
    int numPlayers;
    string playerIDs[MAX_SESSION_PLAYERS];
    string winnerID;
    string recordedBy;
    string timestamp;   // YYYY-MM-DD HH:MM
};

PlayRecord playRecords[MAX_PLAY_RECORDS];
int playRecordCount = 0;

HashTable gameHash;

// ============= SORT MODE ENUM =============
enum SortMode { SORT_BY_YEAR, SORT_BY_TITLE };

// ============= HELPER FUNCTIONS =============

/*
============================================================
Function    : getCurrentDate
Description : Returns the current date as a string in the
              format "YYYY-MM-DD" using the system clock.
Input       : None
Return      : string - today's date
============================================================
*/
string getCurrentDate() {
    time_t now = time(NULL);
    struct tm today;
    localtime_s(&today, &now);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &today);
    return string(buf);
}

/*
============================================================
Function    : clearInputBuffer
Description : Clears any leftover characters in the standard
              input buffer after reading with cin >>.
Input       : None
Return      : None
============================================================
*/
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/*
============================================================
Function    : pauseScreen
Description : Pauses execution until the user presses Enter.
Input       : None
Return      : None
============================================================
*/
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/*
============================================================
Function    : findMember
Description : Performs a linear search through the members
              array to locate a member by their ID.
Input       : string memberID - the ID to search for
Return      : int - index in members[] if found, -1 otherwise
============================================================
*/
int findMember(string memberID) {
    for (int i = 0; i < memberCount; i++) {
        if (members[i].getMemberID() == memberID) {
            return i;
        }
    }
    return -1;
}

/*
============================================================
Function    : calculateAverageRating
Description : Scans the global reviews[] array and computes
              the average rating for a given game.
Input       : string gameID - the game to average
Return      : double - average rating (0.0 if no reviews)
============================================================
*/
double calculateAverageRating(string gameID) {
    int totalRating = 0;
    int count = 0;

    for (int i = 0; i < reviewCount; i++) {
        if (reviews[i].getGameID() == gameID) {
            totalRating += reviews[i].getRating();
            count++;
        }
    }

    if (count == 0) return 0.0;
    return (double)totalRating / count;
}

// ============= ADVANCED FEATURE: PLAY RECORDING FUNCTIONS =============

bool memberExistsByID(const string& memberID) {
    return findMember(memberID) != -1;
}

bool getGameMinMaxPlayers(const string& gameID, int& minP, int& maxP) {
    int idx = gameHash.search(gameID);
    if (idx == -1) return false;
    minP = games[idx].getMinPlayers();
    maxP = games[idx].getMaxPlayers();
    return true;
}

bool isInPlayersList(const string players[], int n, const string& id) {
    for (int i = 0; i < n; i++) {
        if (players[i] == id) return true;
    }
    return false;
}

string getCurrentTimestamp() {
    time_t now = time(NULL);
    struct tm t;
    localtime_s(&t, &now);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &t);
    return string(buf);
}

void recordPlaySession(const string& currentMemberID) {
    if (playRecordCount >= MAX_PLAY_RECORDS) {
        cout << "ERROR: Play record storage is full.\n";
        return;
    }

    cout << "\n=== Record Game Play Session ===\n";

    string gameID;
    cout << "Enter Game ID: ";
    cin >> gameID;
    clearInputBuffer();

    int minP, maxP;
    if (!getGameMinMaxPlayers(gameID, minP, maxP)) {
        cout << "ERROR: Game not found!\n";
        return;
    }

    int n;
    cout << "Number of players (" << minP << " to " << maxP << "): ";
    if (!(cin >> n)) {
        clearInputBuffer();
        cout << "ERROR: Invalid input.\n";
        return;
    }
    clearInputBuffer();

    if (n < minP || n > maxP || n > MAX_SESSION_PLAYERS) {
        cout << "ERROR: Invalid number of players.\n";
        return;
    }

    string players[MAX_SESSION_PLAYERS];

    for (int i = 0; i < n; i++) {
        cout << "Enter Member ID for Player " << (i + 1) << ": ";
        cin >> players[i];
        clearInputBuffer();

        if (!memberExistsByID(players[i])) {
            cout << "ERROR: Member ID not found.\n";
            return;
        }

        for (int j = 0; j < i; j++) {
            if (players[j] == players[i]) {
                cout << "ERROR: Duplicate Member ID entered.\n";
                return;
            }
        }
    }

    string winner;
    cout << "Enter Winner Member ID: ";
    cin >> winner;
    clearInputBuffer();

    if (!memberExistsByID(winner)) {
        cout << "ERROR: Winner Member ID not found.\n";
        return;
    }

    if (!isInPlayersList(players, n, winner)) {
        cout << "ERROR: Winner must be one of the players entered.\n";
        return;
    }

    PlayRecord pr;
    pr.gameID = gameID;
    pr.numPlayers = n;
    for (int i = 0; i < n; i++) pr.playerIDs[i] = players[i];
    pr.winnerID = winner;
    pr.recordedBy = currentMemberID;
    pr.timestamp = getCurrentTimestamp();

    playRecords[playRecordCount++] = pr;

    cout << "SUCCESS: Play session recorded.\n";
}

void viewMyPlayHistory(const string& memberID) {
    cout << "\n=== My Play History (" << memberID << ") ===\n";
    bool found = false;

    for (int i = 0; i < playRecordCount; i++) {
        for (int p = 0; p < playRecords[i].numPlayers; p++) {
            if (playRecords[i].playerIDs[p] == memberID) {
                found = true;
                cout << playRecords[i].timestamp
                    << " | Game: " << playRecords[i].gameID
                    << " | Winner: " << playRecords[i].winnerID
                    << " | Recorded By: " << playRecords[i].recordedBy << "\n";
                break;
            }
        }
    }

    if (!found) cout << "No play sessions found.\n";
}

void viewGamePlayHistory() {
    cout << "\n=== View Game Play History ===\n";

    string gameID;
    cout << "Enter Game ID: ";
    cin >> gameID;
    clearInputBuffer();

    bool found = false;
    cout << "\n=== Play History for Game " << gameID << " ===\n";

    for (int i = 0; i < playRecordCount; i++) {
        if (playRecords[i].gameID == gameID) {
            found = true;
            cout << playRecords[i].timestamp
                << " | Winner: " << playRecords[i].winnerID
                << " | Players: ";

            for (int p = 0; p < playRecords[i].numPlayers; p++) {
                cout << playRecords[i].playerIDs[p];
                if (p != playRecords[i].numPlayers - 1) cout << ", ";
            }
            cout << " | Recorded By: " << playRecords[i].recordedBy << "\n";
        }
    }

    if (!found) cout << "No play sessions recorded for this game.\n";
}

// ============= BORROW / RETURN FUNCTIONS =============

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
        cout << "ERROR: Game is already borrowed by "
            << games[gameIndex].getBorrowedBy() << endl;
        return false;
    }

    games[gameIndex].setStatus("Borrowed");
    games[gameIndex].setBorrowedBy(memberID);
    games[gameIndex].incrementBorrowCount();

    members[memberIndex].addBorrowedGame(gameID);

    records[recordCount] = BorrowRecord(gameID, memberID, getCurrentDate());
    recordCount++;

    cout << "\nSUCCESS: " << members[memberIndex].getName()
        << " borrowed \"" << games[gameIndex].getTitle() << "\"" << endl;

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

    cout << "\nSUCCESS: " << members[memberIndex].getName()
        << " returned \"" << games[gameIndex].getTitle() << "\"" << endl;

    return true;
}

// ============= SEARCH / SORT FUNCTIONS =============

void displayGameDetails(string gameID) {
    int index = gameHash.search(gameID);
    if (index == -1) {
        cout << "Game not found!" << endl;
        return;
    }
    games[index].display();
}

void merge(Game arr[], int left, int mid, int right, SortMode mode) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Game* L = new Game[n1];
    Game* R = new Game[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        bool takeLeft = false;

        if (mode == SORT_BY_YEAR) {
            if (L[i].getYear() < R[j].getYear())
                takeLeft = true;
            else if (L[i].getYear() == R[j].getYear())
                takeLeft = (L[i].getTitle() <= R[j].getTitle());
            else
                takeLeft = false;
        }
        else {   // SORT_BY_TITLE
            takeLeft = (L[i].getTitle() <= R[j].getTitle());
        }

        if (takeLeft) { arr[k] = L[i]; i++; }
        else { arr[k] = R[j]; j++; }
        k++;
    }

    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }

    delete[] L;
    delete[] R;
}

void mergeSort(Game arr[], int left, int right, SortMode mode) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, mode);
        mergeSort(arr, mid + 1, right, mode);
        merge(arr, left, mid, right, mode);
    }
}

int searchByPlayerCount(int numPlayers, Game results[], int maxResults) {
    int count = 0;

    for (int i = 0; i < gameCount && count < maxResults; i++) {
        if (games[i].getMinPlayers() <= numPlayers &&
            games[i].getMaxPlayers() >= numPlayers) {
            results[count] = games[i];
            count++;
        }
    }

    return count;
}

void displaySearchResults(Game results[], int count) {
    if (count == 0) {
        cout << "No games found." << endl;
        return;
    }

    cout << "\nFound " << count << " games:\n" << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
    cout << "ID    | Title                              | Year | Players | Avg Rating | Status" << endl;
    cout << "---------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < count; i++) {
        cout << results[i].getGameID() << " | ";

        string title = results[i].getTitle();
        if (title.length() > 34) {
            title = title.substr(0, 31) + "...";
        }
        cout << title;
        for (int j = (int)title.length(); j < 34; j++) cout << " ";

        cout << " | " << results[i].getYear() << " | ";

        cout << results[i].getMinPlayers() << "-" << results[i].getMaxPlayers();
        if (results[i].getMaxPlayers() < 10) cout << "  ";
        else cout << " ";

        cout << " |    ";
        double avg = calculateAverageRating(results[i].getGameID());
        if (avg > 0.0) {
            int whole = (int)avg;
            int fraction = (int)((avg - whole) * 10 + 0.5);
            if (fraction == 10) { whole++; fraction = 0; }
            cout << whole << "." << fraction << "/10";
            cout << "  ";
        }
        else {
            cout << "  N/A      ";
        }

        cout << " | " << results[i].getStatus() << endl;
    }
    cout << "---------------------------------------------------------------------------------" << endl;
}

// ============= REVIEW FUNCTIONS =============

bool addReview(string memberID, string gameID, int rating, string reviewText) {
    if (rating < 1 || rating > 10) {
        cout << "ERROR: Rating must be between 1 and 10!" << endl;
        return false;
    }

    int memberIndex = findMember(memberID);
    if (memberIndex == -1) {
        cout << "ERROR: Member not found!" << endl;
        return false;
    }

    int gameIndex = gameHash.search(gameID);
    if (gameIndex == -1) {
        cout << "ERROR: Game not found!" << endl;
        return false;
    }

    reviews[reviewCount] = Review(
        gameID,
        memberID,
        members[memberIndex].getName(),
        rating,
        reviewText,
        getCurrentDate()
    );
    reviewCount++;

    cout << "\nSUCCESS: Review added for \"" << games[gameIndex].getTitle() << "\"" << endl;
    return true;
}

void displayReviewsForGame(string gameID) {
    int gameIndex = gameHash.search(gameID);
    if (gameIndex == -1) {
        cout << "ERROR: Game not found!" << endl;
        return;
    }

    cout << "\n======================================" << endl;
    cout << "Reviews for: " << games[gameIndex].getTitle() << endl;
    cout << "======================================" << endl;

    int count = 0;
    int totalRating = 0;

    for (int i = 0; i < reviewCount; i++) {
        if (reviews[i].getGameID() == gameID) {
            reviews[i].display();
            totalRating += reviews[i].getRating();
            count++;
        }
    }

    if (count == 0) {
        cout << "No reviews yet for this game." << endl;
    }
    else {
        double avgRating = (double)totalRating / count;
        cout << "\nAverage Rating: " << avgRating << "/10 (" << count << " reviews)" << endl;
    }
}

// ============= SUMMARY FUNCTIONS =============

void displayMemberSummary(string memberID) {
    int memberIndex = findMember(memberID);
    if (memberIndex == -1) {
        cout << "ERROR: Member not found!" << endl;
        return;
    }

    cout << "\n======================================" << endl;
    cout << "  Borrow/Return Summary for: "
        << members[memberIndex].getName() << endl;
    cout << "======================================" << endl;

    int totalBorrowed = 0;
    int totalReturned = 0;
    int currentlyOwned = 0;

    cout << "\n---- Transaction History ----" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "# | Game ID | Game Title                         | Borrowed   | Returned   | Status" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    int rowNum = 1;
    for (int i = 0; i < recordCount; i++) {
        if (records[i].getMemberID() != memberID) continue;

        totalBorrowed++;

        int gIdx = gameHash.search(records[i].getGameID());
        string title = (gIdx != -1) ? games[gIdx].getTitle() : "Unknown";
        if (title.length() > 34) title = title.substr(0, 31) + "...";

        cout << rowNum;
        if (rowNum < 10) cout << " ";
        cout << "| " << records[i].getGameID() << " | ";

        cout << title;
        for (int j = (int)title.length(); j < 34; j++) cout << " ";

        cout << " | " << records[i].getBorrowDate() << " | ";

        if (records[i].getIsReturned()) {
            totalReturned++;
            cout << records[i].getReturnDate() << " | Returned" << endl;
        }
        else {
            currentlyOwned++;
            cout << "  --      | Borrowed" << endl;
        }

        rowNum++;
    }

    cout << "--------------------------------------------------------------------------------" << endl;

    if (totalBorrowed == 0) {
        cout << "No transactions found for this member." << endl;
    }
    else {
        cout << "\nSummary:" << endl;
        cout << "  Total times borrowed : " << totalBorrowed << endl;
        cout << "  Total times returned : " << totalReturned << endl;
        cout << "  Currently borrowed   : " << currentlyOwned << endl;
    }
    cout << "======================================" << endl;
}

// ============= MEMBER MENU FUNCTIONS =============

void memberBorrowGame(string memberID) {
    cout << "\n=== Borrow Game ===" << endl;

    string gameID;
    cout << "Enter game ID to borrow: ";
    cin >> gameID;
    clearInputBuffer();

    borrowGame(memberID, gameID);
}

void memberReturnGame(string memberID) {
    cout << "\n=== Return Game ===" << endl;

    int memberIndex = findMember(memberID);
    if (memberIndex != -1) {
        cout << "\nYour currently borrowed games:" << endl;
        members[memberIndex].displayBorrowedGames();
    }

    string gameID;
    cout << "\nEnter game ID to return: ";
    cin >> gameID;
    clearInputBuffer();

    int gameIndex = gameHash.search(gameID);
    if (gameIndex == -1) {
        cout << "ERROR: Game not found!" << endl;
        return;
    }

    if (games[gameIndex].getStatus() == "Available") {
        cout << "ERROR: This game is not currently borrowed!" << endl;
        return;
    }

    if (games[gameIndex].getBorrowedBy() != memberID) {
        cout << "ERROR: You cannot return this game!" << endl;
        cout << "This game was borrowed by member " << games[gameIndex].getBorrowedBy() << endl;
        return;
    }

    returnGame(gameID);
}

void memberAddReview(string memberID) {
    cout << "\n=== Write a Review ===" << endl;

    string gameID;
    int rating;
    string reviewText;

    cout << "Enter game ID      : ";
    cin >> gameID;
    clearInputBuffer();

    cout << "Enter rating (1-10): ";
    cin >> rating;
    clearInputBuffer();

    cout << "Enter your review  : ";
    getline(cin, reviewText);

    addReview(memberID, gameID, rating, reviewText);
}

// ============= SEARCH MENU FUNCTIONS =============

void searchGamesByPlayers() {
    cout << "\n=== Search Games by Player Count ===" << endl;

    int numPlayers;
    cout << "Enter number of players: ";
    cin >> numPlayers;
    clearInputBuffer();

    Game results[MAX_GAMES];
    int count = searchByPlayerCount(numPlayers, results, MAX_GAMES);

    if (count == 0) {
        cout << "\nNo games found for " << numPlayers << " players." << endl;
        return;
    }

    cout << "\nHow would you like the results sorted?" << endl;
    cout << "1. By Year of Publication (oldest first)" << endl;
    cout << "2. By Title (A - Z)" << endl;
    cout << "Enter choice: ";

    int sortChoice;
    cin >> sortChoice;
    clearInputBuffer();

    SortMode mode;
    switch (sortChoice) {
    case 2:
        mode = SORT_BY_TITLE;
        cout << "\nSorting by Title..." << endl;
        break;
    default:
        mode = SORT_BY_YEAR;
        cout << "\nSorting by Year..." << endl;
        break;
    }

    mergeSort(results, 0, count - 1, mode);
    displaySearchResults(results, count);
}

void displayGameWithReviews() {
    cout << "\n=== View Game Details ===" << endl;

    string gameID;
    cout << "Enter game ID: ";
    cin >> gameID;
    clearInputBuffer();

    displayGameDetails(gameID);

    double avgRating = calculateAverageRating(gameID);
    if (avgRating > 0) {
        cout << "\nAverage Rating: " << avgRating << "/10" << endl;
    }
    else {
        cout << "\nNo ratings yet for this game." << endl;
    }

    cout << "\nWould you like to see reviews? (y/n): ";
    char choice;
    cin >> choice;
    clearInputBuffer();

    if (choice == 'y' || choice == 'Y') {
        displayReviewsForGame(gameID);
    }
}

// ============= MENU FUNCTIONS =============

void adminMenu() {
    int choice;

    do {
        cout << "\n======================================" << endl;
        cout << "               ADMIN MENU" << endl;
        cout << "======================================" << endl;
        cout << "1. Add New Game" << endl;
        cout << "2. Remove Game" << endl;
        cout << "3. Add New Member" << endl;
        cout << "4. Display Summary of Games Borrowed/Returned" << endl;
        cout << "5. View All Games by Borrow Count" << endl;
        cout << "6. Back to Main Menu" << endl;
        cout << "======================================" << endl;
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "Invalid input! Please enter a number." << endl;
            pauseScreen();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
        case 1:
            adminAddGame(games, gameCount, MAX_GAMES, gameHash);
            pauseScreen();
            break;
        case 2:
            adminRemoveGame(games, gameCount, gameHash);
            pauseScreen();
            break;
        case 3:
            adminAddMember(members, memberCount, MAX_MEMBERS);
            pauseScreen();
            break;
        case 4:
            adminDisplaySummary(games, gameCount);
            pauseScreen();
            break;
        case 5:
            adminDisplayAllGamesSorted(games, gameCount);
            pauseScreen();
            break;
        case 6:
            cout << "Returning to main menu..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
            break;
        }

    } while (choice != 6);
}

void memberMenu() {
    string memberID;

    cout << "\n=== Member Login ===" << endl;
    cout << "Enter your Member ID: ";
    cin >> memberID;
    clearInputBuffer();

    int memberIndex = findMember(memberID);
    if (memberIndex == -1) {
        cout << "ERROR: Member ID not found!" << endl;
        pauseScreen();
        return;
    }

    cout << "\nWelcome, " << members[memberIndex].getName() << "!" << endl;

    int choice;

    do {
        cout << "\n======================================" << endl;
        cout << "         MEMBER MENU" << endl;
        cout << "======================================" << endl;
        cout << "1. Borrow Game" << endl;
        cout << "2. Return Game" << endl;
        cout << "3. View My Borrow/Return Summary" << endl;
        cout << "4. Write a Review" << endl;
        cout << "5. Record a Play Session (Players + Winner)" << endl;
        cout << "6. View My Play History" << endl;
        cout << "7. View Game Play History" << endl;
        cout << "8. Logout" << endl;
        cout << "======================================" << endl;
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        clearInputBuffer();

        switch (choice) {
        case 1:
            memberBorrowGame(memberID);
            pauseScreen();
            break;
        case 2:
            memberReturnGame(memberID);
            pauseScreen();
            break;
        case 3:
            displayMemberSummary(memberID);
            pauseScreen();
            break;
        case 4:
            memberAddReview(memberID);
            pauseScreen();
            break;
        case 5:
            recordPlaySession(memberID);
            pauseScreen();
            break;
        case 6:
            viewMyPlayHistory(memberID);
            pauseScreen();
            break;
        case 7:
            viewGamePlayHistory();
            pauseScreen();
            break;
        case 8:
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 8);
}

void searchMenu() {
    int choice;

    do {
        cout << "\n======================================" << endl;
        cout << "      SEARCH & DISPLAY MENU" << endl;
        cout << "======================================" << endl;
        cout << "1. View Game Details" << endl;
        cout << "2. Search Games by Player Count" << endl;
        cout << "3. Back to Main Menu" << endl;
        cout << "======================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1:
            displayGameWithReviews();
            pauseScreen();
            break;
        case 2:
            searchGamesByPlayers();
            pauseScreen();
            break;
        case 3:
            cout << "Returning to main menu..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 3);
}

void mainMenu() {
    int choice;

    do {
        cout << "\n======================================" << endl;
        cout << "   NPTTGC BOARD GAME MANAGEMENT" << endl;
        cout << "======================================" << endl;
        cout << "1. Admin Menu" << endl;
        cout << "2. Member Menu" << endl;
        cout << "3. Search & Display" << endl;
        cout << "4. Exit" << endl;
        cout << "======================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1:
            adminMenu();
            break;
        case 2:
            memberMenu();
            break;
        case 3:
            searchMenu();
            break;
        case 4:
            cout << "\nThank you for using NPTTGC Board Game Management System!" << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 4);
}

// ============= MAIN =============

int main() {
    cout << "Loading games from database..." << endl;
    gameCount = loadGamesFromCSV("games.csv", games, MAX_GAMES);

    if (gameCount == 0) {
        cout << "Failed to load games. Exiting." << endl;
        return 1;
    }

    buildHashTable(games, gameCount, gameHash);

    members[0] = Member("M001", "Alice Tan", "alice@email.com");
    members[1] = Member("M002", "Bob Lee", "bob@email.com");
    members[2] = Member("M003", "Charlie Wong", "charlie@email.com");
    memberCount = 3;

    cout << "\nSystem initialized successfully!" << endl;
    cout << "Test members created: M001, M002, M003" << endl;
    cout << "Today's date: " << getCurrentDate() << endl;

    mainMenu();

    return 0;
}
