#pragma once

#include <queue>
#include <string>
#include <vector>

struct EmergencyCase {
	int id;
	std::string patientName;
	std::string emergencyType;
	int priority; // higher number = more critical
};

struct EmergencyCaseComparator {
	bool operator()(const EmergencyCase &a, const EmergencyCase &b) const;
};

class EmergencyDepartmentSystem {
public:
	EmergencyDepartmentSystem();

	void logEmergencyCase(const std::string &patientName, const std::string &emergencyType, int priority);
	bool processMostCriticalCase();
	void viewPendingCases() const;

private:
	int nextId;
	std::priority_queue<EmergencyCase, std::vector<EmergencyCase>, EmergencyCaseComparator> cases;
};



