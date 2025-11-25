#pragma once
/**
 * PriorityQueue.hpp - Priority-Ordered Container (Heap-based)
 * 
 * Elements are ordered by priority using a binary heap.
 * Default is min-heap (smallest element has highest priority).
 * 
 * Operations:
 * - enqueue() / push(): O(log n) - Add element
 * - dequeue() / pop(): O(log n) - Remove highest priority element
 * - front(): O(1) - Peek at highest priority element
 * 
 * Template parameter Comparator determines ordering:
 * - MinComparator: smallest first (min-heap)
 * - MaxComparator: largest first (max-heap)
 * 
 * Inherits from: AbstractList
 */

#include "AbstractList.hpp"
#include <vector>
#include <functional>

// Comparator for min-heap (smallest first)
struct MinComparator {
    bool operator()(const Value& a, const Value& b) const { return a < b; }
};

// Comparator for max-heap (largest first)
struct MaxComparator {
    bool operator()(const Value& a, const Value& b) const { return a > b; }
};

template<typename Comparator = MinComparator>
class PriorityQueue : public AbstractList {
protected:
    std::vector<Value> heap_;
    Comparator comp_;

    void heapifyUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (!comp_(heap_[i], heap_[parent])) break;
            std::swap(heap_[i], heap_[parent]);
            i = parent;
        }
    }

    void heapifyDown(int i) {
        int size = static_cast<int>(heap_.size());
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int best = i;
            
            if (left < size && comp_(heap_[left], heap_[best])) best = left;
            if (right < size && comp_(heap_[right], heap_[best])) best = right;
            
            if (best == i) break;
            std::swap(heap_[i], heap_[best]);
            i = best;
        }
    }

public:
    PriorityQueue() = default;

    // ==================== Priority Queue Operations ====================
    
    void enqueue(const Value& element) {
        heap_.push_back(element);
        length_++;
        heapifyUp(static_cast<int>(heap_.size()) - 1);
    }

    Value dequeue() {
        checkNotEmpty("dequeue");
        Value result = std::move(heap_[0]);
        heap_[0] = std::move(heap_.back());
        heap_.pop_back();
        length_--;
        if (!heap_.empty()) heapifyDown(0);
        return result;
    }

    Value& front() {
        checkNotEmpty("front");
        return heap_[0];
    }

    const Value& front() const {
        checkNotEmpty("front");
        return heap_[0];
    }

    // ==================== AbstractList Interface ====================
    
    void push(const Value& element) override { enqueue(element); }
    Value pop() override { return dequeue(); }

    Value& get(int index) override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        return heap_[index];
    }

    const Value& get(int index) const override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        return heap_[index];
    }

    void insert(int index, const Value& element) override {
        // For priority queue, insert always uses heap ordering
        (void)index;  // Ignore index
        enqueue(element);
    }

    Value remove(int index) override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        if (index == 0) return dequeue();
        
        // Remove arbitrary element - swap with last, remove, then fix heap
        Value result = std::move(heap_[index]);
        heap_[index] = std::move(heap_.back());
        heap_.pop_back();
        length_--;
        
        if (index < static_cast<int>(heap_.size())) {
            heapifyDown(index);
            heapifyUp(index);
        }
        return result;
    }

    // ==================== Utility ====================
    
    void clear() {
        heap_.clear();
        length_ = 0;
    }
};

// Type aliases for convenience
using MinHeap = PriorityQueue<MinComparator>;
using MaxHeap = PriorityQueue<MaxComparator>;
