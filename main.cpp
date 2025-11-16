#include "functionality.hpp"
#include "PatientAdmission.hpp"
#include "SupplyStack.hpp"
#include <iostream>
#include <string>
#include <limits>

// Forward declarations for other role modules
int runAmbulanceDispatcher();
int runPatientAdmissionClerk();
int runMedicalSupplyManager();

static int readIntInRange(const std::string &prompt, int minVal, int maxVal) {
	while (true) {
		std::cout << prompt;
		std::string line;
		if (!std::getline(std::cin, line)) {
			return minVal; // fallback on EOF
		}
		try {
			int val = std::stoi(line);
			if (val < minVal || val > maxVal) {
				std::cout << "Please enter a number between " << minVal << " and " << maxVal << ".\n";
				continue;
			}
			return val;
		} catch (...) {
			std::cout << "Invalid number. Try again.\n";
		}
	}
}

static std::string readNonEmptyLine(const std::string &prompt) {
	while (true) {
		std::cout << prompt;
		std::string line;
		if (!std::getline(std::cin, line)) return "";
		size_t start = line.find_first_not_of(" \t\r\n");
		size_t end = line.find_last_not_of(" \t\r\n");
		std::string trimmed = (start == std::string::npos) ? std::string() : line.substr(start, end - start + 1);
		if (!trimmed.empty()) return trimmed;
		std::cout << "Input cannot be empty. Try again.\n";
	}
}

static void showMenu() {
	std::cout << "\n=== Emergency Department Officer Menu ===\n";
	std::cout << "1. Log Emergency Case\n";
	std::cout << "2. Process Most Critical Case\n";
	std::cout << "3. View Pending Emergency Cases\n";
	std::cout << "0. Exit\n";
}

/**
 * Runs the patient admission clerk module.
 * Integrates the PatientQueue role with its own submenu.
 */
int runPatientAdmissionClerk() {
	PatientQueue queue;
	while (true) {
		std::cout << "\n=== Patient Admission Clerk Menu ===\n";
		std::cout << "1. Admit patient\n";
		std::cout << "2. Discharge earliest admitted patient\n";
		std::cout << "3. View patient queue\n";
		std::cout << "0. Return to central menu\n";

		int choice = readIntInRange("Select an option: ", 0, 3);
		switch (choice) {
			case 1: {
				std::string id = readNonEmptyLine("Enter patient ID: ");
				std::string name = readNonEmptyLine("Enter patient name: ");
				std::string condition = readNonEmptyLine("Enter condition type: ");
				queue.admitPatient(id, name, condition);
				break;
			}
			case 2: {
				queue.dischargePatient();
				break;
			}
			case 3: {
				queue.viewPatientQueue();
				break;
			}
			case 0:
				std::cout << "Returning to central menu...\n";
				return 0;
		}
	}
}

/**
 * Runs the medical supply manager module.
 * Integrates the SupplyStack role with its own submenu.
 */
int runMedicalSupplyManager() {
	SupplyStack stack;
	const std::string csvFilename = "data/MedicalSupplies.csv";

	// Attempt to load existing supplies from CSV
	if (stack.loadFromCsv(csvFilename)) {
		std::cout << "Loaded existing supplies from '" << csvFilename << "'.\n";
	} else {
		std::cout << "No existing supplies file found. Starting with empty inventory.\n";
	}
	while (true) {
		std::cout << "\n=== Medical Supply Manager Menu ===\n";
		std::cout << "1. Add supply stock\n";
		std::cout << "2. Use last added supply\n";
		std::cout << "3. View current supplies\n";
		std::cout << "0. Return to central menu\n";

		int choice = readIntInRange("Select an option: ", 0, 3);
		switch (choice) {
			case 1: {
				std::string type = readNonEmptyLine("Enter supply type: ");
				int quantity = readIntInRange("Enter quantity: ", 1, std::numeric_limits<int>::max());
				std::string batch = readNonEmptyLine("Enter batch identifier: ");
				stack.addSupplyStock(type, quantity, batch);
				std::cout << "Supply stock added.\n";
				stack.saveToCsv(csvFilename);
				break;
			}
			case 2: {
				Supply used = stack.useLastAddedSupply();
				if (used.type.empty() && used.quantity == 0 && used.batch.empty()) {
					std::cout << "No supplies available to use.\n";
				} else {
					std::cout << "Using supply -> Type: " << used.type
					          << " | Quantity: " << used.quantity
					          << " | Batch: " << used.batch << "\n";
					stack.saveToCsv(csvFilename);
				}
				break;
			}
			case 3: {
				stack.viewCurrentSupplies();
				break;
			}
			case 0:
				std::cout << "Returning to central menu...\n";
				return 0;
		}
	}
}

/**
 * Runs the emergency department officer module.
 * This function contains the original main() logic.
 */
int runEmergencyDepartmentOfficer() {
	EmergencyDepartmentSystem system;
	while (true) {
		showMenu();
		int choice = readIntInRange("Select an option: ", 0, 3);
		switch (choice) {
			case 1: {
				std::string name = readNonEmptyLine("Enter patient name: ");
				std::string type = readNonEmptyLine("Enter type of emergency: ");
				int priority = readIntInRange("Enter priority (1=low, 5=critical): ", 1, 5);
				system.logEmergencyCase(name, type, priority);
				break;
			}
			case 2:
				system.processMostCriticalCase();
				break;
			case 3:
				system.viewPendingCases();
				break;
			case 0:
				std::cout << "Goodbye!\n";
				return 0;
		}
	}
}

/**
 * Displays the central integrated menu.
 */
static void showCentralMenu() {
	std::cout << "\n=== Hospital Patient Care Management System ===\n";
	std::cout << "1. Patient Admission Clerk\n";
	std::cout << "2. Medical Supply Manager\n";
	std::cout << "3. Emergency Department Officer\n";
	std::cout << "4. Ambulance Dispatcher\n";
	std::cout << "0. Exit\n";
}

// Original main() function - kept for standalone compilation
// Uncomment this and comment the integrated main() below to use standalone mode
/*
int main() {
	return runEmergencyDepartmentOfficer();
}
*/

// Integrated main() function with central menu
int main() {
	while (true) {
		showCentralMenu();
		int choice = readIntInRange("Select a module: ", 0, 4);
		switch (choice) {
			case 1:
				runPatientAdmissionClerk();
				break;
			case 2:
				runMedicalSupplyManager();
				break;
			case 3:
				runEmergencyDepartmentOfficer();
				break;
			case 4:
				runAmbulanceDispatcher();
				break;
			case 0:
				std::cout << "Exiting system. Goodbye!\n";
				return 0;
			default:
				std::cout << "Invalid option. Please try again.\n";
				break;
		}
	}
}



