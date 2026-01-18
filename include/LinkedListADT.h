#ifndef LINKEDLISTADT_H
#define LINKEDLISTADT_H

#include "Patient.h"
#include <string>

// Task Owner: Member 2
// Doubly Linked List for permanent patient records
class LinkedListADT {
private:
    Node* head;
    Node* tail;
    int count;

public:
    LinkedListADT();
    ~LinkedListADT();

    // CRUD operations
    void insertPatient(Patient p);              // Add new patient
    bool deletePatient(const std::string& id);  // Remove by ID
    bool updatePatient(const std::string& id, Patient newData);

    // Search & Sort (Task Owner: Member 3)
    Node* searchByID(const std::string& id) const;
    Node* searchByName(const std::string& name) const;
    void sortByName();                          // Sorting algorithm
    void sortByPriority();

    // Display
    void displayAll() const;
    void displayReverse() const;                // Doubly linked feature

    // File I/O
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    // Getters for integration
    Node* getHead() const { return head; }
    int getCount() const { return count; }
};

#endif