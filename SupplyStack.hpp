#ifndef SUPPLYSTACK_HPP
#define SUPPLYSTACK_HPP

#include <string>

// Supply data structure
struct Supply {
    std::string type;
    int quantity;
    std::string batch;
};

// Node structure for linked list implementation
struct Node {
    Supply data;
    Node* next;
    
    Node(const Supply& supply) : data(supply), next(nullptr) {}
};

// Stack class for managing medical supplies
class SupplyStack {
private:
    Node* top;  // Pointer to the top of the stack
    
public:
    // Constructor: Initialize empty stack
    SupplyStack();
    
    // Destructor: Clean up all nodes
    ~SupplyStack();
    
    // Core stack operations
    void push(const Supply& item);  // Add supply to top of stack
    Supply pop();                   // Remove and return top supply
    bool isEmpty() const;           // Check if stack is empty
    Supply peek() const;            // View top item without removing
    
    // Required role functions
    void addSupplyStock(const std::string& type, int quantity, const std::string& batch);
    Supply useLastAddedSupply();    // Returns empty supply if stack is empty
    void viewCurrentSupplies() const;  // Display all supplies from top to bottom

    // Persistence helpers for Medical Supply Manager role
    bool saveToCsv(const std::string& filename) const; // Save current supplies to CSV
    bool loadFromCsv(const std::string& filename);     // Load supplies from CSV (replaces current stack)
};

#endif // SUPPLYSTACK_HPP

