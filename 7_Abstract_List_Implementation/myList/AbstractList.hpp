#pragma once
#include <cstddef>

template<typename T>
class AbstractList {
protected:
    int length = 0;
public:
    using Iterator = T*;
    virtual ~AbstractList() = default;
    int size() const { return length; }
    bool empty() const { return length == 0; }
    virtual void push(const T& element) = 0;
    virtual T pop() = 0;
    virtual T& get(size_t index) = 0;
    virtual void insert(size_t index, const T& element) = 0;
    virtual T remove(size_t index) = 0;
};
