/**
 * HeteroList CLI - Command-line interface for HeteroList operations
 * 
 * This program provides a CLI interface to the HeteroList library,
 * allowing external programs (like Python/Flask) to use the C++ implementation.
 * 
 * Usage: ./hetero_cli <command> [args...]
 * 
 * Commands:
 *   create <json_array>           - Create a list and return its string representation
 *   sort <json_array> [asc|desc]  - Sort the list
 *   filter <json_array> <type>    - Filter by type (int|double|string|bool)
 *   map <json_array> <op>         - Map operation (double|square|negate|uppercase)
 *   reduce <json_array> <op>      - Reduce operation (sum|product|concat)
 *   stats <json_array>            - Get statistics (sum, avg, min, max)
 *   unique <json_array>           - Get unique elements
 *   reverse <json_array>          - Reverse the list
 *   take <json_array> <n>         - Take first n elements
 *   drop <json_array> <n>         - Drop first n elements
 *   count <json_array>            - Count elements by type
 *   inversions <json_array>       - Count inversions
 *   frequencies <json_array>      - Get frequency of each element
 *   partition <json_array> <type> - Partition by type
 *   push <json_array> <value>     - Push value to list
 *   pop <json_array>              - Pop last element
 */

#include "../../Implementation/myList.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>

// Parse a JSON-like array string into HeteroList
HeteroList parseJsonArray(const std::string& json) {
    HeteroList list;
    std::string s = json;
    
    // Remove brackets and whitespace
    size_t start = s.find('[');
    size_t end = s.rfind(']');
    if (start == std::string::npos || end == std::string::npos) {
        return list;
    }
    s = s.substr(start + 1, end - start - 1);
    
    // Parse elements
    std::string current;
    bool inString = false;
    int depth = 0;
    
    for (size_t i = 0; i <= s.length(); ++i) {
        char c = (i < s.length()) ? s[i] : ',';
        
        if (c == '"' && (i == 0 || s[i-1] != '\\')) {
            inString = !inString;
            current += c;
        } else if (c == '[') {
            depth++;
            current += c;
        } else if (c == ']') {
            depth--;
            current += c;
        } else if (c == ',' && !inString && depth == 0) {
            // Process current element
            // Trim whitespace
            size_t first = current.find_first_not_of(" \t\n\r");
            size_t last = current.find_last_not_of(" \t\n\r");
            if (first != std::string::npos) {
                current = current.substr(first, last - first + 1);
                
                if (current == "null") {
                    list.push(Value());
                } else if (current == "true") {
                    list.push(true);
                } else if (current == "false") {
                    list.push(false);
                } else if (current[0] == '"') {
                    // String - remove quotes
                    list.push(current.substr(1, current.length() - 2));
                } else if (current.find('.') != std::string::npos) {
                    // Double
                    list.push(std::stod(current));
                } else {
                    // Integer
                    try {
                        list.push(std::stoi(current));
                    } catch (...) {
                        list.push(current);
                    }
                }
            }
            current.clear();
        } else {
            current += c;
        }
    }
    
    return list;
}

// Convert HeteroList to JSON array string
std::string toJson(const HeteroList& list) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < list.size(); ++i) {
        if (i > 0) oss << ", ";
        const Value& v = list[i];
        if (v.isNull()) {
            oss << "null";
        } else if (v.isBool()) {
            oss << (v.asBool() ? "true" : "false");
        } else if (v.isInt()) {
            oss << v.asInt();
        } else if (v.isDouble()) {
            oss << v.asDouble();
        } else if (v.isString()) {
            oss << "\"" << v.asString() << "\"";
        }
    }
    oss << "]";
    return oss.str();
}

void printHelp() {
    std::cout << R"({
    "error": "Invalid command. Available commands: create, sort, filter, map, reduce, stats, unique, reverse, take, drop, count, inversions, frequencies, partition, push, pop"
})";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }
    
    std::string command = argv[1];
    
    try {
        if (command == "create" && argc >= 3) {
            HeteroList list = parseJsonArray(argv[2]);
            std::cout << "{\"result\": " << toJson(list) << ", \"size\": " << list.size() << "}" << std::endl;
        }
        else if (command == "sort" && argc >= 3) {
            HeteroList list = parseJsonArray(argv[2]);
            bool ascending = (argc < 4 || std::string(argv[3]) != "desc");
            if (ascending) {
                list.sort();
            } else {
                list.sortDescending();
            }
            std::cout << "{\"result\": " << toJson(list) << "}" << std::endl;
        }
        else if (command == "filter" && argc >= 4) {
            HeteroList list = parseJsonArray(argv[2]);
            std::string type = argv[3];
            HeteroList filtered;
            if (type == "int") {
                filtered = list.filter([](const Value& v) { return v.isInt(); });
            } else if (type == "double") {
                filtered = list.filter([](const Value& v) { return v.isDouble(); });
            } else if (type == "string") {
                filtered = list.filter([](const Value& v) { return v.isString(); });
            } else if (type == "bool") {
                filtered = list.filter([](const Value& v) { return v.isBool(); });
            } else if (type == "numeric") {
                filtered = list.filter([](const Value& v) { return v.isNumeric(); });
            } else if (type == "even") {
                filtered = list.filter([](const Value& v) { return v.isInt() && v.asInt() % 2 == 0; });
            } else if (type == "odd") {
                filtered = list.filter([](const Value& v) { return v.isInt() && v.asInt() % 2 != 0; });
            } else if (type == "positive") {
                filtered = list.filter([](const Value& v) { return v.isNumeric() && v.asDouble() > 0; });
            } else if (type == "negative") {
                filtered = list.filter([](const Value& v) { return v.isNumeric() && v.asDouble() < 0; });
            }
            std::cout << "{\"result\": " << toJson(filtered) << ", \"original_size\": " << list.size() << ", \"filtered_size\": " << filtered.size() << "}" << std::endl;
        }
        else if (command == "map" && argc >= 4) {
            HeteroList list = parseJsonArray(argv[2]);
            std::string op = argv[3];
            HeteroList mapped;
            if (op == "double") {
                mapped = list.map([](const Value& v) {
                    if (v.isNumeric()) return v * Value(2);
                    return v;
                });
            } else if (op == "square") {
                mapped = list.map([](const Value& v) {
                    if (v.isNumeric()) return v * v;
                    return v;
                });
            } else if (op == "negate") {
                mapped = list.map([](const Value& v) {
                    if (v.isNumeric()) return v * Value(-1);
                    return v;
                });
            } else if (op == "increment") {
                mapped = list.map([](const Value& v) {
                    if (v.isNumeric()) return v + Value(1);
                    return v;
                });
            } else if (op == "decrement") {
                mapped = list.map([](const Value& v) {
                    if (v.isNumeric()) return v - Value(1);
                    return v;
                });
            } else if (op == "uppercase") {
                mapped = list.map([](const Value& v) {
                    if (v.isString()) {
                        std::string s = v.asString();
                        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
                        return Value(s);
                    }
                    return v;
                });
            } else if (op == "lowercase") {
                mapped = list.map([](const Value& v) {
                    if (v.isString()) {
                        std::string s = v.asString();
                        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
                        return Value(s);
                    }
                    return v;
                });
            } else {
                mapped = list;
            }
            std::cout << "{\"result\": " << toJson(mapped) << "}" << std::endl;
        }
        else if (command == "reduce" && argc >= 4) {
            HeteroList list = parseJsonArray(argv[2]);
            std::string op = argv[3];
            Value result;
            if (op == "sum") {
                HeteroList numeric = list.filter([](const Value& v) { return v.isNumeric(); });
                if (numeric.size() == 0) {
                    std::cout << "{\"result\": 0, \"note\": \"No numeric values found\"}" << std::endl;
                } else {
                    result = numeric.sum();
                    std::cout << "{\"result\": " << result.asDouble() << ", \"numeric_count\": " << numeric.size() << ", \"total_count\": " << list.size() << "}" << std::endl;
                }
            } else if (op == "product") {
                HeteroList numeric = list.filter([](const Value& v) { return v.isNumeric(); });
                if (numeric.size() == 0) {
                    std::cout << "{\"result\": 1, \"note\": \"No numeric values found\"}" << std::endl;
                } else {
                    result = numeric.reduce(Value(1), [](const Value& acc, const Value& v) {
                        return acc * v;
                    });
                    std::cout << "{\"result\": " << result.asDouble() << ", \"numeric_count\": " << numeric.size() << ", \"total_count\": " << list.size() << "}" << std::endl;
                }
            } else if (op == "concat") {
                result = list.reduce(Value(""), [](const Value& acc, const Value& v) {
                    std::string valStr = v.toString();
                    // Remove quotes from string representation
                    if (valStr.length() >= 2 && valStr[0] == '"' && valStr[valStr.length()-1] == '"') {
                        valStr = valStr.substr(1, valStr.length() - 2);
                    }
                    return Value(acc.asString() + valStr);
                });
                std::cout << "{\"result\": \"" << result.asString() << "\"}" << std::endl;
            }
        }
        else if (command == "stats" && argc >= 3) {
            HeteroList list = parseJsonArray(argv[2]);
            HeteroList numeric = list.filter([](const Value& v) { return v.isNumeric(); });
            std::cout << "{";
            if (numeric.size() == 0) {
                std::cout << "\"sum\": 0, \"average\": 0, \"min\": null, \"max\": null, ";
                std::cout << "\"count\": 0, \"total_elements\": " << list.size() << ", ";
                std::cout << "\"note\": \"No numeric values found in the list\"";
            } else {
                std::cout << "\"sum\": " << numeric.sum().asDouble() << ", ";
                std::cout << "\"average\": " << numeric.average().asDouble() << ", ";
                std::cout << "\"min\": " << numeric.min().asDouble() << ", ";
                std::cout << "\"max\": " << numeric.max().asDouble() << ", ";
                std::cout << "\"count\": " << numeric.size() << ", ";
                std::cout << "\"total_elements\": " << list.size();
            }
            std::cout << "}" << std::endl;
        }
        else if (command == "unique" && argc >= 3) {
            HeteroList list = parseJsonArray(argv[2]);
            HeteroList unique = list.unique();
            std::cout << "{\"result\": " << toJson(unique) << ", \"original_size\": " << list.size() << ", \"unique_size\": " << unique.size() << "}" << std::endl;
        }
        else if (command == "reverse" && argc >= 3) {
            HeteroList list = parseJsonArray(argv[2]);
            list.reverse();
            std::cout << "{\"result\": " << toJson(list) << "}" << std::endl;
        }
        else if (command == "take" && argc >= 4) {
            HeteroList list = parseJsonArray(argv[2]);
            int n = std::stoi(argv[3]);
            HeteroList taken = list.take(n);
            std::cout << "{\"result\": " << toJson(taken) << "}" << std::endl;
        }
        else if (command == "drop" && argc >= 4) {
            HeteroList list = parseJsonArray(argv[2]);
            int n = std::stoi(argv[3]);
            HeteroList dropped = list.drop(n);
            std::cout << "{\"result\": " << toJson(dropped) << "}" << std::endl;
        }
        else if (command == "count" && argc >= 3) {
            HeteroList list = parseJsonArray(argv[2]);
            int ints = 0, doubles = 0, strings = 0, bools = 0, nulls = 0;
            for (const auto& v : list) {
                if (v.isNull()) nulls++;
                else if (v.isBool()) bools++;
                else if (v.isInt()) ints++;
                else if (v.isDouble()) doubles++;
                else if (v.isString()) strings++;
            }
            std::cout << "{\"total\": " << list.size() << ", \"integers\": " << ints << ", \"doubles\": " << doubles << ", \"strings\": " << strings << ", \"booleans\": " << bools << ", \"nulls\": " << nulls << "}" << std::endl;
        }
        else if (command == "inversions" && argc >= 3) {
            HeteroList list = parseJsonArray(argv[2]);
            HeteroList numeric = list.filter([](const Value& v) { return v.isNumeric(); });
            if (numeric.size() == 0) {
                std::cout << "{\"inversions\": 0, \"size\": 0, \"total_elements\": " << list.size() << ", \"note\": \"No numeric values found - inversions only count for numeric elements\"}" << std::endl;
            } else {
                size_t inv = numeric.countInversions();
                std::cout << "{\"inversions\": " << inv << ", \"numeric_count\": " << numeric.size() << ", \"total_elements\": " << list.size() << "}" << std::endl;
            }
        }
        else if (command == "frequencies" && argc >= 3) {
            HeteroList list = parseJsonArray(argv[2]);
            auto freqMap = list.frequencies();
            std::cout << "{\"frequencies\": [";
            bool first = true;
            for (const auto& pair : freqMap) {
                if (!first) std::cout << ", ";
                first = false;
                // pair.first is string representation, pair.second is count
                std::cout << "{\"value\": " << pair.first << ", \"count\": " << pair.second << "}";
            }
            std::cout << "]}" << std::endl;
        }
        else if (command == "partition" && argc >= 4) {
            HeteroList list = parseJsonArray(argv[2]);
            std::string type = argv[3];
            HeteroList matching, nonMatching;
            if (type == "int") {
                auto [m, n] = list.partition([](const Value& v) { return v.isInt(); });
                matching = m; nonMatching = n;
            } else if (type == "double") {
                auto [m, n] = list.partition([](const Value& v) { return v.isDouble(); });
                matching = m; nonMatching = n;
            } else if (type == "string") {
                auto [m, n] = list.partition([](const Value& v) { return v.isString(); });
                matching = m; nonMatching = n;
            } else if (type == "numeric") {
                auto [m, n] = list.partition([](const Value& v) { return v.isNumeric(); });
                matching = m; nonMatching = n;
            } else if (type == "even") {
                auto [m, n] = list.partition([](const Value& v) { return v.isInt() && v.asInt() % 2 == 0; });
                matching = m; nonMatching = n;
            } else if (type == "odd") {
                auto [m, n] = list.partition([](const Value& v) { return v.isInt() && v.asInt() % 2 != 0; });
                matching = m; nonMatching = n;
            }
            std::cout << "{\"matching\": " << toJson(matching) << ", \"non_matching\": " << toJson(nonMatching) << "}" << std::endl;
        }
        else if (command == "push" && argc >= 4) {
            HeteroList list = parseJsonArray(argv[2]);
            std::string val = argv[3];
            // Parse value
            if (val == "null") {
                list.push(Value());
            } else if (val == "true") {
                list.push(true);
            } else if (val == "false") {
                list.push(false);
            } else if (val[0] == '"') {
                list.push(val.substr(1, val.length() - 2));
            } else if (val.find('.') != std::string::npos) {
                list.push(std::stod(val));
            } else {
                try {
                    list.push(std::stoi(val));
                } catch (...) {
                    list.push(val);
                }
            }
            std::cout << "{\"result\": " << toJson(list) << ", \"size\": " << list.size() << "}" << std::endl;
        }
        else if (command == "pop" && argc >= 3) {
            HeteroList list = parseJsonArray(argv[2]);
            if (list.size() > 0) {
                Value popped = list.pop();
                std::cout << "{\"result\": " << toJson(list) << ", \"popped\": ";
                if (popped.isString()) {
                    std::cout << "\"" << popped.asString() << "\"";
                } else if (popped.isNull()) {
                    std::cout << "null";
                } else if (popped.isBool()) {
                    std::cout << (popped.asBool() ? "true" : "false");
                } else {
                    std::cout << popped.toString();
                }
                std::cout << ", \"size\": " << list.size() << "}" << std::endl;
            } else {
                std::cout << "{\"error\": \"Cannot pop from empty list\"}" << std::endl;
            }
        }
        else {
            printHelp();
            return 1;
        }
    } catch (const std::exception& e) {
        std::cout << "{\"error\": \"" << e.what() << "\"}" << std::endl;
        return 1;
    }
    
    return 0;
}
