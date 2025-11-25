/**
 * Server.cpp - Heterogeneous Data Stream Server
 * 
 * A TCP server that sends a continuous stream of mixed-type data:
 * - Integers (random numbers)
 * - Doubles (decimal values)
 * - Strings (random words)
 * 
 * Each value is sent as a line with format: TYPE:VALUE
 * Example: INT:42, DOUBLE:3.14, STRING:hello
 * 
 * Usage: Server.exe [port]
 * Default port: 9000
 */

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <vector>
#include <ctime>
#include <atomic>

// Word pool for generating random strings
const std::vector<std::string> WORD_POOL = {
    "alpha", "beta", "gamma", "delta", "epsilon",
    "zeta", "eta", "theta", "iota", "kappa",
    "lambda", "mu", "nu", "xi", "omicron",
    "pi", "rho", "sigma", "tau", "upsilon",
    "apple", "banana", "cherry", "date", "elderberry",
    "fig", "grape", "honeydew", "kiwi", "lemon",
    "mango", "nectarine", "orange", "papaya", "quince",
    "stack", "queue", "list", "tree", "graph",
    "sort", "search", "hash", "heap", "array"
};

class DataServer {
private:
    SOCKET serverSocket_;
    int port_;
    std::mt19937 rng_;
    std::atomic<bool> running_;
    
    // Distributions for random data
    std::uniform_int_distribution<int> intDist_{-1000, 1000};
    std::uniform_real_distribution<double> doubleDist_{-100.0, 100.0};
    std::uniform_int_distribution<size_t> wordDist_{0, WORD_POOL.size() - 1};
    std::uniform_int_distribution<int> typeDist_{0, 2};  // 0=INT, 1=DOUBLE, 2=STRING
    std::uniform_int_distribution<int> delayDist_{100, 500};  // ms between sends

public:
    DataServer(int port = 9000) : port_(port), running_(false) {
        // Seed random number generator
        rng_.seed(static_cast<unsigned>(std::time(nullptr)));
    }

    bool initialize() {
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed" << std::endl;
            return false;
        }
#endif

        serverSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket_ == INVALID_SOCKET) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }

        // Allow socket reuse
        int opt = 1;
        setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, 
                   reinterpret_cast<const char*>(&opt), sizeof(opt));

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(static_cast<u_short>(port_));

        if (bind(serverSocket_, reinterpret_cast<sockaddr*>(&serverAddr), 
                 sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed" << std::endl;
            closesocket(serverSocket_);
            return false;
        }

        if (listen(serverSocket_, 5) == SOCKET_ERROR) {
            std::cerr << "Listen failed" << std::endl;
            closesocket(serverSocket_);
            return false;
        }

        return true;
    }

    std::string generateValue() {
        int type = typeDist_(rng_);
        std::string result;
        
        switch (type) {
            case 0: {  // Integer
                int val = intDist_(rng_);
                result = "INT:" + std::to_string(val);
                break;
            }
            case 1: {  // Double
                double val = doubleDist_(rng_);
                // Format to 2 decimal places
                char buf[32];
                snprintf(buf, sizeof(buf), "%.2f", val);
                result = "DOUBLE:" + std::string(buf);
                break;
            }
            case 2: {  // String
                result = "STRING:" + WORD_POOL[wordDist_(rng_)];
                break;
            }
        }
        
        return result + "\n";
    }

    void handleClient(SOCKET clientSocket) {
        std::cout << "Client connected! Streaming data..." << std::endl;
        running_ = true;
        int messageCount = 0;
        
        while (running_) {
            std::string data = generateValue();
            
            int sent = send(clientSocket, data.c_str(), 
                           static_cast<int>(data.length()), 0);
            
            if (sent == SOCKET_ERROR) {
                std::cout << "Client disconnected." << std::endl;
                break;
            }
            
            messageCount++;
            std::cout << "[" << messageCount << "] Sent: " << data;
            
            // Random delay between sends
            int delay = delayDist_(rng_);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
        
        closesocket(clientSocket);
    }

    void run() {
        std::cout << "=== Heterogeneous Data Stream Server ===" << std::endl;
        std::cout << "Listening on port " << port_ << "..." << std::endl;
        std::cout << "Data types: INT, DOUBLE, STRING" << std::endl;
        std::cout << "Press Ctrl+C to stop." << std::endl;
        std::cout << std::endl;

        while (true) {
            sockaddr_in clientAddr{};
            socklen_t clientLen = sizeof(clientAddr);
            
            SOCKET clientSocket = accept(serverSocket_, 
                                         reinterpret_cast<sockaddr*>(&clientAddr),
                                         &clientLen);
            
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed" << std::endl;
                continue;
            }

            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
            std::cout << "Connection from: " << clientIP << std::endl;

            handleClient(clientSocket);
        }
    }

    ~DataServer() {
        running_ = false;
        if (serverSocket_ != INVALID_SOCKET) {
            closesocket(serverSocket_);
        }
#ifdef _WIN32
        WSACleanup();
#endif
    }
};

int main(int argc, char* argv[]) {
    int port = 9000;
    
    if (argc > 1) {
        port = std::atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Invalid port number. Using default: 9000" << std::endl;
            port = 9000;
        }
    }

    DataServer server(port);
    
    if (!server.initialize()) {
        std::cerr << "Failed to initialize server" << std::endl;
        return 1;
    }

    server.run();
    
    return 0;
}
