#pragma once
#include "TraversableList.hpp"
#include <stdexcept>

template<typename T>
class LinkedList : public TraversableList<T> {
protected:
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };
public:
    virtual void insert(size_t index, const T& element) override = 0;
    virtual T remove(size_t index) override = 0;
    virtual T& get(size_t index) override = 0;
    virtual void set(size_t index, const T& element) override = 0;
};
