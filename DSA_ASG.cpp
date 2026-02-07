#include <iostream>
#include <string>
#include <limits>
#include <cctype>

#include "Game.h"
#include "Member.h"
#include "GameManager.h"
#include "HashTable.h"
#include "BorrowRecord.h"
#include "Review.h"
#include "Admin.h"

using namespace std;

// ===================== GLOBAL STORAGE =====================
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

// ===================== FORWARD DECLARATIONS =====================
string getCurrentDate();
void clearInputBuffer();
void pauseScreen();

int findMember(string memberID);

bool borrowGame(string memberID, string gameID);
bool returnGame(string gameID);

void displayGameDetails(string gameID);
void merge(Game arr[], int left, int mid, int right);
void mergeSort(Game arr[], int left, int right);
int searchByPlayerCount(int numPlayers, Game results[], int maxResults);
void displaySearchResults(Game results[], int count);

bool addReview(string memberID, string gameID, int rating, string reviewText);
double calculateAverageRating(string gameID);
void displayReviewsForGame(string gameID);

void memberBorrowGame(string memberID);
void memberReturnGame(string memberID);
void displayMemberBorrowedGames(string memberID);
void memberAddReview(string memberID);
void memberMenu();

void searchGamesByPlayers();
void displayGameWithReviews();
void searchMenu();

void adminMenu();   // stays in this file
void mainMenu();

// (optional but safe) forward declare save (GameManager.h already has it)
bool saveGamesToCSV(string filename, Game games[], int gameCount);

// ===================== HELPER FUNCTIONS =====================
string getCurrentDate() {
    return "2026-01-18";
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// find member by ID
int findMember(string memberID) {
    for (int i = 0; i < memberCount; i++) {
        if (members[i].getMemberID() == memberID) {
            return i;
        }
    }
    return -1;
}

// ===================== BORROW / RETURN =====================
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

// ===================== SEARCH / DISPLAY =====================
void displayGameDetails(string gameID) {
    int index = gameHash.search(gameID);
    if (index == -1) {
        cout << "Game not found!" << endl;
        return;
    }
    games[index].display();
}

// merge sort by year (for search results)
void merge(Game arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Game* L = new Game[n1];
    Game* R = new Game[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].getYear() <= R[j].getYear()) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(Game arr[], int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int searchByPlayerCount(int numPlayers, Game results[], int maxResults) {
    int count = 0;

    for (int i = 0; i < gameCount && count < maxResults; i++) {
        if (games[i].getMinPlayers() <= numPlayers &&
            games[i].getMaxPlayers() >= numPlayers) {
            results[count++] = games[i];
        }
    }

    if (count > 0) mergeSort(results, 0, count - 1);
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
        if (title.length() > 34) title = title.substr(0, 31) + "...";
        cout << title;
        for (int j = (int)title.length(); j < 34; j++) cout << " ";

        cout << " | " << results[i].getYear() << " | ";
        cout << results[i].getMinPlayers() << "-" << results[i].getMaxPlayers();
        if (results[i].getMaxPlayers() < 10) cout << "  ";
        else cout << " ";
        cout << " | " << results[i].getStatus() << endl;
    }

    cout << "--------------------------------------------------------------------------------" << endl;
}

// ===================== REVIEW FUNCTIONS =====================
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
    int totalRating = 0, count = 0;

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

// ===================== MEMBER MENU FUNCTIONS =====================
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
        case 1: memberBorrowGame(memberID); pauseScreen(); break;
        case 2: memberReturnGame(memberID); pauseScreen(); break;
        case 3: displayMemberBorrowedGames(memberID); pauseScreen(); break;
        case 4: memberAddReview(memberID); pauseScreen(); break;
        case 5: cout << "Logging out..." << endl; break;
        default: cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 5);
}

// ===================== SEARCH MENU =====================
void searchGamesByPlayers() {
    cout << "\n=== Search Games by Player Count ===" << endl;

    int numPlayers;
    cout << "Enter number of players: ";
    cin >> numPlayers;
    clearInputBuffer();

    // heap allocate to avoid big stack warning
    Game* results = new Game[MAX_GAMES];
    int count = searchByPlayerCount(numPlayers, results, MAX_GAMES);
    displaySearchResults(results, count);
    delete[] results;
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

    if (choice == 'y' || choice == 'Y') displayReviewsForGame(gameID);
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
        case 1: displayGameWithReviews(); pauseScreen(); break;
        case 2: searchGamesByPlayers(); pauseScreen(); break;
        case 3: cout << "Returning to main menu..." << endl; break;
        default: cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 3);
}

// ===================== ADMIN SORTING DISPLAY ALL =====================

static string toLowerCopy(string s) {
    for (char& c : s) c = (char)tolower((unsigned char)c);
    return s;
}

static void printAllGamesTable(Game arr[], int count) {
    cout << "\n--------------------------------------------------------------------------------\n";
    cout << "No | GameID | Title                               | Year | BorrowCount | Status\n";
    cout << "--------------------------------------------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        cout << (i + 1) << "  | ";
        cout << arr[i].getGameID() << "  | ";

        string title = arr[i].getTitle();
        if (title.length() > 35) title = title.substr(0, 32) + "...";
        cout << title;
        for (int s = (int)title.length(); s < 35; s++) cout << " ";

        cout << " | " << arr[i].getYear() << " | ";
        cout << arr[i].getBorrowCount() << "          | ";
        cout << arr[i].getStatus() << "\n";
    }

    cout << "--------------------------------------------------------------------------------\n";
}

// Tie-breakers:
// Title sort: Title -> GameID
// Borrow sort: BorrowCount -> Title -> GameID
static bool lessTitleAsc(Game& a, Game& b) {
    string ta = toLowerCopy(a.getTitle());
    string tb = toLowerCopy(b.getTitle());
    if (ta != tb) return ta < tb;
    return a.getGameID() < b.getGameID();
}

static bool lessTitleDesc(Game& a, Game& b) {
    string ta = toLowerCopy(a.getTitle());
    string tb = toLowerCopy(b.getTitle());
    if (ta != tb) return ta > tb;
    return a.getGameID() < b.getGameID();
}

static bool lessBorrowDesc(Game& a, Game& b) {
    int ba = a.getBorrowCount();
    int bb = b.getBorrowCount();
    if (ba != bb) return ba > bb;

    string ta = toLowerCopy(a.getTitle());
    string tb = toLowerCopy(b.getTitle());
    if (ta != tb) return ta < tb;

    return a.getGameID() < b.getGameID();
}

static bool lessBorrowAsc(Game& a, Game& b) {
    int ba = a.getBorrowCount();
    int bb = b.getBorrowCount();
    if (ba != bb) return ba < bb;

    string ta = toLowerCopy(a.getTitle());
    string tb = toLowerCopy(b.getTitle());
    if (ta != tb) return ta < tb;

    return a.getGameID() < b.getGameID();
}

static void mergeGeneric(Game arr[], int left, int mid, int right,
    bool (*lessFn)(Game&, Game&)) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Game* L = new Game[n1];
    Game* R = new Game[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (lessFn(L[i], R[j])) arr[k++] = L[i++];
        else                    arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

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

        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "❌ Invalid input.\n";
            continue;
        }
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
            mergeSortGeneric(temp, 0, count - 1, lessBorrowDesc);
            cout << "\n=== ALL GAMES SORTED BY BORROWCOUNT (HIGH -> LOW) ===\n";
            printAllGamesTable(temp, count);
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

// ===================== ADMIN MENU (UPDATED WITH SAVE) =====================
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

// ===================== MAIN MENU =====================
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

// ===================== MAIN =====================
int main() {
    cout << "Loading games from database..." << endl;

    // ✅ Working Directory is $(ProjectDir), and games.csv is in the project folder
    gameCount = loadGamesFromCSV("games.csv", games, MAX_GAMES);

    if (gameCount == 0) {
        cout << "Failed to load games. Exiting." << endl;
        return 1;
    }

    buildHashTable(games, gameCount, gameHash);

    // test members
    members[0] = Member("M001", "Alice Tan", "alice@email.com");
    members[1] = Member("M002", "Bob Lee", "bob@email.com");
    members[2] = Member("M003", "Charlie Wong", "charlie@email.com");
    memberCount = 3;

    cout << "\nSystem initialized successfully!" << endl;
    cout << "Test members created: M001, M002, M003" << endl;

    mainMenu();
    return 0;
}
    