#ifndef QUEUEADT_H
#define QUEUEADT_H

#include "Patient.h"

// Task Owner: Member 1
// Priority Queue for patient waiting list
class QueueADT {
private:
    Node* front;
    Node* rear;
    int size;

public:
    QueueADT();
    ~QueueADT();

    // Core operations
    void enqueue(Patient p);           // Add patient to queue
    Patient dequeue();                  // Remove front patient
    Patient peek() const;               // View front patient without removing

    // Utility
    bool isEmpty() const;
    int getSize() const;
    void display() const;               // Show all patients in queue

    // File operations
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

#endif