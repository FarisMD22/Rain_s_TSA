#include <iostream>

#include "../include/Patient.h"
#include "../include/QueueADT.h"
#include "../include/LinkedListADT.h"
#include "../include/UserManager.h"
#include "../include/Utils.h"
using namespace std;

// Forward declarations - ADD PARAMETERS HERE
void showLoginMenu(UserManager& userMgr, QueueADT& queue, LinkedListADT& patientList);
void showMainMenu(UserManager& userMgr, QueueADT& queue, LinkedListADT& patientList);
void showAdminMenu(UserManager& userMgr, QueueADT& queue, LinkedListADT& patientList);
void showDoctorMenu(UserManager& userMgr, QueueADT& queue, LinkedListADT& patientList);

// Patient management functions
void addPatientRecord(LinkedListADT& patientList);
void searchPatient(LinkedListADT& patientList);
void deletePatientRecord(LinkedListADT& patientList);
void displayAllPatients(LinkedListADT& patientList);
void sortPatients(LinkedListADT& patientList);

// Queue management functions
void addPatientToQueue(QueueADT& queue);
void processNextPatient(QueueADT& queue);
void viewQueue(QueueADT& queue);

// User management functions
void manageUsers(UserManager& userMgr);

int main() {
    // Initialize system components
    UserManager userMgr;
    QueueADT queue;
    LinkedListADT patientList;

    // Load data from files
    userMgr.loadFromFile("data/users.txt");
    patientList.loadFromFile("data/patients.txt");
    queue.loadFromFile("data/queue_log.txt");

    Utils::printHeader("MEDIFLOW PATIENT MANAGEMENT SYSTEM");
    cout << "Welcome to MediFlow - Your Medical Records Manager\n";

    // Main application loop
    while (true) {
        if (!userMgr.isAuthenticated()) {
            showLoginMenu(userMgr, queue, patientList);  // PASS PARAMETERS
        } else {
            showMainMenu(userMgr, queue, patientList);
        }
    }

    return 0;
}

// UPDATE FUNCTION SIGNATURE - ADD PARAMETERS
void showLoginMenu(UserManager& userMgr, QueueADT& queue, LinkedListADT& patientList) {
    Utils::clearScreen();
    Utils::printHeader("LOGIN");

    cout << "1. Login\n";
    cout << "2. Exit System\n";

    int choice = Utils::getIntInput("Enter choice: ", 1, 2);

    if (choice == 1) {
        string username = Utils::getStringInput("Username: ");
        string password = Utils::getStringInput("Password: ");

        bool success = userMgr.login(username, password);

        if (success) {
            Utils::sleep(3);  // ADD THIS - 1 second delay after successful login
        } else {
            Utils::pauseScreen();  // Pause on failed login
        }
    } else {
        // Save all data before exit
        cout << "\nSaving data...\n";
        userMgr.saveToFile("data/users.txt");
        patientList.saveToFile("data/patients.txt");
        queue.saveToFile("data/queue_log.txt");
        cout << "Exiting MediFlow. Goodbye!\n";
        exit(0);
    }
}void showMainMenu(UserManager& userMgr, QueueADT& queue, LinkedListADT& patientList) {
    // REMOVED: Utils::clearScreen(); - Don't clear here, let each menu function handle it

    if (userMgr.getCurrentRole() == ADMIN) {
        showAdminMenu(userMgr, queue, patientList);
    } else {
        showDoctorMenu(userMgr, queue, patientList);
    }
}

void showAdminMenu(UserManager& userMgr, QueueADT& queue, LinkedListADT& patientList) {
    Utils::clearScreen();  // Clear at the start of menu display
    Utils::printHeader("ADMIN MENU - " + userMgr.getCurrentUsername());

    cout << "=== Patient Records Management ===\n";
    cout << "1.  Add Patient Record\n";
    cout << "2.  Search Patient\n";
    cout << "3.  Delete Patient Record\n";
    cout << "4.  Display All Patients\n";
    cout << "5.  Sort Patient Records\n";
    cout << "\n=== Queue Management ===\n";
    cout << "6.  Add Patient to Queue\n";
    cout << "7.  Process Next Patient\n";
    cout << "8.  View Current Queue\n";
    cout << "\n=== User Management ===\n";
    cout << "9.  Manage Users\n";
    cout << "\n10. Logout\n";

    int choice = Utils::getIntInput("\nEnter choice: ", 1, 10);
    switch (choice) {
        case 1: addPatientRecord(patientList); break;
        case 2: searchPatient(patientList); break;
        case 3: deletePatientRecord(patientList); break;
        case 4: displayAllPatients(patientList); break;
        case 5: sortPatients(patientList); break;
        case 6: addPatientToQueue(queue); break;
        case 7: processNextPatient(queue); break;
        case 8: viewQueue(queue); break;
        case 9: manageUsers(userMgr); break;
        case 10:
            // Save before logout
            patientList.saveToFile("data/patients.txt");
            queue.saveToFile("data/queue_log.txt");
            userMgr.logout();
            break;
    }
}

void showDoctorMenu(UserManager& userMgr, QueueADT& queue, LinkedListADT& patientList) {
    Utils::clearScreen();  // Clear at the start of menu display
    Utils::printHeader("DOCTOR MENU - " + userMgr.getCurrentUsername());

    cout << "1. View Patient Records\n";
    cout << "2. Search Patient\n";
    cout << "3. View Current Queue\n";
    cout << "4. Process Next Patient\n";
    cout << "5. Logout\n";

    int choice = Utils::getIntInput("\nEnter choice: ", 1, 5);

    switch (choice) {
        case 1: displayAllPatients(patientList); break;
        case 2: searchPatient(patientList); break;
        case 3: viewQueue(queue); break;
        case 4: processNextPatient(queue); break;
        case 5:
            // Save before logout
            patientList.saveToFile("data/patients.txt");
            queue.saveToFile("data/queue_log.txt");
            userMgr.logout();
            break;
    }
}

// ============= PATIENT MANAGEMENT FUNCTIONS =============

void addPatientRecord(LinkedListADT& patientList) {
    Utils::printHeader("ADD NEW PATIENT");

    string id = Utils::getStringInput("Patient ID (e.g., P001): ");
    string name = Utils::getStringInput("Patient Name: ");
    int age = Utils::getIntInput("Age: ", 0, 150);
    string condition = Utils::getStringInput("Condition: ");
    int priority = Utils::getIntInput("Priority (1=Critical, 2=Urgent, 3=Normal): ", 1, 3);
    string date = Utils::getStringInput("Admission Date (YYYY-MM-DD): ");

    Patient p(id, name, age, condition, priority, date);
    patientList.insertPatient(p);

    Utils::printSuccess("Patient record added successfully!");
    Utils::pauseScreen();
}

void searchPatient(LinkedListADT& patientList) {
    Utils::printHeader("SEARCH PATIENT");

    cout << "1. Search by ID\n";
    cout << "2. Search by Name\n";
    int choice = Utils::getIntInput("Enter choice: ", 1, 2);

    if (choice == 1) {
        string id = Utils::getStringInput("Enter Patient ID: ");
        Node* result = patientList.searchByID(id);

        if (result) {
            cout << "\n[FOUND]\n";
            cout << "ID: " << result->data.id << "\n";
            cout << "Name: " << result->data.name << "\n";
            cout << "Age: " << result->data.age << "\n";
            cout << "Condition: " << result->data.condition << "\n";
        } else {
            Utils::printError("Patient not found.");
        }
    } else {
        string name = Utils::getStringInput("Enter Patient Name: ");
        Node* result = patientList.searchByName(name);

        if (result) {
            cout << "\n[FOUND]\n";
            cout << "ID: " << result->data.id << "\n";
            cout << "Name: " << result->data.name << "\n";
        } else {
            Utils::printError("Patient not found.");
        }
    }

    Utils::pauseScreen();
}

void deletePatientRecord(LinkedListADT& patientList) {
    Utils::printHeader("DELETE PATIENT RECORD");

    string id = Utils::getStringInput("Enter Patient ID to delete: ");

    if (Utils::confirmAction("Are you sure you want to delete this patient?")) {
        if (patientList.deletePatient(id)) {
            Utils::printSuccess("Patient deleted successfully.");
        } else {
            Utils::printError("Patient not found.");
        }
    }

    Utils::pauseScreen();
}

void displayAllPatients(LinkedListADT& patientList) {
    Utils::printHeader("ALL PATIENT RECORDS");
    patientList.displayAll();
    Utils::pauseScreen();
}

void sortPatients(LinkedListADT& patientList) {
    Utils::printHeader("SORT PATIENTS");

    cout << "1. Sort by Name\n";
    cout << "2. Sort by Priority\n";
    int choice = Utils::getIntInput("Enter choice: ", 1, 2);

    if (choice == 1) {
        patientList.sortByName();
        Utils::printSuccess("Patients sorted by name.");
    } else {
        patientList.sortByPriority();
        Utils::printSuccess("Patients sorted by priority.");
    }

    Utils::pauseScreen();
}

// ============= QUEUE MANAGEMENT FUNCTIONS =============

void addPatientToQueue(QueueADT& queue) {
    Utils::printHeader("ADD PATIENT TO QUEUE");

    string id = Utils::getStringInput("Patient ID: ");
    string name = Utils::getStringInput("Patient Name: ");
    int age = Utils::getIntInput("Age: ", 0, 150);
    string condition = Utils::getStringInput("Condition: ");
    int priority = Utils::getIntInput("Priority (1=Critical, 2=Urgent, 3=Normal): ", 1, 3);
    string date = Utils::getStringInput("Date: ");

    Patient p(id, name, age, condition, priority, date);
    queue.enqueue(p);

    Utils::printSuccess("Patient added to queue!");
    Utils::pauseScreen();
}

void processNextPatient(QueueADT& queue) {
    Utils::printHeader("PROCESS NEXT PATIENT");

    if (queue.isEmpty()) {
        Utils::printError("Queue is empty!");
    } else {
        Patient p = queue.dequeue();
        cout << "Processing patient: " << p.name << " (ID: " << p.id << ")\n";
        Utils::printSuccess("Patient processed and removed from queue.");
    }

    Utils::pauseScreen();
}

void viewQueue(QueueADT& queue) {
    Utils::printHeader("CURRENT QUEUE");
    queue.display();
    Utils::pauseScreen();
}

// ============= USER MANAGEMENT FUNCTIONS =============

void manageUsers(UserManager& userMgr) {
    Utils::printHeader("USER MANAGEMENT");

    cout << "1. View All Users\n";
    cout << "2. Add New User\n";
    cout << "3. Delete User\n";
    cout << "4. Back to Main Menu\n";

    int choice = Utils::getIntInput("Enter choice: ", 1, 4);

    switch (choice) {
        case 1:
            userMgr.displayAllUsers();
            Utils::pauseScreen();
            break;
        case 2: {
            string username = Utils::getStringInput("New Username: ");
            string password = Utils::getStringInput("New Password: ");
            int role = Utils::getIntInput("Role (0=Admin, 1=Doctor): ", 0, 1);
            userMgr.addUser(username, password, static_cast<UserRole>(role));
            Utils::pauseScreen();
            break;
        }
        case 3: {
            string username = Utils::getStringInput("Username to delete: ");
            userMgr.deleteUser(username);
            Utils::pauseScreen();
            break;
        }
        case 4:
            break;
    }
}