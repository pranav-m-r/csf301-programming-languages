#pragma once
/**
 * Value.hpp - Type-erased container for heterogeneous data
 * 
 * This is the core type that enables Python-like heterogeneous lists in C++.
 * Supports: int, double, string, bool, null, and nested lists.
 * 
 * Design Philosophy:
 * - Implicit conversions from primitive types for ease of use
 * - Explicit type checking methods for safety
 * - Arithmetic and comparison operators for functional operations
 * - Hash support for use in associative containers
 */

#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <functional>

// Forward declaration for nested list support
class HeteroList;

class Value {
public:
    enum class Type { Null, Bool, Int, Double, String, List };

private:
    Type type_;
    
    // Using a simple struct instead of union for safety with non-trivial types
    long long intVal_ = 0;
    double doubleVal_ = 0.0;
    bool boolVal_ = false;
    std::string strVal_;
    std::shared_ptr<HeteroList> listVal_;

public:
    // ==================== Constructors ====================
    
    Value() : type_(Type::Null) {}
    Value(std::nullptr_t) : type_(Type::Null) {}
    
    Value(bool v) : type_(Type::Bool), boolVal_(v) {}
    
    Value(int v) : type_(Type::Int), intVal_(v) {}
    Value(long v) : type_(Type::Int), intVal_(v) {}
    Value(long long v) : type_(Type::Int), intVal_(v) {}
    
    Value(float v) : type_(Type::Double), doubleVal_(v) {}
    Value(double v) : type_(Type::Double), doubleVal_(v) {}
    
    Value(const char* v) : type_(Type::String), strVal_(v) {}
    Value(const std::string& v) : type_(Type::String), strVal_(v) {}
    Value(std::string&& v) : type_(Type::String), strVal_(std::move(v)) {}
    
    Value(std::shared_ptr<HeteroList> list) : type_(Type::List), listVal_(list) {}

    // Copy and move (defaulted - all members handle themselves)
    Value(const Value&) = default;
    Value(Value&&) noexcept = default;
    Value& operator=(const Value&) = default;
    Value& operator=(Value&&) noexcept = default;
    ~Value() = default;

    // ==================== Type Checking ====================
    
    Type type() const { return type_; }
    
    bool isNull() const { return type_ == Type::Null; }
    bool isBool() const { return type_ == Type::Bool; }
    bool isInt() const { return type_ == Type::Int; }
    bool isDouble() const { return type_ == Type::Double; }
    bool isString() const { return type_ == Type::String; }
    bool isList() const { return type_ == Type::List; }
    bool isNumeric() const { return type_ == Type::Int || type_ == Type::Double; }

    std::string typeName() const {
        switch (type_) {
            case Type::Null:   return "null";
            case Type::Bool:   return "bool";
            case Type::Int:    return "int";
            case Type::Double: return "double";
            case Type::String: return "string";
            case Type::List:   return "list";
            default:           return "unknown";
        }
    }

    // ==================== Value Extraction ====================
    
    long long asInt() const {
        switch (type_) {
            case Type::Int:    return intVal_;
            case Type::Double: return static_cast<long long>(doubleVal_);
            case Type::Bool:   return boolVal_ ? 1 : 0;
            default: throw std::runtime_error("Cannot convert " + typeName() + " to int");
        }
    }

    double asDouble() const {
        switch (type_) {
            case Type::Double: return doubleVal_;
            case Type::Int:    return static_cast<double>(intVal_);
            case Type::Bool:   return boolVal_ ? 1.0 : 0.0;
            default: throw std::runtime_error("Cannot convert " + typeName() + " to double");
        }
    }

    bool asBool() const {
        switch (type_) {
            case Type::Bool:   return boolVal_;
            case Type::Int:    return intVal_ != 0;
            case Type::Double: return doubleVal_ != 0.0;
            case Type::String: return !strVal_.empty();
            case Type::Null:   return false;
            case Type::List:   return listVal_ != nullptr;
            default:           return false;
        }
    }

    const std::string& asString() const {
        if (type_ != Type::String) {
            throw std::runtime_error("Cannot convert " + typeName() + " to string");
        }
        return strVal_;
    }

    std::shared_ptr<HeteroList> asList() const {
        if (type_ != Type::List) {
            throw std::runtime_error("Cannot convert " + typeName() + " to list");
        }
        return listVal_;
    }

    // ==================== String Representation ====================
    
    std::string toString() const {
        std::ostringstream oss;
        switch (type_) {
            case Type::Null:   oss << "null"; break;
            case Type::Bool:   oss << (boolVal_ ? "true" : "false"); break;
            case Type::Int:    oss << intVal_; break;
            case Type::Double: oss << doubleVal_; break;
            case Type::String: oss << "\"" << strVal_ << "\""; break;
            case Type::List:   oss << "[List]"; break;
        }
        return oss.str();
    }

    // ==================== Comparison Operators ====================
    
    bool operator==(const Value& other) const {
        if (type_ != other.type_) {
            // Allow numeric comparisons across int/double
            if (isNumeric() && other.isNumeric()) {
                return asDouble() == other.asDouble();
            }
            return false;
        }
        switch (type_) {
            case Type::Null:   return true;
            case Type::Bool:   return boolVal_ == other.boolVal_;
            case Type::Int:    return intVal_ == other.intVal_;
            case Type::Double: return std::abs(doubleVal_ - other.doubleVal_) < 1e-10;
            case Type::String: return strVal_ == other.strVal_;
            case Type::List:   return listVal_ == other.listVal_;  // Pointer equality
            default:           return false;
        }
    }

    bool operator!=(const Value& other) const { return !(*this == other); }

    bool operator<(const Value& other) const {
        // Same type comparison
        if (type_ == other.type_) {
            switch (type_) {
                case Type::Bool:   return boolVal_ < other.boolVal_;
                case Type::Int:    return intVal_ < other.intVal_;
                case Type::Double: return doubleVal_ < other.doubleVal_;
                case Type::String: return strVal_ < other.strVal_;
                default:           return false;
            }
        }
        // Cross-type numeric comparison
        if (isNumeric() && other.isNumeric()) {
            return asDouble() < other.asDouble();
        }
        // Type ordering: Null < Bool < Int < Double < String < List
        return static_cast<int>(type_) < static_cast<int>(other.type_);
    }

    bool operator>(const Value& other) const { return other < *this; }
    bool operator<=(const Value& other) const { return !(other < *this); }
    bool operator>=(const Value& other) const { return !(*this < other); }

    // ==================== Arithmetic Operators ====================
    
    Value operator+(const Value& other) const {
        // String concatenation
        if (isString() && other.isString()) {
            return Value(strVal_ + other.strVal_);
        }
        // Numeric addition
        if (isNumeric() && other.isNumeric()) {
            if (type_ == Type::Double || other.type_ == Type::Double) {
                return Value(asDouble() + other.asDouble());
            }
            return Value(asInt() + other.asInt());
        }
        throw std::runtime_error("Cannot add " + typeName() + " and " + other.typeName());
    }

    Value operator-(const Value& other) const {
        if (isNumeric() && other.isNumeric()) {
            if (type_ == Type::Double || other.type_ == Type::Double) {
                return Value(asDouble() - other.asDouble());
            }
            return Value(asInt() - other.asInt());
        }
        throw std::runtime_error("Cannot subtract " + typeName() + " and " + other.typeName());
    }

    Value operator*(const Value& other) const {
        if (isNumeric() && other.isNumeric()) {
            if (type_ == Type::Double || other.type_ == Type::Double) {
                return Value(asDouble() * other.asDouble());
            }
            return Value(asInt() * other.asInt());
        }
        throw std::runtime_error("Cannot multiply " + typeName() + " and " + other.typeName());
    }

    Value operator/(const Value& other) const {
        if (isNumeric() && other.isNumeric()) {
            double divisor = other.asDouble();
            if (divisor == 0.0) throw std::runtime_error("Division by zero");
            return Value(asDouble() / divisor);
        }
        throw std::runtime_error("Cannot divide " + typeName() + " by " + other.typeName());
    }

    Value operator%(const Value& other) const {
        if (isInt() && other.isInt()) {
            if (other.intVal_ == 0) throw std::runtime_error("Modulo by zero");
            return Value(intVal_ % other.intVal_);
        }
        throw std::runtime_error("Modulo requires integer operands");
    }

    // Unary minus
    Value operator-() const {
        if (type_ == Type::Int) return Value(-intVal_);
        if (type_ == Type::Double) return Value(-doubleVal_);
        throw std::runtime_error("Cannot negate " + typeName());
    }

    // ==================== Hash Support ====================
    
    size_t hash() const {
        switch (type_) {
            case Type::Null:   return 0;
            case Type::Bool:   return std::hash<bool>{}(boolVal_);
            case Type::Int:    return std::hash<long long>{}(intVal_);
            case Type::Double: return std::hash<double>{}(doubleVal_);
            case Type::String: return std::hash<std::string>{}(strVal_);
            case Type::List:   return std::hash<void*>{}(listVal_.get());
            default:           return 0;
        }
    }
};

// Hash specialization for std::unordered_map/set
namespace std {
    template<>
    struct hash<Value> {
        size_t operator()(const Value& v) const { return v.hash(); }
    };
}
