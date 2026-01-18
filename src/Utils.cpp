#include "../include/Utils.h"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>

#ifdef _WIN32
    #include <direct.h>  // For _mkdir on Windows
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

using namespace std;

namespace Utils {

    void createDirectoryIfNotExists(const std::string& path) {
#ifdef _WIN32
        int result = _mkdir(path.c_str());
        if (result == 0) {
            cout << "[INFO] Created directory: " << path << "\n";
        }
#else
        int result = mkdir(path.c_str(), 0777);
        if (result == 0) {
            cout << "[INFO] Created directory: " << path << "\n";
        }
#endif
    }

    void clearScreen() {
        cout.flush();  // Flush output buffer first

        #ifdef _WIN32
                system("cls");
        #else
                system("clear");
        #endif

        // Small delay to ensure screen is cleared
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    void pauseScreen() {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    void sleep(int seconds) {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }
    int getIntInput(const string& prompt, int min, int max) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
            } else if (value < min || value > max) {
                cout << "Value must be between " << min << " and " << max << ".\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

    string getStringInput(const string& prompt) {
        string value;
        cout << prompt;
        getline(cin, value);
        return value;
    }

    bool confirmAction(const string& message) {
        cout << message << " (y/n): ";
        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return (choice == 'y' || choice == 'Y');
    }

    void printHeader(const string& title) {
        cout << "\n========================================\n";
        cout << "  " << title << "\n";
        cout << "========================================\n";
    }

    void printSeparator() {
        cout << "----------------------------------------\n";
    }

    void printError(const string& message) {
        cout << "[ERROR] " << message << "\n";
    }

    void printSuccess(const string& message) {
        cout << "[SUCCESS] " << message << "\n";
    }

} // End of namespace Utils