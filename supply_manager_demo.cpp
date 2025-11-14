#include "SupplyStack.hpp"
#include <iostream>
#include <string>
#include <limits>

void displayMenu() {
    std::cout << "\n=== Medical Supply Manager ===" << std::endl;
    std::cout << "1. Add Supply Stock" << std::endl;
    std::cout << "2. Use Last Added Supply" << std::endl;
    std::cout << "3. View Current Supplies" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    SupplyStack supplyStack;
    int choice;
    
    std::cout << "Welcome to the Medical Supply Management System!" << std::endl;
    std::cout << "This system uses a Stack (LIFO) to manage supplies." << std::endl;
    std::cout << "The last added supply will be the first one used.\n" << std::endl;
    
    while (true) {
        displayMenu();
        
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            std::cout << "Invalid input. Please enter a number between 1-4." << std::endl;
            continue;
        }
        clearInputBuffer();
        
        switch (choice) {
            case 1: {
                // Add Supply Stock
                std::string type, batch;
                int quantity;
                
                std::cout << "\n--- Add Supply Stock ---" << std::endl;
                std::cout << "Enter supply type: ";
                std::getline(std::cin, type);
                
                std::cout << "Enter quantity: ";
                if (!(std::cin >> quantity)) {
                    clearInputBuffer();
                    std::cout << "Invalid quantity. Operation cancelled." << std::endl;
                    break;
                }
                clearInputBuffer();
                
                std::cout << "Enter batch number: ";
                std::getline(std::cin, batch);
                
                supplyStack.addSupplyStock(type, quantity, batch);
                std::cout << "Supply added successfully!" << std::endl;
                break;
            }
            
            case 2: {
                // Use Last Added Supply
                std::cout << "\n--- Use Last Added Supply ---" << std::endl;
                Supply usedSupply = supplyStack.useLastAddedSupply();
                
                if (usedSupply.type.empty()) {
                    std::cout << "No supplies available to use. Stack is empty." << std::endl;
                } else {
                    std::cout << "Supply used successfully!" << std::endl;
                    std::cout << "Details:" << std::endl;
                    std::cout << "  Type: " << usedSupply.type << std::endl;
                    std::cout << "  Quantity: " << usedSupply.quantity << std::endl;
                    std::cout << "  Batch: " << usedSupply.batch << std::endl;
                }
                break;
            }
            
            case 3: {
                // View Current Supplies
                supplyStack.viewCurrentSupplies();
                break;
            }
            
            case 4: {
                std::cout << "\nExiting Medical Supply Management System. Goodbye!" << std::endl;
                return 0;
            }
            
            default:
                std::cout << "Invalid choice. Please enter a number between 1-4." << std::endl;
                break;
        }
    }
    
    return 0;
}

