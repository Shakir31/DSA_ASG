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
- Merge Sort with user-selectable sort key
  (Year / Title / Average Rating)
- Game Reviews: write, display, average rating
- Full borrow/return summary for members and admin
============================================================
*/

#include <iostream>
#include <string>
#include <limits>
#include "Game.h"
#include "Member.h"
#include "GameManager.h"
#include "HashTable.h"
#include "BorrowRecord.h"
#include "Review.h"
#include "Admin.h"

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
    time_t now = time(NULL);
    struct tm today;
    localtime_s(&today, &now);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &today);
    return string(buf);
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
        cout << "ERROR: Game is already borrowed by "
            << games[gameIndex].getBorrowedBy() << endl;
        return false;
    }

    // Update game
    games[gameIndex].setStatus("Borrowed");
    games[gameIndex].setBorrowedBy(memberID);
    games[gameIndex].incrementBorrowCount();

    // Update member's linked list
    members[memberIndex].addBorrowedGame(gameID);

    // Create borrow record
    records[recordCount] = BorrowRecord(gameID, memberID, getCurrentDate());
    recordCount++;

    cout << "\nSUCCESS: " << members[memberIndex].getName()
        << " borrowed \"" << games[gameIndex].getTitle() << "\"" << endl;

    return true;
}

/*
============================================================
Function : returnGame
Description: Validates the game exists and is currently
             borrowed, then updates the game status, the
             member's linked list, and marks the matching
             BorrowRecord as returned with today's date.
Input    : string gameID - game to return
Return   : bool – true if return succeeded, false otherwise
============================================================
*/
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
    int    memberIndex = findMember(memberID);
    if (memberIndex == -1) {
        cout << "ERROR: Member not found!" << endl;
        return false;
    }

    // Update game
    games[gameIndex].setStatus("Available");
    games[gameIndex].setBorrowedBy("");

    // Update member's linked list
    members[memberIndex].removeBorrowedGame(gameID);

    // Find the most recent unresolved record and mark returned
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

//SEARCH FUNCTIONS
void displayGameDetails(string gameID) {
    int index = gameHash.search(gameID);
    if (index == -1) {
        cout << "Game not found!" << endl;
        return;
    }
    games[index].display();
}

void merge(Game arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Temporary arrays for left and right halves
    Game* L = new Game[n1];
    Game* R = new Game[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].getYear() <= R[j].getYear()) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void mergeSort(Game arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

/*
============================================================
Function : searchByPlayerCount
Description: Linear scan through games[] collecting every
             game whose player range includes numPlayers.
             Results are copied into the caller-supplied
             results[] array (up to maxResults entries).
             Does NOT sort – the caller chooses sort order
             after this returns.
Input    : int  numPlayers – target player count
           Game results[]  – output array for matches
           int  maxResults – capacity of results[]
Return   : int – number of games stored in results[]
============================================================
*/
int searchByPlayerCount(int numPlayers, Game results[], int maxResults) {
    int count = 0;

    for (int i = 0; i < gameCount && count < maxResults; i++) {
        if (games[i].getMinPlayers() <= numPlayers &&
            games[i].getMaxPlayers() >= numPlayers) {
            results[count++] = games[i];
        }
    }

    if (count > 0) {
        mergeSort(results, 0, count - 1);
    }

    return count;
}

void displaySearchResults(Game results[], int count) {
    if (count == 0) {
        cout << "No games found." << endl;
        return;
    }

    cout << "\nFound " << count << " games (sorted by year):\n" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "ID    | Title                              | Year | Players | Status" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < count; i++) {
        cout << results[i].getGameID() << " | ";

        // Truncate long titles
        string title = results[i].getTitle();
        if (title.length() > 34) title = title.substr(0, 31) + "...";
        cout << title;
        for (int j = (int)title.length(); j < 34; j++) cout << " ";

        cout << " | " << results[i].getYear() << " | ";

        // Players column
        cout << results[i].getMinPlayers() << "-" << results[i].getMaxPlayers();
        if (results[i].getMaxPlayers() < 10) cout << "  ";
        else                                  cout << " ";

        // Average rating column
        cout << " |    ";
        double avg = calculateAverageRating(results[i].getGameID());
        if (avg > 0.0) {
            // Print to 1 decimal place manually (no <iomanip> needed)
            int whole = (int)avg;
            int fraction = (int)((avg - whole) * 10 + 0.5);   // round
            if (fraction == 10) { whole++; fraction = 0; }
            cout << whole << "." << fraction << "/10";
            cout << "  ";
        }
        else {
            cout << "  N/A      ";
        }

        cout << " | " << results[i].getStatus() << endl;
    }
    cout << "--------------------------------------------------------------------------------" << endl;
}

//REVIEW FUNCTIONS
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

    if (count == 0) cout << "No reviews yet for this game." << endl;
    else {
        double avgRating = (double)totalRating / count;
        cout << "\nAverage Rating: " << avgRating << "/10 (" << count << " reviews)" << endl;
    }
}

// ============= SUMMARY FUNCTIONS =============

/*
============================================================
Function : displayMemberSummary
Description: Shows a full borrow/return history for one
             member.  Scans records[] and prints each
             transaction, clearly marking whether each game
             is still out or has been returned (with date).
             Ends with a count of currently borrowed games.
Input    : string memberID – the member whose summary to show
Return   : None
============================================================
*/
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

        // Look up the game title via hash table
        int gIdx = gameHash.search(records[i].getGameID());
        string title = (gIdx != -1) ? games[gIdx].getTitle() : "Unknown";
        if (title.length() > 34) title = title.substr(0, 31) + "...";

        // Row number (pad to 1 char width)
        cout << rowNum;
        if (rowNum < 10) cout << " ";
        cout << "| " << records[i].getGameID() << " | ";

        // Title (padded to 34 chars)
        cout << title;
        for (int j = (int)title.length(); j < 34; j++) cout << " ";

        // Borrow date
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

//MEMBER FUNCTIONS
void memberBorrowGame(string memberID) {
    cout << "\n=== Borrow Game ===" << endl;

    string gameID;
    cout << "Enter game ID to borrow: ";
    cin >> gameID;
    clearInputBuffer();

    borrowGame(memberID, gameID);
}

/*
============================================================
Function : memberReturnGame
Description: Shows the member's currently borrowed games,
             asks for a game ID, verifies ownership, then
             delegates to returnGame().
Input    : string memberID – the currently logged-in member
Return   : None
============================================================
*/
void memberReturnGame(string memberID) {
    cout << "\n=== Return Game ===" << endl;

    //show member's borrowed games first
    int memberIndex = findMember(memberID);
    if (memberIndex != -1) {
        cout << "\nYour currently borrowed games:" << endl;
        members[memberIndex].displayBorrowedGames();
    }

    string gameID;
    cout << "\nEnter game ID to return: ";
    cin >> gameID;
    clearInputBuffer();

    //check if this member borrowed the game
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

/*
============================================================
Function : memberAddReview
Description: Prompts the logged-in member for a game ID,
             rating and review text, then calls addReview().
Input    : string memberID – the currently logged-in member
Return   : None
============================================================
*/
void memberAddReview(string memberID) {
    cout << "\n=== Write a Review ===" << endl;

    string gameID;
    int    rating;
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

//SEARCH MENU FUNCTIONS
void searchGamesByPlayers() {
    cout << "\n=== Search Games by Player Count ===" << endl;

    int numPlayers;
    cout << "Enter number of players: ";
    cin >> numPlayers;
    clearInputBuffer();

    Game results[MAX_GAMES];
    int count = searchByPlayerCount(numPlayers, results, MAX_GAMES);

    displaySearchResults(results, count);
    delete[] results;
}

/*
============================================================
Function : displayGameWithReviews
Description: Asks for a game ID, displays its details and
             average rating, then optionally shows all
             reviews.
Input    : None (reads from cin)
Return   : None
============================================================
*/
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

    if (choice == 'y' || choice == 'Y') displayReviewsForGame(gameID);
}

//MENU FUNCTIONS
void adminMenu() {
    cout << "\n======================================" << endl;
    cout << "         ADMIN MENU (HAVENT IMPLEMENT)" << endl;
    cout << "======================================" << endl;
    cout << "1. Add New Game" << endl;
    cout << "2. Remove Game" << endl;
    cout << "3. Add New Member" << endl;
    cout << "4. Display All Transactions" << endl;
    cout << "5. Back to Main Menu" << endl;
    cout << "======================================" << endl;
    pauseScreen();
}

void memberMenu() {
    string memberID;

static void mergeSortGeneric(Game arr[], int left, int right,
    bool (*lessFn)(Game&, Game&)) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortGeneric(arr, left, mid, lessFn);
    mergeSortGeneric(arr, mid + 1, right, lessFn);
    mergeGeneric(arr, left, mid, right, lessFn);
}

static void adminDisplayAllGamesSortedMenu(Game gamesArr[], int count) {
    if (count <= 0) {
        cout << "No games to display.\n";
        return;
    }

    int choice;
    do {
        cout << "\n======================================\n";
        cout << "   DISPLAY ALL GAMES (SORT OPTIONS)\n";
        cout << "======================================\n";
        cout << "1) Title A -> Z (tie: GameID A->Z)\n";
        cout << "2) Title Z -> A (tie: GameID A->Z)\n";
        cout << "3) BorrowCount High -> Low (tie: Title A->Z, then GameID A->Z)\n";
        cout << "4) BorrowCount Low -> High (tie: Title A->Z, then GameID A->Z)\n";
        cout << "0) Back\n";
        cout << "Choice: ";

    do {
        cout << "\n======================================" << endl;
        cout << "         MEMBER MENU" << endl;
        cout << "======================================" << endl;
        cout << "1. Borrow Game" << endl;
        cout << "2. Return Game" << endl;
        cout << "3. View My Borrowed Games" << endl;
        cout << "4. Write a Review" << endl;
        cout << "5. Logout" << endl;
        cout << "======================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        clearInputBuffer();

        if (choice == 0) break;

        Game* temp = new Game[count];
        for (int i = 0; i < count; i++) temp[i] = gamesArr[i];

        switch (choice) {
        case 1:
            mergeSortGeneric(temp, 0, count - 1, lessTitleAsc);
            cout << "\n=== ALL GAMES SORTED BY TITLE (A -> Z) ===\n";
            printAllGamesTable(temp, count);
            break;
        case 2:
            mergeSortGeneric(temp, 0, count - 1, lessTitleDesc);
            cout << "\n=== ALL GAMES SORTED BY TITLE (Z -> A) ===\n";
            printAllGamesTable(temp, count);
            break;
        case 3:
            displayMemberBorrowedGames(memberID);
            pauseScreen();
            break;
        case 4:
            mergeSortGeneric(temp, 0, count - 1, lessBorrowAsc);
            cout << "\n=== ALL GAMES SORTED BY BORROWCOUNT (LOW -> HIGH) ===\n";
            printAllGamesTable(temp, count);
            break;
        default:
            cout << "❌ Invalid option.\n";
            break;
        }

        delete[] temp;

    } while (choice != 0);
}

void searchMenu() {
    int choice;

    do {
        cout << "\n======================================" << endl;
        cout << "               ADMIN MENU" << endl;
        cout << "======================================" << endl;
        cout << "1. Add New Game" << endl;
        cout << "2. Remove Game" << endl;
        cout << "3. Add New Member" << endl;
        cout << "4. Display Summary of Games Borrowed/Returned" << endl;
        cout << "5. Display ALL Games (Sorted)" << endl;
        cout << "6. Save Games to CSV (Persist Changes)" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "======================================" << endl;
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "Invalid input! Please enter a number.\n";
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
            adminDisplayAllGamesSortedMenu(games, gameCount);
            pauseScreen();
            break;
        case 6:
            if (saveGamesToCSV("games.csv", games, gameCount)) {
                cout << "✅ Saved successfully.\n";
            }
            else {
                cout << "❌ Save failed.\n";
            }
            pauseScreen();
            break;
        case 7:
            cout << "Returning to main menu...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
            pauseScreen();
            break;
        }

    } while (choice != 7);
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
        case 1: adminMenu(); break;
        case 2: memberMenu(); break;
        case 3: searchMenu(); break;
        case 4:
            cout << "\nThank you for using NPTTGC Board Game Management System!" << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 4);
}

int main()
{
    //load games from CSV
    cout << "Loading games from database..." << endl;

    // ✅ Working Directory is $(ProjectDir), and games.csv is in the project folder
    gameCount = loadGamesFromCSV("games.csv", games, MAX_GAMES);

    if (gameCount == 0) {
        cout << "Failed to load games. Exiting." << endl;
        return 1;
    }

    //build hash table
    buildHashTable(games, gameCount, gameHash);

    //create some test members
    members[0] = Member("M001", "Alice Tan", "alice@email.com");
    members[1] = Member("M002", "Bob Lee", "bob@email.com");
    members[2] = Member("M003", "Charlie Wong", "charlie@email.com");
    memberCount = 3;

    cout << "\nSystem initialized successfully!" << endl;
    cout << "Test members created: M001, M002, M003" << endl;
    cout << "Today's date: " << getCurrentDate() << endl;

    mainMenu();
}
