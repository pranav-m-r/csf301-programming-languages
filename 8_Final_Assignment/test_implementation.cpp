#include "Implementation/myList.hpp"
#include <iostream>
#include <cassert>

void printHeader(const std::string& title) {
    std::cout << "\n=== " << title << " ===" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Heterogeneous List Library Tests" << std::endl;
    std::cout << "========================================" << std::endl;

    // Test 1: Basic HeteroList with mixed types
    printHeader("Test 1: Mixed Types");
    HeteroList list;
    list.push(42);
    list.push(3.14);
    list.push("Hello");
    list.push(true);
    list.push(Value());  // null
    std::cout << "List: " << list.toString() << std::endl;
    std::cout << "Size: " << list.size() << std::endl;

    // Test 2: Initializer list
    printHeader("Test 2: Initializer List");
    HeteroList list2 = {1, 2.5, "world", false};
    std::cout << "List2: " << list2.toString() << std::endl;

    // Test 3: Functional map
    printHeader("Test 3: Map (double numeric values)");
    HeteroList nums = {1, 2, 3, 4, 5};
    HeteroList doubled = nums.map([](const Value& v) {
        if (v.isNumeric()) return v * Value(2);
        return v;
    });
    std::cout << "Original: " << nums.toString() << std::endl;
    std::cout << "Doubled:  " << doubled.toString() << std::endl;

    // Test 4: Filter
    printHeader("Test 4: Filter (keep even numbers)");
    HeteroList mixed = {1, 2, "skip", 3, 4, true, 5, 6};
    HeteroList evens = mixed.filter([](const Value& v) {
        return v.isInt() && v.asInt() % 2 == 0;
    });
    std::cout << "Original: " << mixed.toString() << std::endl;
    std::cout << "Evens:    " << evens.toString() << std::endl;

    // Test 5: Reduce
    printHeader("Test 5: Reduce (sum)");
    HeteroList numbers = {10, 20, 30, 40, 50};
    Value sum = numbers.reduce(Value(0), [](const Value& acc, const Value& v) {
        return acc + v;
    });
    std::cout << "Numbers: " << numbers.toString() << std::endl;
    std::cout << "Sum: " << sum.toString() << std::endl;

    // Test 6: Sorting
    printHeader("Test 6: Sorting");
    HeteroList unsorted = {5, 2, 8, 1, 9, 3};
    std::cout << "Before sort: " << unsorted.toString() << std::endl;
    unsorted.sort();
    std::cout << "After sort:  " << unsorted.toString() << std::endl;
    unsorted.sortDescending();
    std::cout << "Descending:  " << unsorted.toString() << std::endl;

    // Test 7: Aggregations
    printHeader("Test 7: Aggregations");
    HeteroList data = {10, 20, 30, 40, 50};
    std::cout << "Data:    " << data.toString() << std::endl;
    std::cout << "Sum:     " << data.sum().toString() << std::endl;
    std::cout << "Average: " << data.average().toString() << std::endl;
    std::cout << "Min:     " << data.min().toString() << std::endl;
    std::cout << "Max:     " << data.max().toString() << std::endl;

    // Test 8: Count inversions
    printHeader("Test 8: Count Inversions");
    HeteroList inv = {5, 3, 2, 4, 1};
    std::cout << "List: " << inv.toString() << std::endl;
    std::cout << "Inversions: " << inv.countInversions() << std::endl;

    // Test 9: Range-based for loop
    printHeader("Test 9: Range-based For Loop");
    HeteroList items = {"apple", "banana", "cherry"};
    std::cout << "Items:" << std::endl;
    for (const auto& item : items) {
        std::cout << "  - " << item.toString() << std::endl;
    }

    // Test 10: Chaining operations
    printHeader("Test 10: Chaining Operations");
    HeteroList chain = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    HeteroList result = chain
        .filter([](const Value& v) { return v.asInt() % 2 == 0; })  // evens
        .map([](const Value& v) { return v * v; })                   // square
        .take(3);                                                     // first 3
    std::cout << "Original: " << chain.toString() << std::endl;
    std::cout << "Evens squared (first 3): " << result.toString() << std::endl;

    // Test 11: Nested lists
    printHeader("Test 11: Nested Lists");
    auto inner1 = makeList({1, 2, 3});
    auto inner2 = makeList({"a", "b", "c"});
    HeteroList outer;
    outer.push(Value(inner1));
    outer.push(Value(inner2));
    outer.push(42);
    std::cout << "Outer list size: " << outer.size() << std::endl;
    std::cout << "First inner: " << outer[0].asList()->toString() << std::endl;
    std::cout << "Second inner: " << outer[1].asList()->toString() << std::endl;

    // Test 12: String operations
    printHeader("Test 12: String Concatenation");
    Value s1("Hello, ");
    Value s2("World!");
    Value s3 = s1 + s2;
    std::cout << "Concatenated: " << s3.toString() << std::endl;

    // Test 13: Stack
    printHeader("Test 13: Stack");
    Stack stack;
    stack.push(1);
    stack.push("two");
    stack.push(3.0);
    std::cout << "Stack size: " << stack.size() << std::endl;
    std::cout << "Top: " << stack.top().toString() << std::endl;
    std::cout << "Popped: " << stack.pop().toString() << std::endl;
    std::cout << "New top: " << stack.top().toString() << std::endl;

    // Test 14: Queue
    printHeader("Test 14: Queue");
    Queue queue;
    queue.enqueue("first");
    queue.enqueue("second");
    queue.enqueue("third");
    std::cout << "Front: " << queue.front().toString() << std::endl;
    std::cout << "Dequeued: " << queue.dequeue().toString() << std::endl;
    std::cout << "New front: " << queue.front().toString() << std::endl;

    // Test 15: Priority Queue (Min Heap)
    printHeader("Test 15: Priority Queue (Min Heap)");
    MinHeap minHeap;
    minHeap.push(5);
    minHeap.push(2);
    minHeap.push(8);
    minHeap.push(1);
    std::cout << "Extracting in order:" << std::endl;
    while (!minHeap.empty()) {
        std::cout << "  " << minHeap.pop().toString() << std::endl;
    }

    // Test 16: Deque
    printHeader("Test 16: Deque");
    Deque deque;
    deque.push_back(2);
    deque.push_front(1);
    deque.push_back(3);
    std::cout << "Front: " << deque.front().toString() << std::endl;
    std::cout << "Back: " << deque.back().toString() << std::endl;
    std::cout << "Pop front: " << deque.pop_front().toString() << std::endl;
    std::cout << "Pop back: " << deque.pop_back().toString() << std::endl;

    // Test 17: ArrayList specific
    printHeader("Test 17: ArrayList");
    ArrayList arr = {5, 3, 1, 4, 2};
    std::cout << "Original: " << arr.toString() << std::endl;
    arr.sort();
    std::cout << "Sorted: " << arr.toString() << std::endl;
    arr.reverse();
    std::cout << "Reversed: " << arr.toString() << std::endl;

    // Test 18: Linked Lists
    printHeader("Test 18: Linked Lists");
    SinglyLinkedList sll;
    sll.push(1);
    sll.push(2);
    sll.push(3);
    std::cout << "Singly Linked: " << sll.toString() << std::endl;
    
    DoublyLinkedList dll;
    dll.push(1);
    dll.push(2);
    dll.push(3);
    dll.pushFront(0);
    std::cout << "Doubly Linked: " << dll.toString() << std::endl;

    // Test 19: Unique
    printHeader("Test 19: Unique");
    HeteroList withDups = {1, 2, 2, 3, 3, 3, 4};
    std::cout << "With duplicates: " << withDups.toString() << std::endl;
    std::cout << "Unique: " << withDups.unique().toString() << std::endl;

    // Test 20: Partition
    printHeader("Test 20: Partition");
    HeteroList toPartition = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto [evensP, oddsP] = toPartition.partition([](const Value& v) {
        return v.asInt() % 2 == 0;
    });
    std::cout << "Original: " << toPartition.toString() << std::endl;
    std::cout << "Evens: " << evensP.toString() << std::endl;
    std::cout << "Odds: " << oddsP.toString() << std::endl;

    std::cout << "\n========================================" << std::endl;
    std::cout << "  All Tests Completed Successfully!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
