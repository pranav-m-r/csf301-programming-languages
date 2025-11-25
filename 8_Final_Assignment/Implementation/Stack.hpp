#pragma once
/**
 * Stack.hpp - LIFO Container Implementation
 * 
 * Implements a Last-In-First-Out stack using a linked list.
 * 
 * Operations:
 * - push(element): O(1) - Add to top
 * - pop(): O(1) - Remove from top
 * - top(): O(1) - Peek at top
 * - get(index): O(n) - Access by index (0 = top)
 * 
 * Inherits from: AbstractList
 */

#include "AbstractList.hpp"

class Stack : public AbstractList {
private:
    struct Node {
        Value data;
        Node* next;
        Node(const Value& val, Node* nxt = nullptr) : data(val), next(nxt) {}
    };
    
    Node* top_ = nullptr;

public:
    Stack() = default;
    
    ~Stack() override {
        while (top_) {
            Node* tmp = top_;
            top_ = top_->next;
            delete tmp;
        }
    }

    // Copy constructor
    Stack(const Stack& other) : AbstractList() {
        if (other.empty()) return;
        
        // Need to reverse order while copying
        Stack temp;
        Node* cur = other.top_;
        while (cur) {
            temp.push(cur->data);
            cur = cur->next;
        }
        // Now reverse again to get correct order
        while (!temp.empty()) {
            push(temp.pop());
        }
    }

    // Move constructor
    Stack(Stack&& other) noexcept : AbstractList() {
        top_ = other.top_;
        length_ = other.length_;
        other.top_ = nullptr;
        other.length_ = 0;
    }

    // Copy assignment
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            Stack temp(other);
            std::swap(top_, temp.top_);
            std::swap(length_, temp.length_);
        }
        return *this;
    }

    // Move assignment
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            // Clear current
            while (top_) {
                Node* tmp = top_;
                top_ = top_->next;
                delete tmp;
            }
            top_ = other.top_;
            length_ = other.length_;
            other.top_ = nullptr;
            other.length_ = 0;
        }
        return *this;
    }

    // ==================== Stack-Specific Operations ====================
    
    void push(const Value& element) override {
        top_ = new Node(element, top_);
        length_++;
    }

    Value pop() override {
        checkNotEmpty("pop");
        Node* tmp = top_;
        Value val = std::move(tmp->data);
        top_ = top_->next;
        delete tmp;
        length_--;
        return val;
    }

    Value& top() {
        checkNotEmpty("top");
        return top_->data;
    }

    const Value& top() const {
        checkNotEmpty("top");
        return top_->data;
    }

    // ==================== AbstractList Interface ====================
    
    Value& get(int index) override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        Node* cur = top_;
        for (int i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    const Value& get(int index) const override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        Node* cur = top_;
        for (int i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    void insert(int index, const Value& element) override {
        if (index < 0 || index > length_) {
            throw std::out_of_range("Index out of range");
        }
        if (index == 0) {
            push(element);
            return;
        }
        Node* cur = top_;
        for (int i = 0; i < index - 1; ++i) cur = cur->next;
        cur->next = new Node(element, cur->next);
        length_++;
    }

    Value remove(int index) override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        if (index == 0) return pop();
        
        Node* cur = top_;
        for (int i = 0; i < index - 1; ++i) cur = cur->next;
        Node* target = cur->next;
        Value val = std::move(target->data);
        cur->next = target->next;
        delete target;
        length_--;
        return val;
    }

    // ==================== Utility ====================
    
    void clear() {
        while (top_) {
            Node* tmp = top_;
            top_ = top_->next;
            delete tmp;
        }
        length_ = 0;
    }
};
