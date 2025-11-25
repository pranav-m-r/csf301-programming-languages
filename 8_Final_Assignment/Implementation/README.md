# Heterogeneous List Library

A C++17 library implementing heterogeneous (mixed-type) data structures with functional programming support. Inspired by Python's dynamic typing and functional languages like Haskell and Lisp.

## Table of Contents

1. [Overview](#overview)
2. [Design Philosophy](#design-philosophy)
3. [Class Hierarchy](#class-hierarchy)
4. [Core Components](#core-components)
   - [Value](#value---type-erased-container)
   - [AbstractList](#abstractlist---root-interface)
5. [Container Implementations](#container-implementations)
   - [Stack](#stack---lifo-container)
   - [Queue](#queue---fifo-container)
   - [Deque](#deque---double-ended-queue)
   - [PriorityQueue](#priorityqueue---heap-based-container)
   - [ArrayList](#arraylist---dynamic-array)
   - [SinglyLinkedList](#singlylinkedlist---forward-linked-list)
   - [DoublyLinkedList](#doublylinkedlist---bidirectional-linked-list)
6. [HeteroList - The Main Interface](#heterolist---the-main-interface)
7. [Functional Programming Methods](#functional-programming-methods)
8. [Performance Characteristics](#performance-characteristics)
9. [Usage Examples](#usage-examples)
10. [Build Instructions](#build-instructions)

---

## Overview

This library provides a heterogeneous list implementation in C++ that allows storing mixed types (integers, doubles, strings, booleans, null, nested lists) in a single container, similar to Python lists. It combines object-oriented design with functional programming paradigms.

### Key Features

- **Heterogeneous Storage**: Store `int`, `double`, `string`, `bool`, `null`, and nested lists in one container
- **Functional Operations**: `map`, `filter`, `reduce`, `forEach`, and more
- **Aggregation Methods**: `sum`, `average`, `min`, `max`, `countInversions`, `frequencies`
- **Multiple Implementations**: ArrayList, LinkedList variants, Stack, Queue, Deque, PriorityQueue
- **Type-Safe**: Runtime type checking with clear error messages
- **Header-Only**: No compilation needed—just `#include "myList.hpp"`

---

## Design Philosophy

### Separation of "What" vs "How"

The library follows the principle of separating specification from implementation:

| Layer | Description | Examples |
|-------|-------------|----------|
| **What** (Specification) | High-level operations describing intent | `HeteroList::map()`, `filter()`, `reduce()` |
| **How** (Implementation) | Low-level details of execution | `ArrayList`, `SinglyLinkedList`, `DoublyLinkedList` |

Users interact primarily with `HeteroList` (the "What"), while the underlying `ArrayList` handles the "How".

### Type Erasure via `Value`

C++ is statically typed, but we achieve Python-like dynamism through type erasure:

```cpp
// Python-style mixed list
HeteroList list = {1, 2.5, "hello", true, nullptr};

// Each element is stored as a Value
Value v1 = 42;        // Integer
Value v2 = 3.14;      // Double
Value v3 = "world";   // String
```

The `Value` class wraps any supported type and provides:
- **Implicit construction** from primitives
- **Type checking** at runtime (`isInt()`, `isString()`, etc.)
- **Type conversion** with validation (`asInt()`, `asString()`, etc.)
- **Arithmetic operators** that work across numeric types

### Functional-OO Hybrid

The library combines:
- **Object-Oriented**: Class hierarchy with inheritance and polymorphism
- **Functional**: Higher-order functions (`map`, `filter`, `reduce`) that take lambdas

```cpp
// Functional chaining
auto result = list
    .filter([](const Value& v) { return v.isNumeric(); })
    .map([](const Value& v) { return v * Value(2); })
    .reduce(Value(0), [](const Value& a, const Value& b) { return a + b; });
```

---

## Class Hierarchy

```
AbstractList (root abstract class)
├── Stack (LIFO)
├── Queue (FIFO)
│   ├── Deque (double-ended)
│   └── (inheritance path only)
├── PriorityQueue<Comparator> (heap-based)
│   ├── MinHeap (alias)
│   └── MaxHeap (alias)
├── ArrayList (dynamic array)
├── SinglyLinkedList (forward-only)
└── DoublyLinkedList (bidirectional)

HeteroList (user-facing, wraps ArrayList)
    └── Provides functional methods: map, filter, reduce, etc.

Value (type-erased container)
    └── Supports: Null, Bool, Int, Double, String, List
```

### UML Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                        AbstractList                          │
│─────────────────────────────────────────────────────────────│
│ # length_: int                                               │
│─────────────────────────────────────────────────────────────│
│ + size(): int                                                │
│ + empty(): bool                                              │
│ + push(element: Value): void        «pure virtual»          │
│ + pop(): Value                      «pure virtual»          │
│ + get(index: int): Value&           «pure virtual»          │
│ + insert(index: int, element: Value)«pure virtual»          │
│ + remove(index: int): Value         «pure virtual»          │
│ + operator[](index: int): Value&                            │
└─────────────────────────────────────────────────────────────┘
                              △
            ┌─────────────────┼─────────────────┐
            │                 │                 │
     ┌──────┴──────┐  ┌───────┴───────┐  ┌──────┴──────┐
     │    Stack    │  │     Queue     │  │  ArrayList  │
     │─────────────│  │───────────────│  │─────────────│
     │ - top_:Node*│  │ - front_:Node*│  │ - data_:Val*│
     │             │  │ - back_:Node* │  │ - capacity_ │
     │─────────────│  │───────────────│  │─────────────│
     │ + push()    │  │ + enqueue()   │  │ + push()    │
     │ + pop()     │  │ + dequeue()   │  │ + pop()     │
     │ + top()     │  │ + front()     │  │ + get()     │
     └─────────────┘  │ + back()      │  │ + sort()    │
                      └───────────────┘  └─────────────┘
                              △
                       ┌──────┴──────┐
                       │    Deque    │
                       │─────────────│
                       │+ push_front │
                       │+ push_back  │
                       │+ pop_front  │
                       │+ pop_back   │
                       └─────────────┘
```

---

## Core Components

### Value - Type-Erased Container

**File**: `Value.hpp`

The `Value` class is the foundation of heterogeneous storage. It can hold any of six types:

| Type | C++ Representation | Example |
|------|-------------------|---------|
| `Null` | `nullptr` | `Value()` or `Value(nullptr)` |
| `Bool` | `bool` | `Value(true)` |
| `Int` | `long long` | `Value(42)` |
| `Double` | `double` | `Value(3.14)` |
| `String` | `std::string` | `Value("hello")` |
| `List` | `std::shared_ptr<HeteroList>` | `Value(makeList({1, 2, 3}))` |

#### Design Decisions

1. **Implicit Constructors**: Values can be created from primitives without explicit casting
   ```cpp
   HeteroList list;
   list.push(42);        // Implicitly Value(42)
   list.push("hello");   // Implicitly Value("hello")
   ```

2. **Struct over Union**: We use separate member variables instead of `std::variant` or `union` for simplicity with non-trivial types like `std::string` and `std::shared_ptr`.

3. **Numeric Coercion**: Comparisons between `Int` and `Double` automatically convert to `Double`.

4. **Reference-Counted Lists**: Nested lists use `shared_ptr` for automatic memory management.

#### API Reference

| Method | Description |
|--------|-------------|
| `type()` | Returns the `Type` enum value |
| `isNull()`, `isBool()`, `isInt()`, `isDouble()`, `isString()`, `isList()` | Type predicates |
| `isNumeric()` | Returns `true` if `Int` or `Double` |
| `typeName()` | Returns human-readable type name |
| `asInt()`, `asDouble()`, `asBool()`, `asString()`, `asList()` | Type conversion (throws on mismatch) |
| `toString()` | String representation |
| `==`, `!=`, `<`, `>`, `<=`, `>=` | Comparison operators |
| `+`, `-`, `*`, `/`, `%` | Arithmetic operators |
| `-` (unary) | Negation |
| `hash()` | Hash value for use in hash maps |

---

### AbstractList - Root Interface

**File**: `AbstractList.hpp`

The abstract base class defining the minimal interface for all list-like containers.

#### Design Decisions

1. **Pure Virtual Methods**: Forces all subclasses to implement core operations
2. **Protected `length_`**: Subclasses manage their own size
3. **Virtual Destructor**: Enables proper polymorphic deletion
4. **Index Bounds Checking**: Helper methods `checkIndex()` and `checkNotEmpty()`

#### API Reference

| Method | Description |
|--------|-------------|
| `size()` | Number of elements |
| `empty()` | Returns `true` if size is 0 |
| `push(element)` | Add element (semantics vary by subclass) |
| `pop()` | Remove and return element (semantics vary by subclass) |
| `get(index)` | Access element at index |
| `insert(index, element)` | Insert at position |
| `remove(index)` | Remove from position |
| `operator[]` | Alias for `get()` |

---

## Container Implementations

### Stack - LIFO Container

**File**: `Stack.hpp`

A Last-In-First-Out container implemented with a singly-linked list.

#### Design Decisions

- **Linked List Implementation**: O(1) push/pop without resizing
- **No Random Access**: `get(index)` is O(n) but rarely needed for stacks
- **Top Pointer**: Single pointer to the top node

#### API Reference

| Method | Complexity | Description |
|--------|------------|-------------|
| `push(element)` | O(1) | Push to top |
| `pop()` | O(1) | Pop from top |
| `top()` | O(1) | Peek at top (no removal) |
| `get(index)` | O(n) | Access by index (0 = top) |
| `insert(index, element)` | O(n) | Insert at position |
| `remove(index)` | O(n) | Remove from position |
| `clear()` | O(n) | Remove all elements |

---

### Queue - FIFO Container

**File**: `Queue.hpp`

A First-In-First-Out container with front and back pointers.

#### Design Decisions

- **Two Pointers**: `front_` for dequeue, `back_` for enqueue
- **O(1) Both Ends**: Enqueue and dequeue are both constant time
- **Linked List**: Nodes allocated individually for simplicity

#### API Reference

| Method | Complexity | Description |
|--------|------------|-------------|
| `enqueue(element)` / `push()` | O(1) | Add to back |
| `dequeue()` / `pop()` | O(1) | Remove from front |
| `front()` | O(1) | Peek at front |
| `back()` | O(1) | Peek at back |
| `get(index)` | O(n) | Access by index (0 = front) |
| `insert(index, element)` | O(n) | Insert at position |
| `remove(index)` | O(n) | Remove from position |
| `clear()` | O(n) | Remove all elements |

---

### Deque - Double-Ended Queue

**File**: `Deque.hpp`

Extends `Queue` to allow insertion/removal from both ends.

#### Design Decisions

- **Inherits from Queue**: Reuses queue implementation
- **Asymmetric Performance**: `pop_back()` is O(n) due to singly-linked nodes
- **For O(1) Both Ends**: Use `DoublyLinkedList` directly

#### API Reference

| Method | Complexity | Description |
|--------|------------|-------------|
| `push_front(element)` | O(1) | Add to front |
| `push_back(element)` | O(1) | Add to back |
| `pop_front()` | O(1) | Remove from front |
| `pop_back()` | O(n) | Remove from back (needs traversal) |
| `push()` | O(1) | Alias for `push_back()` |
| `pop()` | O(1) | Alias for `pop_front()` |

---

### PriorityQueue - Heap-Based Container

**File**: `PriorityQueue.hpp`

Elements are ordered by priority using a binary heap stored in a vector.

#### Design Decisions

- **Template Comparator**: Supports min-heap (default) and max-heap
- **Vector Storage**: Cache-friendly array-based heap
- **Type Aliases**: `MinHeap` and `MaxHeap` for convenience

#### API Reference

| Method | Complexity | Description |
|--------|------------|-------------|
| `enqueue(element)` / `push()` | O(log n) | Add with heap ordering |
| `dequeue()` / `pop()` | O(log n) | Remove highest priority |
| `front()` | O(1) | Peek at highest priority |
| `get(index)` | O(1) | Access by heap index |
| `remove(index)` | O(log n) | Remove arbitrary element |
| `clear()` | O(1) | Remove all elements |

#### Usage

```cpp
MinHeap minHeap;
minHeap.push(5);
minHeap.push(1);
minHeap.push(3);
// minHeap.front() == 1 (smallest)

MaxHeap maxHeap;
maxHeap.push(5);
maxHeap.push(1);
maxHeap.push(3);
// maxHeap.front() == 5 (largest)
```

---

### ArrayList - Dynamic Array

**File**: `ArrayList.hpp`

A resizable array with amortized O(1) append, similar to `std::vector`.

#### Design Decisions

1. **Growth Factor 2.0**: Doubles capacity when full (amortized O(1) push)
2. **Initial Capacity 8**: Avoids many reallocations for small lists
3. **Proper Copy Semantics**: Uses `new Value[]` and element-wise copy (not `memcpy`)
4. **Move Support**: Efficient move constructor/assignment

#### Why Not Use `memcpy`?

The `Value` class contains non-trivial types (`std::string`, `std::shared_ptr`). Using `memcpy` or `memmove` would:
- Skip copy constructors, leaving shared_ptr reference counts wrong
- Potentially double-free strings
- Cause undefined behavior

Instead, we use explicit loops with `std::move` for efficiency.

#### API Reference

| Method | Complexity | Description |
|--------|------------|-------------|
| `push(element)` | O(1) amortized | Append to end |
| `pop()` | O(1) | Remove from end |
| `get(index)` / `operator[]` | O(1) | Random access |
| `set(index, element)` | O(1) | Update element |
| `insert(index, element)` | O(n) | Insert at position |
| `remove(index)` | O(n) | Remove from position |
| `indexOf(element)` | O(n) | Find element |
| `contains(element)` | O(n) | Check existence |
| `sort()` | O(n log n) | Sort ascending |
| `sort(comparator)` | O(n log n) | Sort with custom comparator |
| `sortDescending()` | O(n log n) | Sort descending |
| `reverse()` | O(n) | Reverse in place |
| `clear()` | O(1) | Remove all elements |
| `shrinkToFit()` | O(n) | Reduce capacity to size |
| `capacity()` | O(1) | Current capacity |
| `begin()` / `end()` | O(1) | Iterator support |
| `toString()` | O(n) | String representation |

---

### SinglyLinkedList - Forward-Linked List

**File**: `SinglyLinkedList.hpp`

A linked list where each node points only to the next node.

#### Design Decisions

- **Tail Pointer**: Enables O(1) push to end
- **O(n) Pop**: Must traverse to find second-to-last node
- **Memory Efficient**: Only one pointer per node

#### API Reference

| Method | Complexity | Description |
|--------|------------|-------------|
| `push(element)` | O(1) | Append to end (using tail pointer) |
| `pop()` | O(n) | Remove from end (needs traversal) |
| `get(index)` | O(n) | Access by index |
| `set(index, element)` | O(n) | Update element |
| `insert(0, element)` | O(1) | Insert at front |
| `insert(index, element)` | O(n) | Insert at position |
| `remove(0)` | O(1) | Remove from front |
| `remove(index)` | O(n) | Remove from position |
| `indexOf(element)` | O(n) | Find element |
| `contains(element)` | O(n) | Check existence |
| `reverse()` | O(n) | Reverse in place |
| `clear()` | O(n) | Remove all elements |
| `toString()` | O(n) | String representation |

---

### DoublyLinkedList - Bidirectional Linked List

**File**: `DoublyLinkedList.hpp`

A linked list where each node has pointers to both next and previous nodes.

#### Design Decisions

- **Head and Tail Pointers**: O(1) access to both ends
- **O(1) Pop**: No traversal needed to remove from end
- **Optimized Get**: Starts from closer end (head or tail)
- **More Memory**: Two pointers per node

#### API Reference

| Method | Complexity | Description |
|--------|------------|-------------|
| `push(element)` | O(1) | Append to end |
| `pop()` | O(1) | Remove from end |
| `pushFront(element)` | O(1) | Insert at front |
| `popFront()` | O(1) | Remove from front |
| `get(index)` | O(n/2) | Access by index (optimized) |
| `set(index, element)` | O(n/2) | Update element |
| `insert(index, element)` | O(n/2) | Insert at position |
| `remove(index)` | O(n/2) | Remove from position |
| `front()` | O(1) | Peek at front |
| `back()` | O(1) | Peek at back |
| `indexOf(element)` | O(n) | Find element |
| `contains(element)` | O(n) | Check existence |
| `reverse()` | O(n) | Reverse in place |
| `clear()` | O(n) | Remove all elements |
| `toString()` | O(n) | String representation |

---

## HeteroList - The Main Interface

**File**: `HeteroList.hpp`

The primary user-facing class that wraps `ArrayList` and provides functional programming methods.

### Why HeteroList?

While you can use `ArrayList` directly, `HeteroList` provides:
1. **Convenience overloads** for `push()` that accept primitives
2. **Functional methods** like `map`, `filter`, `reduce`
3. **Aggregation methods** like `sum`, `average`, `countInversions`
4. **A cleaner interface** focused on the "what" rather than "how"

### API Reference - Basic Operations

| Method | Description |
|--------|-------------|
| `size()` | Number of elements |
| `empty()` | Returns `true` if size is 0 |
| `clear()` | Remove all elements |
| `get(index)` / `operator[]` | Access element |
| `set(index, value)` | Update element |
| `push(value)` | Append element (overloaded for primitives) |
| `pop()` | Remove and return last element |
| `insert(index, value)` | Insert at position |
| `remove(index)` | Remove from position |
| `indexOf(value)` | Find element index (-1 if not found) |
| `contains(value)` | Check if element exists |
| `sort()` | Sort ascending |
| `sort(comparator)` | Sort with custom comparator |
| `sortDescending()` | Sort descending |
| `reverse()` | Reverse in place |
| `toString()` | String representation |

---

## Functional Programming Methods

### Transformation Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `map` | `HeteroList map(function<Value(const Value&)>)` | Transform each element |
| `filter` | `HeteroList filter(function<bool(const Value&)>)` | Keep matching elements |
| `reduce` | `Value reduce(initial, function<Value(Value, Value)>)` | Combine into single value |
| `flatten` | `HeteroList flatten()` | Flatten nested lists one level |
| `unique` | `HeteroList unique()` | Remove duplicates |

### Iteration Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `forEach` | `void forEach(function<void(const Value&)>)` | Execute for each element |
| `forEachIndexed` | `void forEachIndexed(function<void(int, const Value&)>)` | Execute with index |

### Search Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `any` | `bool any(function<bool(const Value&)>)` | True if any match |
| `all` | `bool all(function<bool(const Value&)>)` | True if all match |
| `none` | `bool none(function<bool(const Value&)>)` | True if none match |
| `count` | `int count(function<bool(const Value&)>)` | Count matching elements |
| `find` | `Value find(function<bool(const Value&)>)` | Find first match (or null) |
| `findIndex` | `int findIndex(function<bool(const Value&)>)` | Find index of first match |

### Slicing Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `take` | `HeteroList take(int n)` | First n elements |
| `drop` | `HeteroList drop(int n)` | Skip first n elements |
| `slice` | `HeteroList slice(int start, int end)` | Elements from start to end |

### Combining Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `concat` | `HeteroList concat(const HeteroList&)` | Concatenate two lists |
| `zip` | `HeteroList zip(const HeteroList&)` | Pair elements from two lists |
| `partition` | `pair<HeteroList, HeteroList> partition(predicate)` | Split by predicate |
| `groupBy` | `unordered_map<string, HeteroList> groupBy(keyFunc)` | Group by key |

### Aggregation Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `sum` | `Value sum()` | Sum of numeric elements |
| `product` | `Value product()` | Product of numeric elements |
| `average` | `Value average()` | Average of numeric elements |
| `min` | `Value min()` | Minimum element |
| `max` | `Value max()` | Maximum element |
| `countInversions` | `long long countInversions()` | Count inversions (merge sort) |
| `frequencies` | `unordered_map<string, int> frequencies()` | Count each unique element |

---

## Performance Characteristics

### Time Complexity Summary

| Operation | ArrayList | SinglyLinkedList | DoublyLinkedList | Stack | Queue |
|-----------|-----------|------------------|------------------|-------|-------|
| `push` (end) | O(1)* | O(1) | O(1) | O(1) | O(1) |
| `pop` (end) | O(1) | O(n) | O(1) | O(1) | O(n)† |
| `get(index)` | O(1) | O(n) | O(n/2) | O(n) | O(n) |
| `insert(0)` | O(n) | O(1) | O(1) | O(1) | O(1) |
| `insert(middle)` | O(n) | O(n) | O(n/2) | O(n) | O(n) |
| `remove(0)` | O(n) | O(1) | O(1) | O(1) | O(1) |
| `remove(middle)` | O(n) | O(n) | O(n/2) | O(n) | O(n) |
| `search` | O(n) | O(n) | O(n) | O(n) | O(n) |
| `sort` | O(n log n) | N/A | N/A | N/A | N/A |

\* Amortized O(1), worst case O(n) during reallocation  
† Queue pops from front, not end

### Space Complexity

| Structure | Space per Element | Overhead |
|-----------|-------------------|----------|
| ArrayList | sizeof(Value) | Capacity × sizeof(Value) |
| SinglyLinkedList | sizeof(Value) + sizeof(Node*) | 2 pointers (head, tail) |
| DoublyLinkedList | sizeof(Value) + 2×sizeof(Node*) | 2 pointers (head, tail) |
| Stack | sizeof(Value) + sizeof(Node*) | 1 pointer (top) |
| Queue | sizeof(Value) + sizeof(Node*) | 2 pointers (front, back) |
| PriorityQueue | sizeof(Value) | Vector overhead |

---

## Usage Examples

### Basic Usage

```cpp
#include "myList.hpp"

// Create a heterogeneous list
HeteroList list = {1, 2.5, "hello", true};

// Access elements
std::cout << list[0].asInt();      // 1
std::cout << list[2].asString();   // "hello"

// Add elements
list.push(42);
list.push("world");

// Check types
if (list[1].isDouble()) {
    std::cout << list[1].asDouble();  // 2.5
}
```

### Functional Operations

```cpp
HeteroList numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// Filter even numbers
auto evens = numbers.filter([](const Value& v) {
    return v.asInt() % 2 == 0;
});
// evens = [2, 4, 6, 8, 10]

// Double each number
auto doubled = numbers.map([](const Value& v) {
    return v * Value(2);
});
// doubled = [2, 4, 6, 8, 10, 12, 14, 16, 18, 20]

// Sum all numbers
Value total = numbers.reduce(Value(0), [](const Value& acc, const Value& v) {
    return acc + v;
});
// total = 55

// Chain operations
auto result = numbers
    .filter([](const Value& v) { return v.asInt() % 2 == 0; })
    .map([](const Value& v) { return v * Value(2); })
    .sum();
// result = 60 (2+4+6+8+10 doubled)
```

### Aggregations

```cpp
HeteroList data = {5, 2, 8, 1, 9, 3, 7, 4, 6};

std::cout << data.sum().asInt();        // 45
std::cout << data.average().asDouble(); // 5.0
std::cout << data.min().asInt();        // 1
std::cout << data.max().asInt();        // 9
std::cout << data.countInversions();    // Number of inversions

auto freq = data.frequencies();
// {"5": 1, "2": 1, "8": 1, ...}
```

### Nested Lists

```cpp
HeteroList matrix = {
    Value(makeList({1, 2, 3})),
    Value(makeList({4, 5, 6})),
    Value(makeList({7, 8, 9}))
};

// Access nested element
auto row0 = matrix[0].asList();
std::cout << (*row0)[1].asInt();  // 2

// Flatten
auto flat = matrix.flatten();
// flat = [1, 2, 3, 4, 5, 6, 7, 8, 9]
```

### Using Different Containers

```cpp
// Stack (LIFO)
Stack stack;
stack.push(1);
stack.push(2);
stack.push(3);
std::cout << stack.pop().asInt();  // 3

// Queue (FIFO)
Queue queue;
queue.enqueue(1);
queue.enqueue(2);
queue.enqueue(3);
std::cout << queue.dequeue().asInt();  // 1

// Priority Queue (Min-Heap)
MinHeap heap;
heap.push(5);
heap.push(1);
heap.push(3);
std::cout << heap.pop().asInt();  // 1 (smallest)

// Max-Heap
MaxHeap maxHeap;
maxHeap.push(5);
maxHeap.push(1);
maxHeap.push(3);
std::cout << maxHeap.pop().asInt();  // 5 (largest)
```

---

## Build Instructions

### Requirements

- C++17 compiler (for `std::filesystem`, `std::shared_ptr`, structured bindings)
- No external dependencies

### Compilation

The library is **header-only**. Simply include the main header:

```cpp
#include "myList.hpp"
```

Compile with C++17:

```bash
# g++
g++ -std=c++17 -o myprogram myprogram.cpp

# clang++
clang++ -std=c++17 -o myprogram myprogram.cpp

# MSVC
cl /std:c++17 myprogram.cpp
```

### File Structure

```
Implementation/
├── myList.hpp           # Master header (include this)
├── Value.hpp            # Type-erased container
├── AbstractList.hpp     # Root abstract class
├── Stack.hpp            # LIFO container
├── Queue.hpp            # FIFO container
├── Deque.hpp            # Double-ended queue
├── PriorityQueue.hpp    # Heap-based priority queue
├── ArrayList.hpp        # Dynamic array
├── SinglyLinkedList.hpp # Forward-linked list
├── DoublyLinkedList.hpp # Bidirectional linked list
├── HeteroList.hpp       # Main interface with functional methods
└── README.md            # This file
```

---

## License

This library was developed as part of CS F301 Programming Languages coursework at BITS Pilani.