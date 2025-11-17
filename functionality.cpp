#include "functionality.hpp"
#include <iostream>

bool EmergencyCaseComparator::operator()(const EmergencyCase &a, const EmergencyCase &b) const {
	if (a.priority != b.priority) return a.priority < b.priority; // lower priority goes after
	return a.id > b.id; // later arrivals go after earlier ones
}

EmergencyDepartmentSystem::EmergencyDepartmentSystem() : nextId(1) {}

void EmergencyDepartmentSystem::logEmergencyCase(const std::string &patientName, const std::string &emergencyType, int priority) {
	EmergencyCase c{nextId++, patientName, emergencyType, priority};
	cases.push(c);
	std::cout << "Case logged: [ID " << c.id << "] " << c.patientName
		  << " | Type: " << c.emergencyType << " | Priority: " << c.priority << "\n";
}

bool EmergencyDepartmentSystem::processMostCriticalCase() {
	if (cases.empty()) {
		std::cout << "No pending emergency cases.\n";
		return false;
	}
	EmergencyCase c = cases.top();
	cases.pop();
	std::cout << "Processing most critical case -> [ID " << c.id << "] "
		  << c.patientName << " | Type: " << c.emergencyType
		  << " | Priority: " << c.priority << "\n";
	return true;
}

void EmergencyDepartmentSystem::viewPendingCases() const {
	if (cases.empty()) {
		std::cout << "No pending emergency cases.\n";
		return;
	}
	std::priority_queue<EmergencyCase, std::deque<EmergencyCase>, EmergencyCaseComparator> temp = cases;
	std::cout << "Pending Emergency Cases (highest priority first):\n";
	while (!temp.empty()) {
		const EmergencyCase &c = temp.top();
		std::cout << "  [ID " << c.id << "] " << c.patientName
			  << " | Type: " << c.emergencyType
			  << " | Priority: " << c.priority << "\n";
		temp.pop();
	}
}



