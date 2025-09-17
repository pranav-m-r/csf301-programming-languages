#include <iostream>
#include "FullClass.hpp"  // Implementation view

extern void userDemo();

int main() {
    std::cout << "=== CLASS MODULE (IMPLEMENTATION VIEW) ===\n";
    std::cout << "Implementation module has access to all private members\n\n";
    
    // Test from implementation perspective
    FullClass impl1;
    FullClass impl2(3);
    
    int data[] = {10, 20, 30};
    FullClass impl3(3, data);
    FullClass impl4(impl3);
    
    std::cout << "\nTesting all objects:\n";
    impl1.tell();
    impl2.tell();
    impl3.tell();
    impl4.tell();
    
    std::cout << "\nModifying elements:\n";
    impl1.setNthElement(0, 777);
    impl2.setNthElement(1, 888);
    
    impl1.tell();
    impl2.tell();
    
    // Call user module
    userDemo();
    
    std::cout << "\n=== PROGRAM COMPLETE ===\n";
    return 0;
}