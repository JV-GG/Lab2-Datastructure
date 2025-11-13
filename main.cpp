#include "functionality.hpp"
#include <iostream>
#include <string>
#include <limits>

// Forward declaration for ambulance dispatcher module
int runAmbulanceDispatcher();

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
	std::cout << "\n=== Central System Menu ===\n";
	std::cout << "1. Emergency Department Officer\n";
	std::cout << "2. Ambulance Dispatcher\n";
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
		int choice = readIntInRange("Select a module: ", 0, 2);
		switch (choice) {
			case 1:
				runEmergencyDepartmentOfficer();
				break;
			case 2:
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



