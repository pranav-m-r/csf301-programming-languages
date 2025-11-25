# Heterogeneous List Library with Functional-OO Abstractions

**Course:** CS F301 - Programming Languages  
**Date:** 26 November, 2025  
**Team:** Pranav M R, Arinjay Jain, Kanishk Rai, Anshul Rakesh

---

## Table of Contents

1. [Interpretation of Specifications](#1-interpretation-of-specifications)
2. [Team Contributions](#2-team-contributions)
3. [Development Timeline](#3-development-timeline)
4. [Design Decisions and Architecture](#4-design-decisions-and-architecture)
5. [Library Implementation](#5-library-implementation)
6. [Use Case 1: Keyword Frequency Counter](#6-use-case-1-keyword-frequency-counter)
7. [Use Case 2: Network Stream Processor](#7-use-case-2-network-stream-processor)
8. [Use Case 3: CLI Application](#8-use-case-3-cli-application)
9. [Use Case 4: Web Application](#9-use-case-4-web-application)
10. [Testing Strategy](#10-testing-strategy)
11. [Packaging and Build System](#11-packaging-and-build-system)
12. [LLM Assistance Documentation](#12-llm-assistance-documentation)
13. [Conclusion](#13-conclusion)

---

## 1. Interpretation of Specifications

### 1.1 Understanding the Problem

The assignment was a continuation of our previous work on list structures, now to be extended with a high-level interface for reading, searching, and sorting files and network streams. The key requirements were:

1. **Focus on the "What"** - The assignment emphasized specifying *what* the system does (functional specification) rather than *how* it does it (procedural details)
2. **Functional-OO Integration** - Combine object-oriented structure with functional programming patterns (lambdas, higher-order functions)
3. **Aggregation Methods** - Include operations like counting inversions, computing averages, and mapping transformation functions to create new lists
4. **Menu-driven/Library API** - Provide a usable interface, not just raw code

### 1.2 Our Interpretation of "Functional-OO"

Based on the assignment description, we understood that Functional-OO means:
- **OO for the "How"**: Class hierarchies, encapsulation, polymorphism handle implementation details
- **Functional for the "What"**: Higher-order functions like `map`, `filter`, `reduce` specify transformations declaratively

As the assignment stated: *"When we specify the What part in functional programming in terms of higher and higher composites of more elementary abstractions, we infuse functional programming with OOn."*

We aimed to create an interface where users express *what* they want (e.g., "filter all even numbers and double them") without worrying about *how* it's done internally (array resizing, pointer management, etc.).

### 1.3 Prior Design Work

In our previous assignment, we had already designed the basic architecture:
- **Contain-and-delegate pattern**: We decided that `HeteroList` should contain an `ArrayList` internally rather than inherit from it
- **Abstract base class**: `AbstractList` defines the interface that all concrete implementations must follow
- **Multiple implementations**: ArrayList, SinglyLinkedList, DoublyLinkedList, Stack, Queue, Deque, PriorityQueue

This design was documented and planned before coding began, following the OO principle of deriving the "How" from the specification of the "What".

### 1.4 The Contain-and-Delegate Approach

We adopted this pattern consciously (as emphasized in the assignment instructions) for the main `HeteroList` class:

```
HeteroList (user-facing specification - the "What")
    └── contains an ArrayList (implementation - the "How")
    └── delegates storage operations to ArrayList
    └── adds functional methods on top
```

**Why this decision?**
- **Flexibility**: We can swap `ArrayList` for `DoublyLinkedList` without changing the HeteroList interface
- **Separation of concerns**: Functional methods (map, filter, reduce) are defined at the specification level
- **Encapsulation**: Users never see capacity, resizing, or memory management details

This was recorded in our design documentation before implementation began.

### 1.5 Building on Previous Work

Our previous assignment provided a working list library with homogeneous types. For this assignment, we needed to:
1. Add a `Value` class for type erasure (to support heterogeneous data)
2. Modify all containers to store `Value` instead of template types
3. Add functional programming methods (`map`, `filter`, `reduce`, etc.)
4. Build practical use cases demonstrating the library
5. Create a demonstration-ready bundle with build scripts

---

## 2. Team Contributions

The work was distributed among team members as follows:

| Team Member | Primary Contribution | Description |
|-------------|---------------------|-------------|
| **Pranav M R** | Library API + Web App (UC4) | Designed and implemented the core `HeteroList` and `Value` classes, functional operations, and the Flask-based web application |
| **Kanishk Rai** | CLI Application (UC3) | Built the JSON-based command-line interface that bridges the C++ library with external programs |
| **Arinjay Jain** | Keyword Frequency (UC1) | Implemented the keyword frequency counter demonstrating the pure functional approach |
| **Anshul Rakesh** | Network Stream (UC2) | Created the client-server application for real-time heterogeneous data streaming |

**Collaborative Work:**
- All team members participated in design discussions and architecture decisions
- Testing was done collaboratively, with each member testing their own component and cross-testing others
- Documentation and the final report were reviewed by all members
- Everyone is familiar with all aspects of the project as required by the assignment

---

## 3. Development Timeline

### Day 1: Library Foundation

**Focus:** Core `Value` class and type erasure

**Work Done:**
- Implemented the `Value` class with support for six types (Null, Bool, Int, Double, String, List)
- Added type predicates (`isInt()`, `isString()`, etc.) and conversion methods (`asInt()`, `asDouble()`, etc.)
- Implemented comparison operators with cross-type handling
- Implemented arithmetic operators for numeric types
- Initial testing of Value class behavior

**Challenges:**
- Deciding between `std::variant` and separate member variables (chose the latter for simplicity)
- Handling cross-type comparisons (Int vs Double)

---

### Day 2: HeteroList and Functional Operations

**Focus:** Main interface and functional methods

**Work Done:**
- Created `HeteroList` class wrapping `ArrayList`
- Implemented basic operations: `push`, `pop`, `get`, `set`, `insert`, `remove`
- Added functional methods: `map`, `filter`, `reduce`, `forEach`
- Added search methods: `any`, `all`, `count`, `find`, `findIndex`
- Added slicing methods: `take`, `drop`, `slice`
- Added aggregation methods: `sum`, `average`, `min`, `max`, `countInversions`, `frequencies`
- Implemented `sort` with custom comparators
- Created comprehensive unit tests (20 test cases)

**Testing:**
- All 20 unit tests passing
- Manual testing of functional operation chaining

---

### Day 3: Use Case 1 - Keyword Frequency Counter

**Focus:** Demonstrating pure functional approach

**Work Done:**
- Implemented `KeywordFrequency.cpp` using only functional operations
- Used `map()` with `count()` instead of traditional hash maps
- Added recursive directory scanning for text files
- Implemented case-insensitive matching and punctuation handling
- Added summary statistics output
- Created sample data files for testing

**Key Insight:**
- `keywords.map([&](kw) { return [kw, allWords.count(matches_kw)]; })` replaces imperative counting loops
- Demonstrates that functional operations can express the "What" cleanly

---

### Day 4: Use Case 2 - Network Stream Processor

**Focus:** Real-time streaming and aggregation

**Work Done:**
- Implemented TCP server that generates random heterogeneous data
- Implemented client that receives, categorizes, and displays live statistics
- Used library's `sum()`, `average()`, `min()`, `max()`, `countInversions()`, `frequencies()`
- Added thread-safe list operations with mutex protection
- Created formatted console output with live updates

**Key Insight:**
- Library aggregation methods work seamlessly on streaming data
- Type checking (`isInt()`, `isDouble()`) enables automatic categorization

---

### Day 5: Use Case 3 - CLI Application

**Focus:** JSON-based interface for external programs

**Work Done:**
- Implemented `hetero_cli.cpp` with JSON parsing/serialization
- Added 16 commands covering all library operations
- Implemented graceful error handling for heterogeneous lists
- Added informative metadata in responses (sizes, counts, notes)
- Tested with various inputs including edge cases

**Key Insight:**
- The CLI acts as a bridge, allowing Python/JavaScript to use our C++ implementation
- JSON is a natural format for heterogeneous data representation

---

### Day 6: Use Case 4 - Web Application + Packaging

**Focus:** Interactive UI and final deliverable

**Work Done:**
- Created Flask server with REST API endpoints
- Built Bootstrap 5 dark-themed UI
- Implemented cross-platform support (WSL for Windows)
- Added features: type badges, history, statistics cards, frequency bars
- Created Makefile with targets for all use cases
- Added automatic Flask installation
- Added browser auto-open on startup
- Wrote comprehensive documentation (READMEs, this report)
- Final testing of all components

**Key Insight:**
- By calling the C++ CLI from Python, we demonstrated true cross-language interoperability
- The UI makes the library accessible to non-programmers

---

## 4. Design Decisions and Architecture

### 4.1 The Value Class - Type Erasure Foundation

The biggest challenge was making C++ store heterogeneous data. We created a `Value` class that can hold any of six types:

| Type | C++ Storage | Example |
|------|-------------|---------|
| Null | (flag) | `Value()` |
| Bool | `bool` | `Value(true)` |
| Int | `long long` | `Value(42)` |
| Double | `double` | `Value(3.14)` |
| String | `std::string` | `Value("hello")` |
| List | `shared_ptr<HeteroList>` | Nested lists |

**Design Decision:** We used separate member variables instead of `std::variant` because:
- Simpler to understand and debug
- Easier to handle non-trivial types like `std::string`
- More control over comparison and arithmetic operators

**Design Decision:** We used `shared_ptr` for nested lists to:
- Avoid deep copying when lists are passed around
- Enable reference counting for automatic memory management
- Allow circular references (though we don't specifically use them)

### 4.2 Class Hierarchy

```
AbstractList (abstract base class)
├── Stack (LIFO - linked list based)
├── Queue (FIFO - linked list based)
│   └── Deque (extends Queue)
├── PriorityQueue<Comparator>
│   ├── MinHeap (alias)
│   └── MaxHeap (alias)
├── ArrayList (dynamic array)
├── SinglyLinkedList
└── DoublyLinkedList

HeteroList (wraps ArrayList, adds functional methods)

Value (type-erased container)
```

### 4.3 UML Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                        AbstractList                         │
│─────────────────────────────────────────────────────────────│
│ # length_: int                                              │
│─────────────────────────────────────────────────────────────│
│ + size(): int                                               │
│ + empty(): bool                                             │
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
     └─────────────┘  └───────┬───────┘  │ + sort()    │
                              │          └──────┬──────┘
                       ┌──────┴──────┐          │
                       │    Deque    │   ┌──────┴──────┐
                       │─────────────│   │  HeteroList │
                       │+ push_front │   │─────────────│
                       │+ pop_back   │   │ + map()     │
                       └─────────────┘   │ + filter()  │
                                         │ + reduce()  │
                                         │ + sum()     │
                                         │ + ...       │
                                         └─────────────┘
```

---

## 5. Library Implementation

### 5.1 Value Class API

| Method | Description |
|--------|-------------|
| `type()` | Returns the Type enum |
| `isNull()`, `isBool()`, `isInt()`, `isDouble()`, `isString()`, `isList()` | Type predicates |
| `isNumeric()` | True if Int or Double |
| `asInt()`, `asDouble()`, `asBool()`, `asString()`, `asList()` | Type conversion (throws on mismatch) |
| `toString()` | String representation |
| `==`, `!=`, `<`, `>`, `<=`, `>=` | Comparison operators |
| `+`, `-`, `*`, `/`, `%` | Arithmetic operators |
| `hash()` | For use in hash maps |

### 5.2 HeteroList Basic Operations

| Method | Description |
|--------|-------------|
| `size()` | Number of elements |
| `empty()` | Check if empty |
| `push(value)` | Append element |
| `pop()` | Remove and return last |
| `get(index)` / `operator[]` | Access by index |
| `set(index, value)` | Update element |
| `insert(index, value)` | Insert at position |
| `remove(index)` | Remove from position |
| `indexOf(value)` | Find element (-1 if not found) |
| `contains(value)` | Check existence |
| `sort()` / `sortDescending()` | Sort in place |
| `reverse()` | Reverse in place |

### 5.3 Functional Operations

| Method | Signature | Description |
|--------|-----------|-------------|
| `map` | `HeteroList map(function<Value(const Value&)>)` | Transform each element |
| `filter` | `HeteroList filter(function<bool(const Value&)>)` | Keep matching elements |
| `reduce` | `Value reduce(initial, function<Value(Value, Value)>)` | Combine into single value |
| `forEach` | `void forEach(function<void(const Value&)>)` | Execute for each element |
| `any` | `bool any(predicate)` | True if any match |
| `all` | `bool all(predicate)` | True if all match |
| `count` | `int count(predicate)` | Count matches |
| `find` | `Value find(predicate)` | Find first match |
| `take` | `HeteroList take(n)` | First n elements |
| `drop` | `HeteroList drop(n)` | Skip first n elements |
| `partition` | `pair<HeteroList, HeteroList> partition(pred)` | Split by predicate |
| `unique` | `HeteroList unique()` | Remove duplicates |
| `flatten` | `HeteroList flatten()` | Flatten nested lists |

### 5.4 Aggregation Operations

| Method | Description |
|--------|-------------|
| `sum()` | Sum of numeric elements |
| `product()` | Product of numeric elements |
| `average()` | Mean of numeric elements |
| `min()` / `max()` | Extreme values |
| `countInversions()` | Count out-of-order pairs |
| `frequencies()` | Count each unique element |

### 5.5 Performance Characteristics

| Operation | ArrayList | SinglyLinkedList | DoublyLinkedList |
|-----------|-----------|------------------|------------------|
| `push` (end) | O(1)* | O(1) | O(1) |
| `pop` (end) | O(1) | O(n) | O(1) |
| `get(index)` | O(1) | O(n) | O(n/2) |
| `insert(0)` | O(n) | O(1) | O(1) |
| `search` | O(n) | O(n) | O(n) |
| `sort` | O(n log n) | N/A | N/A |

\* Amortized, with occasional O(n) for reallocation

---

## 6. Use Case 1: Keyword Frequency Counter

### 6.1 Overview

This use case demonstrates a **pure functional programming approach** to counting keyword occurrences in text files. Instead of using traditional loops and hash maps, we use `map()`, `count()`, `forEach()`, and `sort()`.

### 6.2 How It Uses the Library

```cpp
// Step 1: Read keywords into HeteroList
HeteroList keywords = readKeywords(keywordFile);

// Step 2: Read ALL words from all files into one HeteroList
HeteroList allWords;
files.forEach([&](const Value& filePath) {
    HeteroList fileWords = readWordsFromFile(filePath.asString());
    // Add all words to allWords
});

// Step 3: Transform keywords to [keyword, count] pairs using map() and count()
HeteroList results = keywords.map([&allWords](const Value& keyword) {
    int freq = allWords.count([&](const Value& word) {
        return word == keyword;
    });
    return Value(makeList({keyword, Value(freq)}));
});

// Step 4: Sort by count descending
results.sort(/* comparator */);

// Step 5: Output using forEach
results.forEach([](const Value& pair) {
    // Print each [keyword, count] pair
});
```

### 6.3 Functional-OO Concepts Demonstrated

| Concept | How It's Used |
|---------|---------------|
| Higher-Order Functions | `map()`, `forEach()`, `count()` take lambda functions |
| Transformation | `map()` transforms keywords to [keyword, count] pairs |
| Aggregation | `count()` with predicate replaces manual counting loops |
| Custom Sorting | `sort()` with comparator for descending order |
| Nested Lists | Results stored as `[[kw1, count1], [kw2, count2], ...]` |

### 6.4 Architecture Diagram

```
┌─────────────────┐     ┌─────────────────┐
│  keywords.txt   │     │   data/*.txt    │
└────────┬────────┘     └────────┬────────┘
         │                       │
         ▼                       ▼
┌─────────────────┐     ┌─────────────────┐
│   HeteroList    │     │   HeteroList    │
│   [keywords]    │     │   [all words]   │
└────────┬────────┘     └────────┬────────┘
         │                       │
         └───────────┬───────────┘
                     ▼
┌─────────────────────────────────────────┐
│  keywords.map([&](keyword) {            │
│      return [keyword, allWords.count()] │
│  })                                     │
└────────────────────┬────────────────────┘
                     ▼
         ┌─────────────────────┐
         │  results.sort()     │
         │  results.forEach()  │
         └─────────────────────┘
```

---

## 7. Use Case 2: Network Stream Processor

### 7.1 Overview

A client-server application demonstrating **real-time streaming** of heterogeneous data over TCP/IP sockets. The server generates random data of mixed types, and the client receives, categorizes, sorts, and displays live statistics.

### 7.2 How It Uses the Library

```cpp
// Client maintains three lists by type
HeteroList integers_, doubles_, strings_;

// Parse incoming data
Value parseValue(const std::string& line) {
    auto [type, valueStr] = split(line, ':');
    if (type == "INT")    return Value(std::stoi(valueStr));
    if (type == "DOUBLE") return Value(std::stod(valueStr));
    if (type == "STRING") return Value(valueStr);
    return Value();
}

// Categorize by type
if (val.isInt()) integers_.push(val);
else if (val.isDouble()) doubles_.push(val);
else if (val.isString()) strings_.push(val);

// Real-time aggregations
integers_.sum();
integers_.average();
integers_.min();
integers_.max();
integers_.countInversions();
strings_.frequencies();
```

### 7.3 Features Demonstrated

| Feature | Library Methods Used |
|---------|---------------------|
| Type Checking | `isInt()`, `isDouble()`, `isString()` |
| Real-Time Sorting | `sort()` called on each update |
| Aggregations | `sum()`, `average()`, `min()`, `max()` |
| Disorder Metric | `countInversions()` |
| Frequency Analysis | `frequencies()` |
| Heterogeneous Storage | All types in `HeteroList` |

### 7.4 Architecture Diagram

```
┌─────────────────┐         TCP/IP          ┌─────────────────────────────┐
│     SERVER      │ ────────────────────▶   │          CLIENT             │
│                 │   INT:42\n               │                             │
│ Generates:      │   DOUBLE:3.14\n          │  ┌─────────────────────┐   │
│ - Random ints   │   STRING:alpha\n         │  │     HeteroList      │   │
│ - Random doubles│                          │  │  [mixed types]      │   │
│ - Random strings│                          │  └─────────────────────┘   │
└─────────────────┘                          │            │               │
                                             │            ▼               │
                                             │  ┌─────────────────────┐   │
                                             │  │   Sort & Aggregate  │   │
                                             │  │   Display Stats     │   │
                                             │  └─────────────────────┘   │
                                             └─────────────────────────────┘
```

---

## 8. Use Case 3: CLI Application

### 8.1 Overview

A command-line interface that wraps the HeteroList library with **JSON-based input/output**. This allows external programs (like Python scripts) to use our C++ implementation through subprocess calls.

### 8.2 Design Rationale

We wanted to demonstrate that our library could be used from other languages. The CLI accepts JSON arrays as input and returns JSON results, making it easy to integrate with Python, Node.js, or shell scripts.

### 8.3 Commands Supported

| Command | Example | Output |
|---------|---------|--------|
| `create` | `./hetero_cli create "[1, 2, \"hello\"]"` | `{"result": [...], "size": 3}` |
| `sort` | `./hetero_cli sort "[5, 3, 8]" asc` | `{"result": [3, 5, 8]}` |
| `filter` | `./hetero_cli filter "[1, \"hi\", 2]" int` | `{"result": [1, 2]}` |
| `map` | `./hetero_cli map "[1, 2, 3]" square` | `{"result": [1, 4, 9]}` |
| `reduce` | `./hetero_cli reduce "[10, 20]" sum` | `{"result": 30}` |
| `stats` | `./hetero_cli stats "[1, 2, 3]"` | `{"sum": 6, "average": 2, ...}` |
| `partition` | `./hetero_cli partition "[1, 2, 3]" even` | `{"matching": [2], "non_matching": [1, 3]}` |

### 8.4 Map Operations

| Operation | Effect |
|-----------|--------|
| `double` | Multiply numeric values by 2 |
| `square` | Square numeric values |
| `negate` | Negate numeric values |
| `increment` | Add 1 |
| `decrement` | Subtract 1 |
| `uppercase` | Convert strings to uppercase |
| `lowercase` | Convert strings to lowercase |

### 8.5 Filter Types

| Type | Matches |
|------|---------|
| `int` | Integer values only |
| `double` | Floating-point values only |
| `string` | String values only |
| `bool` | Boolean values only |
| `numeric` | Int + Double |
| `even` / `odd` | Even/odd integers |
| `positive` / `negative` | Sign-based filtering |

### 8.6 Graceful Error Handling

For operations requiring numeric values on heterogeneous lists, the CLI filters to numeric values first and includes metadata:

```bash
./hetero_cli stats "[1, \"hello\", 2, true, 3]"
# Output: {"sum": 6, "average": 2, "min": 1, "max": 3, "count": 3, "total_elements": 5}
```

---

## 9. Use Case 4: Web Application

### 9.1 Overview

A Flask-based web interface with Bootstrap 5 dark theme that provides an **interactive UI** for all HeteroList operations. This is the showcase use case that ties everything together.

### 9.2 Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     Web Browser (UI)                            │
│                  Bootstrap 5 + JavaScript                       │
└──────────────────────────┬──────────────────────────────────────┘
                           │ HTTP/JSON
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│                    Flask Server (app.py)                        │
│                    REST API Endpoints                           │
└──────────────────────────┬──────────────────────────────────────┘
                           │ subprocess
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│             C++ CLI (../CLIApp/hetero_cli)                      │
│         Uses HeteroList Implementation Library                  │
└─────────────────────────────────────────────────────────────────┘
```

**Key Design Decision:** We deliberately did NOT reimplement the library in Python. Instead, the Flask server calls the compiled C++ CLI via subprocess. This demonstrates:
1. Cross-language interoperability
2. Reuse of the same implementation
3. The CLI as an integration layer

### 9.3 API Endpoints

| Endpoint | Parameters | Description |
|----------|------------|-------------|
| `/api/create` | `list` | Validate a list |
| `/api/sort` | `list`, `order` | Sort asc/desc |
| `/api/filter` | `list`, `type` | Filter by predicate |
| `/api/map` | `list`, `operation` | Apply transformation |
| `/api/reduce` | `list`, `operation` | Reduce to single value |
| `/api/stats` | `list` | Get statistics |
| `/api/unique` | `list` | Remove duplicates |
| `/api/reverse` | `list` | Reverse order |
| `/api/take` | `list`, `n` | Take first n |
| `/api/drop` | `list`, `n` | Drop first n |
| `/api/count` | `list` | Count by type |
| `/api/inversions` | `list` | Count inversions |
| `/api/frequencies` | `list` | Element frequencies |
| `/api/partition` | `list`, `type` | Split by predicate |
| `/api/push` | `list`, `value` | Add element |
| `/api/pop` | `list` | Remove last |

### 9.4 UI Features

- **Dark Theme**: Modern gradient-based styling
- **Type Badges**: Color-coded badges (int=blue, double=purple, string=green, bool=red)
- **Quick Add**: Buttons to add random values
- **Sample Data**: Pre-loaded datasets
- **Apply Results**: Use operation result as new input
- **History**: Track and restore previous operations
- **Statistics Cards**: Visual display of sum, average, min, max
- **Frequency Bars**: Visual bar chart for frequencies

### 9.5 Cross-Platform Support

On Windows, the Flask server detects the OS and uses WSL to run the Linux-compiled CLI:

```python
def get_wsl_path(windows_path):
    # Convert D:\path\to\file to /mnt/d/path/to/file
    ...

if platform.system() == 'Windows':
    cmd = ['wsl'] + cmd  # Prepend wsl
```

---

## 10. Testing Strategy

### 10.1 Unit Tests

We created a comprehensive test file (`Tests/test_implementation.cpp`) with 20 test cases covering:

| Test Category | What's Tested |
|---------------|---------------|
| Basic Operations | push, pop, insert, remove, get, set |
| Functional Operations | map, filter, reduce, forEach |
| Aggregations | sum, average, min, max |
| Sorting | sort, sortDescending, countInversions |
| Type Handling | int, double, string, bool, null, nested lists |
| Edge Cases | empty lists, single elements, type mismatches |

### 10.2 Running Tests

```bash
make test
```

All 20 tests pass:

```
Running Tests...

Test 1: Basic Push and Pop... PASSED
Test 2: Insert and Remove... PASSED
Test 3: Get and Set... PASSED
...
Test 20: Nested Lists... PASSED

=== All 20 tests passed! ===
```

### 10.3 Incremental Testing

We tested each component as it was built:
1. **Value class**: Created standalone test for type conversion and operators
2. **ArrayList**: Tested push/pop/insert/remove before building HeteroList
3. **HeteroList**: Tested functional methods one by one
4. **CLI**: Tested each command manually with various inputs
5. **Web App**: Tested each API endpoint via browser and curl

---

## 11. Packaging and Build System

### 11.1 Makefile Targets

We created a single Makefile that builds and runs all components:

| Target | Description |
|--------|-------------|
| `make` | Build and run UC4 (Web App) - **default** |
| `make uc1` | Build and run Keyword Frequency Counter |
| `make uc2` | Build and run Network Stream (server + client) |
| `make uc3` | Build CLI App and show usage |
| `make uc4` | Build CLI and run Flask Web App |
| `make all` | Build all use cases |
| `make test` | Build and run unit tests |
| `make clean` | Remove all executables |
| `make help` | Show all targets |

### 11.2 Automatic Dependency Installation

The Makefile automatically installs Flask if not present:

```makefile
uc4: $(CLI_EXE)
    @python3 -c "import flask" 2>/dev/null || pip3 install flask
    cd $(WEB_DIR) && python3 app.py
```

### 11.3 Single Command Execution

As required by the assignment, a single command runs the entire application:

```bash
# Default: Run the Web Application
make

# Or specifically:
make uc4
```

This will:
1. Compile the C++ CLI if needed
2. Install Flask if not present
3. Start the Flask server
4. Open the browser to http://127.0.0.1:5000

### 11.4 Project Structure

```
Final_Assignment/
├── Implementation/          # Core library (header-only)
│   ├── myList.hpp          # Main include file
│   ├── Value.hpp           # Type-erased value wrapper
│   ├── HeteroList.hpp      # Main interface
│   ├── AbstractList.hpp    # Abstract base class
│   ├── ArrayList.hpp       # Dynamic array
│   ├── Stack.hpp, Queue.hpp, Deque.hpp
│   ├── PriorityQueue.hpp
│   ├── SinglyLinkedList.hpp, DoublyLinkedList.hpp
│   └── README.md
│
├── Tests/
│   └── test_implementation.cpp
│
├── UseCases/
│   ├── KeywordFrequency/   # UC1
│   ├── NetworkStream/      # UC2
│   ├── CLIApp/             # UC3
│   ├── WebApp/             # UC4
│   └── README.md
│
├── Makefile                # Build system
├── README.md               # Quick start guide
└── Report.md               # This file
```

---

## 12. LLM Assistance Documentation

We used Claude (Anthropic's AI assistant) throughout the development process. Here we document the prompts used, responses received, and how we incorporated the insights.

### 12.1 Initial Library Design

**Our Prompt:**
> "We need to extend our heterogeneous list library to support functional operations. We already have the contain-and-delegate design from earlier. Help us implement the Value class for type erasure."

**Claude's Response (summarized):**
Claude suggested a Value class with a Type enum, separate member variables for each type, and provided implementations for constructors, type predicates, and conversion methods.

**Our Interpretation:**
We agreed with this approach because it cleanly separates specification from implementation. We asked for more details on handling cross-type comparisons.

**Incorporation:**
Claude helped implement the `Value` class with its type enum, constructors, and operators. We reviewed the code and made some adjustments to the comparison operators to handle Int vs Double comparisons properly.

---

### 12.2 Functional Operations

**Our Prompt:**
> "Add functional programming methods like map, filter, and reduce to the HeteroList class. These should work with lambda functions."

**Claude's Response:**
Claude provided implementations using `std::function` to accept lambdas. The `map` function creates a new list with transformed elements, `filter` keeps elements matching a predicate, and `reduce` combines elements into a single value.

**Our Interpretation:**
The implementations looked correct. We tested them with simple examples to verify behavior.

**Incorporation:**
We added these methods to `HeteroList.hpp`. Later, we asked Claude to add more methods like `any`, `all`, `count`, `find`, `take`, `drop`, etc.

---

### 12.3 Use Case 1: Keyword Frequency

**Our Prompt:**
> "Create a keyword frequency counter that demonstrates the functional programming approach. Instead of using hash maps and loops, use our library's map, count, and forEach methods."

**Claude's Response:**
Claude created `KeywordFrequency.cpp` that reads keywords into a HeteroList, reads all words from files into another HeteroList, then uses `map()` with `count()` to transform keywords into [keyword, count] pairs.

**Our Suggestion:**
> "Can you also add a summary section that shows total files processed, most frequent keyword, etc.?"

Claude added the summary output with statistics.

**Incorporation:**
This became our UC1, demonstrating that functional operations can replace traditional imperative patterns.

---

### 12.4 Use Case 2: Network Stream

**Our Prompt:**
> "Create a client-server application that streams heterogeneous data over sockets. The server should generate random ints, doubles, and strings. The client should receive them, store in HeteroList, and show real-time sorted data and statistics."

**Claude's Response:**
Claude created `Server.cpp` and `Client.cpp` with TCP socket communication. The client maintains separate lists by type and displays live statistics using our library's aggregation methods.

**Incorporation:**
We tested this locally and it worked well. The real-time display shows sorted data and statistics updating as new data arrives.

---

### 12.5 Use Case 3 & 4: CLI and Web App

**Our Prompt:**
> "Make a menu-driven app which uses our library to perform functions on heterogeneous lists. Don't create a library in Python - use the underlying C++ compiled versions. Make it locally hosted and basically create APIs which use our C++ implementation."

**Claude's Response:**
Claude suggested creating a CLI wrapper (`hetero_cli.cpp`) that accepts JSON input and returns JSON output, then a Flask server that calls this CLI via subprocess. The web UI would be built with Bootstrap.

**Our Suggestion:**
> "When I click Stats or Inversions on a heterogeneous list, it gives errors. Make it more graceful - filter to numeric values first."

Claude modified the CLI to handle heterogeneous lists gracefully by filtering to numeric values before operations like stats and inversions, and including metadata about how many values were used.

**Another Suggestion:**
> "Add descriptions of the specific options provided under each type of operation too. Examples: x² under map."

Claude added detailed operation tables to the README with examples showing input → output.

**Incorporation:**
The CLI and Web App became UC3 and UC4. The architecture demonstrates cross-language interoperability while reusing the same C++ implementation.

---

### 12.6 Makefile and Packaging

**Our Prompt:**
> "Create a Makefile for Linux which can run all the use cases depending on which one they make. By default run UC4 (web app)."

**Claude's Response:**
Claude created a Makefile with targets for each use case, plus `all`, `test`, `clean`, and `help`.

**Our Suggestion:**
> "I changed uc2 slightly so that both server and client run. Is there a better way to do this? Also, web app requires flask, so can you install it using pip if it doesn't exist?"

Claude improved the UC2 target to use backgrounding with sleep, and added Flask auto-install to UC4.

---

### 12.7 Summary of LLM Usage

| Task | Our Role | Claude's Role |
|------|----------|---------------|
| Architecture Design | Defined requirements, chose patterns | Suggested implementations |
| Value Class | Reviewed and tested | Implemented type erasure |
| Functional Methods | Specified which methods to add | Implemented map, filter, reduce, etc. |
| Use Cases | Defined what each should demonstrate | Implemented the code |
| Error Handling | Identified edge cases | Added graceful handling |
| Documentation | Specified structure and content | Wrote detailed READMEs |
| Testing | Ran tests, reported issues | Fixed bugs, added test cases |
| Packaging | Specified requirements | Created Makefile |

**Key Insight:**
Using an LLM significantly accelerated development, especially for boilerplate code and documentation. However, we still needed to:
- Define clear requirements and architecture
- Review all generated code
- Test thoroughly
- Suggest improvements and edge cases
- Make design decisions

The LLM was most helpful for implementing well-defined tasks and creating documentation, but the design decisions and problem interpretation remained our responsibility.

---

## 13. Conclusion

### 13.1 What We Built

We created a comprehensive heterogeneous list library in C++ that demonstrates:
1. **Data Abstraction**: The `Value` class hides type details, the `HeteroList` class hides storage details
2. **Data Independence**: Users interact with `HeteroList` without knowing about `ArrayList` internals
3. **Functional-OO Integration**: Higher-order functions (`map`, `filter`, `reduce`) with object-oriented structure
4. **Practical Applications**: Four use cases showing real-world applicability

### 13.2 Design Philosophy Validation

Our contain-and-delegate approach proved effective:
- Clean separation between specification (HeteroList) and implementation (ArrayList)
- Easy to add new functional methods without changing storage
- Could swap ArrayList for LinkedList without affecting user code

### 13.3 Lessons Learned

1. **Type Erasure is Powerful**: The `Value` class enables Python-like flexibility in C++
2. **Functional Operations are Composable**: Chaining `filter().map().reduce()` is elegant
3. **Cross-Language Integration Works**: JSON CLI enables use from Python without reimplementation
4. **Testing Early Helps**: Catching issues at the unit test level saved debugging time later

### 13.4 Final Deliverable

A single command runs the complete application:

```bash
make
```

This compiles everything, installs dependencies, and launches the web interface at http://127.0.0.1:5000, providing interactive access to all library features.

---

*Report prepared for CS F301 - Programming Languages, November 2025*
