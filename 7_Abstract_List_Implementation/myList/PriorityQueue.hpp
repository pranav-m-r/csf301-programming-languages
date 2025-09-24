#pragma once
#include "Queue.hpp"
#include <cstdlib>
#include <stdexcept>

template <typename T, typename Comparator>
class PriorityQueue : public Queue<T> {
protected:
    class Heap {
        T* data;
        int capacity;
        int size;
        Comparator comparator;

        void resize(int new_capacity) {
            T* new_data = (T*)std::calloc(new_capacity, sizeof(T));
            for (int i = 0; i < size; i++) new_data[i] = data[i];
            std::free(data);
            data = new_data;
            capacity = new_capacity;
        }

        void heapify_up(int i) {
            while (i > 0) {
                int parent = (i - 1) / 2;
                if (!comparator(data[i], data[parent])) break;
                std::swap(data[i], data[parent]);
                i = parent;
            }
        }

        void heapify_down(int i) {
            while (true) {
                int left = 2 * i + 1, right = 2 * i + 2, best = i;
                if (left < size && comparator(data[left], data[best])) best = left;
                if (right < size && comparator(data[right], data[best])) best = right;
                if (best == i) break;
                std::swap(data[i], data[best]);
                i = best;
            }
        }

    public:
        Heap(int initial_capacity = 4) : capacity(initial_capacity), size(0) {
            data = (T*)std::calloc(capacity, sizeof(T));
        }

        ~Heap() { std::free(data); }

        void insert(const T& element) {
            if (size == capacity) resize(capacity * 2);
            data[size] = element;
            heapify_up(size);
            size++;
        }

        T extract() {
            if (size == 0) throw std::out_of_range("Heap empty");
            T root = data[0];
            data[0] = data[size - 1];
            size--;
            if (size > 0) heapify_down(0);
            return root;
        }

        T top() const {
            if (size == 0) throw std::out_of_range("Heap empty");
            return data[0];
        }

        T last() const {
            if (size == 0) throw std::out_of_range("Heap empty");
            return data[size - 1];
        }

        int get_size() const { return size; }
    };

    Heap heap;

public:
    PriorityQueue() : heap(4) {}

    void enqueue(const T& element) override {
        heap.insert(element);
        this->length++;
    }

    T dequeue() override {
        this->length--;
        return heap.extract();
    }

    T front() override { return heap.top(); }
    T back() override { return heap.last(); }
};
