#pragma once
/**
 * Queue.hpp - FIFO Container Implementation
 * 
 * Implements a First-In-First-Out queue using a linked list.
 * 
 * Operations:
 * - enqueue(element) / push(): O(1) - Add to back
 * - dequeue() / pop(): O(1) - Remove from front
 * - front(): O(1) - Peek at front
 * - back(): O(1) - Peek at back
 * - get(index): O(n) - Access by index (0 = front)
 * 
 * Inherits from: AbstractList
 */

#include "AbstractList.hpp"

class Queue : public AbstractList {
protected:
    struct Node {
        Value data;
        Node* next;
        Node(const Value& d) : data(d), next(nullptr) {}
    };
    
    Node* front_ = nullptr;
    Node* back_ = nullptr;

public:
    Queue() = default;
    
    ~Queue() override {
        while (front_) {
            Node* tmp = front_;
            front_ = front_->next;
            delete tmp;
        }
        back_ = nullptr;
    }

    // Copy constructor
    Queue(const Queue& other) : AbstractList() {
        Node* cur = other.front_;
        while (cur) {
            enqueue(cur->data);
            cur = cur->next;
        }
    }

    // Move constructor
    Queue(Queue&& other) noexcept : AbstractList() {
        front_ = other.front_;
        back_ = other.back_;
        length_ = other.length_;
        other.front_ = other.back_ = nullptr;
        other.length_ = 0;
    }

    // Copy assignment
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            Queue temp(other);
            std::swap(front_, temp.front_);
            std::swap(back_, temp.back_);
            std::swap(length_, temp.length_);
        }
        return *this;
    }

    // Move assignment
    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            while (front_) {
                Node* tmp = front_;
                front_ = front_->next;
                delete tmp;
            }
            front_ = other.front_;
            back_ = other.back_;
            length_ = other.length_;
            other.front_ = other.back_ = nullptr;
            other.length_ = 0;
        }
        return *this;
    }

    // ==================== Queue-Specific Operations ====================
    
    void enqueue(const Value& element) {
        Node* n = new Node(element);
        if (!back_) {
            front_ = back_ = n;
        } else {
            back_->next = n;
            back_ = n;
        }
        length_++;
    }

    Value dequeue() {
        checkNotEmpty("dequeue");
        Node* tmp = front_;
        Value val = std::move(tmp->data);
        front_ = front_->next;
        if (!front_) back_ = nullptr;
        delete tmp;
        length_--;
        return val;
    }

    Value& front() {
        checkNotEmpty("front");
        return front_->data;
    }

    const Value& front() const {
        checkNotEmpty("front");
        return front_->data;
    }

    Value& back() {
        checkNotEmpty("back");
        return back_->data;
    }

    const Value& back() const {
        checkNotEmpty("back");
        return back_->data;
    }

    // ==================== AbstractList Interface ====================
    
    void push(const Value& element) override { enqueue(element); }
    Value pop() override { return dequeue(); }

    Value& get(int index) override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        Node* cur = front_;
        for (int i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    const Value& get(int index) const override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        Node* cur = front_;
        for (int i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    void insert(int index, const Value& element) override {
        if (index < 0 || index > length_) {
            throw std::out_of_range("Index out of range");
        }
        if (index == length_) {
            enqueue(element);
            return;
        }
        Node* n = new Node(element);
        if (index == 0) {
            n->next = front_;
            front_ = n;
            if (!back_) back_ = n;
        } else {
            Node* cur = front_;
            for (int i = 0; i < index - 1; ++i) cur = cur->next;
            n->next = cur->next;
            cur->next = n;
        }
        length_++;
    }

    Value remove(int index) override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        if (index == 0) return dequeue();
        
        Node* cur = front_;
        for (int i = 0; i < index - 1; ++i) cur = cur->next;
        Node* target = cur->next;
        Value val = std::move(target->data);
        cur->next = target->next;
        if (target == back_) back_ = cur;
        delete target;
        length_--;
        return val;
    }

    // ==================== Utility ====================
    
    void clear() {
        while (front_) {
            Node* tmp = front_;
            front_ = front_->next;
            delete tmp;
        }
        back_ = nullptr;
        length_ = 0;
    }
};
