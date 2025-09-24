#pragma once
#include "AbstractList.hpp"
#include <functional>

template<typename T>
class TraversableList : public AbstractList<T> {
protected:
    using Iterator = typename AbstractList<T>::Iterator;
    Iterator current_position = nullptr;
public:
    virtual T& get(size_t index) = 0;
    virtual void set(size_t index, const T& element) = 0;
    virtual int index(const T& element) const = 0;
    virtual void reverse() = 0;
    virtual void sort() = 0;
    virtual void sort(std::function<bool(const T&, const T&)> comp) = 0;
    virtual Iterator iterator() = 0;
    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;
};
