/**
 * Ambulance Dispatcher Module
 *
 * Implements a simple circular queue to manage ambulance rotations.
 * This module focuses on Role 4 requirements only.
 */

#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <ctime>
#include <cctype>

// Fixed settings
const int MAX_AMBULANCES = 10;
const int DUTY_HOURS = 8;
const std::time_t DUTY_SECONDS = static_cast<std::time_t>(DUTY_HOURS) * 3600;
const int BASE_HOUR = 0; // shifts always start counting from midnight
const char* const SCHEDULE_FILENAME = "ambulance_schedule.csv";

/**
 * Basic data holder for ambulance information.
 */
struct Ambulance {
    std::string id;
    std::string driverName;
};

/**
 * Circular queue implementation dedicated to ambulance scheduling.
 */
class AmbulanceScheduler {
public:
    AmbulanceScheduler()
        : frontIndex(0), count(0), currentStartDate(todayAtMidnight()), nextId(1) {}

    /**
     * Adds a new ambulance to the active duty rotation.
     * Returns the assigned ambulance ID, or an empty string if the queue is full.
     */
    std::string registerAmbulance(const Ambulance& ambulance) {
        if (isFull()) {
            return "";
        }

        int insertionIndex = (frontIndex + count) % MAX_AMBULANCES;
        queue[insertionIndex] = ambulance;
        if (queue[insertionIndex].id.empty()) {
            queue[insertionIndex].id = formatAmbulanceId(nextId++);
        }
        ++count;
        if (count == 1) {
            currentStartDate = todayAtMidnight();
        }
        return queue[insertionIndex].id;
    }

    /**
     * Rotates the queue so the next ambulance takes the upcoming shift.
     * Returns false if there are fewer than two ambulances to rotate.
     * Also advances the schedule start time by one duty block.
     */
    bool rotateShift() {
        if (count <= 1) {
            return false;
        }

        Ambulance completedShift;
        dequeue(completedShift);
        registerAmbulance(completedShift);
        currentStartDate += DUTY_SECONDS;
        return true;
    }

    /**
     * Displays the current rotation order in a readable table.
     * Shows the fixed duty duration for clarity.
     */
    void displaySchedule() const {
        if (isEmpty()) {
            std::cout << "\nNo ambulances registered yet.\n";
            return;
        }

        std::cout << "\nCurrent Ambulance Rotation (each shift: "
                  << DUTY_HOURS << " hours)\n";

        int currentIndex = frontIndex;
        std::cout << "Current duty ambulance: Ambulance "
                  << queue[currentIndex].id << " ("
                  << queue[currentIndex].driverName << ")\n";

        if (count >= 2) {
            int nextIndex = (frontIndex + 1) % MAX_AMBULANCES;
            std::cout << "Next duty ambulance: Ambulance "
                      << queue[nextIndex].id << " ("
                      << queue[nextIndex].driverName << ")\n";
        } else if (count == 1) {
            std::cout << "No standby ambulances. Only one ambulance in rotation.\n";
        }

        std::cout << std::left
                  << std::setw(10) << "Position"
                  << std::setw(15) << "Ambulance ID"
                  << std::setw(20) << "Driver"
                  << std::setw(16) << "Duty Status"
                  << std::setw(20) << "Start Time"
                  << std::setw(20) << "End Time" << '\n';
        std::cout << std::string(101, '-') << '\n';

        for (int i = 0; i < count; ++i) {
            int index = (frontIndex + i) % MAX_AMBULANCES;
            std::time_t slotTime = currentStartDate + (static_cast<std::time_t>(i) * DUTY_SECONDS);
            std::string timeLabel = "N/A";
            std::string endTimeLabel = "N/A";

            if (std::tm* startPtr = std::localtime(&slotTime)) {
                std::tm startTime = *startPtr;
                char buffer[32];
                if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &startTime)) {
                    timeLabel = buffer;
                }

                std::tm endTime = startTime;
                endTime.tm_hour += DUTY_HOURS;
                std::mktime(&endTime); // normalize date rollovers
                if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &endTime)) {
                    endTimeLabel = buffer;
                }
            }

            std::string dutyStatus = (i == 0) ? "In Duty" : "Not in Duty";

            std::cout << std::setw(10) << (i + 1)
                      << std::setw(15) << queue[index].id
                      << std::setw(20) << queue[index].driverName
                      << std::setw(16) << dutyStatus
                      << std::setw(20) << timeLabel
                      << std::setw(20) << endTimeLabel << '\n';
        }
    }

    /**
     * Writes the current schedule to a CSV file on disk.
     * Returns false if the queue is empty or the file cannot be opened.
     */
    bool saveScheduleToCsv(const std::string& filename) const {
        std::ofstream outFile(filename.c_str());
        if (!outFile) {
            return false;
        }

        outFile << "Position,Ambulance ID,Driver,Duty Status,Start Time,End Time\n";

        for (int i = 0; i < count; ++i) {
            int index = (frontIndex + i) % MAX_AMBULANCES;
            std::time_t slotTime = currentStartDate + (static_cast<std::time_t>(i) * DUTY_SECONDS);
            std::string startLabel = "N/A";
            std::string endLabel = "N/A";

            if (std::tm* startPtr = std::localtime(&slotTime)) {
                std::tm startTime = *startPtr;
                char buffer[32];
                if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &startTime)) {
                    startLabel = buffer;
                }

                std::tm endTime = startTime;
                endTime.tm_hour += DUTY_HOURS;
                std::mktime(&endTime);
                if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &endTime)) {
                    endLabel = buffer;
                }
            }

            outFile << (i + 1) << ','
                    << queue[index].id << ','
                    << queue[index].driverName << ','
                    << ((i == 0) ? "In Duty" : "Not in Duty") << ','
                    << startLabel << ','
                    << endLabel << '\n';
        }

        return true;
    }

    /**
     * Loads a schedule from a CSV file, replacing the current queue contents.
     * Returns false if the file cannot be opened or no valid rows are found.
     */
    bool loadScheduleFromCsv(const std::string& filename) {
        std::ifstream inFile(filename.c_str());
        if (!inFile) {
            std::ofstream newFile(filename.c_str());
            if (!newFile) {
                return false;
            }
            newFile << "Position,Ambulance ID,Driver,Duty Status,Start Time,End Time\n";
            resetScheduleState();
            return true;
        }

        Ambulance tempQueue[MAX_AMBULANCES];
        int tempCount = 0;
        std::time_t tempStartDate = todayAtMidnight();
        int highestId = 0;
        bool firstRow = true;

        std::string line;
        if (!std::getline(inFile, line)) {
            resetScheduleState();
            return true;
        }

        while (std::getline(inFile, line)) {
            if (line.empty()) {
                continue;
            }

            std::string columns[6];
            if (!splitCsvLine(line, columns, 6)) {
                continue;
            }

            Ambulance ambulance{};
            int numericId = 0;

            if (!columns[1].empty()) {
                numericId = extractNumericId(columns[1]);
            }

            if (numericId <= 0) {
                numericId = highestId + 1;
            }

            ambulance.id = formatAmbulanceId(numericId);
            if (numericId > highestId) {
                highestId = numericId;
            }

            ambulance.driverName = columns[2];

            if (ambulance.driverName.empty()) {
                continue;
            }

            if (firstRow) {
                std::time_t parsedStart;
                if (parseDateTime(columns[4], parsedStart)) {
                    tempStartDate = parsedStart;
                }
                firstRow = false;
            }

            if (tempCount >= MAX_AMBULANCES) {
                break;
            }

            tempQueue[tempCount++] = ambulance;
        }

        resetScheduleState();
        currentStartDate = tempStartDate;
        nextId = (highestId >= 1) ? (highestId + 1) : 1;

        for (int i = 0; i < tempCount; ++i) {
            queue[i] = tempQueue[i];
        }
        count = tempCount;

        return true;
    }

private:
    Ambulance queue[MAX_AMBULANCES];
    int frontIndex;
    int count;
    std::time_t currentStartDate; // midnight of the scheduling day
    int nextId;

    /**
     * Resets the scheduler to an empty state with default timing and IDs.
     */
    void resetScheduleState() {
        frontIndex = 0;
        count = 0;
        currentStartDate = todayAtMidnight();
        nextId = 1;
    }

    /**
     * Splits a CSV line into the expected number of columns (no quoted commas).
     */
    bool splitCsvLine(const std::string& line, std::string* columns, int expectedColumns) const {
        int col = 0;
        std::size_t start = 0;

        while (col < expectedColumns - 1) {
            std::size_t commaPos = line.find(',', start);
            if (commaPos == std::string::npos) {
                return false;
            }
            columns[col++] = line.substr(start, commaPos - start);
            start = commaPos + 1;
        }

        columns[col] = line.substr(start);
        return true;
    }

    /**
     * Extracts the numeric portion from an ambulance ID like "A01".
     */
    int extractNumericId(const std::string& id) const {
        if (id.size() < 2 || id[0] != 'A') {
            return 0;
        }

        int value = 0;
        for (std::size_t i = 1; i < id.size(); ++i) {
            unsigned char ch = static_cast<unsigned char>(id[i]);
            if (!std::isdigit(ch)) {
                return 0;
            }
            value = value * 10 + (id[i] - '0');
        }
        return value;
    }

    /**
     * Parses a datetime string formatted as "YYYY-MM-DD HH:MM".
     */
    bool parseDateTime(const std::string& text, std::time_t& result) const {
        if (text.size() < 16) {
            return false;
        }

        int year = parseNumber(text, 0, 4);
        int month = parseNumber(text, 5, 2);
        int day = parseNumber(text, 8, 2);
        int hour = parseNumber(text, 11, 2);
        int minute = parseNumber(text, 14, 2);

        if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31 ||
            hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            return false;
        }

        std::tm tmValue{};
        tmValue.tm_year = year - 1900;
        tmValue.tm_mon = month - 1;
        tmValue.tm_mday = day;
        tmValue.tm_hour = hour;
        tmValue.tm_min = minute;
        tmValue.tm_sec = 0;
        tmValue.tm_isdst = -1;

        std::time_t converted = std::mktime(&tmValue);
        if (converted == static_cast<std::time_t>(-1)) {
            return false;
        }

        result = converted;
        return true;
    }

    /**
     * Parses a substring of digits into an integer, returning -1 if invalid.
     */
    int parseNumber(const std::string& text, std::size_t start, std::size_t length) const {
        if (start + length > text.size()) {
            return -1;
        }

        int value = 0;
        for (std::size_t i = 0; i < length; ++i) {
            unsigned char ch = static_cast<unsigned char>(text[start + i]);
            if (!std::isdigit(ch)) {
                return -1;
            }
            value = value * 10 + (ch - '0');
        }
        return value;
    }

    /**
     * Helper to compute today's date at 00:00:00 local time.
     */
    std::time_t todayAtMidnight() const {
        std::time_t now = std::time(nullptr);
        std::tm local = *std::localtime(&now);
        local.tm_hour = BASE_HOUR;
        local.tm_min = 0;
        local.tm_sec = 0;
        return std::mktime(&local);
    }
    /**
     * Formats the numeric ambulance counter into ID style "A01", "A02", etc.
     */
    std::string formatAmbulanceId(int number) const {
        std::ostringstream oss;
        oss << 'A' << std::setfill('0') << std::setw(2) << number;
        return oss.str();
    }
    /**
     * Removes the ambulance at the front of the queue.
     * Returns false when the queue is empty.
     */
    bool dequeue(Ambulance& removed) {
        if (isEmpty()) {
            return false;
        }

        removed = queue[frontIndex];
        frontIndex = (frontIndex + 1) % MAX_AMBULANCES;
        --count;
        return true;
    }

    /**
     * Checks if the queue already holds the maximum number of ambulances.
     */
    bool isFull() const {
        return count == MAX_AMBULANCES;
    }

    /**
     * Checks if the queue currently holds no ambulances.
     */
    bool isEmpty() const {
        return count == 0;
    }
};

/**
 * Utility to safely obtain a line of input after numeric reads.
 */
void discardLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * Collects ambulance details from the user and pushes them into the scheduler.
 * Returns the assigned ambulance ID, or an empty string when registration fails.
 */
std::string promptRegisterAmbulance(AmbulanceScheduler& scheduler) {
    Ambulance ambulance{};

    std::cout << "Enter driver name: ";
    std::getline(std::cin, ambulance.driverName);

    std::string assignedId = scheduler.registerAmbulance(ambulance);
    if (!assignedId.empty()) {
        std::cout << "Assigned ambulance ID: " << assignedId << '\n';
    }
    return assignedId;
}

/**
 * Presents the menu options to the dispatcher.
 */
void printMenu() {
    std::cout << "\nAmbulance Dispatcher Menu\n"
              << "1. Register ambulance\n"
              << "2. Rotate ambulance shift\n"
              << "3. Display ambulance schedule\n"
              << "4. Exit\n"
              << "Choose an option: ";
}

/**
 * Runs the ambulance dispatcher module.
 * This function contains the original main() logic.
 */
int runAmbulanceDispatcher() {
    AmbulanceScheduler scheduler;
    const std::string scheduleFilename = SCHEDULE_FILENAME;

    if (!scheduler.loadScheduleFromCsv(scheduleFilename)) {
        std::cout << "Warning: Unable to initialize schedule file '"
                  << scheduleFilename << "'.\n";
    }

    bool running = true;

    while (running) {
        printMenu();

        int choice = 0;
        if (!(std::cin >> choice)) {
            std::cout << "\nInvalid input. Please enter a number from 1 to 4.\n";
            std::cin.clear();
            discardLine();
            continue;
        }
        discardLine();

        switch (choice) {
            case 1: {
                std::string newId = promptRegisterAmbulance(scheduler);
                if (!newId.empty()) {
                    std::cout << "\nAmbulance registered successfully.\n";
                    if (!scheduler.saveScheduleToCsv(scheduleFilename)) {
                        std::cout << "Warning: Failed to update schedule file.\n";
                    }
                } else {
                    std::cout << "\nUnable to register ambulance. Queue is full.\n";
                }
                break;
            }
            case 2: {
                if (scheduler.rotateShift()) {
                    std::cout << "\nShift rotation completed. Next ambulance is on duty.\n";
                    if (!scheduler.saveScheduleToCsv(scheduleFilename)) {
                        std::cout << "Warning: Failed to update schedule file.\n";
                    }
                } else {
                    std::cout << "\nNeed at least two ambulances to rotate shifts.\n";
                }
                break;
            }
            case 3: {
                scheduler.displaySchedule();
                break;
            }
            case 4: {
                running = false;
                std::cout << "\nExiting dispatcher module. Goodbye!\n";
                break;
            }
            default:
                std::cout << "\nUnknown option. Please choose between 1 and 4.\n";
                break;
        }
    }

    return 0;
}

// Original main() function - kept for standalone compilation
// Uncomment this and comment the main() in main.cpp to use standalone mode
/*
int main() {
    return runAmbulanceDispatcher();
}
*/

