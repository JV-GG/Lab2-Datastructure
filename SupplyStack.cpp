#include "SupplyStack.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

// Constructor: Initialize empty stack
SupplyStack::SupplyStack() : top(nullptr) {}

// Destructor: Pop all items to prevent memory leaks
SupplyStack::~SupplyStack() {
    while (!isEmpty()) {
        pop();
    }
}

// Check if stack is empty
bool SupplyStack::isEmpty() const {
    return top == nullptr;
}

// Add supply to top of stack
void SupplyStack::push(const Supply& item) {
    Node* newNode = new Node(item);
    newNode->next = top;
    top = newNode;
}

// Remove and return top supply (most recently added)
Supply SupplyStack::pop() {
    if (isEmpty()) {
        // Return empty supply if stack is empty
        Supply emptySupply;
        emptySupply.type = "";
        emptySupply.quantity = 0;
        emptySupply.batch = "";
        return emptySupply;
    }
    
    Node* nodeToRemove = top;
    Supply data = top->data;
    top = top->next;
    delete nodeToRemove;
    return data;
}

// View top item without removing
Supply SupplyStack::peek() const {
    if (isEmpty()) {
        Supply emptySupply;
        emptySupply.type = "";
        emptySupply.quantity = 0;
        emptySupply.batch = "";
        return emptySupply;
    }
    return top->data;
}

// Add Supply Stock: Record a new supply item
void SupplyStack::addSupplyStock(const std::string& type, int quantity, const std::string& batch) {
    Supply newSupply;
    newSupply.type = type;
    newSupply.quantity = quantity;
    newSupply.batch = batch;
    push(newSupply);
}

// Use 'Last Added' Supply: Remove the most recently added supply
Supply SupplyStack::useLastAddedSupply() {
    return pop();
}

// View Current Supplies: Show all supplies from top to bottom
void SupplyStack::viewCurrentSupplies() const {
    if (isEmpty()) {
        std::cout << "\n=== Current Supplies ===" << std::endl;
        std::cout << "No supplies available.\n" << std::endl;
        return;
    }
    
    std::cout << "\n=== Current Supplies (Top to Bottom) ===" << std::endl;
    std::cout << std::left << std::setw(20) << "Type" 
              << std::setw(15) << "Quantity" 
              << std::setw(15) << "Batch" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    // Iterate through stack without removing items
    Node* current = top;
    int position = 1;
    while (current != nullptr) {
        std::cout << position << ". " 
                  << std::setw(17) << current->data.type
                  << std::setw(15) << current->data.quantity
                  << std::setw(15) << current->data.batch << std::endl;
        current = current->next;
        position++;
    }
    std::cout << std::endl;
}

// Save current supplies to a CSV file
bool SupplyStack::saveToCsv(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cout << "Error: Unable to write supplies to file '" << filename << "'.\n";
        return false;
    }

    // CSV header
    outFile << "Position,Type,Quantity,Batch\n";

    Node* current = top;
    int position = 1;
    while (current != nullptr) {
        outFile << position << ","
                << current->data.type << ","
                << current->data.quantity << ","
                << current->data.batch << "\n";
        current = current->next;
        ++position;
    }

    return true;
}

// Load supplies from a CSV file, replacing current stack contents
bool SupplyStack::loadFromCsv(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        // If file doesn't exist, treat as empty inventory but not an error
        return false;
    }

    // Clear existing stack
    while (!isEmpty()) {
        pop();
    }

    std::string line;
    bool firstLine = true;

    // We will reconstruct the stack in the same top-to-bottom order
    // by temporarily storing entries and then pushing them in reverse.
    std::vector<Supply> supplies;

    while (std::getline(inFile, line)) {
        if (firstLine) {
            firstLine = false; // skip header
            continue;
        }
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string positionStr, type, quantityStr, batch;

        std::getline(ss, positionStr, ',');
        std::getline(ss, type, ',');
        std::getline(ss, quantityStr, ',');
        std::getline(ss, batch, ',');

        if (type.empty()) continue;

        int quantity = 0;
        try {
            quantity = std::stoi(quantityStr);
        } catch (...) {
            continue;
        }

        Supply s;
        s.type = type;
        s.quantity = quantity;
        s.batch = batch;
        supplies.push_back(s);
    }

    // Rebuild stack: last element in vector should be pushed first
    for (auto it = supplies.rbegin(); it != supplies.rend(); ++it) {
        push(*it);
    }

    return true;
}

