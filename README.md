# 🏥 MediFlow Patient Management System

A comprehensive hospital patient management system built with C++ featuring advanced data structures, algorithms, and role-based access control for efficient patient care coordination.

---

## 📋 Project Information

| Detail | Information |
|--------|-------------|
| **Course** | TDS6213 Data Structures and Algorithms |
| **Institution** | MMU (Multimedia University) |
| **Academic Year** | 2024/2025 |
| **Project Weight** | 40% of final grade |
| **Submission Deadline** | January 26, 2025 (before 5pm) |
| **Presentation** | Week 13 (10 minutes per group) |
| **Repository** | [github.com/FarisMD22/Rain_s_TSA](https://github.com/FarisMD22/Rain_s_TSA) |

---

## 🎯 System Overview

MediFlow is a command-line patient management system designed for hospitals to efficiently manage patient records, prioritize treatment queues, and maintain data persistence. The system implements core data structures and algorithms taught in TDS6213 while providing practical healthcare management functionality.

### Key Capabilities
- **Patient Record Management** - CRUD operations using doubly linked list
- **Priority-Based Queue System** - Auto-sorts patients by medical urgency
- **Intelligent Sorting** - Multiple sorting algorithms for data organization
- **Fast Search** - Linear search with O(n) complexity
- **Data Persistence** - File I/O for long-term storage
- **Role-Based Access Control** - Admin and Doctor privileges
- **User Management** - Dynamic user account creation/deletion

---

## 🏗️ Architecture Design

### Three-Layer Pattern

The system follows a **clean separation of concerns** to maximize code maintainability and modularity:

```
┌─────────────────────────────────────┐
│   Presentation Layer (UI)           │
│   - Menu system                     │
│   - Input validation                │
│   - Display formatting              │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│   Service Layer (Business Logic)    │
│   - Sorting algorithms              │
│   - Search algorithms               │
│   - Authentication                  │
│   - File I/O operations             │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│   Data Layer (ADTs)                 │
│   - LinkedListADT                   │
│   - QueueADT                        │
│   - Patient structure               │
│   - Node structure                  │
└─────────────────────────────────────┘
```

**Benefits:**
- ✅ Clear task distribution among team members
- ✅ Independent testing of each layer
- ✅ Easy to modify without breaking other components
- ✅ Professional software engineering practice

---

## 📊 Data Structures Implementation

### 1. Doubly Linked List (Patient Records)

**Purpose:** Permanent storage of all patient records with bidirectional traversal capability.

**Structure:**
```cpp
struct Node {
    Patient data;      // Patient information
    Node* next;        // Forward pointer
    Node* prev;        // Backward pointer (doubly linked feature)
};
```

**Operations:**
| Operation | Time Complexity | Description |
|-----------|----------------|-------------|
| `insertPatient()` | O(1) | Insert at tail |
| `deletePatient()` | O(n) | Search + delete |
| `searchByID()` | O(n) | Linear search |
| `searchByName()` | O(n) | Linear search |
| `sortByName()` | O(n²) | Bubble sort |
| `sortByPriority()` | O(n²) | Bubble sort |
| `displayAll()` | O(n) | Forward traversal |
| `displayReverse()` | O(n) | Backward traversal |

**Justification:**
- **Why Doubly Linked List?**
  - Efficient insertion/deletion at any position
  - Bidirectional traversal for flexible display
  - Dynamic memory allocation (no fixed size limit)
  - O(1) insertion when position is known
  
- **Trade-offs:**
  - Extra memory for `prev` pointer
  - Slightly more complex pointer management
  - Worth it for bidirectional traversal capability

**Implementation Highlights:**
```cpp
void insertPatient(Patient p) {
    Node* newNode = new Node(p);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    count++;
}
```

---

### 2. Priority Queue (Waiting List)

**Purpose:** Manage patient waiting list with automatic priority-based ordering (Critical > Urgent > Normal).

**Structure:**
```cpp
class QueueADT {
private:
    Node* front;    // Highest priority patient
    Node* rear;     // Lowest priority patient
    int size;
};
```

**Priority Levels:**
| Priority | Value | Description | Example |
|----------|-------|-------------|---------|
| Critical | 1 | Life-threatening | Heart attack, stroke |
| Urgent | 2 | Serious but stable | Broken bones, severe pain |
| Normal | 3 | Non-emergency | Checkups, minor ailments |

**Operations:**
| Operation | Time Complexity | Description |
|-----------|----------------|-------------|
| `enqueue()` | O(n) | Insert based on priority |
| `dequeue()` | O(1) | Remove front (highest priority) |
| `peek()` | O(1) | View front without removing |
| `isEmpty()` | O(1) | Check if queue empty |
| `display()` | O(n) | Show all in queue order |

**Justification:**
- **Why Priority Queue?**
  - Medical triage system requires priority-based treatment
  - Critical patients automatically move to front
  - Fair treatment order within same priority level (FIFO)
  - Real-world hospital workflow simulation

- **Implementation Choice:**
  - Used linked list over array for dynamic sizing
  - Priority-based insertion ensures always sorted
  - No need for separate sorting operation

**Implementation Highlights:**
```cpp
void enqueue(Patient p) {
    Node* newNode = new Node(p);
    
    // Empty queue - insert directly
    if (isEmpty()) {
        front = rear = newNode;
        return;
    }
    
    // Insert at front if highest priority
    if (p.priority < front->data.priority) {
        newNode->next = front;
        front = newNode;
        return;
    }
    
    // Find correct position based on priority
    Node* current = front;
    while (current->next != nullptr && 
           current->next->data.priority <= p.priority) {
        current = current->next;
    }
    
    // Insert after current
    newNode->next = current->next;
    current->next = newNode;
    
    if (newNode->next == nullptr) {
        rear = newNode;
    }
}
```

---

## 🔍 Algorithms Implementation

### 1. Bubble Sort Algorithm

**Purpose:** Sort patient records by name (alphabetical) or priority (numerical).

**Algorithm Choice Justification:**
- **Why Bubble Sort?**
  - Simple to implement and understand
  - Stable sorting (maintains relative order of equal elements)
  - In-place sorting (no extra memory needed)
  - Efficient for small-to-medium datasets (suitable for hospital records)
  - O(n) best case when data is already sorted

**Time Complexity:**
- **Best Case:** O(n) - Already sorted
- **Average Case:** O(n²)
- **Worst Case:** O(n²) - Reverse sorted
- **Space Complexity:** O(1) - In-place sorting

**Implementation (Sort by Name):**
```cpp
void sortByName() {
    if (head == nullptr || head->next == nullptr) return;
    
    bool swapped;
    Node* current;
    Node* last = nullptr;
    
    do {
        swapped = false;
        current = head;
        
        while (current->next != last) {
            // Compare current name with next name
            if (current->data.name > current->next->data.name) {
                // Swap patient data
                Patient temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                swapped = true;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);
}
```

**Alternative Considered:**
- **Merge Sort** - O(n log n) but more complex, overkill for small datasets
- **Quick Sort** - O(n log n) average but unstable, not in-place
- **Selection Sort** - O(n²) but worse performance than bubble sort

---

### 2. Linear Search Algorithm

**Purpose:** Find patient by ID or name in the linked list.

**Algorithm Choice Justification:**
- **Why Linear Search?**
  - Linked list doesn't support random access (can't use binary search)
  - O(n) complexity is acceptable for hospital-sized datasets
  - Simple implementation with clear logic
  - Works on unsorted data

**Time Complexity:**
- **Best Case:** O(1) - Found at head
- **Average Case:** O(n/2) = O(n)
- **Worst Case:** O(n) - Not found or at tail
- **Space Complexity:** O(1)

**Implementation (Search by ID):**
```cpp
Node* searchByID(const string& id) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.id == id) {
            return current;  // Found
        }
        current = current->next;
    }
    return nullptr;  // Not found
}
```

---

## 💾 File I/O System

### Data Persistence Strategy

**File Format:** CSV (Comma-Separated Values)

**Why CSV?**
- Human-readable for debugging
- Easy to parse in C++
- Can be opened in Excel for inspection
- Lightweight and efficient
- Standard format for data exchange

### File Structure

**1. patients.txt**
```
P001,Alice Johnson,45,Heart Disease,1,2025-01-15
P002,Bob Smith,30,Flu,3,2025-01-16
P003,Carol White,55,Stroke,1,2025-01-17
```

**Format:** `ID,Name,Age,Condition,Priority,AdmissionDate`

**2. queue_log.txt**
```
P005,Emma Green,35,Checkup,3,2025-01-18
P006,Frank Red,60,Chest Pain,1,2025-01-18
```

**Format:** Same as patients.txt (snapshot of current queue)

**3. users.txt**
```
admin,admin123,0
doctor,doc123,1
faris,faros,1
```

**Format:** `Username,Password,Role` (0=Admin, 1=Doctor)

### Save/Load Operations

**Save Implementation:**
```cpp
void saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        Utils::printError("Cannot open file for writing");
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
}
```

**Load Implementation:**
```cpp
void loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[INFO] No existing records found. Starting fresh.\n";
        return;
    }
    
    string line;
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
        insertPatient(p);
    }
    
    file.close();
}
```

**Auto-Save Triggers:**
- User logout
- System exit
- After critical operations (optional enhancement)

---

## 🔐 Authentication System

### Role-Based Access Control (RBAC)

**Purpose:** Restrict functionality based on user role for security and workflow management.

**Roles:**
| Role | Code | Capabilities |
|------|------|--------------|
| Administrator | 0 | Full system access, user management |
| Doctor | 1 | View/search patients, manage queue |

### Authentication Flow

```
┌─────────────┐
│ Login Screen│
└──────┬──────┘
       │
       ▼
┌─────────────────┐
│ Credential Check│
└────┬────────┬───┘
     │        │
  Valid    Invalid
     │        │
     ▼        ▼
┌─────────┐ ┌──────┐
│Load Role│ │Retry │
└────┬────┘ └──────┘
     │
     ▼
┌──────────────┐
│ Role-Based   │
│ Menu Display │
└──────────────┘
```

**Default Accounts:**
| Username | Password | Role |
|----------|----------|------|
| admin | admin123 | Administrator |
| doctor | doc123 | Doctor |

**Security Features:**
- Password-protected accounts
- Session-based authentication
- Role verification before each operation
- Cannot delete default admin account

**Implementation:**
```cpp
bool login(const string& username, const string& password) {
    auto it = users.find(username);
    
    if (it != users.end() && it->second.password == password) {
        currentUser = it->second;
        isLoggedIn = true;
        Utils::printSuccess("Login successful! Welcome, " + username);
        return true;
    }
    
    Utils::printError("Invalid username or password.");
    return false;
}
```

---

## 📁 Complete Project Structure

```
Rain_s_TSA/
│
├── 📂 include/              # Header files - Interface contracts
│   ├── Patient.h           # Patient & Node data structures
│   ├── LinkedListADT.h     # Doubly linked list interface
│   ├── QueueADT.h          # Priority queue interface
│   ├── UserManager.h       # Authentication system interface
│   └── Utils.h             # Utility functions interface
│
├── 📂 src/                  # Implementation files
│   ├── main.cpp            # Entry point, menu system (500+ lines)
│   ├── LinkedListADT.cpp   # Linked list implementation
│   ├── QueueADT.cpp        # Priority queue implementation
│   ├── UserManager.cpp     # Authentication implementation
│   └── Utils.cpp           # Utility functions implementation
│
├── 📂 data/                 # Persistent storage (auto-created)
│   ├── patients.txt        # Patient records database
│   ├── queue_log.txt       # Queue snapshot
│   └── users.txt           # User credentials
│
├── 📂 cmake-build-debug/    # Build output directory
│   ├── data/               # Runtime data folder
│   └── Rain_s_TSA.exe      # Compiled executable
│
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file
├── CONTRIBUTING.md         # Development guidelines
└── .gitignore             # Git ignore rules
```

---

## 🚀 Setup & Installation

### Prerequisites

**Required:**
- **C++ Compiler:** GCC 7.0+ or MSVC 2017+ (C++11 standard minimum)
- **CMake:** 3.28 or higher (for CLion users)
- **Git:** For repository cloning

**Recommended:**
- **CLion IDE:** For best development experience
- **VS Code:** With C/C++ extension
- **Code::Blocks:** Alternative lightweight IDE

### Installation Steps

#### Method 1: CLion (Recommended)

**1. Clone Repository**
```bash
git clone https://github.com/FarisMD22/Rain_s_TSA.git
```

**2. Open in CLion**
- Launch CLion
- `File` → `Open`
- Select `Rain_s_TSA` folder
- Wait for CMake indexing to complete

**3. Create Data Directory**
```bash
# In CLion terminal
mkdir cmake-build-debug\data
```

**4. Build Project**
- Click `Build` icon (🔨) or press `Ctrl+F9`
- Wait for compilation to complete

**5. Run Application**
- Click `Run` icon (▶️) or press `Shift+F10`
- Login with: `admin` / `admin123`

---

#### Method 2: Command Line

**1. Clone Repository**
```bash
git clone https://github.com/FarisMD22/Rain_s_TSA.git
cd Rain_s_TSA
```

**2. Create Directories**
```bash
# Windows
mkdir bin
mkdir cmake-build-debug\data

# Linux/Mac
mkdir -p bin
mkdir -p cmake-build-debug/data
```

**3. Compile**
```bash
# Windows (MinGW)
g++ -std=c++11 -Iinclude -o bin/mediflow.exe ^
    src/main.cpp ^
    src/Utils.cpp ^
    src/UserManager.cpp ^
    src/LinkedListADT.cpp ^
    src/QueueADT.cpp

# Linux/Mac
g++ -std=c++11 -Iinclude -o bin/mediflow \
    src/main.cpp \
    src/Utils.cpp \
    src/UserManager.cpp \
    src/LinkedListADT.cpp \
    src/QueueADT.cpp
```

**4. Run**
```bash
# Windows
bin\mediflow.exe

# Linux/Mac
./bin/mediflow
```

---

## 🧪 Testing Guide

### Comprehensive Test Scenario

**Test 1: Patient Record Management**
```
1. Login as admin (admin/admin123)
2. Add patients:
   - P001, Alice Johnson, 45, Heart Disease, 1, 2025-01-15
   - P002, Bob Smith, 30, Flu, 3, 2025-01-16
   - P003, Carol White, 55, Stroke, 1, 2025-01-17
   - P004, David Brown, 28, Sprain, 2, 2025-01-18
3. Display all → Should show 4 patients
4. Search by ID "P002" → Should find Bob
5. Search by Name "Carol" → Should find Carol White
```

**Expected Output:**
```
=== PATIENT RECORDS ===
ID    | Name          | Age | Priority | Condition
------|---------------|-----|----------|-------------
P001  | Alice Johnson | 45  | 1        | Heart Disease
P002  | Bob Smith     | 30  | 3        | Flu
P003  | Carol White   | 55  | 1        | Stroke
P004  | David Brown   | 28  | 2        | Sprain

Total patients: 4
```

---

**Test 2: Sorting Algorithms**
```
1. Sort by Name (Option 5 → 1)
2. Display → Should show alphabetical order:
   Alice → Bob → Carol → David

3. Sort by Priority (Option 5 → 2)
4. Display → Should show priority order:
   Alice (1) → Carol (1) → David (2) → Bob (3)
```

**Expected Order (Priority Sort):**
```
P001 | Alice Johnson | Priority 1 (Critical)
P003 | Carol White   | Priority 1 (Critical)
P004 | David Brown   | Priority 2 (Urgent)
P002 | Bob Smith     | Priority 3 (Normal)
```

---

**Test 3: Priority Queue System**
```
1. Add to queue (Option 6):
   - P005, Emma Green, 35, Checkup, 3, 2025-01-18
   - P006, Frank Red, 60, Chest Pain, 1, 2025-01-18
   - P007, Grace Blue, 40, Sprain, 2, 2025-01-18

2. View queue (Option 8)
```

**Expected Queue Order (Auto-Sorted):**
```
Position | ID   | Name        | Priority | Condition
---------|------|-------------|----------|------------
1        | P006 | Frank Red   | 1        | Chest Pain  ← Critical first!
2        | P007 | Grace Blue  | 2        | Sprain
3        | P005 | Emma Green  | 3        | Checkup
```

```
3. Process next patient (Option 7)
   → Frank Red removed (highest priority)
   
4. View queue again
   → Should only show Grace and Emma
```

---

**Test 4: Delete Operation**
```
1. Delete patient (Option 3)
2. Enter ID: P002
3. Confirm: y
4. Display all → Bob should be gone
```

**Expected Result:**
```
Total patients: 3
(Bob Smith no longer in list)
```

---

**Test 5: Data Persistence**
```
1. Logout (Option 10)
   → Should see: "[INFO] 4 patient records saved..."
   
2. Exit system (Option 2)

3. Restart application

4. Login as admin

5. Display all patients
   → All data should still be there!
```

---

**Test 6: Authentication & Roles**
```
1. Logout
2. Login as doctor (doctor/doc123)
3. Verify limited menu (only 5 options vs admin's 10)
4. Try to access patient records → Should work
5. User management should NOT be visible
```

**Doctor Menu (Limited Access):**
```
1. View Patient Records
2. Search Patient
3. View Current Queue
4. Process Next Patient
5. Logout
```

---

**Test 7: User Management (Admin Only)**
```
1. Login as admin
2. Select User Management (Option 9)
3. Add new user:
   - Username: testdoc
   - Password: test123
   - Role: 1 (Doctor)
4. View all users → Should show new user
5. Logout and login as testdoc/test123
6. Verify doctor-level access
```

---

### Edge Cases to Test

**Empty List Scenarios:**
```
- Display empty patient list
- Search in empty list
- Sort empty list
- Delete from empty list
```

**Single Element:**
```
- Sort list with one patient
- Delete only patient
- Queue operations with one patient
```

**Boundary Values:**
```
- Age: 0, 150
- Priority: 1, 2, 3
- Empty strings (should be validated)
```

**Error Handling:**
```
- Invalid login credentials
- Non-existent patient ID
- Delete default admin account (should fail)
- Invalid menu choices
```

---

## 👥 Team Contributions

### Task Distribution

| Member | Primary Responsibility | Files Owned | Status |
|--------|----------------------|-------------|--------|
| **Member 1** | Priority Queue Implementation | QueueADT.cpp, QueueADT.h | ✅ Complete |
| **Member 2** | Doubly Linked List Implementation | LinkedListADT.cpp, LinkedListADT.h | ✅ Complete |
| **Member 3** | Sorting & Search Algorithms | sortByName(), sortByPriority(), search functions | ✅ Complete |
| **Member 4** | File I/O System | saveToFile(), loadFromFile() in all ADTs | ✅ Complete |
| **Faris (Lead)** | Main System Integration | main.cpp, Utils, UserManager, Authentication | ✅ Complete |

### Code Statistics

```
Total Lines of Code: ~1,500 lines
Total Files: 10 files (.h + .cpp)
Comments: ~200 lines
Test Scenarios: 7 comprehensive tests
Time Complexity Optimizations: 3 algorithms
Memory Management: Destructor implementations for no leaks
```

---

## ✨ Features Implemented

### Core Requirements (100%)
- ✅ **Add Patient** - Insert operation in linked list
- ✅ **Edit Patient** - Update operation (via delete + add)
- ✅ **Delete Patient** - Remove operation with confirmation
- ✅ **Display Patients** - Forward and reverse traversal
- ✅ **Search Patients** - By ID and by name (linear search)
- ✅ **Sort Patients** - By name and priority (bubble sort)

### Advanced Features (Extra Credit)
- ✅ **Priority Queue** - Auto-sorting by medical urgency
- ✅ **Authentication System** - Login with role-based access
- ✅ **User Management** - Dynamic account creation/deletion
- ✅ **Data Persistence** - File I/O for permanent storage
- ✅ **Input Validation** - Age limits, priority range checks
- ✅ **Bidirectional Traversal** - Doubly linked list feature
- ✅ **Professional UI** - Clean menu system with headers
- ✅ **Error Handling** - Graceful handling of invalid inputs
- ✅ **Memory Management** - Destructors to prevent leaks
- ✅ **Session Management** - Auto-save on logout/exit

---

## 🎓 Academic Justifications

### Why This Design?

**1. Doubly Linked List for Patient Records**

**Decision:** Use doubly linked list instead of singly linked or array.

**Justification:**
- **Bidirectional Traversal:** Required for `displayReverse()` feature
- **Efficient Deletion:** Can traverse backward from tail
- **Dynamic Size:** No need to pre-allocate fixed array size
- **O(1) Insertion:** At head or tail positions
- **Hospital Context:** Patient records change frequently (admissions/discharges)

**Trade-off Analysis:**
```
Memory: Extra pointer per node (~8 bytes on 64-bit)
Benefit: O(1) backward traversal vs O(n) with singly linked
Verdict: Worth it for flexibility in medical records system
```

---

**2. Priority Queue Implementation**

**Decision:** Implement priority queue using sorted linked list.

**Alternatives Considered:**
| Approach | Enqueue | Dequeue | Memory | Decision |
|----------|---------|---------|--------|----------|
| **Unsorted Array** | O(1) | O(n) | Fixed | ❌ No dynamic sizing |
| **Sorted Array** | O(n) | O(1) | Fixed | ❌ Expensive insertions |
| **Heap** | O(log n) | O(log n) | Dynamic | ❌ Too complex for course |
| **Sorted Linked List** | O(n) | O(1) | Dynamic | ✅ **CHOSEN** |

**Justification:**
- Balances complexity and efficiency
- Suitable for hospital queue sizes (typically <100 patients)
- Always maintains sorted order
- Dequeue is O(1) - critical for fast patient processing
- Dynamic memory - no queue size limit

---

**3. Bubble Sort Algorithm**

**Decision:** Use bubble sort instead of merge sort or quick sort.

**Justification:**
```
Dataset Size: Small-to-medium (hospital records: ~100-1000 patients)
Stability: Required (preserve order of equal elements)
Simplicity: Team understanding and implementation time
Space: In-place sorting (no extra memory)

Result: Bubble sort O(n²) acceptable for this scale
```

**Performance Analysis:**
```
For n=100 patients:
- Bubble Sort: ~10,000 comparisons
- Merge Sort: ~664 comparisons
- Reality: Difference is ~0.001 seconds on modern CPUs
- Verdict: Bubble sort simplicity outweighs slight performance loss
```

---

**4. Linear Search**

**Decision:** Use linear search instead of binary search.

**Constraint:** Linked lists don't support O(1) random access.

**Justification:**
```
Binary Search Requires:
- Random access: O(1) - Linked list is O(n) ❌
- Sorted data: Can be maintained ✅
- Array structure: Not our choice ❌

Linear Search Benefits:
- Works on any list (sorted or not) ✅
- Simple implementation ✅
- Acceptable O(n) for hospital datasets ✅
```

---

**5. CSV File Format**

**Decision:** Use CSV instead of binary files.

**Justification:**
| Format | Pros | Cons | Decision |
|--------|------|------|----------|
| **Binary** | Fast, compact | Not human-readable | ❌ |
| **JSON** | Structured | Requires library | ❌ |
| **XML** | Structured | Verbose, complex | ❌ |
| **CSV** | Simple, readable, portable | Manual parsing | ✅ |

**Reasoning:**
- Debuggable (can open in Excel/Notepad)
- No external dependencies
- Easy to parse with C++ `stringstream`
- Standard format for data exchange

---

## 🐛 Known Issues & Limitations

### Current Limitations

**1. Date Validation**
- **Issue:** Date input accepts any string format
- **Impact:** User could enter invalid dates like "99-99-9999"
- **Workaround:** Trust user input, document expected format
- **Future Fix:** Add regex validation for YYYY-MM-DD format

**2. Duplicate ID Handling**
- **Issue:** System allows duplicate patient IDs
- **Impact:** Search returns first match only
- **Workaround:** Use unique IDs manually (P001, P002, etc.)
- **Future Fix:** Check for duplicates before insertion

**3. Password Security**
- **Issue:** Passwords stored in plaintext
- **Impact:** Not suitable for production use
- **Justification:** Academic project focus on data structures
- **Future Fix:** Implement password hashing (SHA-256)

**4. Concurrent Access**
- **Issue:** Not thread-safe, single-user only
- **Impact:** Multi-user access could corrupt data
- **Context:** CLI application, single terminal session
- **Future Fix:** Add file locking mechanisms

### Not Bugs (By Design)

❌ **"Menu disappears after selection"**
→ Expected behavior, new menu appears after operation

❌ **"Have to press Enter to continue"**
→ Intentional, lets you read operation results

❌ **"Queue reorders when I add patient"**
→ Correct! Priority queue auto-sorts by urgency

---

## 🔮 Future Enhancements

### Potential Improvements (Post-Submission)

**1. Advanced Search**
```cpp
// Search by age range
vector<Patient> searchByAgeRange(int min, int max);

// Search by condition
vector<Patient> searchByCondition(string condition);

// Multi-criteria search
vector<Patient> advancedSearch(SearchCriteria criteria);
```

**2. Statistics Dashboard**
```
╔════════════════════════════════════╗
║       PATIENT STATISTICS           ║
╠════════════════════════════════════╣
║ Total Patients:          25        ║
║ In Queue:                7         ║
║ Critical Priority:       3         ║
║ Average Age:             42.5      ║
║ Most Common Condition:   Flu       ║
╚════════════════════════════════════╝
```

**3. Export Functionality**
```
- Export to Excel (.xlsx)
- Generate PDF reports
- Email patient summaries
- Backup/restore database
```

**4. Advanced UI**
```cpp
// Color-coded priorities
RED for Critical (Priority 1)
YELLOW for Urgent (Priority 2)
GREEN for Normal (Priority 3)

// Progress bars
[████████████████████░░░░] 80% Capacity
```

**5. Better Sorting**
```cpp
// Merge sort implementation for large datasets
void mergeSortByName();  // O(n log n)

// Quick sort with median-of-three pivot
void quickSortByPriority();
```

**6. Appointment System**
```cpp
class AppointmentQueue {
    // Schedule future appointments
    void scheduleAppointment(Patient p, DateTime time);
    
    // Reminder system
    void sendReminders();
};
```

---

## 📖 Lessons Learned

### Technical Insights

**1. Pointer Management**
- Doubly linked lists require careful `prev` and `next` handling
- Always update both pointers during insertion/deletion
- Memory leaks prevented with proper destructor implementation

**2. File I/O Challenges**
- Windows vs Linux path separators (`\` vs `/`)
- Working directory differs between IDE and command line
- Solution: Relative paths and `cmake-build-debug/data/` structure

**3. Input Validation**
- `cin` can fail with invalid input types
- Always check `cin.fail()` and clear error state
- Use `cin.ignore()` to flush input buffer

**4. Code Organization**
- Header-first approach prevents circular dependencies
- Three-layer architecture enables parallel development
- Clear separation of concerns simplifies debugging

### Teamwork Insights

**What Worked Well:**
✅ GitHub for version control and collaboration
✅ Clear task distribution from day one
✅ Regular code reviews and testing
✅ Documentation as we code (not last-minute)

**Challenges Faced:**
⚠️ Merge conflicts when editing same file
⚠️ Different coding styles across team members
⚠️ Time zone coordination for meetings

**Solutions Applied:**
✅ Branching strategy (feature branches)
✅ Agreed coding standards in CONTRIBUTING.md
✅ Async communication via GitHub issues

---

## 📚 References & Resources

### Course Materials
- TDS6213 Lecture Notes (Weeks 1-12)
- Data Structures and Algorithms textbook
- Lab session tutorials

### External Resources
- [C++ Reference](https://en.cppreference.com/) - Standard library documentation
- [LearnCpp.com](https://www.learncpp.com/) - C++ tutorials
- [Big-O Cheat Sheet](https://www.bigocheatsheet.com/) - Algorithm complexity reference

### Development Tools
- [CLion IDE](https://www.jetbrains.com/clion/) - Development environment
- [Git Documentation](https://git-scm.com/doc) - Version control
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/) - Build system

---

## 📝 Submission Checklist

### Required Deliverables (Due: Jan 26, 2025)

- [x] **Final Report** - System documentation and justifications
- [x] **C++ Implementation** - All source code files
- [x] **Task Declaration Form** - Individual contributions
- [x] **Project Evaluation Sheet** - Self-assessment
- [x] **Presentation Slide** - 10-minute demo slides
- [x] **Demo Video** - Working system demonstration

### Pre-Submission Verification

- [x] Code compiles without errors
- [x] All features tested and working
- [x] Data persistence verified
- [x] Memory leaks checked (no crashes)
- [x] Documentation complete
- [x] Comments added to complex sections
- [x] GitHub repository up-to-date
- [x] README.md comprehensive
- [x] Team member contributions documented

---

## 🏆 Project Highlights

### Achievements

🎯 **100% Feature Complete** - All requirements met  
🧪 **Fully Tested** - 7 comprehensive test scenarios passed  
📚 **Well Documented** - 1,500+ lines including comments  
🔒 **Memory Safe** - Proper destructors, no leaks  
💾 **Data Persistent** - File I/O working flawlessly  
🎨 **Clean Architecture** - Three-layer design pattern  
👥 **Team Collaboration** - GitHub-based workflow  
🚀 **Production Ready** - Suitable for demo and grading  

---

## 📞 Contact & Support

### Team Information

**Project Lead:** Faris  
**Repository:** [github.com/FarisMD22/Rain_s_TSA](https://github.com/FarisMD22/Rain_s_TSA)  
**Course:** TDS6213 Data Structures and Algorithms  
**Institution:** MMU (Multimedia University)  

### Getting Help

**For Technical Issues:**
1. Check this README first
2. Search [GitHub Issues](https://github.com/FarisMD22/Rain_s_TSA/issues)
3. Contact team lead via GitHub

**For Compilation Errors:**
1. Verify all files present in correct directories
2. Check `cmake-build-debug/data/` folder exists
3. Ensure C++11 compiler installed
4. Review error messages in build log

---

## 📄 License & Academic Integrity

**License:** This project is for academic purposes only (TDS6213 coursework).

**Academic Integrity Statement:**
This code represents original work by our team. All implementations follow course requirements and best practices taught in TDS6213. We have properly cited any external references used.

**Anti-Plagiarism Notice:**
- Code is original implementation
- No copying from seniors or online sources
- Algorithms based on course materials
- Team collaboration documented in Git history

---

## 🎓 Conclusion

MediFlow demonstrates practical application of data structures and algorithms in a real-world healthcare context. The system successfully implements:

✅ **Advanced Data Structures** - Doubly linked lists, priority queues  
✅ **Efficient Algorithms** - Sorting, searching with complexity analysis  
✅ **Software Engineering** - Clean architecture, version control, documentation  
✅ **Problem Solving** - Healthcare workflow automation  

**Project Status:** ✅ **PRODUCTION READY FOR SUBMISSION**

---

**Last Updated:** January 19, 2025  
**Version:** 1.0.0  
**Build Status:** ✅ Passing All Tests  

---

*Built with ❤️ by the MediFlow Team for TDS6213*
```
