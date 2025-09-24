#pragma once
#include "LinkedList.hpp"
#include <stdexcept>

template<typename T>
class DoublyLinkedList : public LinkedList<T> {
protected:
    struct DNode {
        T data;
        DNode* next;
        DNode* prev;
        DNode(const T& d) : data(d), next(nullptr), prev(nullptr) {}
    };
    DNode* head = nullptr;
    DNode* tail = nullptr;
public:
    DoublyLinkedList() = default;
    ~DoublyLinkedList() {
        while (head) {
            auto* tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
        this->length = 0;
    }

    void insert(size_t index, const T& element) override {
        if (index > static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        auto* n = new DNode(element);
        if (index == 0) {
            n->next = head;
            if (head) head->prev = n;
            head = n;
            if (!tail) tail = n;
        } else if (index == static_cast<size_t>(this->length)) {
            n->prev = tail;
            if (tail) tail->next = n;
            tail = n;
            if (!head) head = n;
        } else {
            DNode* cur = head;
            for (size_t i = 0; i < index; ++i) cur = cur->next;
            n->next = cur;
            n->prev = cur->prev;
            cur->prev->next = n;
            cur->prev = n;
        }
        this->length++;
    }

    T remove(size_t index) override {
        if (index >= static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        DNode* target = head;
        for (size_t i = 0; i < index; ++i) target = target->next;
        T val = target->data;
        if (target->prev) target->prev->next = target->next; else head = target->next;
        if (target->next) target->next->prev = target->prev; else tail = target->prev;
        delete target;
        this->length--;
        return val;
    }

    T& get(size_t index) override {
        if (index >= static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        DNode* cur = head;
        for (size_t i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    void set(size_t index, const T& element) override {
        if (index >= static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        DNode* cur = head;
        for (size_t i = 0; i < index; ++i) cur = cur->next;
        cur->data = element;
    }

    int index(const T& element) const override {
        DNode* cur = head;
        int idx = 0;
        while (cur) {
            if (cur->data == element) return idx;
            cur = cur->next;
            ++idx;
        }
        return -1;
    }

    void reverse() override {
        DNode* cur = head;
        while (cur) {
            DNode* nxt = cur->next;
            cur->next = cur->prev;
            cur->prev = nxt;
            cur = nxt;
        }
        DNode* tmp = head;
        head = tail;
        tail = tmp;
    }

    void sort() override {}
    void sort(std::function<bool(const T&, const T&)> comp) override {}

    typename TraversableList<T>::Iterator iterator() override { return nullptr; }
    typename TraversableList<T>::Iterator begin() override { return nullptr; }
    typename TraversableList<T>::Iterator end() override { return nullptr; }
};
