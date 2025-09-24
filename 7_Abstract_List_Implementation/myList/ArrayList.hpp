#pragma once
#include "TraversableList.hpp"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <functional>

template<typename T>
class ArrayList : public TraversableList<T> {
private:
    int capacity;
    T* data_array;
    double growth_factor;
public:
    class Iterator {
    private:
        T* ptr;
    public:
        Iterator(T* p = nullptr) : ptr(p) {}
        T& operator*() { return *ptr; }
        Iterator& operator++() { ++ptr; return *this; }
        Iterator operator++(int) { Iterator tmp(*this); ++ptr; return tmp; }
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    ArrayList(int length = 0) {
        capacity = std::max(1, length);
        data_array = static_cast<T*>(std::calloc(capacity, sizeof(T)));
        if (!data_array) throw std::bad_alloc();
        this->length = length;
        growth_factor = 2.0;
    }

    ~ArrayList() { std::free(data_array); }

    void resize(int new_capacity) {
        T* new_array = static_cast<T*>(std::calloc(new_capacity, sizeof(T)));
        if (!new_array) throw std::bad_alloc();
        std::memcpy(new_array, data_array, this->length * sizeof(T));
        std::free(data_array);
        data_array = new_array;
        capacity = new_capacity;
    }

    void shrink_to_fit() {
        if (this->length < capacity) resize(this->length == 0 ? 1 : this->length);
    }

    void insert(size_t index, const T& element) override {
        if (index > static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        if (this->length == capacity) resize(static_cast<int>(capacity * growth_factor));
        std::memmove(&data_array[index + 1], &data_array[index], (this->length - index) * sizeof(T));
        data_array[index] = element;
        this->length++;
    }

    T remove(size_t index) override {
        if (index >= static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        T removed = data_array[index];
        std::memmove(&data_array[index], &data_array[index + 1], (this->length - index - 1) * sizeof(T));
        this->length--;
        return removed;
    }

    T& get(size_t index) override {
        if (index >= static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        return data_array[index];
    }

    void set(size_t index, const T& element) override {
        if (index >= static_cast<size_t>(this->length)) throw std::out_of_range("Index out of range");
        data_array[index] = element;
    }

    int index(const T& element) const override {
        for (int i = 0; i < this->length; ++i) if (data_array[i] == element) return i;
        return -1;
    }

    void reverse() override {
        for (int i = 0, j = this->length - 1; i < j; ++i, --j) {
            T tmp = data_array[i]; data_array[i] = data_array[j]; data_array[j] = tmp;
        }
    }

    void sort() override { std::sort(data_array, data_array + this->length); }

    void sort(std::function<bool(const T&, const T&)> comp) override {
        std::sort(data_array, data_array + this->length, comp);
    }

    typename TraversableList<T>::Iterator iterator() override { return begin(); }
    typename TraversableList<T>::Iterator begin() override { return Iterator(data_array); }
    typename TraversableList<T>::Iterator end() override { return Iterator(data_array + this->length); }
};
