#pragma once
#include "LinkedList.hpp"
#include <stdexcept>

template<typename T>
class SinglyLinkedList : public LinkedList<T> {
private:
    typename LinkedList<T>::Node* head = nullptr;
public:
    SinglyLinkedList() = default;
    ~SinglyLinkedList() {
        while (head) {
            auto* tmp = head;
            head = head->next;
            delete tmp;
        }
        this->length = 0;
    }

    void insert(size_t index, const T& element) override {
        if (index > static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        auto* n = new typename LinkedList<T>::Node(element);
        if (index == 0) {
            n->next = head;
            head = n;
        } else {
            auto* cur = head;
            for (size_t i = 0; i < index - 1; ++i) cur = cur->next;
            n->next = cur->next;
            cur->next = n;
        }
        this->length++;
    }

    T remove(size_t index) override {
        if (index >= static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        if (index == 0) {
            auto* tmp = head;
            T val = tmp->data;
            head = head->next;
            delete tmp;
            this->length--;
            return val;
        }
        auto* cur = head;
        for (size_t i = 0; i < index - 1; ++i) cur = cur->next;
        auto* target = cur->next;
        T val = target->data;
        cur->next = target->next;
        delete target;
        this->length--;
        return val;
    }

    T& get(size_t index) override {
        if (index >= static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        auto* cur = head;
        for (size_t i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    void set(size_t index, const T& element) override {
        if (index >= static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        auto* cur = head;
        for (size_t i = 0; i < index; ++i) cur = cur->next;
        cur->data = element;
    }

    int index(const T& element) const override {
        auto* cur = head;
        int idx = 0;
        while (cur) {
            if (cur->data == element) return idx;
            cur = cur->next;
            ++idx;
        }
        return -1;
    }

    void reverse() override {
        typename LinkedList<T>::Node* prev = nullptr;
        auto* cur = head;
        while (cur) {
            auto* nxt = cur->next;
            cur->next = prev;
            prev = cur;
            cur = nxt;
        }
        head = prev;
    }

    void sort() override {}
    void sort(std::function<bool(const T&, const T&)> comp) override {}

    typename TraversableList<T>::Iterator iterator() override { return nullptr; }
    typename TraversableList<T>::Iterator begin() override { return nullptr; }
    typename TraversableList<T>::Iterator end() override { return nullptr; }
};
