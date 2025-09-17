#include "FullClass.hpp"
#include <iostream>

static int nextId = 1000;

FullClass::FullClass(void) {
    objectId = nextId++;
    size = 10;
    data = new int[size];
    for(int i = 0; i < size; i++) {
        data[i] = 0;
    }
    std::cout << "FullClass[" << objectId << "] created with default constructor\n";
}

FullClass::FullClass(int sz) {
    objectId = nextId++;
    size = sz > 0 ? sz : 1;
    data = new int[size];
    for(int i = 0; i < size; i++) {
        data[i] = i;
    }
    std::cout << "FullClass[" << objectId << "] created with size " << size << "\n";
}

FullClass::FullClass(int sz, int* srcData) {
    objectId = nextId++;
    size = sz > 0 ? sz : 1;
    data = new int[size];
    if(srcData != nullptr) {
        for(int i = 0; i < size; i++) {
            data[i] = srcData[i];
        }
    } else {
        for(int i = 0; i < size; i++) {
            data[i] = -1;
        }
    }
    std::cout << "FullClass[" << objectId << "] created with data array\n";
}

FullClass::FullClass(const FullClass& other) {
    objectId = nextId++;
    size = other.size;
    data = new int[size];
    for(int i = 0; i < size; i++) {
        data[i] = other.data[i];
    }
    std::cout << "FullClass[" << objectId << "] created as copy of [" << other.objectId << "]\n";
}

void FullClass::tell(void) {
    std::cout << "FullClass[" << objectId << "] size=" << size << " data=[";
    for(int i = 0; i < size; i++) {
        std::cout << data[i];
        if(i < size-1) std::cout << ",";
    }
    std::cout << "]\n";
}

void FullClass::setNthElement(int index, int value) {
    if(index >= 0 && index < size) {
        data[index] = value;
        std::cout << "FullClass[" << objectId << "] set element[" << index << "] = " << value << "\n";
    } else {
        std::cout << "FullClass[" << objectId << "] ERROR: index " << index << " out of bounds\n";
    }
}

FullClass::~FullClass(void) {
    std::cout << "FullClass[" << objectId << "] destructor called\n";
    delete[] data;
}