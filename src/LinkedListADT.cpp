#include "../include/LinkedListADT.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;

LinkedListADT::LinkedListADT() : head(nullptr), tail(nullptr), count(0) {}

LinkedListADT::~LinkedListADT() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void LinkedListADT::insertPatient(Patient p) {
    Node* newNode = new Node(p);

    if (head == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    count++;
    cout << "Patient " << p.name << " added to records.\n";
}

bool LinkedListADT::deletePatient(const string& id) {
    Node* current = head;

    while (current != nullptr) {
        if (current->data.id == id) {
            // Found the node to delete
            if (current->prev != nullptr) {
                current->prev->next = current->next;
            } else {
                head = current->next; // Deleting head
            }

            if (current->next != nullptr) {
                current->next->prev = current->prev;
            } else {
                tail = current->prev; // Deleting tail
            }

            delete current;
            count--;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool LinkedListADT::updatePatient(const string& id, Patient newData) {
    Node* node = searchByID(id);
    if (node != nullptr) {
        node->data = newData;
        return true;
    }
    return false;
}

Node* LinkedListADT::searchByID(const string& id) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.id == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

Node* LinkedListADT::searchByName(const string& name) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.name == name) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void LinkedListADT::sortByName() {
    if (head == nullptr || head->next == nullptr) {
        cout << "[INFO] List has 0 or 1 patient. No sorting needed.\n";
        return;
    }

    // Bubble Sort by Name (ascending order)
    bool swapped;
    Node* current;
    Node* last = nullptr;

    do {
        swapped = false;
        current = head;

        while (current->next != last) {
            // Compare current name with next name
            if (current->data.name > current->next->data.name) {
                // Swap the patient data
                Patient temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                swapped = true;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);

    cout << "[SUCCESS] Patients sorted by name (A-Z).\n";
}

void LinkedListADT::sortByPriority() {
    if (head == nullptr || head->next == nullptr) {
        cout << "[INFO] List has 0 or 1 patient. No sorting needed.\n";
        return;
    }

    // Bubble Sort by Priority (1=Critical first, 2=Urgent, 3=Normal)
    bool swapped;
    Node* current;
    Node* last = nullptr;

    do {
        swapped = false;
        current = head;

        while (current->next != last) {
            // Lower priority number = higher urgency (1 > 2 > 3)
            if (current->data.priority > current->next->data.priority) {
                // Swap the patient data
                Patient temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                swapped = true;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);

    cout << "[SUCCESS] Patients sorted by priority (Critical -> Urgent -> Normal).\n";
}

void LinkedListADT::displayAll() const {
    if (head == nullptr) {
        cout << "No patient records found.\n";
        return;
    }

    Node* current = head;
    cout << "\n=== PATIENT RECORDS ===\n";
    cout << "ID    | Name                | Age | Priority | Condition\n";
    cout << "------|---------------------|-----|----------|-------------------\n";

    while (current != nullptr) {
        cout << current->data.id << " | "
             << current->data.name << " | "
             << current->data.age << "  | "
             << current->data.priority << "        | "
             << current->data.condition << "\n";
        current = current->next;
    }
    cout << "\nTotal patients: " << count << "\n";
}

void LinkedListADT::displayReverse() const {
    if (tail == nullptr) {
        cout << "No patient records found.\n";
        return;
    }

    Node* current = tail;
    cout << "\n=== PATIENT RECORDS (REVERSE) ===\n";

    while (current != nullptr) {
        cout << current->data.id << " | "
             << current->data.name << " | "
             << current->data.age << "  | "
             << current->data.priority << "\n";
        current = current->prev;
    }
}

void LinkedListADT::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        Utils::printError("Cannot open file for writing: " + filename);
        return;
    }

    Node* current = head;
    while (current != nullptr) {
        file << current->data.id << ","
             << current->data.name << ","
             << current->data.age << ","
             << current->data.condition << ","
             << current->data.priority << ","
             << current->data.admissionDate << "\n";
        current = current->next;
    }

    file.close();
    cout << "[INFO] " << count << " patient records saved to " << filename << "\n";
}

void LinkedListADT::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[INFO] No existing patient records found. Starting fresh.\n";
        return;
    }

    string line;
    int loaded = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string id, name, condition, date;
        int age, priority;

        getline(ss, id, ',');
        getline(ss, name, ',');
        ss >> age;
        ss.ignore(); // Skip comma
        getline(ss, condition, ',');
        ss >> priority;
        ss.ignore(); // Skip comma
        getline(ss, date);

        Patient p(id, name, age, condition, priority, date);
        insertPatient(p);
        loaded++;
    }

    file.close();
    cout << "[INFO] Loaded " << loaded << " patient records from " << filename << "\n";
}