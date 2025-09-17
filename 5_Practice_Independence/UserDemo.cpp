#include "UserFullClass.hpp"
#include <iostream>

void userDemo() {
    std::cout << "\n=== USER MODULE DEMONSTRATION ===\n";
    std::cout << "User module developed independently\n";
    std::cout << "Cannot see private members: objectId, data, size\n\n";
    
    // Test all public constructors
    std::cout << "1. Default constructor:\n";
    FullClass obj1;
    obj1.tell();
    
    std::cout << "\n2. Constructor with size:\n";
    FullClass obj2(5);
    obj2.tell();
    
    std::cout << "\n3. Constructor with data array:\n";
    int testData[] = {100, 200, 300};
    FullClass obj3(3, testData);
    obj3.tell();
    
    std::cout << "\n4. Copy constructor:\n";
    FullClass obj4(obj3);
    obj4.tell();
    
    std::cout << "\n5. Testing valid public interface:\n";
    obj2.setNthElement(2, 999);
    obj2.tell();
    
    std::cout << "\n=== ATTEMPTING TO ACCESS PRIVATE MEMBERS ===\n";
    std::cout << "The following lines would cause COMPILATION ERRORS:\n\n";
    
    // Uncomment any of these lines to see compilation errors:
    
    // std::cout << "Object ID: " << obj1.objectId << std::endl;

    // obj1.data[0] = 555;

    // int arraySize = obj1.size;

    // std::cout << "First element: " << obj1.data[0] << std::endl;

    // for(int i = 0; i < obj1.size; i++) {
    //     std::cout << obj1.data[i] << " ";
    // }
    
    std::cout << "If you uncomment the lines above, you'll get:\n";
    std::cout << "- error: 'objectId' is a private member of 'FullClass'\n";
    std::cout << "- error: 'data' is a private member of 'FullClass'\n"; 
    std::cout << "- error: 'size' is a private member of 'FullClass'\n";
    
    std::cout << "\nThis proves information hiding is working!\n";
    std::cout << "User can ONLY use public methods - private data is invisible!\n";
}