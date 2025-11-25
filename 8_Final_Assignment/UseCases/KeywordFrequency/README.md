# Keyword Frequency Counter

A command-line application that scans text files in a directory and counts occurrences of specified keywords, outputting results in descending frequency order. This application demonstrates a **pure functional programming approach** using the `HeteroList` library.

## Features

- **Recursive directory scanning**: Finds all text files (`.txt`, `.md`, `.csv`, `.log`, `.dat`)
- **Case-insensitive matching**: Keywords and text are normalized to lowercase
- **Punctuation handling**: Removes punctuation when matching words
- **Sorted output**: Results displayed in descending frequency order
- **Summary statistics**: Total files, keywords, occurrences, most/least frequent
- **Pure functional design**: Uses `map`, `count`, `forEach`, and `sort` instead of imperative loops

## Usage

```
KeywordFrequency.exe <data_directory> <keyword_file>
```

### Arguments

| Argument | Description |
|----------|-------------|
| `data_directory` | Directory containing text files to analyze (scanned recursively) |
| `keyword_file` | File containing keywords to search for (whitespace-separated) |

### Example

```bash
# Compile (requires C++17 for std::filesystem)
g++ -std=c++17 -o KeywordFrequency.exe KeywordFrequency.cpp

# Run with sample data
./KeywordFrequency.exe sample_data/data sample_data/keywords.txt
```

## Sample Data

The `sample_data/` directory contains:
- `keywords.txt`: 25 programming-related keywords
- `data/`: 4 text files about programming concepts
  - `intro_ds.txt`: Introduction to data structures
  - `oop_concepts.txt`: Object-oriented programming
  - `algorithms.txt`: Sorting and searching algorithms
  - `memory_pointers.txt`: Memory management and pointers

## Output Format

```
=== Results (Descending Frequency) ===

Rank  Keyword                    Count
----  -------------------------  -----
   1  data                          32
   2  memory                        20
   ...

=== Summary ===
Total files processed: 4
Total keywords tracked: 25
Total words in corpus: 1119
Total keyword occurrences: 188
Most frequent: "data" (32 occurrences)
Least frequent: "loop" (1 occurrences)
```

---

## Library Features Demonstrated

This use case demonstrates the following `HeteroList` operations:

| Operation | Purpose | Description |
|-----------|---------|-------------|
| `HeteroList` | Storage | Heterogeneous list storing strings, integers, and nested lists |
| `push()` | Building | Adding elements to the list |
| `forEach()` | Iteration | Applying a function to each element (side effects) |
| `map()` | Transformation | Transforming each element into a new form |
| `count()` | Aggregation | Counting elements matching a predicate |
| `sort()` | Ordering | Custom sorting with comparator functions |
| `contains()` | Searching | Checking if an element exists |
| `makeList()` | Nesting | Creating nested lists (keyword-count pairs) |
| `asList()` | Extraction | Accessing nested list contents |

---

## Functional Programming Approach

### Design Philosophy

This application follows **functional programming principles**:

1. **Data as Lists**: All data is stored in `HeteroList` objects, not standard containers
2. **Transformation over Mutation**: We transform data using `map()` rather than mutating in-place
3. **Aggregation Functions**: We use `count()` to aggregate rather than manual counting loops
4. **Higher-Order Functions**: Functions like `map()`, `forEach()`, and `sort()` take lambda functions as arguments

### Step-by-Step Breakdown

#### Step 1: Read Keywords into a HeteroList

```cpp
HeteroList keywords = readKeywords(keywordFile);
// Result: ["programming", "algorithm", "data", "structure", ...]
```

Keywords are stored in a `HeteroList` of strings. The `contains()` method ensures no duplicates.

#### Step 2: Collect File Paths using forEach

```cpp
HeteroList files = getTextFiles(dataDir);
// Result: ["data/intro_ds.txt", "data/oop_concepts.txt", ...]
```

File paths are stored in another `HeteroList`, enabling functional iteration.

#### Step 3: Read ALL Words into One Giant HeteroList

```cpp
HeteroList allWords;

files.forEach([&](const Value& filePath) {
    HeteroList fileWords = readWordsFromFile(filePath.asString());
    for (int i = 0; i < fileWords.size(); ++i) {
        allWords.push(fileWords[i]);
    }
});
// Result: ["introduction", "to", "data", "structures", "data", "is", ...]
// (1119 words total)
```

Instead of counting on-the-fly, we collect **all words** into one list. This enables us to use `count()` for aggregation.

#### Step 4: Transform Keywords to [Keyword, Count] Pairs using map() and count()

This is the **core functional operation**:

```cpp
HeteroList results = keywords.map([&allWords](const Value& keyword) {
    // Use count() with a predicate to find occurrences
    int freq = allWords.count([&keyword](const Value& word) {
        return word == keyword;
    });
    // Return a nested list: [keyword, count]
    return Value(makeList({keyword, Value(freq)}));
});
// Result: [["programming", 10], ["algorithm", 12], ["data", 32], ...]
```

**What's happening here:**

1. `keywords.map(...)` transforms each keyword into something else
2. For each keyword, `allWords.count(predicate)` counts how many words match
3. The result is a nested list `[keyword, count]` created with `makeList()`
4. The final `results` is a list of pairs: `[["programming", 10], ["algorithm", 12], ...]`

**Why this is better than imperative:**
- No manual `std::unordered_map` for counting
- No nested `for` loops
- Declarative: "map each keyword to its count"
- Composable: operations chain naturally

#### Step 5: Sort Results by Frequency (Descending)

```cpp
results.sort([](const Value& a, const Value& b) {
    long long countA = (*a.asList())[1].asInt();  // Get count from [keyword, count]
    long long countB = (*b.asList())[1].asInt();
    if (countA != countB) return countA > countB; // Descending
    return (*a.asList())[0].asString() < (*b.asList())[0].asString(); // Alphabetical tie-breaker
});
```

Custom comparator extracts the count from each `[keyword, count]` pair and sorts descending.

#### Step 6: Output Results using forEach

```cpp
results.forEach([&](const Value& pair) {
    auto pairList = pair.asList();
    std::string keyword = (*pairList)[0].asString();
    long long count = (*pairList)[1].asInt();
    
    std::cout << keyword << ": " << count << std::endl;
});
```

`forEach()` applies a side-effect function (printing) to each element.

---

## Comparison: Imperative vs Functional

### Imperative Approach (Traditional)

```cpp
// Using std::unordered_map and manual loops
std::unordered_map<std::string, int> counts;
for (const auto& file : files) {
    std::ifstream in(file);
    std::string word;
    while (in >> word) {
        word = normalize(word);
        if (keywords.count(word)) {
            counts[word]++;
        }
    }
}
// Then convert map to vector for sorting...
```

### Functional Approach (This Implementation)

```cpp
// Using HeteroList functional operations
HeteroList allWords;
files.forEach([&](const Value& f) {
    HeteroList words = readWordsFromFile(f.asString());
    for (int i = 0; i < words.size(); ++i) allWords.push(words[i]);
});

HeteroList results = keywords.map([&](const Value& kw) {
    int freq = allWords.count([&](const Value& w) { return w == kw; });
    return Value(makeList({kw, Value(freq)}));
});

results.sort(/* comparator */);
results.forEach(/* print */);
```

### Key Differences

| Aspect | Imperative | Functional |
|--------|------------|------------|
| Data Structure | `std::unordered_map` | `HeteroList` everywhere |
| Counting | Manual `++` in loop | `count(predicate)` |
| Transformation | Build new container manually | `map()` returns new list |
| Iteration | `for` loops | `forEach()` with lambdas |
| Sorting | Convert to `vector`, use `std::sort` | `sort()` on HeteroList directly |
| Style | Mutation-heavy | Transformation-based |

---

## Architecture Diagram

```
┌─────────────────┐     ┌─────────────────┐
│  keywords.txt   │     │   data/*.txt    │
└────────┬────────┘     └────────┬────────┘
         │                       │
         ▼                       ▼
┌─────────────────┐     ┌─────────────────┐
│   HeteroList    │     │   HeteroList    │
│   [keywords]    │     │   [all words]   │
│ 25 unique items │     │  1119 words     │
└────────┬────────┘     └────────┬────────┘
         │                       │
         │    ┌──────────────────┘
         │    │
         ▼    ▼
┌─────────────────────────────────────────┐
│  keywords.map([&](keyword) {            │
│      return [keyword, allWords.count()] │
│  })                                     │
└────────────────────┬────────────────────┘
                     │
                     ▼
         ┌─────────────────────┐
         │    HeteroList       │
         │ [[kw1, count1],     │
         │  [kw2, count2], ...] │
         └──────────┬──────────┘
                    │
                    ▼
         ┌─────────────────────┐
         │  results.sort()     │
         │  (by count desc)    │
         └──────────┬──────────┘
                    │
                    ▼
         ┌─────────────────────┐
         │  results.forEach()  │
         │  (print output)     │
         └─────────────────────┘
```

---

## Dependencies

- **C++17** (for `std::filesystem`)
- **HeteroList library** (header-only, in `../../Implementation/`)

## Files

```
KeywordFrequency/
├── KeywordFrequency.cpp    # Main application
├── KeywordFrequency.exe    # Compiled executable
├── README.md               # This file
└── sample_data/
    ├── keywords.txt        # 25 keywords to search for
    └── data/
        ├── intro_ds.txt
        ├── oop_concepts.txt
        ├── algorithms.txt
        └── memory_pointers.txt
```
