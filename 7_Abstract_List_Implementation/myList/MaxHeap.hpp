#pragma once
#include "PriorityQueue.hpp"

template <typename T>
struct MaxComparator {
    bool operator()(const T& a, const T& b) const { return a > b; }
};

template <typename T>
class MaxHeap : public PriorityQueue<T, MaxComparator<T>> {
public:
    T get_max() { return this->front(); }
    T extract_max() { return this->dequeue(); }
};
