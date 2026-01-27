#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    void clearScreen();
    void pauseScreen();
    void sleep(int seconds);

    int getIntInput(const std::string& prompt, int min, int max);
    std::string getStringInput(const std::string& prompt);
    bool confirmAction(const std::string& message);

    void printHeader(const std::string& title);
    void printSeparator();
    void printError(const std::string& message);
    void printSuccess(const std::string& message);

    void createDirectoryIfNotExists(const std::string& path);
}

#endif