#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>


// User roles
enum UserRole {
    ADMIN,
    DOCTOR,
    NONE
};

// User structure
struct User {
    std::string username;
    std::string password;
    UserRole role;

    User() : role(NONE) {}
    User(std::string u, std::string p, UserRole r)
        : username(u), password(p), role(r) {}
};

// Constant for maximum users
const int MAX_USERS = 50;

class UserManager {
private:

    User users[MAX_USERS];
    int userCount;

    User currentUser;
    bool isLoggedIn;

    void initializeDefaultUsers();

public:
    UserManager();

    // Authentication
    bool login(const std::string& username, const std::string& password);
    void logout();

    // User management (Admin only)
    bool addUser(const std::string& username, const std::string& password, UserRole role);
    bool deleteUser(const std::string& username);
    void displayAllUsers() const;

    // Access control
    bool isAuthenticated() const { return isLoggedIn; }
    UserRole getCurrentRole() const { return currentUser.role; }
    std::string getCurrentUsername() const { return currentUser.username; }

    // File operations
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

#endif