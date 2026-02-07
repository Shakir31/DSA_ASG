#pragma once
#include "Game.h"
#include "Member.h"
#include "HashTable.h"

// Admin functions
void adminAddGame(Game games[], int& gameCount, int maxGames, HashTable& gameHash);
void adminRemoveGame(Game games[], int& gameCount, HashTable& gameHash);
void adminAddMember(Member members[], int& memberCount, int maxMembers);

// Existing summary (keep if you want)
void adminDisplaySummary(Game games[], int gameCount);

// ✅ NEW: display ALL games sorted by different ways
void adminDisplayAllGamesSorted(Game games[], int gameCount);
