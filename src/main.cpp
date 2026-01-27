#include <iostream>
#include <unistd.h>  // For getcwd() on macOS/Linux
#include "../include/Patient.h"
#include "../include/QueueADT.h"
#include "../include/LinkedListADT.h"
#include "../include/UserManager.h"
#include "../include/Utils.h"
#include <iomanip>

using namespace std;

// Helper function to get correct data path
string getDataPath(const string& filename) {
    return "data/" + filename;
}
#ifdef __APPLE__
#include <unistd.h>
#endif

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
void editPatientRecord(LinkedListADT& patientList);

// Queue management functions
// void addPatientToQueue(QueueADT& queue);
void addPatientToQueue(QueueADT& queue, LinkedListADT& patientList);
void processNextPatient(QueueADT& queue);
void viewQueue(QueueADT& queue);

// User management functions
void manageUsers(UserManager& userMgr);

int main() {

    // Create data directory
    Utils::createDirectoryIfNotExists("data");

    // Initialize system components
    UserManager userMgr;
    QueueADT queue;
    LinkedListADT patientList;;

    // Load data from files
    userMgr.loadFromFile("users.txt");
    patientList.loadFromFile("patients.txt");
    queue.loadFromFile("queue_log.txt");

    Utils::printHeader("INTELLIGENT CLINIC QUEUE MANAGEMENT SYSTEM");
    cout << "Welcome to the Intelligent Clinic Queue Management System\n";

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
            Utils::sleep(3);  //1 second delay after successful login
        } else {
            Utils::pauseScreen();  //Pause on failed login
        }
    } else {
        // Save all data before exit
        cout << "\nSaving data...\n";
        userMgr.saveToFile("users.txt");
        patientList.saveToFile("patients.txt");
        queue.saveToFile("queue_log.txt");
        cout << "Exiting Intelligent Clinic Queue Management System. Goodbye!\n";
        exit(0);
    }
}void showMainMenu(UserManager& userMgr, QueueADT& queue, LinkedListADT& patientList) {

    if (userMgr.getCurrentRole() == ADMIN) {
        showAdminMenu(userMgr, queue, patientList);
    } else {
        showDoctorMenu(userMgr, queue, patientList);
    }
}
void showAdminMenu(UserManager& userMgr, QueueADT& queue, LinkedListADT& patientList) {
    Utils::clearScreen();
    Utils::printHeader("ADMIN MENU - " + userMgr.getCurrentUsername());

    cout << "=== Patient Records Management ===\n";
    cout << "1.  Add Patient Record\n";
    cout << "2.  Search Patient\n";
    cout << "3.  Delete Patient Record\n";
    cout << "4.  Display All Patients\n";
    cout << "5.  Sort Patient Records\n";
    cout << "6.  Edit Patient Record\n";

    cout << "\n=== Queue Management ===\n";
    cout << "7.  Add Patient to Queue\n";
    cout << "8.  Process Next Patient\n";
    cout << "9.  View Current Queue\n";

    cout << "\n=== User Management ===\n";
    cout << "10. Manage Users\n";
    cout << "\n11. Logout\n";


    int choice = Utils::getIntInput("\nEnter choice: ", 1, 11);

    switch (choice) {
        case 1: addPatientRecord(patientList); break;
        case 2: searchPatient(patientList); break;
        case 3: deletePatientRecord(patientList); break;
        case 4: displayAllPatients(patientList); break;
        case 5: sortPatients(patientList); break;
        case 6: editPatientRecord(patientList);break;
        case 7: addPatientToQueue(queue, patientList); break;
        case 8: processNextPatient(queue); break;
        case 9: viewQueue(queue); break;
        case 10: manageUsers(userMgr); break;
        case 11:
            patientList.saveToFile("patients.txt");
            queue.saveToFile("queue_log.txt");
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
            patientList.saveToFile("patients.txt");
            queue.saveToFile("queue_log.txt");
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

    Node* result = nullptr;

    if (choice == 1) {
        string id = Utils::getStringInput("Enter Patient ID: ");
        result = patientList.searchByID(id);
    } else {
        string name = Utils::getStringInput("Enter Patient Name: ");
        result = patientList.searchByName(name);
    }

    if (result) {
        Utils::printSuccess("Patient Found!");

        // Table Header
        cout << "\n" << string(110, '=') << "\n";
        cout << left
             << setw(10) << "ID" << " | "
             << setw(25) << "Name" << " | "
             << setw(5) << "Age" << " | "
             << setw(10) << "Priority" << " | "
             << setw(15) << "Date" << " | "
             << "Condition\n";
        cout << string(110, '-') << "\n";

        // Patient Data Row
        cout << left
             << setw(10) << result->data.id << " | "
             << setw(25) << result->data.name << " | "
             << setw(5) << result->data.age << " | "
             << setw(10) << result->data.priority << " | "
             << setw(15) << result->data.admissionDate << " | "
             << result->data.condition << "\n";

        cout << string(110, '-') << "\n";

    } else {
        Utils::printError("Patient not found.");
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

void addPatientToQueue(QueueADT& queue, LinkedListADT& patientList) {
    Utils::printHeader("ADD PATIENT TO QUEUE");

    string id = Utils::getStringInput("Enter Patient ID: ");

    // Step 1: Search for the ID in the permanent records
    Node* existingRecord = patientList.searchByID(id);

    if (existingRecord != nullptr) {
        // === CASE 1: PATIENT FOUND (AUTO-FILL) ===
        // Create a copy of the patient data
        Patient p = existingRecord->data;

        Utils::printSuccess("Record Found: " + p.name + " (Age: " + to_string(p.age) + ")");
        cout << "----------------------------------------\n";

        // We ask for the NEW Condition/Priority because the reason for visiting today changes!
        string newCondition = Utils::getStringInput("Reason for visit (Condition): ");
        p.condition = newCondition;

        int newPriority = Utils::getIntInput("Triage Priority (1=Critical, 2=Urgent, 3=Normal): ", 1, 3);
        p.priority = newPriority;

        // Optionally update the date to today
        p.admissionDate = Utils::getStringInput("Visit Date (YYYY-MM-DD): ");

        // Add to queue
        queue.enqueue(p);
        Utils::printSuccess("Existing patient " + p.name + " added to queue successfully!");

    } else {
        // === CASE 2: PATIENT NOT FOUND (MANUAL ENTRY) ===
        Utils::printError("ID not found in permanent records.");

        if (Utils::confirmAction("Register as new walk-in patient?")) {
            string name = Utils::getStringInput("Patient Name: ");
            int age = Utils::getIntInput("Age: ", 0, 150);
            string condition = Utils::getStringInput("Condition: ");
            int priority = Utils::getIntInput("Priority (1=Critical, 2=Urgent, 3=Normal): ", 1, 3);
            string date = Utils::getStringInput("Date: ");

            // Create patient and add to queue ONLY (not saving to permanent list unless you want to)
            Patient p(id, name, age, condition, priority, date);
            queue.enqueue(p);

            Utils::printSuccess("Walk-in patient added to queue!");
            cout << "(Note: Use 'Add Patient Record' in the menu if you want to save them permanently)\n";
        }
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
void processNextPatient(QueueADT& queue) {
    Utils::printHeader("PROCESS NEXT PATIENT");

    if (queue.isEmpty()) {
        Utils::printError("The queue is empty. No patients to serve.");
    } else {
        // Dequeue removes the first person (highest priority)
        Patient p = queue.dequeue();

        cout << "Now Calling: \n";
        cout << "--------------------------------\n";
        cout << "Name:     " << p.name << "\n";
        cout << "ID:       " << p.id << "\n";
        cout << "Priority: " << p.priority << "\n";
        cout << "Condition: " << p.condition << "\n";
        cout << "--------------------------------\n";

        Utils::printSuccess("Patient processed successfully.");
    }

    Utils::pauseScreen();
}
void editPatientRecord(LinkedListADT& patientList) {
    Utils::printHeader("EDIT PATIENT RECORD");

    string id = Utils::getStringInput("Enter Patient ID to edit: ");

    // 1. Search for the patient first
    Node* node = patientList.searchByID(id);

    if (node == nullptr) {
        Utils::printError("Patient not found.");
        Utils::pauseScreen();
        return;
    }

    // 2. Create a copy of the current data so we don't lose the Name/Age
    Patient currentData = node->data;

    cout << "Editing Patient: " << currentData.name << "\n";
    cout << "1. Update Condition\n";
    cout << "2. Update Priority\n";
    cout << "3. Cancel\n";

    int choice = Utils::getIntInput("Enter choice: ", 1, 3);

    if (choice == 3) return;

    if (choice == 1) {
        string newCond = Utils::getStringInput("Enter new Condition: ");
        currentData.condition = newCond; // Only change condition
    } else if (choice == 2) {
        int newPrio = Utils::getIntInput("Enter new Priority (1-3): ", 1, 3);
        currentData.priority = newPrio; // Only change priority
    }

    // 3. Save the updated object back to the list
    patientList.updatePatient(id, currentData);

    Utils::printSuccess("Patient record updated successfully!");
    Utils::pauseScreen();
}