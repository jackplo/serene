# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g $(shell pkg-config gtkmm-4.0 --cflags)
GTK_LDFLAGS = $(shell pkg-config gtkmm-4.0 --libs)

# Directories
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
COMPONENTS_DIR = $(SRC_DIR)/components
BACKEND_DIR = $(SRC_DIR)/backend
BUILD_DIR = build
MODEL_DIR = $(SRC_DIR)/models

# Source files
CORE_SRC = $(CORE_DIR)/main.cpp $(CORE_DIR)/spotlight.cpp
COMPONENTS_SRC = $(COMPONENTS_DIR)/listview.cpp $(COMPONENTS_DIR)/scrollview.cpp $(COMPONENTS_DIR)/customsearchentry.cpp
BACKEND_SRC = $(BACKEND_DIR)/app_searcher.cpp
MODEL_SRC = $(MODEL_DIR)/applicationobject.cpp $(MODEL_DIR)/applicationlistmodel.cpp
TEST_SRC = $(BACKEND_DIR)/test_app_searcher.cpp
CUSTOM_SEARCH_TEST_SRC = $(SRC_DIR)/test/main.cpp $(SRC_DIR)/test/test_window.cpp $(COMPONENTS_DIR)/customsearchentry.cpp

# Object files
CORE_OBJ = $(CORE_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
COMPONENTS_OBJ = $(COMPONENTS_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
BACKEND_OBJ = $(BACKEND_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
MODEL_OBJ = $(MODEL_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJ = $(TEST_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
CUSTOM_SEARCH_TEST_OBJ = $(CUSTOM_SEARCH_TEST_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)


# Target executable
TARGET = serene
TEST_BIN = $(BUILD_DIR)/test_app_searcher
CUSTOM_SEARCH_TEST_BIN = $(BUILD_DIR)/custom_search_test

# All target
all: $(TARGET)

# Main target
$(TARGET): $(CORE_OBJ) $(COMPONENTS_OBJ) $(BACKEND_OBJ) $(MODEL_OBJ)
	@echo "Linking..."
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(GTK_LDFLAGS)

# Test target
test: $(TEST_BIN)
	@echo "Running application searcher test..."
	@$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJ) $(BACKEND_OBJ)
	@echo "Linking test program..."
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -o $@ $^

# Pattern rule for object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

custom-search-test: $(CUSTOM_SEARCH_TEST_BIN)
	@echo "Running custom search entry test..."
	@$(CUSTOM_SEARCH_TEST_BIN)

$(CUSTOM_SEARCH_TEST_BIN): $(CUSTOM_SEARCH_TEST_OBJ)
	@echo "Linking custom search test program..."
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(GTK_LDFLAGS)


# Clean target
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean test custom-search-test
