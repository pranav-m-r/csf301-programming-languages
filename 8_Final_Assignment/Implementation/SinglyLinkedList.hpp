#pragma once
/**
 * SinglyLinkedList.hpp - Forward-Only Linked List Implementation
 * 
 * A linked list where each node points only to the next node.
 * 
 * Operations:
 * - push(): O(n) - Append to end (must traverse)
 * - pop(): O(n) - Remove from end (must traverse)
 * - get(index): O(n) - Access by index
 * - insert(0): O(1) - Insert at front
 * - remove(0): O(1) - Remove from front
 * 
 * Inherits from: AbstractList
 */

#include "AbstractList.hpp"
#include <functional>
#include <sstream>

class SinglyLinkedList : public AbstractList {
protected:
    struct Node {
        Value data;
        Node* next;
        Node(const Value& d) : data(d), next(nullptr) {}
    };
    
    Node* head_ = nullptr;
    Node* tail_ = nullptr;  // For O(1) push

public:
    SinglyLinkedList() = default;
    
    ~SinglyLinkedList() override {
        while (head_) {
            Node* tmp = head_;
            head_ = head_->next;
            delete tmp;
        }
        tail_ = nullptr;
    }

    // Copy constructor
    SinglyLinkedList(const SinglyLinkedList& other) {
        Node* cur = other.head_;
        while (cur) {
            push(cur->data);
            cur = cur->next;
        }
    }

    // Move constructor
    SinglyLinkedList(SinglyLinkedList&& other) noexcept {
        head_ = other.head_;
        tail_ = other.tail_;
        length_ = other.length_;
        other.head_ = other.tail_ = nullptr;
        other.length_ = 0;
    }

    // Copy assignment
    SinglyLinkedList& operator=(const SinglyLinkedList& other) {
        if (this != &other) {
            clear();
            Node* cur = other.head_;
            while (cur) {
                push(cur->data);
                cur = cur->next;
            }
        }
        return *this;
    }

    // Move assignment
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = other.head_;
            tail_ = other.tail_;
            length_ = other.length_;
            other.head_ = other.tail_ = nullptr;
            other.length_ = 0;
        }
        return *this;
    }

    // Initializer list
    SinglyLinkedList(std::initializer_list<Value> init) {
        for (const auto& v : init) {
            push(v);
        }
    }

    // ==================== AbstractList Interface ====================
    
    void push(const Value& element) override {
        Node* n = new Node(element);
        if (!tail_) {
            head_ = tail_ = n;
        } else {
            tail_->next = n;
            tail_ = n;
        }
        length_++;
    }

    Value pop() override {
        checkNotEmpty("pop");
        
        if (head_ == tail_) {
            Value val = std::move(head_->data);
            delete head_;
            head_ = tail_ = nullptr;
            length_--;
            return val;
        }
        
        // Find second-to-last
        Node* cur = head_;
        while (cur->next != tail_) {
            cur = cur->next;
        }
        
        Value val = std::move(tail_->data);
        delete tail_;
        tail_ = cur;
        tail_->next = nullptr;
        length_--;
        return val;
    }

    Value& get(int index) override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        Node* cur = head_;
        for (int i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    const Value& get(int index) const override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        Node* cur = head_;
        for (int i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    void insert(int index, const Value& element) override {
        if (index < 0 || index > length_) {
            throw std::out_of_range("Index out of range");
        }
        
        if (index == length_) {
            push(element);
            return;
        }
        
        Node* n = new Node(element);
        if (index == 0) {
            n->next = head_;
            head_ = n;
            if (!tail_) tail_ = n;
        } else {
            Node* cur = head_;
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
        
        if (index == 0) {
            Node* tmp = head_;
            Value val = std::move(tmp->data);
            head_ = head_->next;
            if (!head_) tail_ = nullptr;
            delete tmp;
            length_--;
            return val;
        }
        
        Node* cur = head_;
        for (int i = 0; i < index - 1; ++i) cur = cur->next;
        Node* target = cur->next;
        Value val = std::move(target->data);
        cur->next = target->next;
        if (target == tail_) tail_ = cur;
        delete target;
        length_--;
        return val;
    }

    // ==================== Additional Operations ====================
    
    void set(int index, const Value& element) {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        Node* cur = head_;
        for (int i = 0; i < index; ++i) cur = cur->next;
        cur->data = element;
    }

    int indexOf(const Value& element) const {
        Node* cur = head_;
        int idx = 0;
        while (cur) {
            if (cur->data == element) return idx;
            cur = cur->next;
            ++idx;
        }
        return -1;
    }

    bool contains(const Value& element) const {
        return indexOf(element) != -1;
    }

    void reverse() {
        Node* prev = nullptr;
        Node* cur = head_;
        tail_ = head_;
        
        while (cur) {
            Node* next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        head_ = prev;
    }

    void clear() {
        while (head_) {
            Node* tmp = head_;
            head_ = head_->next;
            delete tmp;
        }
        tail_ = nullptr;
        length_ = 0;
    }

    // ==================== Iterator ====================
    
    class Iterator {
        Node* node_;
    public:
        Iterator(Node* n) : node_(n) {}
        Value& operator*() { return node_->data; }
        Iterator& operator++() { node_ = node_->next; return *this; }
        bool operator!=(const Iterator& other) const { return node_ != other.node_; }
    };

    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }

    // ==================== String Representation ====================
    
    std::string toString() const {
        std::ostringstream oss;
        oss << "[";
        Node* cur = head_;
        bool first = true;
        while (cur) {
            if (!first) oss << ", ";
            oss << cur->data.toString();
            first = false;
            cur = cur->next;
        }
        oss << "]";
        return oss.str();
    }
};
