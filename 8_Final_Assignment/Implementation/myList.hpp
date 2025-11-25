#pragma once
/**
 * myList.hpp - Main Header for Heterogeneous List Library
 * 
 * A C++ library for heterogeneous data structures with functional programming support.
 * 
 * Class Hierarchy:
 * 
 *   AbstractList (root)
 *   ├── Stack (LIFO)
 *   ├── Queue (FIFO)
 *   │   ├── Deque (double-ended)
 *   │   └── PriorityQueue (heap-based)
 *   │       ├── MinHeap
 *   │       └── MaxHeap
 *   ├── ArrayList (dynamic array)
 *   ├── SinglyLinkedList
 *   └── DoublyLinkedList
 * 
 *   HeteroList - High-level list with functional methods (built on ArrayList)
 * 
 * Usage:
 *   #include "myList.hpp"
 *   
 *   HeteroList list = {1, 2.5, "hello", true};
 *   auto doubled = list.filter([](const Value& v) { return v.isNumeric(); })
 *                      .map([](const Value& v) { return v * Value(2); });
 * 
 * Features:
 * - Heterogeneous storage (mixed types like Python)
 * - Functional operations: map, filter, reduce, forEach, etc.
 * - Aggregations: sum, average, min, max, countInversions
 * - Multiple data structure implementations
 */

// Core value type for heterogeneous storage
#include "Value.hpp"

// Abstract base class
#include "AbstractList.hpp"

// Basic containers
#include "Stack.hpp"
#include "Queue.hpp"
#include "Deque.hpp"
#include "PriorityQueue.hpp"

// Array and linked list implementations
#include "ArrayList.hpp"
#include "SinglyLinkedList.hpp"
#include "DoublyLinkedList.hpp"

// High-level heterogeneous list with functional programming support
#include "HeteroList.hpp"
