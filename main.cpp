#include "PatientAdmission.hpp"
#include <limits>

void displayMenu() {
    cout << "\n========================================" << endl;
    cout << "   PATIENT ADMISSION CLERK SYSTEM" << endl;
    cout << "========================================" << endl;
    cout << "1. Admit Patient" << endl;
    cout << "2. Discharge Patient" << endl;
    cout << "3. View Patient Queue" << endl;
    cout << "4. Exit" << endl;
    cout << "========================================" << endl;
    cout << "Enter your choice: ";
}

void admitPatientInput(PatientQueue& hospital) {
    string id, name, conditionType;
    
    cout << "\n--- Admit New Patient ---" << endl;
    
    cin.ignore();
    
    cout << "Enter Patient ID: ";
    getline(cin, id);
    
    cout << "Enter Patient Name: ";
    getline(cin, name);
    
    cout << "Enter Condition Type: ";
    getline(cin, conditionType);
    
    hospital.admitPatient(id, name, conditionType);
}

int main() {
    PatientQueue hospital;
    int choice;
    
    do {
        displayMenu();
        
        while (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        switch (choice) {
            case 1:
                admitPatientInput(hospital);
                break;
                
            case 2:
                hospital.dischargePatient();
                break;
                
            case 3:
                hospital.viewPatientQueue();
                break;
                
            case 4:
                cout << "\nExiting system. Thank you!" << endl;
                break;
                
            default:
                cout << "\nInvalid choice! Please select 1-4." << endl;
        }
        
    } while (choice != 4);
    
    return 0;
}