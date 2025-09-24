#pragma once
#include "AbstractList.hpp"
#include <stdexcept>

template<typename T>
class Queue : public AbstractList<T> {
protected:
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };
    Node* frontPtr = nullptr;
    Node* backPtr = nullptr;
public:
    Queue() = default;
    ~Queue() {
        while (frontPtr) {
            Node* tmp = frontPtr;
            frontPtr = frontPtr->next;
            delete tmp;
        }
        backPtr = nullptr;
        this->length = 0;
    }

    void push(const T& element) override { enqueue(element); }
    T pop() override { return dequeue(); }

    virtual void enqueue(const T& element) {
        Node* n = new Node(element);
        if (!backPtr) frontPtr = backPtr = n;
        else {
            backPtr->next = n;
            backPtr = n;
        }
        this->length++;
    }

    virtual T dequeue() {
        if (!frontPtr) throw std::out_of_range("Queue empty");
        Node* tmp = frontPtr;
        T val = tmp->data;
        frontPtr = frontPtr->next;
        if (!frontPtr) backPtr = nullptr;
        delete tmp;
        this->length--;
        return val;
    }

    virtual T front() {
        if (!frontPtr) throw std::out_of_range("Queue empty");
        return frontPtr->data;
    }

    virtual T back() {
        if (!backPtr) throw std::out_of_range("Queue empty");
        return backPtr->data;
    }
};
