#include "functionality.hpp"
#include <iostream>
#include <string>

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

int main() {
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



