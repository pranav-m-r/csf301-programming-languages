#pragma once
/**
 * DoublyLinkedList.hpp - Bidirectional Linked List Implementation
 * 
 * A linked list where each node has pointers to both next and previous nodes.
 * 
 * Operations:
 * - push(): O(1) - Append to end
 * - pop(): O(1) - Remove from end
 * - get(index): O(n) - Access by index (optimized: starts from closer end)
 * - insert(index): O(n) - Insert at position
 * - remove(index): O(n) - Remove from position
 * 
 * Inherits from: AbstractList
 */

#include "AbstractList.hpp"
#include <functional>
#include <sstream>

class DoublyLinkedList : public AbstractList {
protected:
    struct Node {
        Value data;
        Node* next;
        Node* prev;
        Node(const Value& d) : data(d), next(nullptr), prev(nullptr) {}
    };
    
    Node* head_ = nullptr;
    Node* tail_ = nullptr;

public:
    DoublyLinkedList() = default;
    
    ~DoublyLinkedList() override {
        while (head_) {
            Node* tmp = head_;
            head_ = head_->next;
            delete tmp;
        }
        tail_ = nullptr;
    }

    // Copy constructor
    DoublyLinkedList(const DoublyLinkedList& other) {
        Node* cur = other.head_;
        while (cur) {
            push(cur->data);
            cur = cur->next;
        }
    }

    // Move constructor
    DoublyLinkedList(DoublyLinkedList&& other) noexcept {
        head_ = other.head_;
        tail_ = other.tail_;
        length_ = other.length_;
        other.head_ = other.tail_ = nullptr;
        other.length_ = 0;
    }

    // Copy assignment
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
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
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
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
    DoublyLinkedList(std::initializer_list<Value> init) {
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
            n->prev = tail_;
            tail_->next = n;
            tail_ = n;
        }
        length_++;
    }

    Value pop() override {
        checkNotEmpty("pop");
        
        Node* target = tail_;
        Value val = std::move(target->data);
        
        if (head_ == tail_) {
            head_ = tail_ = nullptr;
        } else {
            tail_ = tail_->prev;
            tail_->next = nullptr;
        }
        
        delete target;
        length_--;
        return val;
    }

    // Get node at index, optimized to start from closer end
    Node* getNode(int index) const {
        Node* cur;
        if (index < length_ / 2) {
            cur = head_;
            for (int i = 0; i < index; ++i) cur = cur->next;
        } else {
            cur = tail_;
            for (int i = length_ - 1; i > index; --i) cur = cur->prev;
        }
        return cur;
    }

    Value& get(int index) override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        return getNode(index)->data;
    }

    const Value& get(int index) const override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        return getNode(index)->data;
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
            if (head_) head_->prev = n;
            head_ = n;
            if (!tail_) tail_ = n;
        } else {
            Node* cur = getNode(index);
            n->next = cur;
            n->prev = cur->prev;
            cur->prev->next = n;
            cur->prev = n;
        }
        length_++;
    }

    Value remove(int index) override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        
        Node* target = getNode(index);
        Value val = std::move(target->data);
        
        if (target->prev) target->prev->next = target->next;
        else head_ = target->next;
        
        if (target->next) target->next->prev = target->prev;
        else tail_ = target->prev;
        
        delete target;
        length_--;
        return val;
    }

    // ==================== Additional Operations ====================
    
    void pushFront(const Value& element) {
        insert(0, element);
    }

    Value popFront() {
        return remove(0);
    }

    void set(int index, const Value& element) {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        getNode(index)->data = element;
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
        Node* cur = head_;
        while (cur) {
            std::swap(cur->next, cur->prev);
            cur = cur->prev;  // Was next before swap
        }
        std::swap(head_, tail_);
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

    Value& front() {
        checkNotEmpty("front");
        return head_->data;
    }

    Value& back() {
        checkNotEmpty("back");
        return tail_->data;
    }

    // ==================== Iterator ====================
    
    class Iterator {
        Node* node_;
    public:
        Iterator(Node* n) : node_(n) {}
        Value& operator*() { return node_->data; }
        Iterator& operator++() { node_ = node_->next; return *this; }
        Iterator& operator--() { node_ = node_->prev; return *this; }
        bool operator!=(const Iterator& other) const { return node_ != other.node_; }
        bool operator==(const Iterator& other) const { return node_ == other.node_; }
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
