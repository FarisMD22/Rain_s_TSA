#ifndef UTILS_H
#define UTILS_H

#include <string>

// Utility functions for CLI interface
namespace Utils {
    // Screen management
    void clearScreen();
    void pauseScreen();
    void sleep(int seconds);


    // Input validation
    int getIntInput(const std::string& prompt, int min, int max);
    std::string getStringInput(const std::string& prompt);
    bool confirmAction(const std::string& message);

    // Display helpers
    void printHeader(const std::string& title);
    void printSeparator();
    void printError(const std::string& message);
    void printSuccess(const std::string& message);
}

#endif