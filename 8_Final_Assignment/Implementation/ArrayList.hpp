#pragma once
/**
 * ArrayList.hpp - Dynamic Array Implementation
 * 
 * A resizable array with amortized O(1) push to end.
 * 
 * Operations:
 * - push(): O(1) amortized - Append to end
 * - pop(): O(1) - Remove from end
 * - get(index): O(1) - Random access
 * - insert(index): O(n) - Insert at arbitrary position
 * - remove(index): O(n) - Remove from arbitrary position
 * - sort(): O(n log n) - Sort elements
 * 
 * Inherits from: AbstractList
 */

#include "AbstractList.hpp"
#include <algorithm>
#include <functional>
#include <sstream>

class ArrayList : public AbstractList {
private:
    Value* data_ = nullptr;
    int capacity_ = 0;
    static constexpr double GROWTH_FACTOR = 2.0;
    static constexpr int INITIAL_CAPACITY = 8;

    void ensureCapacity(int minCapacity) {
        if (capacity_ >= minCapacity) return;
        
        int newCapacity = capacity_ == 0 ? INITIAL_CAPACITY : 
                          static_cast<int>(capacity_ * GROWTH_FACTOR);
        if (newCapacity < minCapacity) newCapacity = minCapacity;
        
        Value* newData = new Value[newCapacity];
        for (int i = 0; i < length_; ++i) {
            newData[i] = std::move(data_[i]);
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }

public:
    ArrayList() = default;
    
    explicit ArrayList(int initialCapacity) {
        if (initialCapacity > 0) {
            data_ = new Value[initialCapacity];
            capacity_ = initialCapacity;
        }
    }

    ~ArrayList() override {
        delete[] data_;
    }

    // Copy constructor
    ArrayList(const ArrayList& other) {
        if (other.length_ > 0) {
            capacity_ = other.length_;
            data_ = new Value[capacity_];
            for (int i = 0; i < other.length_; ++i) {
                data_[i] = other.data_[i];
            }
            length_ = other.length_;
        }
    }

    // Move constructor
    ArrayList(ArrayList&& other) noexcept {
        data_ = other.data_;
        capacity_ = other.capacity_;
        length_ = other.length_;
        other.data_ = nullptr;
        other.capacity_ = 0;
        other.length_ = 0;
    }

    // Copy assignment
    ArrayList& operator=(const ArrayList& other) {
        if (this != &other) {
            delete[] data_;
            data_ = nullptr;
            capacity_ = 0;
            length_ = 0;
            
            if (other.length_ > 0) {
                capacity_ = other.length_;
                data_ = new Value[capacity_];
                for (int i = 0; i < other.length_; ++i) {
                    data_[i] = other.data_[i];
                }
                length_ = other.length_;
            }
        }
        return *this;
    }

    // Move assignment
    ArrayList& operator=(ArrayList&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            capacity_ = other.capacity_;
            length_ = other.length_;
            other.data_ = nullptr;
            other.capacity_ = 0;
            other.length_ = 0;
        }
        return *this;
    }

    // Initializer list constructor
    ArrayList(std::initializer_list<Value> init) {
        capacity_ = static_cast<int>(init.size());
        if (capacity_ > 0) {
            data_ = new Value[capacity_];
            for (const auto& v : init) {
                data_[length_++] = v;
            }
        }
    }

    // ==================== AbstractList Interface ====================
    
    void push(const Value& element) override {
        ensureCapacity(length_ + 1);
        data_[length_++] = element;
    }

    Value pop() override {
        checkNotEmpty("pop");
        return std::move(data_[--length_]);
    }

    Value& get(int index) override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index " + std::to_string(index) + " out of range");
        }
        return data_[index];
    }

    const Value& get(int index) const override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index " + std::to_string(index) + " out of range");
        }
        return data_[index];
    }

    void insert(int index, const Value& element) override {
        if (index < 0 || index > length_) {
            throw std::out_of_range("Index out of range");
        }
        ensureCapacity(length_ + 1);
        
        // Shift elements right
        for (int i = length_; i > index; --i) {
            data_[i] = std::move(data_[i - 1]);
        }
        data_[index] = element;
        length_++;
    }

    Value remove(int index) override {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        Value result = std::move(data_[index]);
        
        // Shift elements left
        for (int i = index; i < length_ - 1; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        length_--;
        return result;
    }

    // ==================== ArrayList-Specific Operations ====================
    
    void set(int index, const Value& element) {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Index out of range");
        }
        data_[index] = element;
    }

    int indexOf(const Value& element) const {
        for (int i = 0; i < length_; ++i) {
            if (data_[i] == element) return i;
        }
        return -1;
    }

    bool contains(const Value& element) const {
        return indexOf(element) != -1;
    }

    void reverse() {
        for (int i = 0, j = length_ - 1; i < j; ++i, --j) {
            std::swap(data_[i], data_[j]);
        }
    }

    void sort() {
        std::sort(data_, data_ + length_);
    }

    void sort(std::function<bool(const Value&, const Value&)> comparator) {
        std::sort(data_, data_ + length_, comparator);
    }

    void sortDescending() {
        std::sort(data_, data_ + length_, [](const Value& a, const Value& b) {
            return a > b;
        });
    }

    void clear() {
        length_ = 0;
    }

    void shrinkToFit() {
        if (length_ < capacity_ && length_ > 0) {
            Value* newData = new Value[length_];
            for (int i = 0; i < length_; ++i) {
                newData[i] = std::move(data_[i]);
            }
            delete[] data_;
            data_ = newData;
            capacity_ = length_;
        }
    }

    int capacity() const { return capacity_; }

    // ==================== Iterator Support ====================
    
    Value* begin() { return data_; }
    Value* end() { return data_ + length_; }
    const Value* begin() const { return data_; }
    const Value* end() const { return data_ + length_; }

    // ==================== String Representation ====================
    
    std::string toString() const {
        std::ostringstream oss;
        oss << "[";
        for (int i = 0; i < length_; ++i) {
            if (i > 0) oss << ", ";
            oss << data_[i].toString();
        }
        oss << "]";
        return oss.str();
    }
};
