#pragma once
#include "AbstractList.hpp"

template<typename T>
class Stack : public AbstractList<T> {
    struct Node {
        T data;
        Node* next;
        Node(const T& val, Node* nxt=nullptr) : data(val), next(nxt) {}
    };
    Node* topNode;

public:
    Stack() : topNode(nullptr) {}
    ~Stack() { this->clear(); }

    void push(const T& element) override {
        topNode = new Node(element, topNode);
        this->length++;
    }

    T pop() override {
        if (this->empty()) throw std::underflow_error("Stack empty");
        Node* tmp = topNode;
        T val = tmp->data;
        topNode = topNode->next;
        delete tmp;
        this->length--;
        return val;
    }

    T& top() {
        if (this->empty()) throw std::underflow_error("Stack empty");
        return topNode->data;
    }
};
