#include <iostream>
#include <array>

void test_cpp_array_bounds() {
    int a[11][3];
    
    for(int i = 0; i < 11; i++) {
        for(int j = 0; j < 3; j++) {
            a[i][j] = 0;
        }
    }
    
    std::cout << "C++ with C-style arrays:\n";
    std::cout << "Array declared as int a[11][3]\n";
    
    std::cout << "Executing: a[12][1] = 1;\n";
    a[12][1] = 1;
    
    std::cout << "Undefined behavior occurred (same as C)\n";
}

void test_cpp_std_array() {
    std::array<std::array<int, 3>, 11> a;
    
    for(auto& row : a) {
        row.fill(0);
    }
    
    std::cout << "\nC++ with std::array:\n";
    std::cout << "Using std::array<std::array<int, 3>, 11>\n";
    
    try {
        std::cout << "Attempting: a.at(12).at(1) = 1;\n";
        a.at(12).at(1) = 1;
    } catch(const std::out_of_range& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
}

int main() {
    test_cpp_array_bounds();
    test_cpp_std_array();
    return 0;
}