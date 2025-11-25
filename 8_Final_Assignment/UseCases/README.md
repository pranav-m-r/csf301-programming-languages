# Use Cases for Heterogeneous List Library

This folder contains practical applications demonstrating the capabilities of our Functional-OO heterogeneous list library.

---

## Use Case 1: Keyword Frequency Counter

**Folder:** `KeywordFrequency/`

A command-line application that scans text files in a directory and counts occurrences of specified keywords, outputting results in descending frequency order. Demonstrates a **pure functional programming approach** using the HeteroList library.

**Features demonstrated:**
- File I/O with recursive directory scanning
- HeteroList for storing keyword-count pairs as nested lists
- Functional operations: `map()`, `count()`, `forEach()`, `sort()`
- Aggregation using `count()` with predicates instead of manual loops
- Custom comparators for sorting

**Usage:**
```bash
./KeywordFrequency <data_directory> <keyword_file>
```

---

## Use Case 2: Network Stream Processor

**Folder:** `NetworkStream/`

A client-server application demonstrating real-time ingestion, sorting, and aggregation of mixed-type data streaming over TCP/IP sockets.

**Features demonstrated:**
- Real-time heterogeneous data streaming (INT, DOUBLE, STRING)
- Continuous sorting as data arrives
- Type-based categorization into separate lists
- Live aggregation: `sum()`, `average()`, `min()`, `max()`, `countInversions()`
- Thread-safe list operations with mutex protection
- Heterogeneous comparison for cross-type sorting

**Usage:**
```bash
# Terminal 1: Start server
./StreamServer [port]

# Terminal 2: Start client
./StreamClient [host] [port]
```

---

## Use Case 3: CLI Application

**Folder:** `CLIApp/`

A command-line interface that wraps the HeteroList library, providing JSON-based interaction for external programs. Designed as a bridge between the C++ implementation and other languages.

**Features demonstrated:**
- JSON parsing and serialization of heterogeneous data
- All core operations exposed via command-line arguments
- Graceful error handling for type mismatches
- Integration-ready design for subprocess calls

**Usage:**
```bash
./hetero_cli <command> [arguments...]

# Examples:
./hetero_cli sort "[5, 3, 8, 1]" asc
./hetero_cli filter "[1, \"hello\", 2.5]" numeric
./hetero_cli map "[1, 2, 3]" square
./hetero_cli reduce "[10, 20, 30]" sum
```

---

## Use Case 4: Web Application

**Folder:** `WebApp/`

A Flask-based web interface with Bootstrap 5 dark-themed UI for interactively performing operations on heterogeneous lists. Uses the C++ CLI backend via subprocess calls.

**Features demonstrated:**
- REST API bridging Python/Flask to C++ implementation
- Cross-platform support (Windows via WSL, Linux/Mac native)
- Real-time statistics and frequency visualization
- Operation history with restore capability
- Type-aware display with color-coded badges

**Usage:**
```bash
# 1. Compile CLI (in CLIApp folder)
g++ -std=c++17 -o hetero_cli hetero_cli.cpp

# 2. Run Flask server
cd WebApp
python app.py

# 3. Open browser
# Navigate to http://127.0.0.1:5000
```

---

## Summary of Library Features Demonstrated

| Feature | UC1 | UC2 | UC3 | UC4 |
|---------|:---:|:---:|:---:|:---:|
| `push()` / `pop()` | ✓ | ✓ | ✓ | ✓ |
| `sort()` | ✓ | ✓ | ✓ | ✓ |
| `map()` | ✓ | | ✓ | ✓ |
| `filter()` | | | ✓ | ✓ |
| `reduce()` | | | ✓ | ✓ |
| `count()` | ✓ | | ✓ | ✓ |
| `forEach()` | ✓ | | | |
| `sum()` / `average()` | | ✓ | ✓ | ✓ |
| `min()` / `max()` | | ✓ | ✓ | ✓ |
| `frequencies()` | | ✓ | ✓ | ✓ |
| `countInversions()` | | ✓ | ✓ | ✓ |
| `unique()` | | ✓ | ✓ | ✓ |
| `partition()` | | | ✓ | ✓ |
| `take()` / `drop()` | | | ✓ | ✓ |
| Nested Lists | ✓ | | | |
| Type Checking | | ✓ | ✓ | ✓ |
| JSON I/O | | | ✓ | ✓ |
| Network Streaming | | ✓ | | |
| Web Interface | | | | ✓ |
