# Makefile for HeteroList Library - Linux/Mac
# Usage:
#   make          - Build and run UC4 (Web App) [default]
#   make uc1      - Build and run UC1 (Keyword Frequency)
#   make uc2      - Build and run UC2 (Network Stream)
#   make uc3      - Build UC3 (CLI App)
#   make uc4      - Build and run UC4 (Web App)
#   make all      - Build all use cases
#   make test     - Run tests
#   make clean    - Clean all builds

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
IMPL_DIR = Implementation
UC_DIR = UseCases

# Paths
CLI_DIR = $(UC_DIR)/CLIApp
KW_DIR = $(UC_DIR)/KeywordFrequency
NET_DIR = $(UC_DIR)/NetworkStream
WEB_DIR = $(UC_DIR)/WebApp
TEST_DIR = Tests

# Executables
CLI_EXE = $(CLI_DIR)/hetero_cli
KW_EXE = $(KW_DIR)/KeywordFrequency
SERVER_EXE = $(NET_DIR)/StreamServer
CLIENT_EXE = $(NET_DIR)/StreamClient
TEST_EXE = $(TEST_DIR)/test_implementation

# Default target: UC4 (Web App)
.PHONY: default
default: uc4

# Build all use cases
.PHONY: all
all: $(CLI_EXE) $(KW_EXE) $(SERVER_EXE) $(CLIENT_EXE) $(TEST_EXE)
	@echo "All use cases built successfully!"

# UC1: Keyword Frequency Counter
$(KW_EXE): $(KW_DIR)/KeywordFrequency.cpp
	@echo "Building Keyword Frequency Counter..."
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: uc1
uc1: $(KW_EXE)
	@echo ""
	@echo "=== Running UC1: Keyword Frequency Counter ==="
	@echo ""
	cd $(KW_DIR) && ./KeywordFrequency sample_data/data sample_data/keywords.txt

# UC2: Network Stream Processor
$(SERVER_EXE): $(NET_DIR)/Server.cpp
	@echo "Building Network Stream Server..."
	$(CXX) $(CXXFLAGS) -pthread -o $@ $<

$(CLIENT_EXE): $(NET_DIR)/Client.cpp
	@echo "Building Network Stream Client..."
	$(CXX) $(CXXFLAGS) -pthread -o $@ $<

.PHONY: uc2
uc2: $(SERVER_EXE) $(CLIENT_EXE)
	@echo ""
	@echo "=== UC2: Network Stream Processor ==="
	@echo "Starting server and client..."
	@echo "Press Ctrl+C to stop"
	@echo ""
	$(SERVER_EXE) & sleep 1 && $(CLIENT_EXE)

.PHONY: uc2-server
uc2-server: $(SERVER_EXE)
	$(SERVER_EXE)

.PHONY: uc2-client
uc2-client: $(CLIENT_EXE)
	$(CLIENT_EXE)

# UC3: CLI Application
$(CLI_EXE): $(CLI_DIR)/hetero_cli.cpp
	@echo "Building CLI Application..."
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: uc3
uc3: $(CLI_EXE)
	@echo ""
	@echo "=== UC3: CLI Application Built ==="
	@echo "Usage: $(CLI_EXE) <command> [arguments]"
	@echo ""
	@echo "Example commands:"
	@echo "  $(CLI_EXE) create '[1, 2, 3, \"hello\"]'"
	@echo "  $(CLI_EXE) sort '[5, 3, 8, 1]' asc"
	@echo "  $(CLI_EXE) map '[1, 2, 3]' square"
	@echo ""

# UC4: Web Application (default)
.PHONY: uc4
uc4: $(CLI_EXE)
	@echo ""
	@echo "=== Running UC4: Web Application ==="
	@python3 -c "import flask" 2>/dev/null || pip3 install flask
	@echo "Starting Flask server at http://127.0.0.1:5000"
	@echo "Press Ctrl+C to stop"
	@echo ""
	cd $(WEB_DIR) && python3 app.py

# Tests
$(TEST_EXE): $(TEST_DIR)/test_implementation.cpp
	@echo "Building tests..."
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: test
test: $(TEST_EXE)
	@echo ""
	@echo "=== Running Tests ==="
	@echo ""
	$(TEST_EXE)

# Clean
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(CLI_EXE) $(KW_EXE) $(SERVER_EXE) $(CLIENT_EXE) $(TEST_EXE)
	@echo "Clean complete!"

# Help
.PHONY: help
help:
	@echo "HeteroList Library - Makefile Targets"
	@echo ""
	@echo "  make          Build and run UC4 (Web App) [default]"
	@echo "  make uc1      Build and run UC1 (Keyword Frequency)"
	@echo "  make uc2      Build UC2 and start server (Network Stream)"
	@echo "  make uc2-server  Start UC2 server only"
	@echo "  make uc2-client  Start UC2 client only"
	@echo "  make uc3      Build UC3 (CLI App)"
	@echo "  make uc4      Build and run UC4 (Web App)"
	@echo "  make all      Build all use cases"
	@echo "  make test     Build and run tests"
	@echo "  make clean    Remove all executables"
	@echo "  make help     Show this help message"
