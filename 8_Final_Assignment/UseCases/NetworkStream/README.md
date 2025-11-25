# Real-Time Heterogeneous Data Stream Processor

A client-server application demonstrating real-time ingestion, sorting, and aggregation of mixed-type data using the `HeteroList` library.

## Overview

This use case shows how `HeteroList` can handle **streaming heterogeneous data** in real-time:

1. **Server** continuously generates random data of different types (integers, doubles, strings)
2. **Client** receives the stream, parses values, and stores them in `HeteroList`
3. **Real-time display** shows sorted data and aggregation statistics

## Architecture

```
┌─────────────────┐         TCP/IP          ┌─────────────────────────────┐
│     SERVER      │ ────────────────────▶   │          CLIENT             │
│                 │   INT:42\n               │                             │
│ Generates:      │   DOUBLE:3.14\n          │  ┌─────────────────────┐   │
│ - Random ints   │   STRING:alpha\n         │  │     HeteroList      │   │
│ - Random doubles│   INT:-17\n              │  │  ┌───┬───┬───┬───┐  │   │
│ - Random strings│   ...                    │  │  │42 │3.14│"α"│-17│  │   │
│                 │                          │  │  └───┴───┴───┴───┘  │   │
└─────────────────┘                          │  └─────────────────────┘   │
                                             │            │               │
                                             │            ▼               │
                                             │  ┌─────────────────────┐   │
                                             │  │   Sort & Display    │   │
                                             │  │   - By type         │   │
                                             │  │   - Aggregations    │   │
                                             │  │   - Statistics      │   │
                                             │  └─────────────────────┘   │
                                             └─────────────────────────────┘
```

## Features Demonstrated

### 1. Heterogeneous Storage
```cpp
HeteroList allData;
allData.push(42);           // Integer
allData.push(3.14);         // Double
allData.push("hello");      // String
// All in the same list!
```

### 2. Real-Time Sorting
```cpp
HeteroList sortedInts = integers_;
sortedInts.sort();  // Continuous re-sorting as data arrives
```

### 3. Type-Based Categorization
```cpp
if (val.isInt()) {
    integers_.push(val);
} else if (val.isDouble()) {
    doubles_.push(val);
} else if (val.isString()) {
    strings_.push(val);
}
```

### 4. Aggregation Methods
```cpp
integers_.sum()           // Sum of all integers
integers_.average()       // Average value
integers_.min()           // Minimum
integers_.max()           // Maximum
integers_.countInversions() // Disorder metric
strings_.unique()         // Unique string count
strings_.frequencies()    // Word frequency map
```

### 5. Heterogeneous Comparison
When sorting the combined list, `Value` comparison uses type ordering:
- Numbers (Int/Double) compare numerically
- Strings compare lexicographically
- Cross-type: `Null < Bool < Int < Double < String < List`

## Protocol

Data is sent as newline-delimited text with format `TYPE:VALUE`:

| Format | Example | Parsed As |
|--------|---------|-----------|
| `INT:<number>` | `INT:42` | `Value(42)` (integer) |
| `DOUBLE:<number>` | `DOUBLE:3.14` | `Value(3.14)` (double) |
| `STRING:<text>` | `STRING:hello` | `Value("hello")` (string) |

## Usage

### Step 1: Compile Both Programs

```bash
# Compile server
g++ -std=c++17 -o StreamServer Server.cpp -pthread

# Compile client  
g++ -std=c++17 -o StreamClient Client.cpp -pthread
```

### Step 2: Start the Server

```bash
./StreamServer [port]
```

Default port is 9000. The server will wait for client connections.

### Step 3: Start the Client (in another terminal)

```bash
./StreamClient [host] [port]
```

Default: `127.0.0.1:9000`

### Example Session

**Terminal 1 (Server):**
```
=== Heterogeneous Data Stream Server ===
Listening on port 9000...
Data types: INT, DOUBLE, STRING
Press Ctrl+C to stop.

Connection from: 127.0.0.1
Client connected! Streaming data...
[1] Sent: INT:742
[2] Sent: DOUBLE:-45.23
[3] Sent: STRING:lambda
[4] Sent: INT:-156
[5] Sent: STRING:apple
...
```

**Terminal 2 (Client):**
```
╔══════════════════════════════════════════════════════════════════╗
║       REAL-TIME HETEROGENEOUS DATA STREAM PROCESSOR              ║
╠══════════════════════════════════════════════════════════════════╣
║ Connected to: 127.0.0.1:9000       Total Received: 47            ║
╚══════════════════════════════════════════════════════════════════╝

┌─────────────────────────────────────────────────────────────────┐
│ INTEGERS (18 values) | Sum: 1234 | Avg: 68.6 | Min: -892 | Max: 967
├─────────────────────────────────────────────────────────────────┤
│ Sorted: -892, -654, -321, -156, 42, 88, 167, 234, 456, 742, 967
│ Inversions (unsorted): 45
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ DOUBLES (15 values) | Sum: 234.56 | Avg: 15.64
├─────────────────────────────────────────────────────────────────┤
│ Sorted: -89.23, -45.23, -12.50, 3.14, 15.67, 45.89, 67.23, 99.99
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ STRINGS (14 values) | Unique: 10
├─────────────────────────────────────────────────────────────────┤
│ Sorted: alpha, apple, beta, delta, gamma, kappa, lambda, sigma
│ Frequencies: "apple"(3) "lambda"(2) "alpha"(2) "beta"(1) ...
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ ALL DATA - SORTED (heterogeneous comparison)                    │
├─────────────────────────────────────────────────────────────────┤
│ -892, -654, -321, -89.2, -45.2, 3.1, 42, "alpha", "apple", ...
└─────────────────────────────────────────────────────────────────┘

Press Ctrl+C to stop...
```

## Code Highlights

### Server: Random Data Generation
```cpp
std::string generateValue() {
    int type = typeDist_(rng_);  // Random type: 0=INT, 1=DOUBLE, 2=STRING
    
    switch (type) {
        case 0:  // Integer
            return "INT:" + std::to_string(intDist_(rng_));
        case 1:  // Double
            return "DOUBLE:" + formatDouble(doubleDist_(rng_));
        case 2:  // String
            return "STRING:" + WORD_POOL[wordDist_(rng_)];
    }
}
```

### Client: Parsing into Value
```cpp
Value parseValue(const std::string& line) {
    auto [type, valueStr] = split(line, ':');
    
    if (type == "INT")    return Value(std::stoi(valueStr));
    if (type == "DOUBLE") return Value(std::stod(valueStr));
    if (type == "STRING") return Value(valueStr);
    
    return Value();  // null
}
```

### Client: Real-Time Stats Display
```cpp
// Aggregations on integers
std::cout << "Sum: " << integers_.sum().asInt();
std::cout << "Avg: " << integers_.average().asDouble();
std::cout << "Min: " << integers_.min().asInt();
std::cout << "Max: " << integers_.max().asInt();
std::cout << "Inversions: " << integers_.countInversions();

// Sort and display
HeteroList sorted = integers_;
sorted.sort();

// String frequencies
auto freq = strings_.frequencies();
```

## Library Methods Used

| Method | Purpose |
|--------|---------|
| `push()` | Add incoming values to lists |
| `sort()` | Sort values for display |
| `sum()` | Total of numeric values |
| `average()` | Mean of numeric values |
| `min()` / `max()` | Extreme values |
| `countInversions()` | Measure of disorder |
| `unique()` | Count distinct strings |
| `frequencies()` | Word occurrence counts |
| `isInt()` / `isDouble()` / `isString()` | Type checking for categorization |
| `asInt()` / `asDouble()` / `asString()` | Type conversion for display |

## Design Decisions

1. **Separate Lists by Type**: While all data goes into `allData_`, we also maintain separate lists (`integers_`, `doubles_`, `strings_`) for type-specific aggregations.

2. **Thread Safety**: Uses `std::mutex` to protect list access between receiver thread and display thread.

3. **Protocol Simplicity**: Plain text `TYPE:VALUE` format is human-readable and easy to debug.

4. **Real-Time Updates**: Display refreshes every 500ms to show latest sorted data.

5. **Graceful Degradation**: Shows "(waiting for data...)" when lists are empty.

## Files

```
NetworkStream/
├── Server.cpp      # Data stream generator
├── Client.cpp      # Receiver with HeteroList processing
├── StreamServer    # Compiled server (after build)
├── StreamClient    # Compiled client (after build)
└── README.md       # This file
```

## Requirements

- C++17 compiler (g++ 8+ or clang++ 7+)
- POSIX sockets (built-in on Linux/macOS)
- pthreads for multithreading
