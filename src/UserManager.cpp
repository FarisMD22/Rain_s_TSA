#include "../include/UserManager.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>

using namespace std;

UserManager::UserManager() : isLoggedIn(false), userCount(0) {
    initializeDefaultUsers();
}

void UserManager::initializeDefaultUsers() {
    // Manually add default users to array
    // Check duplication in case loadFromFile was called first
    if (userCount == 0) {
        addUser("admin", "admin123", ADMIN);
        addUser("doctor", "doc123", DOCTOR);
    }
}

bool UserManager::login(const string& username, const string& password) {
    // Linear Search for matching username and password
    for (int i = 0; i < userCount; i++) {
        if (users[i].username == username && users[i].password == password) {
            currentUser = users[i];
            isLoggedIn = true;
            Utils::printSuccess("Login successful! Welcome, " + username);
            return true;
        }
    }

    Utils::printError("Invalid username or password.");
    return false;
}

void UserManager::logout() {
    if (isLoggedIn) {
        cout << "Goodbye, " << currentUser.username << "!\n";
        currentUser = User(); // Reset
        isLoggedIn = false;
    }
}

bool UserManager::addUser(const string& username, const string& password, UserRole role) {
    // 1. Check if array is full
    if (userCount >= MAX_USERS) {
        Utils::printError("User limit reached. Cannot add more users.");
        return false;
    }

    // 2. Check for duplicate username
    for (int i = 0; i < userCount; i++) {
        if (users[i].username == username) {
            Utils::printError("Username already exists.");
            return false;
        }
    }

    // 3. Add new user to the next available slot
    users[userCount] = User(username, password, role);
    userCount++;

    Utils::printSuccess("User added successfully.");
    return true;
}

bool UserManager::deleteUser(const string& username) {
    int indexToDelete = -1;

    // 1. Find the user index
    for (int i = 0; i < userCount; i++) {
        if (users[i].username == username) {
            indexToDelete = i;
            break;
        }
    }

    if (indexToDelete == -1) {
        Utils::printError("User not found.");
        return false;
    }

    // 2. Shift all subsequent users one step LEFT to fill the gap
    for (int i = indexToDelete; i < userCount - 1; i++) {
        users[i] = users[i + 1];
    }

    // 3. Decrease count
    userCount--;
    Utils::printSuccess("User deleted successfully.");
    return true;
}

void UserManager::displayAllUsers() const {
    Utils::printHeader("REGISTERED USERS");
    cout << "Username\t\tRole\n";
    Utils::printSeparator();

    for (int i = 0; i < userCount; i++) {
        string roleStr = (users[i].role == ADMIN) ? "Admin" : "Doctor";
        cout << users[i].username << "\t\t\t" << roleStr << "\n";
    }
}

void UserManager::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        Utils::printError("Cannot save users to file.");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        file << users[i].username << ","
             << users[i].password << ","
             << users[i].role << "\n";
    }

    file.close();
}

void UserManager::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        return;
    }

    // Clear current list to avoid duplicates with defaults
    userCount = 0;

    string username, password;
    int roleInt;

    while (file.good()) {
        getline(file, username, ',');
        if (username.empty()) continue; // Skip empty lines

        getline(file, password, ',');
        file >> roleInt;
        file.ignore(); // Skip newline

        // Add directly to array manually to avoid "Already exists" print spam
        if (userCount < MAX_USERS) {
            users[userCount] = User(username, password, static_cast<UserRole>(roleInt));
            userCount++;
        }
    }
    
    file.close();
}