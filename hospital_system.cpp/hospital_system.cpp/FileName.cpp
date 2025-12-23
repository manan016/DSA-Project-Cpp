// hospital_system.cpp
// Full Hospital Management System (Option B)
// - Patient Management (Linked List, QuickSort, BinarySearch)
// - Appointment Management (Queue for routine appointments)
// - Emergency Management (Priority Queue for emergency cases, Linear search)
// - Doctor/Staff database (unordered_map hash table)
// - Reporting & Analytics (MergeSort and BST for hierarchical analysis)
// - File persistence for patients, appointments, emergencies, doctors
// - Console menu UI
//
// Author: Generated for Option B request
// Compile: g++ -std=c++17 hospital_system.cpp -o hospital_system

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <functional>       // for std::greater / std::less
#include <unordered_map>
#include <iomanip>
#include <algorithm>        // swap, sort
#include <limits>
#include <sstream>

using namespace std;

// ---------------------------- Utility & Input Helpers ---------------------
int getInt(const string& prompt) {
    while (true) {
        cout << prompt;
        int x;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        else {
            cout << "Invalid integer. Try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string getLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

// ---------------------------- Patient Entity ------------------------------
class Patient {
public:
    int id;
    string name;
    int age;
    string gender;
    string ailment;
    int priority;           // severity for emergency (higher = more urgent)
    string phone;
    string assignedDoctor;

    Patient() : id(0), name(""), age(0), gender(""), ailment(""), priority(0), phone(""), assignedDoctor("") {}

    string serialize() const {
        // id|name|age|gender|ailment|priority|phone|assignedDoctor
        ostringstream oss;
        oss << id << '|' << name << '|' << age << '|' << gender << '|' << ailment << '|' << priority << '|' << phone << '|' << assignedDoctor;
        return oss.str();
    }

    static bool deserialize(const string& line, Patient& p) {
        if (line.empty()) return false;
        vector<string> parts;
        string cur;
        for (size_t i = 0; i <= line.size(); ++i) {
            if (i == line.size() || line[i] == '|') {
                parts.push_back(cur);
                cur.clear();
            }
            else cur.push_back(line[i]);
        }
        if (parts.size() < 8) return false;
        try {
            p.id = stoi(parts[0]);
            p.name = parts[1];
            p.age = stoi(parts[2]);
            p.gender = parts[3];
            p.ailment = parts[4];
            p.priority = stoi(parts[5]);
            p.phone = parts[6];
            p.assignedDoctor = parts[7];
            return true;
        }
        catch (...) {
            return false;
        }
    }
};

// ---------------------------- Linked List for Patients --------------------
class PatientNode {
public:
    Patient data;
    PatientNode* next;
    PatientNode(const Patient& p) : data(p), next(nullptr) {}
};

class PatientList {
private:
    PatientNode* head;
    int count;
public:
    PatientList() : head(nullptr), count(0) {}
    ~PatientList() { clear(); }

    bool empty() const { return head == nullptr; }
    int size() const { return count; }

    bool addPatient(const Patient& p) {
        if (findById(p.id) != nullptr) return false; // duplicate id
        PatientNode* node = new PatientNode(p);
        if (!head) head = node;
        else {
            PatientNode* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        ++count;
        return true;
    }

    bool removeById(int id) {
        if (!head) return false;
        if (head->data.id == id) {
            PatientNode* tmp = head;
            head = head->next;
            delete tmp;
            --count;
            return true;
        }
        PatientNode* cur = head;
        while (cur->next && cur->next->data.id != id) cur = cur->next;
        if (cur->next && cur->next->data.id == id) {
            PatientNode* tmp = cur->next;
            cur->next = tmp->next;
            delete tmp;
            --count;
            return true;
        }
        return false;
    }

    PatientNode* findById(int id) const {
        PatientNode* cur = head;
        while (cur) {
            if (cur->data.id == id) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    vector<Patient> toVector() const {
        vector<Patient> v;
        PatientNode* cur = head;
        while (cur) {
            v.push_back(cur->data);
            cur = cur->next;
        }
        return v;
    }

    void replaceFromVector(const vector<Patient>& v) {
        clear();
        for (const auto& p : v) addPatient(p);
    }

    void displayAll() const {
        if (!head) {
            cout << "No patient records.\n";
            return;
        }
        cout << left << setw(6) << "ID" << setw(20) << "Name" << setw(6) << "Age"
            << setw(8) << "Gender" << setw(15) << "Ailment" << setw(9) << "Priority"
            << setw(15) << "Doctor" << setw(12) << "Phone" << '\n';
        cout << string(95, '-') << '\n';
        PatientNode* cur = head;
        while (cur) {
            const Patient& p = cur->data;
            cout << setw(6) << p.id << setw(20) << p.name << setw(6) << p.age
                << setw(8) << p.gender << setw(15) << p.ailment << setw(9) << p.priority
                << setw(15) << p.assignedDoctor << setw(12) << p.phone << '\n';
            cur = cur->next;
        }
    }

    bool saveToFile(const string& filename = "patients.txt") const {
        ofstream fout(filename);
        if (!fout.is_open()) return false;
        PatientNode* cur = head;
        while (cur) {
            fout << cur->data.serialize() << '\n';
            cur = cur->next;
        }
        fout.close();
        return true;
    }

    bool loadFromFile(const string& filename = "patients.txt") {
        ifstream fin(filename);
        if (!fin.is_open()) return false;
        clear();
        string line;
        while (getline(fin, line)) {
            Patient p;
            if (Patient::deserialize(line, p)) addPatient(p);
        }
        fin.close();
        return true;
    }

    void clear() {
        PatientNode* cur = head;
        while (cur) {
            PatientNode* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = nullptr;
        count = 0;
    }
};

// ---------------------------- QuickSort & Binary Search -------------------
// QuickSort by patient id
int partition(vector<Patient>& arr, int low, int high) {
    int pivot = arr[high].id;
    int i = low - 1;
    for (int j = low; j <= high - 1; ++j) {
        if (arr[j].id <= pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<Patient>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Binary search on sorted vector by id
int binarySearchById(const vector<Patient>& arr, int id) {
    int l = 0, r = (int)arr.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (arr[m].id == id) return m;
        if (arr[m].id < id) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

// ---------------------------- Appointment Management ---------------------
struct Appointment {
    int patientId;
    string timeSlot;
    string notes;

    Appointment() : patientId(0), timeSlot(""), notes("") {}
    Appointment(int pid, const string& ts, const string& n) : patientId(pid), timeSlot(ts), notes(n) {}

    string serialize() const {
        // pid|timeslot|notes
        ostringstream oss;
        oss << patientId << '|' << timeSlot << '|' << notes;
        return oss.str();
    }

    static bool deserialize(const string& line, Appointment& a) {
        if (line.empty()) return false;
        vector<string> parts;
        string cur;
        for (size_t i = 0; i <= line.size(); ++i) {
            if (i == line.size() || line[i] == '|') {
                parts.push_back(cur);
                cur.clear();
            }
            else cur.push_back(line[i]);
        }
        if (parts.size() < 3) return false;
        try {
            a.patientId = stoi(parts[0]);
            a.timeSlot = parts[1];
            a.notes = parts[2];
            return true;
        }
        catch (...) {
            return false;
        }
    }
};

class AppointmentManager {
private:
    queue<Appointment> routineQueue;
public:
    AppointmentManager() {}

    void scheduleRoutine(const Appointment& a) { routineQueue.push(a); }
    bool hasRoutine() const { return !routineQueue.empty(); }
    Appointment popNextRoutine() {
        if (routineQueue.empty()) return Appointment();
        Appointment a = routineQueue.front();
        routineQueue.pop();
        return a;
    }
    // persistence
    bool saveToFile(const string& filename = "appointments.txt") const {
        ofstream fout(filename);
        if (!fout.is_open()) return false;
        queue<Appointment> copy = routineQueue;
        while (!copy.empty()) {
            fout << copy.front().serialize() << '\n';
            copy.pop();
        }
        fout.close();
        return true;
    }
    bool loadFromFile(const string& filename = "appointments.txt") {
        ifstream fin(filename);
        if (!fin.is_open()) return false;
        // clear existing
        routineQueue = queue<Appointment>();
        string line;
        while (getline(fin, line)) {
            Appointment a;
            if (Appointment::deserialize(line, a)) routineQueue.push(a);
        }
        fin.close();
        return true;
    }
};

// ---------------------------- Emergency Management -----------------------
struct EmergencyItem {
    int patientId;
    int priority; // higher = more urgent
    string notes;
    EmergencyItem() : patientId(0), priority(0), notes("") {}
    EmergencyItem(int pid, int pr, const string& n) : patientId(pid), priority(pr), notes(n) {}
    string serialize() const {
        ostringstream oss;
        oss << patientId << '|' << priority << '|' << notes;
        return oss.str();
    }
    static bool deserialize(const string& line, EmergencyItem& e) {
        if (line.empty()) return false;
        vector<string> parts;
        string cur;
        for (size_t i = 0; i <= line.size(); ++i) {
            if (i == line.size() || line[i] == '|') {
                parts.push_back(cur);
                cur.clear();
            }
            else cur.push_back(line[i]);
        }
        if (parts.size() < 3) return false;
        try {
            e.patientId = stoi(parts[0]);
            e.priority = stoi(parts[1]);
            e.notes = parts[2];
            return true;
        }
        catch (...) {
            return false;
        }
    }
};

// comparator for priority queue (max-heap)
struct EmergencyCompare {
    bool operator()(const EmergencyItem& a, const EmergencyItem& b) const {
        return a.priority < b.priority; // top = highest priority
    }
};

class EmergencyManager {
private:
    priority_queue<EmergencyItem, vector<EmergencyItem>, EmergencyCompare> emergencyQueue;
public:
    EmergencyManager() {}

    void scheduleEmergency(const EmergencyItem& e) { emergencyQueue.push(e); }
    bool hasEmergency() const { return !emergencyQueue.empty(); }
    EmergencyItem popNextEmergency() {
        if (emergencyQueue.empty()) return EmergencyItem();
        EmergencyItem e = emergencyQueue.top();
        emergencyQueue.pop();
        return e;
    }

    // linear search for patientId in emergency queue (inefficient but requested)
    bool linearSearchPatient(int pid) const {
        // priority_queue doesn't allow iteration; copy it
        auto copy = emergencyQueue;
        while (!copy.empty()) {
            if (copy.top().patientId == pid) return true;
            copy.pop();
        }
        return false;
    }

    bool saveToFile(const string& filename = "emergencies.txt") const {
        ofstream fout(filename);
        if (!fout.is_open()) return false;
        auto copy = emergencyQueue;
        while (!copy.empty()) {
            fout << copy.top().serialize() << '\n';
            copy.pop();
        }
        fout.close();
        return true;
    }

    bool loadFromFile(const string& filename = "emergencies.txt") {
        ifstream fin(filename);
        if (!fin.is_open()) return false;
        emergencyQueue = priority_queue<EmergencyItem, vector<EmergencyItem>, EmergencyCompare>();
        string line;
        while (getline(fin, line)) {
            EmergencyItem e;
            if (EmergencyItem::deserialize(line, e)) emergencyQueue.push(e);
        }
        fin.close();
        return true;
    }
};

// ---------------------------- Doctor / Staff DB (Hash Table) -------------
class DoctorDB {
private:
    // doctorName -> vector of available time slots
    unordered_map<string, vector<string>> doctors;
    // staffName -> role
    unordered_map<string, string> staff; // e.g., "Nurse A" -> "nurse"

public:
    DoctorDB() {}

    void addDoctor(const string& name) {
        if (doctors.find(name) == doctors.end()) doctors[name] = vector<string>();
    }

    bool removeDoctor(const string& name) {
        auto it = doctors.find(name);
        if (it == doctors.end()) return false;
        doctors.erase(it);
        return true;
    }

    bool addAvailability(const string& name, const string& timeslot) {
        auto it = doctors.find(name);
        if (it == doctors.end()) return false;
        it->second.push_back(timeslot);
        return true;
    }

    bool removeAvailability(const string& name, const string& timeslot) {
        auto it = doctors.find(name);
        if (it == doctors.end()) return false;
        auto& vec = it->second;
        auto f = find(vec.begin(), vec.end(), timeslot);
        if (f == vec.end()) return false;
        vec.erase(f);
        return true;
    }

    vector<pair<string, vector<string>>> listAllDoctors() const {
        vector<pair<string, vector<string>>> out;
        for (const auto& kv : doctors) out.push_back(kv);
        return out;
    }

    // Staff functions
    void addStaff(const string& name, const string& role) { staff[name] = role; }
    bool removeStaff(const string& name) {
        auto it = staff.find(name);
        if (it == staff.end()) return false;
        staff.erase(it);
        return true;
    }
    vector<pair<string, string>> listAllStaff() const {
        vector<pair<string, string>> out;
        for (const auto& kv : staff) out.push_back(kv);
        return out;
    }

    // persistence: doctors and staff in separate files
    bool saveDoctors(const string& filename = "doctors.txt") const {
        ofstream fout(filename);
        if (!fout.is_open()) return false;
        // Format: doctor|slot1;slot2;slot3
        for (const auto& kv : doctors) {
            fout << kv.first << '|';
            for (size_t i = 0; i < kv.second.size(); ++i) {
                fout << kv.second[i];
                if (i + 1 < kv.second.size()) fout << ';';
            }
            fout << '\n';
        }
        fout.close();
        return true;
    }

    bool loadDoctors(const string& filename = "doctors.txt") {
        ifstream fin(filename);
        if (!fin.is_open()) return false;
        doctors.clear();
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            size_t pos = line.find('|');
            if (pos == string::npos) continue;
            string name = line.substr(0, pos);
            string slots = line.substr(pos + 1);
            vector<string> v;
            string cur;
            for (size_t i = 0; i <= slots.size(); ++i) {
                if (i == slots.size() || slots[i] == ';') {
                    if (!cur.empty()) v.push_back(cur);
                    cur.clear();
                }
                else cur.push_back(slots[i]);
            }
            doctors[name] = v;
        }
        fin.close();
        return true;
    }

    bool saveStaff(const string& filename = "staff.txt") const {
        ofstream fout(filename);
        if (!fout.is_open()) return false;
        // Format: staffName|role
        for (const auto& kv : staff) fout << kv.first << '|' << kv.second << '\n';
        fout.close();
        return true;
    }

    bool loadStaff(const string& filename = "staff.txt") {
        ifstream fin(filename);
        if (!fin.is_open()) return false;
        staff.clear();
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            size_t pos = line.find('|');
            if (pos == string::npos) continue;
            string name = line.substr(0, pos);
            string role = line.substr(pos + 1);
            staff[name] = role;
        }
        fin.close();
        return true;
    }
};

// ---------------------------- Reporting & Analytics ----------------------
// Merge sort for reports (sort patients by ID for report)
void merge(vector<Patient>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<Patient> L(n1);
    vector<Patient> R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].id <= R[j].id) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<Patient>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// BST for hierarchical data (e.g., ailment counts)
class AnalyticsNode {
public:
    string key;
    int count;
    AnalyticsNode* left, * right;
    AnalyticsNode(const string& k) : key(k), count(1), left(nullptr), right(nullptr) {}
};

class AnalyticsTree {
private:
    AnalyticsNode* root;
    void insertRec(AnalyticsNode*& node, const string& key) {
        if (!node) {
            node = new AnalyticsNode(key);
            return;
        }
        if (key == node->key) node->count++;
        else if (key < node->key) insertRec(node->left, key);
        else insertRec(node->right, key);
    }
    void inorderRec(AnalyticsNode* node) const {
        if (!node) return;
        inorderRec(node->left);
        cout << setw(25) << node->key << " : " << node->count << '\n';
        inorderRec(node->right);
    }
    void freeRec(AnalyticsNode* node) {
        if (!node) return;
        freeRec(node->left);
        freeRec(node->right);
        delete node;
    }
public:
    AnalyticsTree() : root(nullptr) {}
    ~AnalyticsTree() { freeRec(root); }
    void insert(const string& key) { insertRec(root, key); }
    void displayInorder() const {
        cout << "Analytics (key : count)\n";
        cout << string(40, '-') << '\n';
        inorderRec(root);
        cout << string(40, '-') << '\n';
    }
};

// Report generator
class ReportGenerator {
public:
    static void patientReport(const vector<Patient>& patients) {
        if (patients.empty()) {
            cout << "No patient data to generate report.\n";
            return;
        }
        vector<Patient> copy = patients;
        mergeSort(copy, 0, (int)copy.size() - 1);
        cout << "Patient Report (sorted by ID):\n";
        cout << left << setw(6) << "ID" << setw(20) << "Name" << setw(15) << "Ailment" << setw(9) << "Priority" << setw(6) << "Age" << '\n';
        cout << string(60, '-') << '\n';
        for (const auto& p : copy) {
            cout << setw(6) << p.id << setw(20) << p.name << setw(15) << p.ailment << setw(9) << p.priority << setw(6) << p.age << '\n';
        }
        // simple stats
        int total = (int)copy.size();
        double sumAge = 0;
        unordered_map<string, int> genderCount;
        unordered_map<string, int> ailmentCount;
        for (const auto& p : copy) {
            sumAge += p.age;
            genderCount[p.gender]++;
            ailmentCount[p.ailment]++;
        }
        cout << "\nStatistics:\n";
        cout << "Total patients: " << total << '\n';
        cout << "Average age: " << fixed << setprecision(2) << (sumAge / total) << '\n';
        cout << "By gender:\n";
        for (auto& kv : genderCount) cout << "  " << kv.first << " : " << kv.second << '\n';
        // top ailments
        vector<pair<int, string>> ailVec;
        for (auto& kv : ailmentCount) ailVec.push_back({ kv.second, kv.first });
        sort(ailVec.rbegin(), ailVec.rend());
        cout << "Top ailments:\n";
        for (int i = 0; i < min(3, (int)ailVec.size()); ++i) {
            cout << "  " << ailVec[i].second << " : " << ailVec[i].first << '\n';
        }
    }

    static void analyticsByAilment(const vector<Patient>& patients) {
        if (patients.empty()) {
            cout << "No data available.\n";
            return;
        }
        AnalyticsTree tree;
        for (const auto& p : patients) tree.insert(p.ailment);
        tree.displayInorder();
    }
};

// ---------------------------- Console Menus --------------------------------
void showMainMenu() {
    cout << "\n=== Hospital Management System ===\n";
    cout << "1. Patient Management\n";
    cout << "2. Appointments (Routine)\n";
    cout << "3. Emergency Management\n";
    cout << "4. Doctor & Staff Management\n";
    cout << "5. Reporting & Analytics\n";
    cout << "6. Save All Data\n";
    cout << "7. Load All Data\n";
    cout << "0. Exit\n";
}

void patientMenu(PatientList& plist) {
    while (true) {
        cout << "\n--- Patient Management ---\n";
        cout << "1. Add patient\n";
        cout << "2. Remove patient by ID\n";
        cout << "3. Update patient\n";
        cout << "4. Display all patients\n";
        cout << "5. Find patient by ID (binary search on sorted view)\n";
        cout << "6. Save patients to file\n";
        cout << "7. Load patients from file\n";
        cout << "0. Back\n";
        int ch = getInt("Choice: ");
        if (ch == 0) break;
        if (ch == 1) {
            Patient p;
            p.id = getInt("Enter ID (int): ");
            p.name = getLine("Name: ");
            p.age = getInt("Age: ");
            p.gender = getLine("Gender: ");
            p.ailment = getLine("Ailment: ");
            p.priority = getInt("Priority (0 low, higher = more urgent): ");
            p.phone = getLine("Phone: ");
            p.assignedDoctor = getLine("Assigned doctor: ");
            if (plist.addPatient(p)) cout << "Patient added.\n";
            else cout << "Patient with that ID already exists.\n";
        }
        else if (ch == 2) {
            int id = getInt("ID to remove: ");
            if (plist.removeById(id)) cout << "Removed.\n";
            else cout << "Not found.\n";
        }
        else if (ch == 3) {
            int id = getInt("ID to update: ");
            PatientNode* node = plist.findById(id);
            if (!node) { cout << "Patient not found.\n"; continue; }
            Patient& p = node->data;
            cout << "Leave blank to keep existing (press Enter without typing).\n";
            cout << "Current name: " << p.name << '\n';
            string s = getLine("New name: ");
            if (!s.empty()) p.name = s;
            cout << "Current age: " << p.age << '\n';
            s = getLine("New age: ");
            if (!s.empty()) p.age = stoi(s);
            cout << "Current gender: " << p.gender << '\n';
            s = getLine("New gender: ");
            if (!s.empty()) p.gender = s;
            cout << "Current ailment: " << p.ailment << '\n';
            s = getLine("New ailment: ");
            if (!s.empty()) p.ailment = s;
            cout << "Current priority: " << p.priority << '\n';
            s = getLine("New priority: ");
            if (!s.empty()) p.priority = stoi(s);
            cout << "Current phone: " << p.phone << '\n';
            s = getLine("New phone: ");
            if (!s.empty()) p.phone = s;
            cout << "Current assigned doctor: " << p.assignedDoctor << '\n';
            s = getLine("New assigned doctor: ");
            if (!s.empty()) p.assignedDoctor = s;
            cout << "Updated.\n";
        }
        else if (ch == 4) {
            plist.displayAll();
        }
        else if (ch == 5) {
            vector<Patient> arr = plist.toVector();
            if (arr.empty()) { cout << "No patients.\n"; continue; }
            quickSort(arr, 0, (int)arr.size() - 1);
            int id = getInt("Enter ID to search: ");
            int idx = binarySearchById(arr, id);
            if (idx == -1) cout << "Not found.\n";
            else {
                const Patient& p = arr[idx];
                cout << "Found (sorted view):\n";
                cout << "ID: " << p.id << " Name: " << p.name << " Age: " << p.age << " Ailment: " << p.ailment << " Priority: " << p.priority << '\n';
            }
        }
        else if (ch == 6) {
            if (plist.saveToFile()) cout << "Saved patients.txt\n"; else cout << "Save failed.\n";
        }
        else if (ch == 7) {
            if (plist.loadFromFile()) cout << "Loaded patients.txt\n"; else cout << "Load failed or file not found.\n";
        }
        else cout << "Invalid option.\n";
    }
}

void appointmentMenu(AppointmentManager& am, PatientList& plist) {
    while (true) {
        cout << "\n--- Appointments (Routine) ---\n";
        cout << "1. Schedule routine appointment\n";
        cout << "2. Process next routine appointment\n";
        cout << "3. Save routine appointments to file\n";
        cout << "4. Load routine appointments from file\n";
        cout << "0. Back\n";
        int ch = getInt("Choice: ");
        if (ch == 0) break;
        if (ch == 1) {
            int pid = getInt("Patient ID: ");
            if (!plist.findById(pid)) { cout << "Patient not found.\n"; continue; }
            string ts = getLine("Time slot (e.g. 2025-12-15 10:30): ");
            string notes = getLine("Notes: ");
            am.scheduleRoutine(Appointment(pid, ts, notes));
            cout << "Routine appointment scheduled.\n";
        }
        else if (ch == 2) {
            if (!am.hasRoutine()) { cout << "No routine appointments.\n"; continue; }
            Appointment a = am.popNextRoutine();
            cout << "Processing routine appointment: Patient ID " << a.patientId << " Time: " << a.timeSlot << " Notes: " << a.notes << '\n';
        }
        else if (ch == 3) {
            if (am.saveToFile()) cout << "Saved appointments.txt\n"; else cout << "Save failed.\n";
        }
        else if (ch == 4) {
            if (am.loadFromFile()) cout << "Loaded appointments.txt\n"; else cout << "Load failed or file not found.\n";
        }
        else cout << "Invalid option.\n";
    }
}

void emergencyMenu(EmergencyManager& em, PatientList& plist) {
    while (true) {
        cout << "\n--- Emergency Management ---\n";
        cout << "1. Schedule emergency\n";
        cout << "2. Process next emergency\n";
        cout << "3. Check if patient is in emergency queue (linear search)\n";
        cout << "4. Save emergencies to file\n";
        cout << "5. Load emergencies from file\n";
        cout << "0. Back\n";
        int ch = getInt("Choice: ");
        if (ch == 0) break;
        if (ch == 1) {
            int pid = getInt("Patient ID: ");
            PatientNode* node = plist.findById(pid);
            if (!node) { cout << "Patient not found.\n"; continue; }
            int pr = getInt("Priority (higher = more urgent): ");
            string notes = getLine("Notes: ");
            em.scheduleEmergency(EmergencyItem(pid, pr, notes));
            cout << "Emergency scheduled.\n";
        }
        else if (ch == 2) {
            if (!em.hasEmergency()) { cout << "No emergencies.\n"; continue; }
            EmergencyItem e = em.popNextEmergency();
            cout << "Processing EMERGENCY: Patient ID " << e.patientId << " Priority " << e.priority << " Notes: " << e.notes << '\n';
        }
        else if (ch == 3) {
            int pid = getInt("Patient ID to search in emergency queue: ");
            bool found = em.linearSearchPatient(pid);
            cout << (found ? "Patient found in emergency queue.\n" : "Patient not in emergency queue.\n");
        }
        else if (ch == 4) {
            if (em.saveToFile()) cout << "Saved emergencies.txt\n"; else cout << "Save failed.\n";
        }
        else if (ch == 5) {
            if (em.loadFromFile()) cout << "Loaded emergencies.txt\n"; else cout << "Load failed or file not found.\n";
        }
        else cout << "Invalid option.\n";
    }
}

void doctorMenu(DoctorDB& db) {
    while (true) {
        cout << "\n--- Doctor & Staff Management ---\n";
        cout << "1. Add doctor\n";
        cout << "2. Remove doctor\n";
        cout << "3. Add availability (timeslot) for doctor\n";
        cout << "4. Remove availability\n";
        cout << "5. List all doctors & schedules\n";
        cout << "6. Add staff member\n";
        cout << "7. Remove staff member\n";
        cout << "8. List staff\n";
        cout << "9. Save doctors & staff to file\n";
        cout << "10. Load doctors & staff from file\n";
        cout << "0. Back\n";
        int ch = getInt("Choice: ");
        if (ch == 0) break;
        if (ch == 1) {
            string name = getLine("Doctor name: ");
            db.addDoctor(name);
            cout << "Added (or already existed).\n";
        }
        else if (ch == 2) {
            string name = getLine("Doctor name to remove: ");
            if (db.removeDoctor(name)) cout << "Removed.\n"; else cout << "Not found.\n";
        }
        else if (ch == 3) {
            string name = getLine("Doctor name: ");
            string ts = getLine("Time slot to add: ");
            if (db.addAvailability(name, ts)) cout << "Availability added.\n"; else cout << "Doctor not found.\n";
        }
        else if (ch == 4) {
            string name = getLine("Doctor name: ");
            string ts = getLine("Time slot to remove: ");
            if (db.removeAvailability(name, ts)) cout << "Removed.\n"; else cout << "Not found or slot missing.\n";
        }
        else if (ch == 5) {
            auto all = db.listAllDoctors();
            if (all.empty()) cout << "No doctors.\n";
            for (const auto& kv : all) {
                cout << "Doctor: " << kv.first << " -> ";
                for (size_t i = 0; i < kv.second.size(); ++i) {
                    cout << kv.second[i];
                    if (i + 1 < kv.second.size()) cout << ", ";
                }
                cout << '\n';
            }
        }
        else if (ch == 6) {
            string name = getLine("Staff name: ");
            string role = getLine("Role: ");
            db.addStaff(name, role);
            cout << "Staff added.\n";
        }
        else if (ch == 7) {
            string name = getLine("Staff name to remove: ");
            if (db.removeStaff(name)) cout << "Removed.\n"; else cout << "Not found.\n";
        }
        else if (ch == 8) {
            auto s = db.listAllStaff();
            if (s.empty()) cout << "No staff records.\n";
            for (const auto& kv : s) cout << kv.first << " -> " << kv.second << '\n';
        }
        else if (ch == 9) {
            bool ok1 = db.saveDoctors();
            bool ok2 = db.saveStaff();
            cout << "Doctors saved: " << (ok1 ? "OK" : "FAIL") << " | Staff saved: " << (ok2 ? "OK" : "FAIL") << '\n';
        }
        else if (ch == 10) {
            bool ok1 = db.loadDoctors();
            bool ok2 = db.loadStaff();
            cout << "Doctors loaded: " << (ok1 ? "OK" : "FAIL") << " | Staff loaded: " << (ok2 ? "OK" : "FAIL") << '\n';
        }
        else cout << "Invalid option.\n";
    }
}

void reportingMenu(PatientList& plist) {
    while (true) {
        cout << "\n--- Reporting & Analytics ---\n";
        cout << "1. Generate patient report (merge sort)\n";
        cout << "2. Analytics by ailment (BST counts)\n";
        cout << "0. Back\n";
        int ch = getInt("Choice: ");
        if (ch == 0) break;
        if (ch == 1) {
            vector<Patient> v = plist.toVector();
            ReportGenerator::patientReport(v);
        }
        else if (ch == 2) {
            vector<Patient> v = plist.toVector();
            ReportGenerator::analyticsByAilment(v);
        }
        else cout << "Invalid option.\n";
    }
}

// ---------------------------- Main ----------------------------------------
int main() {
    cout << "=== Hospital Management System (Full) ===\n";

    PatientList plist;
    AppointmentManager apptMgr;
    EmergencyManager emergMgr;
    DoctorDB docDB;

    // Auto-load existing files (non-fatal)
    plist.loadFromFile();
    apptMgr.loadFromFile();
    emergMgr.loadFromFile();
    docDB.loadDoctors();
    docDB.loadStaff();

    while (true) {
        showMainMenu();
        int ch = getInt("Enter choice: ");
        if (ch == 0) {
            // Save all before exit
            cout << "Saving data...\n";
            plist.saveToFile();
            apptMgr.saveToFile();
            emergMgr.saveToFile();
            docDB.saveDoctors();
            docDB.saveStaff();
            cout << "Saved. Exiting.\n";
            break;
        }
        if (ch == 1) patientMenu(plist);
        else if (ch == 2) appointmentMenu(apptMgr, plist);
        else if (ch == 3) emergencyMenu(emergMgr, plist);
        else if (ch == 4) doctorMenu(docDB);
        else if (ch == 5) reportingMenu(plist);
        else if (ch == 6) {
            bool ok1 = plist.saveToFile();
            bool ok2 = apptMgr.saveToFile();
            bool ok3 = emergMgr.saveToFile();
            bool ok4 = docDB.saveDoctors();
            bool ok5 = docDB.saveStaff();
            cout << "Save status - Patients: " << (ok1 ? "OK" : "FAIL")
                << " Appointments: " << (ok2 ? "OK" : "FAIL")
                << " Emergencies: " << (ok3 ? "OK" : "FAIL")
                << " Doctors: " << (ok4 ? "OK" : "FAIL")
                << " Staff: " << (ok5 ? "OK" : "FAIL") << '\n';
        }
        else if (ch == 7) {
            bool ok1 = plist.loadFromFile();
            bool ok2 = apptMgr.loadFromFile();
            bool ok3 = emergMgr.loadFromFile();
            bool ok4 = docDB.loadDoctors();
            bool ok5 = docDB.loadStaff();
            cout << "Load status - Patients: " << (ok1 ? "OK" : "FAIL")
                << " Appointments: " << (ok2 ? "OK" : "FAIL")
                << " Emergencies: " << (ok3 ? "OK" : "FAIL")
                << " Doctors: " << (ok4 ? "OK" : "FAIL")
                << " Staff: " << (ok5 ? "OK" : "FAIL") << '\n';
        }
        else cout << "Invalid option.\n";
    }

    return 0;
}