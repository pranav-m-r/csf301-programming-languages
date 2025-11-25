# HeteroList Web Application

A Flask-based web interface that provides a menu-driven UI for performing operations on heterogeneous lists using the underlying C++ HeteroList library.

## Architecture

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
│              (Windows: Uses WSL for C++ backend)                │
└──────────────────────────┬──────────────────────────────────────┘
                           │ subprocess
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│             C++ CLI (../CLIApp/hetero_cli)                      │
│         Uses HeteroList Implementation Library                  │
└─────────────────────────────────────────────────────────────────┘
```

## Features

### Operations Available

| Category   | Operations                                                |
|------------|-----------------------------------------------------------|
| Transform  | Sort (asc/desc), Reverse, Unique                          |
| Map        | Double, Square, Negate, Increment, Decrement, Upper/Lower |
| Filter     | By type (int/double/string/bool), Even/Odd, +ve/-ve       |
| Reduce     | Sum, Product, Concatenate                                 |
| Analysis   | Statistics, Type Count, Frequencies, Inversions           |
| Slice      | Take N, Drop N                                            |
| Partition  | By type or predicate                                      |

### Operation Details

#### Transform Operations
| Button | Description |
|--------|-------------|
| **Sort ↑** | Sort list in ascending order (nulls < bools < numbers < strings) |
| **Sort ↓** | Sort list in descending order |
| **Reverse** | Reverse the order of elements |
| **Unique** | Remove duplicate elements, keeping first occurrence |

#### Map Operations (apply to each element)
| Button | Description | Example |
|--------|-------------|---------|
| **×2** | Double numeric values | `[1, 2, 3]` → `[2, 4, 6]` |
| **x²** | Square numeric values | `[2, 3, 4]` → `[4, 9, 16]` |
| **±** | Negate numeric values | `[1, -2, 3]` → `[-1, 2, -3]` |
| **+1** | Increment numeric values by 1 | `[1, 2, 3]` → `[2, 3, 4]` |
| **-1** | Decrement numeric values by 1 | `[1, 2, 3]` → `[0, 1, 2]` |
| **ABC** | Convert strings to uppercase | `["hello"]` → `["HELLO"]` |
| **abc** | Convert strings to lowercase | `["WORLD"]` → `["world"]` |

#### Filter Operations (keep matching elements)
| Button | Description | Example |
|--------|-------------|---------|
| **Integers** | Keep only integer values | `[1, 2.5, "hi"]` → `[1]` |
| **Doubles** | Keep only floating-point values | `[1, 2.5, "hi"]` → `[2.5]` |
| **Strings** | Keep only string values | `[1, 2.5, "hi"]` → `["hi"]` |
| **Bools** | Keep only boolean values | `[1, true, "hi"]` → `[true]` |
| **Numeric** | Keep integers and doubles | `[1, 2.5, "hi"]` → `[1, 2.5]` |
| **Even** | Keep even integers | `[1, 2, 3, 4]` → `[2, 4]` |
| **Odd** | Keep odd integers | `[1, 2, 3, 4]` → `[1, 3]` |
| **+ve** | Keep positive numeric values | `[-1, 0, 1, 2]` → `[1, 2]` |
| **-ve** | Keep negative numeric values | `[-2, -1, 0, 1]` → `[-2, -1]` |

#### Reduce Operations (combine into single value)
| Button | Description | Example |
|--------|-------------|---------|
| **Sum** | Add all numeric values | `[10, 20, 30]` → `60` |
| **Product** | Multiply all numeric values | `[2, 3, 4]` → `24` |
| **Concat** | Concatenate all values as strings | `[1, "hi", true]` → `"1hitrue"` |

#### Analysis Operations
| Button | Description |
|--------|-------------|
| **Stats** | Get sum, average, min, max of numeric values |
| **Count Types** | Count elements by type (int, double, string, bool, null) |
| **Frequencies** | Show frequency of each unique element |
| **Inversions** | Count pairs (i,j) where i < j but list[i] > list[j] |

#### Slice Operations
| Button | Description | Example (n=2) |
|--------|-------------|---------------|
| **Take** | Keep first n elements | `[1, 2, 3, 4, 5]` → `[1, 2]` |
| **Drop** | Remove first n elements | `[1, 2, 3, 4, 5]` → `[3, 4, 5]` |

#### Partition Operations (split into two lists)
| Button | Description |
|--------|-------------|
| **Numeric** | Split into numeric vs non-numeric |
| **Int** | Split into integers vs others |
| **String** | Split into strings vs others |
| **Even/Odd** | Split integers into even vs odd |

### UI Features

- **Dark Theme**: Modern gradient-based dark UI
- **Type Badges**: Color-coded badges for different types (int=blue, double=purple, string=green, bool=red, null=gray)
- **Quick Add**: Buttons to quickly add random values of each type
- **Sample Data**: Pre-loaded sample datasets (numbers, mixed, strings)
- **Apply Results**: Apply operation results back to the current list
- **History**: Track and restore previous operations
- **Statistics Display**: Visual stats cards with sum, average, min, max
- **Frequency Bars**: Visual bar chart for element frequencies

## Requirements

- Python 3.7+
- Flask (`pip install flask`)
- G++ with C++17 support
- WSL (Windows Subsystem for Linux) - required on Windows

## Setup & Run

### 1. Compile the C++ CLI (in CLIApp folder)

```bash
# Navigate to the CLIApp directory
cd UseCases/CLIApp

# Compile the CLI (Linux/WSL)
g++ -std=c++17 -o hetero_cli hetero_cli.cpp
```

### 2. Install Python Dependencies

```bash
pip install flask
```

### 3. Run the Application

```bash
cd UseCases/WebApp
python app.py
```

### 4. Open in Browser

Navigate to: **http://127.0.0.1:5000**

## API Endpoints

All endpoints accept POST requests with JSON body.

| Endpoint           | Parameters                        | Description                    |
|--------------------|-----------------------------------|--------------------------------|
| `/api/create`      | `list`                            | Create/validate a list         |
| `/api/sort`        | `list`, `order` (asc/desc)        | Sort the list                  |
| `/api/filter`      | `list`, `type`                    | Filter by type/predicate       |
| `/api/map`         | `list`, `operation`               | Apply map operation            |
| `/api/reduce`      | `list`, `operation`               | Apply reduce operation         |
| `/api/stats`       | `list`                            | Get statistics                 |
| `/api/unique`      | `list`                            | Get unique elements            |
| `/api/reverse`     | `list`                            | Reverse the list               |
| `/api/take`        | `list`, `n`                       | Take first n elements          |
| `/api/drop`        | `list`, `n`                       | Drop first n elements          |
| `/api/count`       | `list`                            | Count elements by type         |
| `/api/inversions`  | `list`                            | Count inversions               |
| `/api/frequencies` | `list`                            | Get element frequencies        |
| `/api/partition`   | `list`, `type`                    | Partition by predicate         |
| `/api/push`        | `list`, `value`                   | Push element to list           |
| `/api/pop`         | `list`                            | Pop element from list          |

## Example Usage

### Using the Web UI

1. **Create a List**: Enter values in the input area like `[1, 2.5, "hello", true]` and click "Create List"
2. **Apply Operations**: Click any operation button to transform the list
3. **View Results**: Results appear in the Result panel with metadata
4. **Apply to List**: Click "Apply to List" to use the result as the new current list

### Using the API Directly

```bash
# Sort a list
curl -X POST http://127.0.0.1:5000/api/sort \
  -H "Content-Type: application/json" \
  -d '{"list": "[5, 3, 8, 1]", "order": "asc"}'

# Get statistics
curl -X POST http://127.0.0.1:5000/api/stats \
  -H "Content-Type: application/json" \
  -d '{"list": "[10, 20, 30, 40, 50]"}'

# Filter by type
curl -X POST http://127.0.0.1:5000/api/filter \
  -H "Content-Type: application/json" \
  -d '{"list": "[1, \"hello\", 2.5, true]", "type": "numeric"}'
```

## How It Demonstrates the Library

This use case demonstrates:

1. **Type Erasure**: The HeteroList can store int, double, string, bool, and null values seamlessly
2. **Functional Programming**: Map, filter, reduce operations work on heterogeneous data
3. **Sorting**: Heterogeneous lists can be sorted using type-aware comparison
4. **Analysis**: Statistics, frequencies, and inversions work across types (gracefully handling non-numeric values)
5. **Interoperability**: C++ library exposed via CLI and consumed by Python/JavaScript
6. **Cross-Platform**: Works on Windows (via WSL) and Linux/Mac natively

## Graceful Error Handling

Operations that require numeric values (Stats, Inversions, Sum, Product) gracefully handle heterogeneous lists:

- **Stats**: Filters to numeric values only, shows count of numeric vs total elements
- **Inversions**: Counts inversions only among numeric values
- **Sum/Product**: Operates on numeric values only, reports how many values were used
- **Concat**: Works with all types, converting each to string representation

## File Structure

```
WebApp/
├── app.py              # Flask server with API endpoints
├── README.md           # This file
└── templates/
    └── index.html      # Bootstrap-based web UI

CLIApp/                 # Sibling folder
├── hetero_cli.cpp      # C++ CLI wrapper for HeteroList
├── hetero_cli          # Compiled CLI executable (after build)
└── README.md           # CLI documentation
```

## Screenshots

### Main Interface
- Left panel: List input and operations menu
- Right panel: Current list display, results, stats, and history

### Features Highlighted
- Color-coded type badges
- Real-time statistics
- Frequency visualization
- Operation history with restore capability
