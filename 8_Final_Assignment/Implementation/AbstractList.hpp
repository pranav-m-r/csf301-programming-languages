#pragma once
/**
 * AbstractList.hpp - Root Abstract Class for All List Structures
 * 
 * Defines the minimal interface that ALL list-like containers must implement.
 * This is the foundation of the class hierarchy.
 * 
 * Core Operations:
 * - size(), empty() - Query operations
 * - push(), pop() - Generic add/remove (semantics depend on subclass)
 * - get(), insert(), remove() - Index-based operations
 * 
 * Design Notes:
 * - Uses Value type for heterogeneous storage
 * - Virtual destructor for proper polymorphic deletion
 * - Pure virtual methods enforce implementation in subclasses
 */

#include "Value.hpp"
#include <cstddef>
#include <stdexcept>

class AbstractList {
protected:
    int length_ = 0;

public:
    virtual ~AbstractList() = default;

    // ==================== Query Operations ====================
    
    int size() const { return length_; }
    bool empty() const { return length_ == 0; }

    // ==================== Pure Virtual Operations ====================
    
    /**
     * push - Add element to the container
     * Semantics vary by container type:
     * - Stack: push to top
     * - Queue: enqueue to back
     * - List: append to end
     */
    virtual void push(const Value& element) = 0;

    /**
     * pop - Remove and return element from the container
     * Semantics vary by container type:
     * - Stack: pop from top
     * - Queue: dequeue from front
     * - List: remove from end
     */
    virtual Value pop() = 0;

    /**
     * get - Access element at index
     * @throws std::out_of_range if index >= size()
     */
    virtual Value& get(int index) = 0;
    virtual const Value& get(int index) const = 0;

    /**
     * insert - Insert element at specified index
     * @throws std::out_of_range if index > size()
     */
    virtual void insert(int index, const Value& element) = 0;

    /**
     * remove - Remove and return element at specified index
     * @throws std::out_of_range if index >= size()
     */
    virtual Value remove(int index) = 0;

    // ==================== Convenience Operators ====================
    
    Value& operator[](int index) { return get(index); }
    const Value& operator[](int index) const { return get(index); }

protected:
    // Helper for bounds checking
    void checkIndex(int index, bool allowEnd = false) const {
        int limit = allowEnd ? length_ : length_ - 1;
        if (index < 0 || index > limit || (index == length_ && !allowEnd)) {
            throw std::out_of_range("Index " + std::to_string(index) + 
                                    " out of range [0, " + std::to_string(limit) + "]");
        }
    }

    void checkNotEmpty(const char* operation) const {
        if (empty()) {
            throw std::out_of_range(std::string(operation) + " on empty container");
        }
    }
};
