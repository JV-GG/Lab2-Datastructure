#include "SupplyStack.hpp"
#include <iostream>
#include <iomanip>

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

