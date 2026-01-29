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
        cout << "ERROR: Game is already borrowed by " << games[gameIndex].getBorrowedBy() << endl;
        return false;
    }

    games[gameIndex].setStatus("Borrowed");
    games[gameIndex].setBorrowedBy(memberID);
    games[gameIndex].incrementBorrowCount();

    members[memberIndex].addBorrowedGame(gameID);

    records[recordCount] = BorrowRecord(gameID, memberID, getCurrentDate());
    recordCount++;

    cout << "\nSUCCESS: " << members[memberIndex].getName() << " borrowed \""
        << games[gameIndex].getTitle() << "\"" << endl;

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

    cout << "\nSUCCESS: " << members[memberIndex].getName() << " returned \""
        << games[gameIndex].getTitle() << "\"" << endl;

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

    Game* L = new Game[n1];
    Game* R = new Game[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

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

int searchByPlayerCount(int numPlayers, Game results[], int maxResults) {
    int count = 0;

    for (int i = 0; i < gameCount && count < maxResults; i++) {
        if (games[i].getMinPlayers() <= numPlayers &&
            games[i].getMaxPlayers() >= numPlayers) {
            results[count] = games[i];
            count++;
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

        string title = results[i].getTitle();
        if (title.length() > 34) {
            title = title.substr(0, 31) + "...";
        }
        cout << title;
        for (int j = title.length(); j < 34; j++) cout << " ";

        cout << " | " << results[i].getYear() << " | ";
        cout << results[i].getMinPlayers() << "-" << results[i].getMaxPlayers();
        if (results[i].getMaxPlayers() < 10) cout << "  ";
        else cout << " ";
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

    if (count == 0) {
        cout << "No reviews yet for this game." << endl;
    }
    else {
        double avgRating = (double)totalRating / count;
        cout << "\nAverage Rating: " << avgRating << "/10 (" << count << " reviews)" << endl;
    }
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

void displayMemberBorrowedGames(string memberID) {
    int memberIndex = findMember(memberID);
    if (memberIndex == -1) {
        cout << "ERROR: Member not found!" << endl;
        return;
    }

    cout << "\n=== Your Borrowed Games ===" << endl;
    members[memberIndex].display();
    cout << "\nGame IDs you currently have borrowed:" << endl;
    members[memberIndex].displayBorrowedGames();
}

void memberAddReview(string memberID) {
    cout << "\n=== Write a Review ===" << endl;

    string gameID;
    int rating;
    string reviewText;

    cout << "Enter game ID: ";
    cin >> gameID;
    clearInputBuffer();

    cout << "Enter rating (1-10): ";
    cin >> rating;
    clearInputBuffer();

    cout << "Enter your review: ";
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

    cout << "\nWould you like to see reviews? (y/n): ";
    char choice;
    cin >> choice;
    clearInputBuffer();

    if (choice == 'y' || choice == 'Y') {
        displayReviewsForGame(gameID);
    }
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
        cout << "3. View My Borrowed Games" << endl;
        cout << "4. Write a Review" << endl;
        cout << "5. Logout" << endl;
        cout << "======================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;
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
            displayMemberBorrowedGames(memberID);
            pauseScreen();
            break;
        case 4:
            memberAddReview(memberID);
            pauseScreen();
            break;
        case 5:
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 5);
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

int main()
{
    //load games from CSV
    cout << "Loading games from database..." << endl;
    gameCount = loadGamesFromCSV("C:\\Users\\milok\\Downloads\\games.csv", games, MAX_GAMES);

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

    //start main menu
    mainMenu();
}
