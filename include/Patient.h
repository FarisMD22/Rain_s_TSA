#ifndef PATIENT_H
#define PATIENT_H

#include <string>

// Patient structure - represents a single patient record
struct Patient {
    std::string id;           // Unique identifier (e.g., "P001")
    std::string name;         // Full name
    int age;                  // Age in years
    std::string condition;    // Medical condition/reason for visit
    int priority;             // 1=Critical, 2=Urgent, 3=Normal
    std::string admissionDate; // Format: "YYYY-MM-DD"

    // Default constructor
    Patient() : age(0), priority(3) {}

    // Parameterized constructor
    Patient(std::string id, std::string name, int age,
            std::string condition, int priority, std::string date)
        : id(id), name(name), age(age), condition(condition),
          priority(priority), admissionDate(date) {}
};

// Node structure for Doubly Linked List
struct Node {
    Patient data;      // The patient record
    Node* next;        // Pointer to next node
    Node* prev;        // Pointer to previous node (doubly linked)

    // Constructor
    Node(Patient p) : data(p), next(nullptr), prev(nullptr) {}
};

#endif