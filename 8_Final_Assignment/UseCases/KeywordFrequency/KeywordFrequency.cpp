/**
 * KeywordFrequency.cpp - Keyword Frequency Counter
 * 
 * USE CASE: Given a directory of text files and a keyword file,
 * count occurrences of each keyword across all files and output
 * them in decreasing order of frequency.
 * 
 * This demonstrates:
 * - File and directory I/O
 * - HeteroList functional operations (map, filter, sort, count)
 * - Aggregation methods
 * - Real-world application of the library
 * 
 * Usage: KeywordFrequency.exe <data_directory> <keyword_file>
 * 
 * Keyword file format: keywords separated by whitespace
 * Example:
 *   apple banana cherry
 *   date elderberry
 */

#include "../../Implementation/myList.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

/**
 * Normalize a word: convert to lowercase, remove punctuation
 */
std::string normalizeWord(const std::string& word) {
    std::string result;
    for (char c : word) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            result += std::tolower(static_cast<unsigned char>(c));
        }
    }
    return result;
}

/**
 * Read keywords from a file (whitespace-separated)
 */
HeteroList readKeywords(const std::string& keywordFile) {
    HeteroList keywords;
    std::ifstream file(keywordFile);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open keyword file: " + keywordFile);
    }
    
    std::string word;
    while (file >> word) {
        std::string normalized = normalizeWord(word);
        if (!normalized.empty() && !keywords.contains(Value(normalized))) {
            keywords.push(normalized);
        }
    }
    
    return keywords;
}

/**
 * Read all words from a single file into a HeteroList
 */
HeteroList readWordsFromFile(const std::string& filePath) {
    HeteroList words;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "Warning: Cannot open file: " << filePath << std::endl;
        return words;
    }
    
    std::string word;
    while (file >> word) {
        std::string normalized = normalizeWord(word);
        if (!normalized.empty()) {
            words.push(normalized);
        }
    }
    
    return words;
}

/**
 * Get all text files from a directory (recursive)
 */
HeteroList getTextFiles(const std::string& directory) {
    HeteroList files;
    
    try {
        for (const auto& entry : fs::recursive_directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                
                if (ext == ".txt" || ext == ".text" || ext == ".md" || 
                    ext == ".csv" || ext == ".log" || ext == ".dat" ||
                    ext.empty()) {
                    files.push(entry.path().string());
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    
    return files;
}

/**
 * Main keyword frequency analysis - Using functional HeteroList operations
 */
void analyzeKeywordFrequency(const std::string& dataDir, const std::string& keywordFile) {
    std::cout << "=== Keyword Frequency Analyzer ===" << std::endl;
    std::cout << std::endl;
    
    // Step 1: Read keywords
    std::cout << "Reading keywords from: " << keywordFile << std::endl;
    HeteroList keywords = readKeywords(keywordFile);
    std::cout << "Found " << keywords.size() << " unique keywords: ";
    std::cout << keywords.toString() << std::endl;
    std::cout << std::endl;
    
    if (keywords.empty()) {
        std::cout << "No keywords to search for!" << std::endl;
        return;
    }
    
    // Step 2: Get all text files
    std::cout << "Scanning directory: " << dataDir << std::endl;
    HeteroList files = getTextFiles(dataDir);
    std::cout << "Found " << files.size() << " text files" << std::endl;
    std::cout << std::endl;
    
    if (files.empty()) {
        std::cout << "No text files found in directory!" << std::endl;
        return;
    }
    
    // Step 3: Read ALL words from ALL files into one big HeteroList
    std::cout << "Reading all words from files..." << std::endl;
    HeteroList allWords;
    
    files.forEach([&](const Value& filePath) {
        HeteroList fileWords = readWordsFromFile(filePath.asString());
        // Concatenate words from this file
        for (int i = 0; i < fileWords.size(); ++i) {
            allWords.push(fileWords[i]);
        }
    });
    
    std::cout << "Total words read: " << allWords.size() << std::endl;
    std::cout << std::endl;
    
    // Step 4: For each keyword, use count() to find occurrences
    // Create list of [keyword, count] pairs using map
    std::cout << "Counting keyword occurrences using HeteroList::count()..." << std::endl;
    
    HeteroList results = keywords.map([&allWords](const Value& keyword) {
        // Use the count() function to count occurrences of this keyword
        int freq = allWords.count([&keyword](const Value& word) {
            return word == keyword;
        });
        // Return a pair [keyword, count]
        return Value(makeList({keyword, Value(freq)}));
    });
    
    // Step 5: Sort by frequency (descending)
    results.sort([](const Value& a, const Value& b) {
        long long countA = (*a.asList())[1].asInt();
        long long countB = (*b.asList())[1].asInt();
        if (countA != countB) return countA > countB;
        return (*a.asList())[0].asString() < (*b.asList())[0].asString();
    });
    
    // Step 6: Output results
    std::cout << std::endl;
    std::cout << "=== Results (Descending Frequency) ===" << std::endl;
    std::cout << std::endl;
    std::cout << "Rank  Keyword                    Count" << std::endl;
    std::cout << "----  -------------------------  -----" << std::endl;
    
    long long totalOccurrences = 0;
    int rank = 1;
    
    results.forEach([&](const Value& pair) {
        auto pairList = pair.asList();
        std::string keyword = (*pairList)[0].asString();
        long long count = (*pairList)[1].asInt();
        totalOccurrences += count;
        
        std::cout << std::setw(4) << rank++ << "  ";
        std::cout << std::left << std::setw(25) << keyword << "  ";
        std::cout << std::right << std::setw(5) << count << std::endl;
    });
    
    std::cout << std::endl;
    std::cout << "=== Summary ===" << std::endl;
    std::cout << "Total files processed: " << files.size() << std::endl;
    std::cout << "Total keywords tracked: " << keywords.size() << std::endl;
    std::cout << "Total words in corpus: " << allWords.size() << std::endl;
    std::cout << "Total keyword occurrences: " << totalOccurrences << std::endl;
    
    if (results.size() > 0) {
        auto mostFreq = results[0].asList();
        std::cout << "Most frequent: \"" << (*mostFreq)[0].asString() 
                  << "\" (" << (*mostFreq)[1].asInt() << " occurrences)" << std::endl;
        
        auto leastFreq = results[results.size() - 1].asList();
        std::cout << "Least frequent: \"" << (*leastFreq)[0].asString() 
                  << "\" (" << (*leastFreq)[1].asInt() << " occurrences)" << std::endl;
    }
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <data_directory> <keyword_file>" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << "  data_directory  Directory containing text files to analyze" << std::endl;
    std::cout << "  keyword_file    File containing keywords (whitespace-separated)" << std::endl;
    std::cout << std::endl;
    std::cout << "Example:" << std::endl;
    std::cout << "  " << programName << " ./data ./keywords.txt" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string dataDir = argv[1];
    std::string keywordFile = argv[2];
    
    if (!fs::exists(dataDir)) {
        std::cerr << "Error: Directory does not exist: " << dataDir << std::endl;
        return 1;
    }
    
    if (!fs::is_directory(dataDir)) {
        std::cerr << "Error: Not a directory: " << dataDir << std::endl;
        return 1;
    }
    
    if (!fs::exists(keywordFile)) {
        std::cerr << "Error: Keyword file does not exist: " << keywordFile << std::endl;
        return 1;
    }
    
    try {
        analyzeKeywordFrequency(dataDir, keywordFile);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
