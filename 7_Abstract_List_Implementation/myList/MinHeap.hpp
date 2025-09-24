#pragma once
#include "PriorityQueue.hpp"

template <typename T>
struct MinComparator {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <typename T>
class MinHeap : public PriorityQueue<T, MinComparator<T>> {
public:
    T get_min() { return this->front(); }
    T extract_min() { return this->dequeue(); }
};
