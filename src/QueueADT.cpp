#include "../include/QueueADT.h"
#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

QueueADT::QueueADT() : front(nullptr), rear(nullptr), size(0) {}

QueueADT::~QueueADT() {
    while (!isEmpty()) {
        dequeue();
    }
}

void QueueADT::enqueue(Patient p) {
    Node* newNode = new Node(p);

    // Empty queue - insert directly
    if (isEmpty()) {
        front = rear = newNode;
        size++;
        cout << "Patient " << p.name << " added to queue (Priority: " << p.priority << ")\n";
        return;
    }

    // Priority-based insertion (1=Critical, 2=Urgent, 3=Normal)
    // Lower number = higher priority (insert earlier in queue)

    // Case 1: New patient has highest priority (goes to front)
    if (p.priority < front->data.priority) {
        newNode->next = front;
        front = newNode;
        size++;
        cout << "Patient " << p.name << " added to FRONT of queue (Priority: " << p.priority << ")\n";
        return;
    }

    // Case 2: Find correct position based on priority
    Node* current = front;
    while (current->next != nullptr && current->next->data.priority <= p.priority) {
        current = current->next;
    }

    // Insert after current
    newNode->next = current->next;
    current->next = newNode;

    // Update rear if inserted at end
    if (newNode->next == nullptr) {
        rear = newNode;
    }

    size++;
    cout << "Patient " << p.name << " added to queue (Priority: " << p.priority << ")\n";
}

Patient QueueADT::dequeue() {
    if (isEmpty()) {
        Utils::printError("Queue is empty!");
        return Patient(); // Return empty patient
    }

    Node* temp = front;
    Patient data = temp->data;
    front = front->next;

    if (front == nullptr) {
        rear = nullptr;
    }

    delete temp;
    size--;
    return data;
}

Patient QueueADT::peek() const {
    if (isEmpty()) {
        Utils::printError("Queue is empty!");
        return Patient();
    }
    return front->data;
}

bool QueueADT::isEmpty() const {
    return front == nullptr;
}

int QueueADT::getSize() const {
    return size;
}

void QueueADT::display() const {
    if (isEmpty()) {
        cout << "Queue is empty.\n";
        return;
    }

    Node* current = front;
    int position = 1;

    cout << "\nPosition | ID    | Name                | Priority | Condition\n";
    cout << "---------|-------|---------------------|----------|-------------------\n";

    while (current != nullptr) {
        cout << position << "        | "
             << current->data.id << " | "
             << current->data.name << " | "
             << current->data.priority << "        | "
             << current->data.condition << "\n";
        current = current->next;
        position++;
    }
}

void QueueADT::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        Utils::printError("Cannot open file for writing: " + filename);
        return;
    }

    Node* current = front;
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
    cout << "[INFO] Queue saved to " << filename << "\n";
}

void QueueADT::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[INFO] No existing queue data found. Starting fresh.\n";
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
        ss.ignore();
        getline(ss, condition, ',');
        ss >> priority;
        ss.ignore();
        getline(ss, date);

        Patient p(id, name, age, condition, priority, date);
        enqueue(p);
        loaded++;
    }

    file.close();
    cout << "[INFO] Loaded " << loaded << " patients into queue from " << filename << "\n";
}