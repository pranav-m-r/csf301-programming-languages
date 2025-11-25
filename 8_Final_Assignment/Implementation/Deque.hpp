#pragma once
/**
 * Deque.hpp - Double-Ended Queue Implementation
 * 
 * Allows insertion and removal from both ends.
 * 
 * Operations:
 * - push_front(), push_back(): O(1) - Add to either end
 * - pop_front(), pop_back(): O(1)/O(n) - Remove from either end
 * - front(), back(): O(1) - Peek at either end
 * 
 * Note: pop_back() is O(n) due to singly-linked implementation.
 * For O(1) pop_back(), use DoublyLinkedList-based implementation.
 * 
 * Inherits from: Queue
 */

#include "Queue.hpp"

class Deque : public Queue {
public:
    Deque() = default;

    // ==================== Deque-Specific Operations ====================
    
    void push_front(const Value& element) {
        Node* n = new Node(element);
        n->next = front_;
        front_ = n;
        if (!back_) back_ = n;
        length_++;
    }

    void push_back(const Value& element) {
        enqueue(element);
    }

    Value pop_front() {
        return dequeue();
    }

    Value pop_back() {
        checkNotEmpty("pop_back");
        
        if (front_ == back_) {
            Value val = std::move(back_->data);
            delete back_;
            front_ = back_ = nullptr;
            length_--;
            return val;
        }
        
        // Find second-to-last node
        Node* cur = front_;
        while (cur->next != back_) {
            cur = cur->next;
        }
        
        Value val = std::move(back_->data);
        delete back_;
        back_ = cur;
        back_->next = nullptr;
        length_--;
        return val;
    }

    // Convenience aliases
    void push(const Value& element) override { push_back(element); }
    Value pop() override { return pop_front(); }
};
