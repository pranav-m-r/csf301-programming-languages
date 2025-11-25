# HeteroList CLI

A command-line interface for the C++ HeteroList library, providing JSON-based interaction with heterogeneous lists.

## Overview

This CLI tool wraps the HeteroList library, allowing external programs (like Python/Flask) to leverage the C++ implementation through subprocess calls. It accepts JSON array inputs and returns JSON-formatted results.

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│              External Program (Python, Bash, etc.)              │
└──────────────────────────┬──────────────────────────────────────┘
                           │ subprocess / shell
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│                    hetero_cli executable                        │
│               JSON input → C++ operations → JSON output         │
└──────────────────────────┬──────────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────────┐
│                 HeteroList Implementation                       │
│              (../../Implementation/myList.hpp)                  │
└─────────────────────────────────────────────────────────────────┘
```

## Build

```bash
# Compile with C++17
g++ -std=c++17 -o hetero_cli hetero_cli.cpp
```

## Usage

```bash
./hetero_cli <command> [arguments...]
```

## Commands

### List Operations

| Command | Arguments | Description |
|---------|-----------|-------------|
| `create` | `<json_array>` | Parse and validate a JSON array |
| `push` | `<json_array> <value>` | Add element to end of list |
| `pop` | `<json_array>` | Remove and return last element |

### Transform Operations

| Command | Arguments | Description |
|---------|-----------|-------------|
| `sort` | `<json_array> [asc\|desc]` | Sort the list (default: ascending) |
| `reverse` | `<json_array>` | Reverse the list |
| `unique` | `<json_array>` | Remove duplicate elements |

### Functional Operations

| Command | Arguments | Description |
|---------|-----------|-------------|
| `map` | `<json_array> <operation>` | Apply transformation to elements |
| `filter` | `<json_array> <type>` | Filter elements by predicate |
| `reduce` | `<json_array> <operation>` | Reduce list to single value |
| `partition` | `<json_array> <type>` | Split list into matching/non-matching |

### Analysis Operations

| Command | Arguments | Description |
|---------|-----------|-------------|
| `stats` | `<json_array>` | Get sum, average, min, max of numeric values |
| `count` | `<json_array>` | Count elements by type |
| `frequencies` | `<json_array>` | Get frequency of each unique element |
| `inversions` | `<json_array>` | Count inversions in numeric values |

### Slice Operations

| Command | Arguments | Description |
|---------|-----------|-------------|
| `take` | `<json_array> <n>` | Take first n elements |
| `drop` | `<json_array> <n>` | Drop first n elements |

## Map Operations

| Operation | Effect |
|-----------|--------|
| `double` | Multiply numeric values by 2 |
| `square` | Square numeric values |
| `negate` | Negate numeric values |
| `increment` | Add 1 to numeric values |
| `decrement` | Subtract 1 from numeric values |
| `uppercase` | Convert strings to uppercase |
| `lowercase` | Convert strings to lowercase |

## Filter Types

| Type | Matches |
|------|---------|
| `int` | Integer values |
| `double` | Floating-point values |
| `string` | String values |
| `bool` | Boolean values |
| `numeric` | All numeric values (int + double) |
| `even` | Even integers |
| `odd` | Odd integers |
| `positive` | Positive numeric values |
| `negative` | Negative numeric values |

## Reduce Operations

| Operation | Result |
|-----------|--------|
| `sum` | Sum of numeric values |
| `product` | Product of numeric values |
| `concat` | Concatenation of all values as strings |

## Examples

### Create a List
```bash
./hetero_cli create "[1, 2, 3.14, \"hello\", true]"
# Output: {"result": [1, 2, 3.14, "hello", true], "size": 5}
```

### Sort
```bash
./hetero_cli sort "[5, 3, 8, 1, 9]" asc
# Output: {"result": [1, 3, 5, 8, 9]}

./hetero_cli sort "[5, 3, 8, 1, 9]" desc
# Output: {"result": [9, 8, 5, 3, 1]}
```

### Filter
```bash
./hetero_cli filter "[1, \"hello\", 2, \"world\", 3]" int
# Output: {"result": [1, 2, 3], "original_size": 5, "filtered_size": 3}

./hetero_cli filter "[1, 2, 3, 4, 5, 6]" even
# Output: {"result": [2, 4, 6], "original_size": 6, "filtered_size": 3}
```

### Map
```bash
./hetero_cli map "[1, 2, 3, 4, 5]" double
# Output: {"result": [2, 4, 6, 8, 10]}

./hetero_cli map "[1, 2, 3, 4, 5]" square
# Output: {"result": [1, 4, 9, 16, 25]}

./hetero_cli map "[\"hello\", \"world\"]" uppercase
# Output: {"result": ["HELLO", "WORLD"]}
```

### Reduce
```bash
./hetero_cli reduce "[10, 20, 30]" sum
# Output: {"result": 60, "numeric_count": 3, "total_count": 3}

./hetero_cli reduce "[1, 2, 3, 4]" product
# Output: {"result": 24, "numeric_count": 4, "total_count": 4}

./hetero_cli reduce "[\"Hello\", \" \", \"World\"]" concat
# Output: {"result": "Hello World"}
```

### Statistics
```bash
./hetero_cli stats "[10, 20, 30, 40, 50]"
# Output: {"sum": 150, "average": 30, "min": 10, "max": 50, "count": 5, "total_elements": 5}

# With mixed types (gracefully handles non-numeric values)
./hetero_cli stats "[1, \"hello\", 2, true, 3]"
# Output: {"sum": 6, "average": 2, "min": 1, "max": 3, "count": 3, "total_elements": 5}
```

### Partition
```bash
./hetero_cli partition "[1, 2, 3, 4, 5, 6]" even
# Output: {"matching": [2, 4, 6], "non_matching": [1, 3, 5]}

./hetero_cli partition "[1, \"hello\", 2, \"world\"]" string
# Output: {"matching": ["hello", "world"], "non_matching": [1, 2]}
```

### Frequencies
```bash
./hetero_cli frequencies "[1, 2, 2, 3, 3, 3]"
# Output: {"frequencies": [{"value": 1, "count": 1}, {"value": 2, "count": 2}, {"value": 3, "count": 3}]}
```

### Inversions
```bash
./hetero_cli inversions "[5, 3, 2, 4, 1]"
# Output: {"inversions": 8, "numeric_count": 5, "total_elements": 5}
```

## Error Handling

The CLI returns JSON error messages for invalid inputs:

```bash
./hetero_cli invalid_command
# Output: {"error": "Invalid command. Available commands: create, sort, filter, map, reduce, stats, unique, reverse, take, drop, count, inversions, frequencies, partition, push, pop"}

./hetero_cli pop "[]"
# Output: {"error": "Cannot pop from empty list"}
```

For operations requiring numeric values on heterogeneous lists, the CLI gracefully filters to numeric values and includes metadata:

```bash
./hetero_cli stats "[\"only\", \"strings\"]"
# Output: {"sum": 0, "average": 0, "min": null, "max": null, "count": 0, "total_elements": 2, "note": "No numeric values found in the list"}
```

## Integration Example (Python)

```python
import subprocess
import json

def call_hetero_cli(command, *args):
    cmd = ['./hetero_cli', command] + list(args)
    result = subprocess.run(cmd, capture_output=True, text=True)
    return json.loads(result.stdout)

# Example usage
result = call_hetero_cli('sort', '[5, 3, 8, 1]', 'asc')
print(result)  # {'result': [1, 3, 5, 8]}
```

## File Structure

```
CLIApp/
├── hetero_cli.cpp    # CLI source code
├── hetero_cli        # Compiled executable (after build)
└── README.md         # This file
```

## Dependencies

- C++17 compatible compiler (g++, clang++)
- HeteroList library (`../../Implementation/myList.hpp`)
