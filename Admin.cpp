#include "Admin.h"
#include <iostream>
#include <limits>
#include <cctype>   // tolower
using namespace std;

static void clearCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ===================== HASH REBUILD (after remove shift) =====================
static void rebuildGameHash(HashTable& gameHash, Game games[], int gameCount) {
    gameHash.clear();
    for (int i = 0; i < gameCount; i++) {
        gameHash.insert(games[i].getGameID(), i);
    }
}

// ===================== STRING HELPERS (case-insensitive compare) =====================
static string toLowerCopy(const string& s) {
    string out = s;
    for (char& c : out) c = (char)tolower((unsigned char)c);
    return out;
}

// ===================== MERGE SORT: BORROW COUNT =====================
static void mergeByBorrowCount(Game arr[], int left, int mid, int right, bool descending) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Game* L = new Game[n1];
    Game* R = new Game[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        int a = L[i].getBorrowCount();
        int b = R[j].getBorrowCount();

        bool takeLeft;
        if (descending) takeLeft = (a >= b);
        else           takeLeft = (a <= b);

        if (takeLeft) arr[k++] = L[i++];
        else          arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

static void mergeSortByBorrowCount(Game arr[], int left, int right, bool descending) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortByBorrowCount(arr, left, mid, descending);
    mergeSortByBorrowCount(arr, mid + 1, right, descending);
    mergeByBorrowCount(arr, left, mid, right, descending);
}

// ===================== MERGE SORT: TITLE =====================
static void mergeByTitle(Game arr[], int left, int mid, int right, bool ascending) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Game* L = new Game[n1];
    Game* R = new Game[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        string a = toLowerCopy(L[i].getTitle());
        string b = toLowerCopy(R[j].getTitle());

        bool takeLeft;
        if (ascending) takeLeft = (a <= b);
        else           takeLeft = (a >= b);

        if (takeLeft) arr[k++] = L[i++];
        else          arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

static void mergeSortByTitle(Game arr[], int left, int right, bool ascending) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortByTitle(arr, left, mid, ascending);
    mergeSortByTitle(arr, mid + 1, right, ascending);
    mergeByTitle(arr, left, mid, right, ascending);
}

// ===================== DISPLAY LIST HELPER =====================
static void printGameList(Game arr[], int count) {
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

// ===================== ADMIN: ADD GAME =====================
void adminAddGame(Game games[], int& gameCount, int maxGames, HashTable& gameHash) {
    cout << "\n=== Admin: Add New Board Game ===\n";

    if (gameCount >= maxGames) {
        cout << "❌ Cannot add. Game storage is full.\n";
        return;
    }

    string id, title;
    int minP, maxP, minT, maxT, year;

    cout << "Game ID: ";
    getline(cin, id);
    if (id.empty()) { cout << "❌ Game ID cannot be empty.\n"; return; }

    if (gameHash.search(id) != -1) {
        cout << "❌ Game ID already exists.\n";
        return;
    }

    cout << "Title: ";
    getline(cin, title);
    if (title.empty()) { cout << "❌ Title cannot be empty.\n"; return; }

    cout << "Min Players: ";
    if (!(cin >> minP)) { clearCin(); cout << "❌ Invalid input.\n"; return; }

    cout << "Max Players: ";
    if (!(cin >> maxP)) { clearCin(); cout << "❌ Invalid input.\n"; return; }

    cout << "Min Playtime (mins): ";
    if (!(cin >> minT)) { clearCin(); cout << "❌ Invalid input.\n"; return; }

    cout << "Max Playtime (mins): ";
    if (!(cin >> maxT)) { clearCin(); cout << "❌ Invalid input.\n"; return; }

    cout << "Year Published: ";
    if (!(cin >> year)) { clearCin(); cout << "❌ Invalid input.\n"; return; }

    clearCin();

    if (minP <= 0 || maxP <= 0 || minP > maxP) { cout << "❌ Invalid player range.\n"; return; }
    if (minT <= 0 || maxT <= 0 || minT > maxT) { cout << "❌ Invalid playtime range.\n"; return; }
    if (year <= 0) { cout << "❌ Invalid year.\n"; return; }

    games[gameCount] = Game(id, title, minP, maxP, minT, maxT, year);
    gameHash.insert(id, gameCount);
    gameCount++;

    cout << "✅ Game added.\n";
}

// ===================== ADMIN: REMOVE GAME =====================
void adminRemoveGame(Game games[], int& gameCount, HashTable& gameHash) {
    cout << "\n=== Admin: Remove Board Game ===\n";

    string id;
    cout << "Game ID to remove: ";
    getline(cin, id);
    if (id.empty()) { cout << "❌ Game ID cannot be empty.\n"; return; }

    int idx = gameHash.search(id);
    if (idx == -1) { cout << "❌ Game not found.\n"; return; }

    if (games[idx].getStatus() == "Borrowed") {
        cout << "❌ Cannot remove. Game is borrowed by: " << games[idx].getBorrowedBy() << "\n";
        return;
    }

    for (int i = idx; i < gameCount - 1; i++) {
        games[i] = games[i + 1];
    }
    gameCount--;

    rebuildGameHash(gameHash, games, gameCount);
    cout << "✅ Game removed.\n";
}

// ===================== ADMIN: ADD MEMBER =====================
void adminAddMember(Member members[], int& memberCount, int maxMembers) {
    cout << "\n=== Admin: Add New Member ===\n";

    if (memberCount >= maxMembers) {
        cout << "❌ Cannot add. Member storage is full.\n";
        return;
    }

    string id, name, email;

    cout << "Member ID: ";
    getline(cin, id);
    if (id.empty()) { cout << "❌ Member ID cannot be empty.\n"; return; }

    for (int i = 0; i < memberCount; i++) {
        if (members[i].getMemberID() == id) {
            cout << "❌ Member ID already exists.\n";
            return;
        }
    }

    cout << "Name: ";
    getline(cin, name);
    if (name.empty()) { cout << "❌ Name cannot be empty.\n"; return; }

    cout << "Email: ";
    getline(cin, email);
    if (email.empty()) { cout << "❌ Email cannot be empty.\n"; return; }

    members[memberCount] = Member(id, name, email);
    memberCount++;

    cout << "✅ Member added.\n";
}

// ===================== ADMIN: SUMMARY (no top 5 here anymore) =====================
void adminDisplaySummary(Game games[], int gameCount) {
    cout << "\n=== Admin: Summary of Games Borrowed/Returned ===\n";

    int borrowedNow = 0;
    int availableNow = 0;
    int totalBorrowEvents = 0;

    for (int i = 0; i < gameCount; i++) {
        if (games[i].getStatus() == "Borrowed") borrowedNow++;
        else availableNow++;

        totalBorrowEvents += games[i].getBorrowCount();
    }

    cout << "Total Games: " << gameCount << "\n";
    cout << "Borrowed (Currently): " << borrowedNow << "\n";
    cout << "Returned/Available (Currently): " << availableNow << "\n";
    cout << "Total Times Borrowed (All Games): " << totalBorrowEvents << "\n";

    if (borrowedNow > 0) {
        cout << "\n--- Currently Borrowed Games ---\n";
        for (int i = 0; i < gameCount; i++) {
            if (games[i].getStatus() == "Borrowed") {
                cout << games[i].getGameID()
                    << " | " << games[i].getTitle()
                    << " | Borrowed By: " << games[i].getBorrowedBy()
                    << "\n";
            }
        }
    }

    cout << "============================================\n";
}

// ===================== ✅ NEW: DISPLAY ALL SORTED (submenu) =====================
void adminDisplayAllGamesSorted(Game games[], int gameCount) {
    if (gameCount <= 0) {
        cout << "No games to display.\n";
        return;
    }

    int choice;
    do {
        cout << "\n======================================\n";
        cout << "   DISPLAY ALL GAMES (SORT OPTIONS)\n";
        cout << "======================================\n";
        cout << "1) Title A -> Z\n";
        cout << "2) Title Z -> A\n";
        cout << "3) BorrowCount High -> Low\n";
        cout << "4) BorrowCount Low -> High\n";
        cout << "0) Back\n";
        cout << "Choice: ";

        if (!(cin >> choice)) {
            clearCin();
            cout << "❌ Invalid input.\n";
            continue;
        }
        clearCin();

        if (choice == 0) break;

        Game* temp = new Game[gameCount];
        for (int i = 0; i < gameCount; i++) temp[i] = games[i];

        switch (choice) {
        case 1:
            mergeSortByTitle(temp, 0, gameCount - 1, true);
            cout << "\n=== ALL GAMES SORTED BY TITLE (A -> Z) ===\n";
            printGameList(temp, gameCount);
            break;
        case 2:
            mergeSortByTitle(temp, 0, gameCount - 1, false);
            cout << "\n=== ALL GAMES SORTED BY TITLE (Z -> A) ===\n";
            printGameList(temp, gameCount);
            break;
        case 3:
            mergeSortByBorrowCount(temp, 0, gameCount - 1, true);
            cout << "\n=== ALL GAMES SORTED BY BORROWCOUNT (HIGH -> LOW) ===\n";
            printGameList(temp, gameCount);
            break;
        case 4:
            mergeSortByBorrowCount(temp, 0, gameCount - 1, false);
            cout << "\n=== ALL GAMES SORTED BY BORROWCOUNT (LOW -> HIGH) ===\n";
            printGameList(temp, gameCount);
            break;
        default:
            cout << "❌ Invalid option.\n";
            break;
        }

        delete[] temp;

    } while (choice != 0);
}
