#pragma once
/**
 * HeteroList.hpp - Heterogeneous List with Functional Programming Support
 * 
 * The primary user-facing list class that combines:
 * - Python-like heterogeneous storage (mixed types in one list)
 * - Functional programming operations (map, filter, reduce, etc.)
 * - Aggregation methods (sum, average, count inversions, etc.)
 * 
 * This is the "What" specification - high-level operations that
 * describe WHAT we want to do, not HOW to do it.
 * 
 * Built on ArrayList for efficient random access and sorting.
 */

#include "ArrayList.hpp"
#include <functional>
#include <unordered_map>

// Forward declare for Value to reference HeteroList
class HeteroList;

class HeteroList {
private:
    ArrayList data_;

public:
    // ==================== Constructors ====================
    
    HeteroList() = default;
    
    HeteroList(std::initializer_list<Value> init) : data_(init) {}

    // Copy and move
    HeteroList(const HeteroList& other) : data_(other.data_) {}
    HeteroList(HeteroList&& other) noexcept : data_(std::move(other.data_)) {}
    
    HeteroList& operator=(const HeteroList& other) {
        data_ = other.data_;
        return *this;
    }
    
    HeteroList& operator=(HeteroList&& other) noexcept {
        data_ = std::move(other.data_);
        return *this;
    }

    // ==================== Basic Operations ====================
    
    int size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }
    void clear() { data_.clear(); }

    // Element access
    Value& operator[](int index) { return data_[index]; }
    const Value& operator[](int index) const { return data_.get(index); }
    Value& get(int index) { return data_.get(index); }
    const Value& get(int index) const { return data_.get(index); }
    void set(int index, const Value& value) { data_.set(index, value); }

    // ==================== Modifiers (with implicit Value conversion) ====================
    
    void push(const Value& value) { data_.push(value); }
    void push(int v) { data_.push(Value(v)); }
    void push(double v) { data_.push(Value(v)); }
    void push(const char* v) { data_.push(Value(v)); }
    void push(const std::string& v) { data_.push(Value(v)); }
    void push(bool v) { data_.push(Value(v)); }
    
    Value pop() { return data_.pop(); }
    
    void insert(int index, const Value& value) { data_.insert(index, value); }
    Value remove(int index) { return data_.remove(index); }

    int indexOf(const Value& value) const { return data_.indexOf(value); }
    bool contains(const Value& value) const { return data_.contains(value); }

    // ==================== Sorting and Ordering ====================
    
    void sort() { data_.sort(); }
    
    void sort(std::function<bool(const Value&, const Value&)> comparator) {
        data_.sort(comparator);
    }

    void sortDescending() { data_.sortDescending(); }
    void reverse() { data_.reverse(); }

    // ==================== FUNCTIONAL PROGRAMMING METHODS ====================

    /**
     * map - Transform each element using a function
     * 
     * Example: list.map([](const Value& v) { return v * 2; })
     * 
     * @param transform Function that takes a Value and returns a Value
     * @return New HeteroList with transformed values
     */
    HeteroList map(std::function<Value(const Value&)> transform) const {
        HeteroList result;
        for (int i = 0; i < size(); ++i) {
            result.push(transform(data_.get(i)));
        }
        return result;
    }

    /**
     * filter - Keep only elements that satisfy a predicate
     * 
     * Example: list.filter([](const Value& v) { return v.isInt() && v.asInt() > 0; })
     * 
     * @param predicate Function that returns true for elements to keep
     * @return New HeteroList with only matching elements
     */
    HeteroList filter(std::function<bool(const Value&)> predicate) const {
        HeteroList result;
        for (int i = 0; i < size(); ++i) {
            const Value& v = data_.get(i);
            if (predicate(v)) {
                result.push(v);
            }
        }
        return result;
    }

    /**
     * reduce - Combine all elements into a single value
     * 
     * Example: list.reduce(Value(0), [](const Value& acc, const Value& v) { return acc + v; })
     * 
     * @param initial Starting value for accumulation
     * @param reducer Function taking (accumulator, current) returning new accumulator
     * @return Final accumulated value
     */
    Value reduce(const Value& initial, 
                 std::function<Value(const Value&, const Value&)> reducer) const {
        Value acc = initial;
        for (int i = 0; i < size(); ++i) {
            acc = reducer(acc, data_.get(i));
        }
        return acc;
    }

    /**
     * forEach - Execute a function for each element (for side effects)
     * 
     * Example: list.forEach([](const Value& v) { std::cout << v.toString(); })
     */
    void forEach(std::function<void(const Value&)> action) const {
        for (int i = 0; i < size(); ++i) {
            action(data_.get(i));
        }
    }

    /**
     * forEachIndexed - Execute a function for each element with its index
     */
    void forEachIndexed(std::function<void(int, const Value&)> action) const {
        for (int i = 0; i < size(); ++i) {
            action(i, data_.get(i));
        }
    }

    /**
     * any - Check if any element satisfies a predicate
     */
    bool any(std::function<bool(const Value&)> predicate) const {
        for (int i = 0; i < size(); ++i) {
            if (predicate(data_.get(i))) return true;
        }
        return false;
    }

    /**
     * all - Check if all elements satisfy a predicate
     */
    bool all(std::function<bool(const Value&)> predicate) const {
        for (int i = 0; i < size(); ++i) {
            if (!predicate(data_.get(i))) return false;
        }
        return true;
    }

    /**
     * none - Check if no elements satisfy a predicate
     */
    bool none(std::function<bool(const Value&)> predicate) const {
        return !any(predicate);
    }

    /**
     * count - Count elements satisfying a predicate
     */
    int count(std::function<bool(const Value&)> predicate) const {
        int c = 0;
        for (int i = 0; i < size(); ++i) {
            if (predicate(data_.get(i))) ++c;
        }
        return c;
    }

    /**
     * find - Find first element satisfying a predicate
     * @return The found element, or null if not found
     */
    Value find(std::function<bool(const Value&)> predicate) const {
        for (int i = 0; i < size(); ++i) {
            const Value& v = data_.get(i);
            if (predicate(v)) return v;
        }
        return Value();  // null
    }

    /**
     * findIndex - Find index of first element satisfying a predicate
     * @return Index, or -1 if not found
     */
    int findIndex(std::function<bool(const Value&)> predicate) const {
        for (int i = 0; i < size(); ++i) {
            if (predicate(data_.get(i))) return i;
        }
        return -1;
    }

    /**
     * take - Get first n elements
     */
    HeteroList take(int n) const {
        HeteroList result;
        int limit = n < size() ? n : size();
        for (int i = 0; i < limit; ++i) {
            result.push(data_.get(i));
        }
        return result;
    }

    /**
     * drop - Skip first n elements
     */
    HeteroList drop(int n) const {
        HeteroList result;
        for (int i = n; i < size(); ++i) {
            result.push(data_.get(i));
        }
        return result;
    }

    /**
     * slice - Get elements from start to end (exclusive)
     */
    HeteroList slice(int start, int end) const {
        HeteroList result;
        if (start < 0) start = 0;
        if (end > size()) end = size();
        for (int i = start; i < end; ++i) {
            result.push(data_.get(i));
        }
        return result;
    }

    /**
     * concat - Concatenate with another list
     */
    HeteroList concat(const HeteroList& other) const {
        HeteroList result = *this;
        for (int i = 0; i < other.size(); ++i) {
            result.push(other.data_.get(i));
        }
        return result;
    }

    /**
     * flatten - Flatten nested lists one level
     */
    HeteroList flatten() const {
        HeteroList result;
        for (int i = 0; i < size(); ++i) {
            const Value& v = data_.get(i);
            if (v.isList()) {
                auto nested = v.asList();
                for (int j = 0; j < nested->size(); ++j) {
                    result.push((*nested)[j]);
                }
            } else {
                result.push(v);
            }
        }
        return result;
    }

    /**
     * unique - Remove duplicates (keeps first occurrence)
     */
    HeteroList unique() const {
        HeteroList result;
        for (int i = 0; i < size(); ++i) {
            const Value& v = data_.get(i);
            if (!result.contains(v)) {
                result.push(v);
            }
        }
        return result;
    }

    /**
     * zip - Combine with another list into pairs
     */
    HeteroList zip(const HeteroList& other) const {
        HeteroList result;
        int minLen = size() < other.size() ? size() : other.size();
        for (int i = 0; i < minLen; ++i) {
            auto pair = std::make_shared<HeteroList>();
            pair->push(data_.get(i));
            pair->push(other.data_.get(i));
            result.push(Value(pair));
        }
        return result;
    }

    /**
     * partition - Split list based on predicate
     * @return Pair of lists: (matching, non-matching)
     */
    std::pair<HeteroList, HeteroList> partition(
        std::function<bool(const Value&)> predicate) const {
        HeteroList matching, nonMatching;
        for (int i = 0; i < size(); ++i) {
            const Value& v = data_.get(i);
            if (predicate(v)) {
                matching.push(v);
            } else {
                nonMatching.push(v);
            }
        }
        return {matching, nonMatching};
    }

    /**
     * groupBy - Group elements by a key function
     * @return Map from key to list of elements with that key
     */
    std::unordered_map<std::string, HeteroList> groupBy(
        std::function<std::string(const Value&)> keyFunc) const {
        std::unordered_map<std::string, HeteroList> groups;
        for (int i = 0; i < size(); ++i) {
            const Value& v = data_.get(i);
            std::string key = keyFunc(v);
            groups[key].push(v);
        }
        return groups;
    }

    // ==================== AGGREGATION METHODS ====================

    /**
     * sum - Sum all numeric elements
     */
    Value sum() const {
        return reduce(Value(0), [](const Value& acc, const Value& v) {
            if (v.isNumeric()) return acc + v;
            return acc;
        });
    }

    /**
     * product - Product of all numeric elements
     */
    Value product() const {
        return reduce(Value(1), [](const Value& acc, const Value& v) {
            if (v.isNumeric()) return acc * v;
            return acc;
        });
    }

    /**
     * average - Average of numeric elements
     */
    Value average() const {
        int cnt = 0;
        double total = 0.0;
        for (int i = 0; i < size(); ++i) {
            const Value& v = data_.get(i);
            if (v.isNumeric()) {
                total += v.asDouble();
                ++cnt;
            }
        }
        return cnt > 0 ? Value(total / cnt) : Value();
    }

    /**
     * min - Minimum value
     */
    Value min() const {
        if (empty()) return Value();
        Value minVal = data_.get(0);
        for (int i = 1; i < size(); ++i) {
            const Value& v = data_.get(i);
            if (v < minVal) minVal = v;
        }
        return minVal;
    }

    /**
     * max - Maximum value
     */
    Value max() const {
        if (empty()) return Value();
        Value maxVal = data_.get(0);
        for (int i = 1; i < size(); ++i) {
            const Value& v = data_.get(i);
            if (v > maxVal) maxVal = v;
        }
        return maxVal;
    }

    /**
     * countInversions - Count the number of inversions in the list
     * An inversion is a pair (i, j) where i < j but list[i] > list[j]
     * Uses merge sort for O(n log n) complexity
     */
    long long countInversions() const {
        if (size() <= 1) return 0;
        HeteroList temp = *this;
        return mergeSortCount(temp, 0, size() - 1);
    }

    /**
     * frequencies - Count occurrences of each unique element
     */
    std::unordered_map<std::string, int> frequencies() const {
        std::unordered_map<std::string, int> freq;
        for (int i = 0; i < size(); ++i) {
            freq[data_.get(i).toString()]++;
        }
        return freq;
    }

    // ==================== Iterator Support ====================
    
    Value* begin() { return data_.begin(); }
    Value* end() { return data_.end(); }
    const Value* begin() const { return data_.begin(); }
    const Value* end() const { return data_.end(); }

    // ==================== String Representation ====================
    
    std::string toString() const {
        return data_.toString();
    }

private:
    // Helper for countInversions using merge sort
    static long long mergeSortCount(HeteroList& list, int left, int right) {
        if (left >= right) return 0;
        
        int mid = left + (right - left) / 2;
        long long count = 0;
        count += mergeSortCount(list, left, mid);
        count += mergeSortCount(list, mid + 1, right);
        count += mergeCount(list, left, mid, right);
        return count;
    }

    static long long mergeCount(HeteroList& list, int left, int mid, int right) {
        HeteroList leftList, rightList;
        
        for (int i = left; i <= mid; ++i) {
            leftList.push(list[i]);
        }
        for (int i = mid + 1; i <= right; ++i) {
            rightList.push(list[i]);
        }

        int i = 0, j = 0, k = left;
        long long inversions = 0;

        while (i < leftList.size() && j < rightList.size()) {
            if (leftList[i] <= rightList[j]) {
                list[k++] = leftList[i++];
            } else {
                list[k++] = rightList[j++];
                inversions += (leftList.size() - i);
            }
        }
        
        while (i < leftList.size()) list[k++] = leftList[i++];
        while (j < rightList.size()) list[k++] = rightList[j++];

        return inversions;
    }
};

// Factory function to create a shared HeteroList (for nesting in Values)
inline std::shared_ptr<HeteroList> makeList(std::initializer_list<Value> init) {
    return std::make_shared<HeteroList>(init);
}
