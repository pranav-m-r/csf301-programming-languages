# HeteroList: A Functional-OO Heterogeneous List Library

## Project Structure

```
Final_Assignment/
├── Implementation/          # Core library (header-only)
│   ├── myList.hpp          # Main include file
│   ├── Value.hpp           # Type-erased value wrapper
│   ├── HeteroList.hpp      # Heterogeneous list (extends ArrayList)
│   ├── AbstractList.hpp    # Abstract base class
│   ├── ArrayList.hpp       # Dynamic array implementation
│   ├── SinglyLinkedList.hpp
│   ├── DoublyLinkedList.hpp
│   ├── Stack.hpp
│   ├── Queue.hpp
│   ├── Deque.hpp
│   └── PriorityQueue.hpp
│
├── Tests/                   # Unit tests
│   └── test_implementation.cpp
│
├── UseCases/                # Practical applications
│   ├── KeywordFrequency/   # UC1: Keyword counter (functional approach)
│   ├── NetworkStream/      # UC2: Real-time data stream processor
│   ├── CLIApp/             # UC3: JSON-based CLI interface
│   └── WebApp/             # UC4: Flask web application
│
├── Makefile                # Build and run targets (Linux/Mac)
└── README.md               # This file
```

## Requirements

- **C++17** compatible compiler (g++ 8+ or clang++ 7+)
- **Python 3** with pip (for UC4 Web App)
- **Linux/macOS** (or WSL on Windows)

## Quick Start

```bash
# Run the default use case (Web App)
make

# Or run a specific use case
make uc1    # Keyword Frequency Counter
make uc2    # Network Stream Processor
make uc3    # CLI Application (build only)
make uc4    # Web Application (default)
```

## Makefile Targets

| Target | Description |
|--------|-------------|
| `make` | Build and run UC4 (Web App) - **default** |
| `make uc1` | Build and run Keyword Frequency Counter |
| `make uc2` | Build and run Network Stream (server + client) |
| `make uc3` | Build CLI App and show usage |
| `make uc4` | Build CLI and run Flask Web App |
| `make all` | Build all use cases |
| `make test` | Build and run unit tests |
| `make clean` | Remove all compiled executables |
| `make help` | Show all available targets |

### Additional UC2 Targets

```bash
make uc2-server   # Run server only (in one terminal)
make uc2-client   # Run client only (in another terminal)
```

## Use Cases

### UC1: Keyword Frequency Counter
Scans text files and counts keyword occurrences using a pure functional approach with `map()`, `count()`, and `forEach()`.

```bash
make uc1
```

### UC2: Network Stream Processor
Client-server application demonstrating real-time heterogeneous data streaming with live sorting and aggregation.

```bash
make uc2
```

### UC3: CLI Application
JSON-based command-line interface for external program integration.

```bash
make uc3
# Then use:
./UseCases/CLIApp/hetero_cli sort '[5, 3, 8, 1]' asc
./UseCases/CLIApp/hetero_cli map '[1, 2, 3]' square
./UseCases/CLIApp/hetero_cli filter '[1, "hi", 2.5]' numeric
```

### UC4: Web Application
Interactive web UI with Bootstrap dark theme. Flask auto-installs if not present.

```bash
make uc4
# Opens http://127.0.0.1:5000 in browser
```

## Running Tests

```bash
make test
```

Runs 20 unit tests covering:
- Basic operations (push, pop, insert, remove)
- Functional operations (map, filter, reduce)
- Sorting and searching
- Type handling (int, double, string, bool, null, nested lists)
- Edge cases

## Library Features

| Feature | Description |
|---------|-------------|
| Type Erasure | Store any type (int, double, string, bool, null, nested lists) |
| Functional Ops | `map()`, `filter()`, `reduce()`, `forEach()` |
| Aggregations | `sum()`, `average()`, `min()`, `max()`, `count()` |
| Sorting | Custom comparators, `countInversions()` |
| Analysis | `frequencies()`, `unique()`, `partition()` |
| Slicing | `take()`, `drop()`, `slice()` |

## Example Usage

```cpp
#include "Implementation/myList.hpp"

int main() {
    HeteroList list;
    list.push(42);
    list.push(3.14);
    list.push("hello");
    list.push(true);
    
    // Functional operations
    auto doubled = list.map([](const Value& v) {
        return v.isNumeric() ? Value(v.asDouble() * 2) : v;
    });
    
    auto numbers = list.filter([](const Value& v) {
        return v.isNumeric();
    });
    
    auto sum = list.reduce(Value(0), [](const Value& acc, const Value& v) {
        return v.isNumeric() ? Value(acc.asDouble() + v.asDouble()) : acc;
    });
    
    list.sort();  // Type-aware sorting
    
    return 0;
}
```

## Windows Users

Use WSL (Windows Subsystem for Linux) to run the Makefile:

```powershell
wsl make
wsl make test
wsl make uc1
```
