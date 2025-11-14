#include "PatientAdmission.hpp"

// Constructor
PatientQueue::PatientQueue() : front(nullptr), rear(nullptr), size(0), currentFilename("PatientAdmission.csv") {
    // Load existing data from default file on startup
    loadFromFile(currentFilename);
}

// Destructor
PatientQueue::~PatientQueue() {
    // Clear memory without triggering file updates
    Patient* current = front;
    while (current != nullptr) {
        Patient* temp = current;
        current = current->next;
        delete temp;
    }
    front = rear = nullptr;
    size = 0;
}

// Helper function to convert string to uppercase
string PatientQueue::toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Helper function to trim whitespace
string PatientQueue::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

// Functionality 1: Admit Patient (Enqueue)
void PatientQueue::admitPatient(string id, string name, string conditionType) {
    name = toUpperCase(name);
    conditionType = toUpperCase(conditionType);
    
    Patient* newPatient = new Patient(id, name, conditionType);
    
    if (isEmpty()) {
        front = rear = newPatient;
    } else {
        rear->next = newPatient;
        rear = newPatient;
    }
    
    size++;
    cout << "Patient admitted: " << name << " (ID: " << id << ", Condition: " << conditionType << ")" << endl;
    
    // Auto-update file
    saveToFile(currentFilename);
}

// Function: removes earliest admitted patient
bool PatientQueue::dischargePatient() {
    loadFromFile(currentFilename);
    
    if (isEmpty()) {
        cout << "No patients in queue to discharge." << endl;
        return false;
    }
    
    Patient* temp = front;
    cout << "Discharging patient: " << temp->name << " (ID: " << temp->id << ")" << endl;
    
    front = front->next;
    
    if (front == nullptr) {
        rear = nullptr;
    }
    
    delete temp;
    size--;
    
    // Auto-update file
    if (isEmpty()) {
        // If queue is empty, clear the file
        ofstream clearFile(currentFilename);
        clearFile << "Position,Patient ID,Name,Condition Type" << endl;
        clearFile << "No patients in queue" << endl;
        clearFile.close();
        cout << "File '" << currentFilename << "' updated (queue is now empty)." << endl;
    } else {
        saveToFile(currentFilename);
    }
    
    return true;
}

// Load data from CSV file
bool PatientQueue::loadFromFile(string filename) {
    ifstream inFile(filename);
    
    if (!inFile) {
        return false;
    }
    
    // Clear current queue
    while (!isEmpty()) {
        Patient* temp = front;
        front = front->next;
        delete temp;
        size--;
    }
    front = rear = nullptr;
    size = 0;
    
    string line;
    bool firstLine = true;
    
    while (getline(inFile, line)) {
        // Skip header line
        if (firstLine) {
            firstLine = false;
            continue;
        }
        
        // Check for empty queue message
        if (line.find("No patients in queue") != string::npos) {
            break;
        }
        
        // Parse CSV line
        stringstream ss(line);
        string position, id, name, condition;
        
        getline(ss, position, ',');
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, condition, ',');
        
        // Trim whitespace
        id = trim(id);
        name = trim(name);
        condition = trim(condition);
        
        // Add to queue if data is valid
        if (!id.empty() && !name.empty() && !condition.empty()) {
            Patient* newPatient = new Patient(id, name, condition);
            
            if (isEmpty()) {
                front = rear = newPatient;
            } else {
                rear->next = newPatient;
                rear = newPatient;
            }
            size++;
        }
    }
    
    inFile.close();
    return true;
}

// Functionality 3: View Patient Queue
void PatientQueue::viewPatientQueue() {
    loadFromFile(currentFilename);
    
    if (isEmpty()) {
        cout << "No patients waiting for treatment." << endl;
        return;
    }
    
    cout << "\n=== Patient Queue (Total: " << size << ") ===" << endl;
    Patient* current = front;
    int position = 1;
    
    while (current != nullptr) {
        cout << position << ". ID: " << current->id 
             << " | Name: " << current->name 
             << " | Condition: " << current->conditionType << endl;
        current = current->next;
        position++;
    }
    cout << "================================\n" << endl;
}

// Function: Save Patient Queue to File
bool PatientQueue::saveToFile(string filename) {
    if (isEmpty()) {
        return false;
    }
    
    ofstream outFile(filename);
    
    if (!outFile) {
        cout << "Error: Unable to create file!" << endl;
        return false;
    }
    
    // Write CSV header
    outFile << "Position,Patient ID,Name,Condition Type" << endl;
    
    Patient* current = front;
    int position = 1;
    
    while (current != nullptr) {
        outFile << position << "," 
                << current->id << "," 
                << current->name << "," 
                << current->conditionType << endl;
        current = current->next;
        position++;
    }
    
    outFile.close();
    return true;
}

// Check if queue is empty
bool PatientQueue::isEmpty() {
    return front == nullptr;
}

// Get queue size
int PatientQueue::getSize() {
    return size;
}