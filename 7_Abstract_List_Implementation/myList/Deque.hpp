#pragma once
#include "Queue.hpp"
#include <stdexcept>

template <typename T>
class Deque : public Queue<T> {
    int front_capacity;
    int back_capacity;

public:
    Deque() : front_capacity(0), back_capacity(0) {}

    void push_front(const T& element) {
        typename Queue<T>::Node* n = new typename Queue<T>::Node(element);
        n->next = this->front_ptr;
        this->front_ptr = n;
        if (!this->back_ptr) this->back_ptr = n;
        this->length++;
    }

    void push_back(const T& element) {
        this->enqueue(element);
    }

    T pop_front() {
        return this->dequeue();
    }

    T pop_back() {
        if (!this->back_ptr) throw std::out_of_range("Deque empty");
        if (this->front_ptr == this->back_ptr) {
            T val = this->back_ptr->data;
            delete this->back_ptr;
            this->front_ptr = this->back_ptr = nullptr;
            this->length--;
            return val;
        }
        auto* cur = this->front_ptr;
        while (cur->next != this->back_ptr) cur = cur->next;
        T val = this->back_ptr->data;
        delete this->back_ptr;
        cur->next = nullptr;
        this->back_ptr = cur;
        this->length--;
        return val;
    }
};
