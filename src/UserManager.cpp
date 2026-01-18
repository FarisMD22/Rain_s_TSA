#include "../include/UserManager.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>

using namespace std;

UserManager::UserManager() : isLoggedIn(false) {
    initializeDefaultUsers();
}

void UserManager::initializeDefaultUsers() {
    // Default accounts (can be overridden by loading from file)
    users["admin"] = User("admin", "admin123", ADMIN);
    users["doctor"] = User("doctor", "doc123", DOCTOR);
}

bool UserManager::login(const string& username, const string& password) {
    auto it = users.find(username);
    
    if (it != users.end() && it->second.password == password) {
        currentUser = it->second;
        isLoggedIn = true;
        Utils::printSuccess("Login successful! Welcome, " + username);
        return true;
    }
    
    Utils::printError("Invalid username or password.");
    return false;
}

void UserManager::logout() {
    if (isLoggedIn) {
        cout << "Goodbye, " << currentUser.username << "!\n";
        currentUser = User();
        isLoggedIn = false;
    }
}

bool UserManager::addUser(const string& username, const string& password, UserRole role) {
    if (users.find(username) != users.end()) {
        Utils::printError("Username already exists.");
        return false;
    }
    
    users[username] = User(username, password, role);
    Utils::printSuccess("User added successfully.");
    return true;
}

bool UserManager::deleteUser(const string& username) {
    if (username == "admin") {
        Utils::printError("Cannot delete default admin account.");
        return false;
    }
    
    auto it = users.find(username);
    if (it != users.end()) {
        users.erase(it);
        Utils::printSuccess("User deleted successfully.");
        return true;
    }
    
    Utils::printError("User not found.");
    return false;
}

void UserManager::displayAllUsers() const {
    Utils::printHeader("REGISTERED USERS");
    cout << "Username\t\tRole\n";
    Utils::printSeparator();
    
    for (const auto& pair : users) {
        string roleStr = (pair.second.role == ADMIN) ? "Admin" : "Doctor";
        cout << pair.second.username << "\t\t\t" << roleStr << "\n";
    }
}

void UserManager::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        Utils::printError("Cannot save users to file.");
        return;
    }
    
    for (const auto& pair : users) {
        file << pair.second.username << ","
             << pair.second.password << ","
             << pair.second.role << "\n";
    }
    
    file.close();
}

void UserManager::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        return; // File doesn't exist yet, use defaults
    }
    
    users.clear();
    string username, password;
    int role;
    
    while (file.good()) {
        getline(file, username, ',');
        getline(file, password, ',');
        file >> role;
        file.ignore();
        
        if (!username.empty()) {
            users[username] = User(username, password, static_cast<UserRole>(role));
        }
    }
    
    file.close();
}