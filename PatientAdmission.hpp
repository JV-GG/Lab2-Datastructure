#ifndef PATIENTADMISSION_HPP
#define PATIENTADMISSION_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

// Patient structure
struct Patient {
    string id;
    string name;
    string conditionType;
    Patient* next;
    
    Patient(string patientId, string patientName, string condition) 
        : id(patientId), name(patientName), conditionType(condition), next(nullptr) {}
};

// Queue class for Patient management
class PatientQueue {
private:
    Patient* front;
    Patient* rear;
    int size;
    string currentFilename;
    
    string toUpperCase(string str);
    string trim(const string& str);

public:
    PatientQueue();
    ~PatientQueue();
    
    void admitPatient(string id, string name, string conditionType);
    bool dischargePatient();
    void viewPatientQueue();
    bool saveToFile(string filename);
    bool loadFromFile(string filename);
    
    bool isEmpty();
    int getSize();
};

#endif