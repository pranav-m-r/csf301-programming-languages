/**
 * Client.cpp - Real-Time Heterogeneous Data Receiver and Sorter
 * 
 * A TCP client that:
 * 1. Connects to the data stream server
 * 2. Receives mixed-type data (INT, DOUBLE, STRING)
 * 3. Stores in a HeteroList
 * 4. Continuously sorts and displays the data
 * 
 * Demonstrates:
 * - Real-time data ingestion into HeteroList
 * - Heterogeneous sorting (numbers vs strings)
 * - Functional operations on streaming data
 * - groupBy to categorize by type
 * 
 * Usage: Client.exe [host] [port]
 * Default: localhost:9000
 */

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netdb.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

#include "../../Implementation/myList.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

// ANSI escape codes for console clearing and colors
#ifdef _WIN32
#include <windows.h>
void clearScreen() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hStdOut, coord);
}
#else
void clearScreen() {
    std::cout << "\033[2J\033[H";
}
#endif

class StreamClient {
private:
    SOCKET socket_;
    std::string host_;
    int port_;
    
    HeteroList allData_;          // All received data
    HeteroList integers_;         // Just integers
    HeteroList doubles_;          // Just doubles  
    HeteroList strings_;          // Just strings
    
    std::mutex dataMutex_;
    std::atomic<bool> running_;
    std::atomic<int> totalReceived_;

public:
    StreamClient(const std::string& host = "127.0.0.1", int port = 9000) 
        : socket_(INVALID_SOCKET), host_(host), port_(port), 
          running_(false), totalReceived_(0) {}

    bool connect() {
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed" << std::endl;
            return false;
        }
#endif

        socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socket_ == INVALID_SOCKET) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(static_cast<u_short>(port_));
        
        if (inet_pton(AF_INET, host_.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Invalid address" << std::endl;
            return false;
        }

        if (::connect(socket_, reinterpret_cast<sockaddr*>(&serverAddr), 
                      sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Connection failed" << std::endl;
            closesocket(socket_);
            return false;
        }

        return true;
    }

    Value parseValue(const std::string& line) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) return Value();
        
        std::string type = line.substr(0, colonPos);
        std::string valueStr = line.substr(colonPos + 1);
        
        // Trim whitespace
        while (!valueStr.empty() && (valueStr.back() == '\n' || valueStr.back() == '\r')) {
            valueStr.pop_back();
        }
        
        if (type == "INT") {
            return Value(std::stoi(valueStr));
        } else if (type == "DOUBLE") {
            return Value(std::stod(valueStr));
        } else if (type == "STRING") {
            return Value(valueStr);
        }
        
        return Value();  // null for unknown types
    }

    void receiveData() {
        char buffer[1024];
        std::string lineBuffer;
        
        while (running_) {
            int received = recv(socket_, buffer, sizeof(buffer) - 1, 0);
            
            if (received <= 0) {
                std::cout << "\nConnection closed by server." << std::endl;
                running_ = false;
                break;
            }
            
            buffer[received] = '\0';
            lineBuffer += buffer;
            
            // Process complete lines
            size_t newlinePos;
            while ((newlinePos = lineBuffer.find('\n')) != std::string::npos) {
                std::string line = lineBuffer.substr(0, newlinePos);
                lineBuffer = lineBuffer.substr(newlinePos + 1);
                
                if (!line.empty()) {
                    Value val = parseValue(line);
                    
                    std::lock_guard<std::mutex> lock(dataMutex_);
                    
                    allData_.push(val);
                    totalReceived_++;
                    
                    // Categorize by type
                    if (val.isInt()) {
                        integers_.push(val);
                    } else if (val.isDouble()) {
                        doubles_.push(val);
                    } else if (val.isString()) {
                        strings_.push(val);
                    }
                }
            }
        }
    }

    void displayStats() {
        while (running_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
            clearScreen();
            
            std::lock_guard<std::mutex> lock(dataMutex_);
            
            std::cout << "╔══════════════════════════════════════════════════════════════════╗" << std::endl;
            std::cout << "║       REAL-TIME HETEROGENEOUS DATA STREAM PROCESSOR              ║" << std::endl;
            std::cout << "╠══════════════════════════════════════════════════════════════════╣" << std::endl;
            std::cout << "║ Connected to: " << std::left << std::setw(20) << (host_ + ":" + std::to_string(port_));
            std::cout << " Total Received: " << std::setw(8) << totalReceived_.load() << "  ║" << std::endl;
            std::cout << "╚══════════════════════════════════════════════════════════════════╝" << std::endl;
            std::cout << std::endl;

            // === INTEGERS ===
            std::cout << "┌─────────────────────────────────────────────────────────────────┐" << std::endl;
            std::cout << "│ INTEGERS (" << integers_.size() << " values)";
            if (!integers_.empty()) {
                std::cout << " | Sum: " << integers_.sum().asInt();
                std::cout << " | Avg: " << std::fixed << std::setprecision(1) << integers_.average().asDouble();
                std::cout << " | Min: " << integers_.min().asInt();
                std::cout << " | Max: " << integers_.max().asInt();
            }
            std::cout << std::endl;
            std::cout << "├─────────────────────────────────────────────────────────────────┤" << std::endl;
            
            if (!integers_.empty()) {
                // Sort and show
                HeteroList sortedInts = integers_;
                sortedInts.sort();
                
                std::cout << "│ Sorted: ";
                int showCount = std::min(15, sortedInts.size());
                for (int i = 0; i < showCount; ++i) {
                    std::cout << sortedInts[i].asInt();
                    if (i < showCount - 1) std::cout << ", ";
                }
                if (sortedInts.size() > 15) std::cout << " ... (+" << (sortedInts.size() - 15) << " more)";
                std::cout << std::endl;
                
                // Count inversions in original order
                std::cout << "│ Inversions (unsorted): " << integers_.countInversions() << std::endl;
            } else {
                std::cout << "│ (waiting for data...)" << std::endl;
            }
            std::cout << "└─────────────────────────────────────────────────────────────────┘" << std::endl;
            std::cout << std::endl;

            // === DOUBLES ===
            std::cout << "┌─────────────────────────────────────────────────────────────────┐" << std::endl;
            std::cout << "│ DOUBLES (" << doubles_.size() << " values)";
            if (!doubles_.empty()) {
                std::cout << " | Sum: " << std::fixed << std::setprecision(2) << doubles_.sum().asDouble();
                std::cout << " | Avg: " << doubles_.average().asDouble();
            }
            std::cout << std::endl;
            std::cout << "├─────────────────────────────────────────────────────────────────┤" << std::endl;
            
            if (!doubles_.empty()) {
                HeteroList sortedDoubles = doubles_;
                sortedDoubles.sort();
                
                std::cout << "│ Sorted: ";
                int showCount = std::min(10, sortedDoubles.size());
                for (int i = 0; i < showCount; ++i) {
                    std::cout << std::fixed << std::setprecision(2) << sortedDoubles[i].asDouble();
                    if (i < showCount - 1) std::cout << ", ";
                }
                if (sortedDoubles.size() > 10) std::cout << " ... (+" << (sortedDoubles.size() - 10) << " more)";
                std::cout << std::endl;
            } else {
                std::cout << "│ (waiting for data...)" << std::endl;
            }
            std::cout << "└─────────────────────────────────────────────────────────────────┘" << std::endl;
            std::cout << std::endl;

            // === STRINGS ===
            std::cout << "┌─────────────────────────────────────────────────────────────────┐" << std::endl;
            std::cout << "│ STRINGS (" << strings_.size() << " values)";
            if (!strings_.empty()) {
                std::cout << " | Unique: " << strings_.unique().size();
            }
            std::cout << std::endl;
            std::cout << "├─────────────────────────────────────────────────────────────────┤" << std::endl;
            
            if (!strings_.empty()) {
                HeteroList sortedStrings = strings_;
                sortedStrings.sort();
                
                std::cout << "│ Sorted: ";
                int showCount = std::min(8, sortedStrings.size());
                for (int i = 0; i < showCount; ++i) {
                    std::cout << sortedStrings[i].asString();
                    if (i < showCount - 1) std::cout << ", ";
                }
                if (sortedStrings.size() > 8) std::cout << " ... (+" << (sortedStrings.size() - 8) << " more)";
                std::cout << std::endl;
                
                // Show frequencies of top strings
                auto freq = strings_.frequencies();
                std::cout << "│ Frequencies: ";
                int freqCount = 0;
                for (const auto& [word, count] : freq) {
                    if (freqCount >= 5) {
                        std::cout << "...";
                        break;
                    }
                    std::cout << word << "(" << count << ") ";
                    freqCount++;
                }
                std::cout << std::endl;
            } else {
                std::cout << "│ (waiting for data...)" << std::endl;
            }
            std::cout << "└─────────────────────────────────────────────────────────────────┘" << std::endl;
            std::cout << std::endl;

            // === COMBINED SORTED (All Types) ===
            std::cout << "┌─────────────────────────────────────────────────────────────────┐" << std::endl;
            std::cout << "│ ALL DATA - SORTED (heterogeneous comparison)                    │" << std::endl;
            std::cout << "├─────────────────────────────────────────────────────────────────┤" << std::endl;
            
            if (!allData_.empty()) {
                HeteroList sortedAll = allData_;
                sortedAll.sort();
                
                std::cout << "│ ";
                int showCount = std::min(12, sortedAll.size());
                for (int i = 0; i < showCount; ++i) {
                    const Value& v = sortedAll[i];
                    if (v.isInt()) {
                        std::cout << v.asInt();
                    } else if (v.isDouble()) {
                        std::cout << std::fixed << std::setprecision(1) << v.asDouble();
                    } else if (v.isString()) {
                        std::cout << "\"" << v.asString() << "\"";
                    }
                    if (i < showCount - 1) std::cout << ", ";
                }
                if (sortedAll.size() > 12) std::cout << " ...";
                std::cout << std::endl;
            } else {
                std::cout << "│ (waiting for data...)" << std::endl;
            }
            std::cout << "└─────────────────────────────────────────────────────────────────┘" << std::endl;
            
            std::cout << std::endl;
            std::cout << "Press Ctrl+C to stop..." << std::endl;
        }
    }

    void run() {
        running_ = true;
        
        // Start receiver thread
        std::thread receiverThread(&StreamClient::receiveData, this);
        
        // Display in main thread
        displayStats();
        
        receiverThread.join();
    }

    ~StreamClient() {
        running_ = false;
        if (socket_ != INVALID_SOCKET) {
            closesocket(socket_);
        }
#ifdef _WIN32
        WSACleanup();
#endif
    }
};

void printUsage(const char* program) {
    std::cout << "Usage: " << program << " [host] [port]" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << "  host    Server hostname or IP (default: 127.0.0.1)" << std::endl;
    std::cout << "  port    Server port (default: 9000)" << std::endl;
    std::cout << std::endl;
    std::cout << "Example:" << std::endl;
    std::cout << "  " << program << " localhost 9000" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    int port = 9000;
    
    if (argc > 1) {
        if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
            printUsage(argv[0]);
            return 0;
        }
        host = argv[1];
    }
    
    if (argc > 2) {
        port = std::atoi(argv[2]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Invalid port. Using default: 9000" << std::endl;
            port = 9000;
        }
    }

    std::cout << "=== Heterogeneous Data Stream Client ===" << std::endl;
    std::cout << "Connecting to " << host << ":" << port << "..." << std::endl;

    StreamClient client(host, port);
    
    if (!client.connect()) {
        std::cerr << "Failed to connect to server." << std::endl;
        std::cerr << "Make sure the server is running first!" << std::endl;
        return 1;
    }

    std::cout << "Connected! Starting real-time display..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    client.run();
    
    return 0;
}
