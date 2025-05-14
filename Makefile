# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g $(shell pkg-config gtkmm-4.0 --cflags) -I. -I$(SRC_DIR)
GTK_LDFLAGS = $(shell pkg-config gtkmm-4.0 --libs)

# Directories
SRC_DIR = src
LIB_DIR = lib
INCLUDE_DIR = include
CORE_DIR = $(SRC_DIR)/core
COMPONENTS_DIR = $(SRC_DIR)/components
ENGINE_DIR = $(LIB_DIR)/engine
BUILD_DIR = build
MODEL_DIR = $(SRC_DIR)/models

# Source files
CORE_SRC = $(CORE_DIR)/main.cpp $(CORE_DIR)/serene.cpp
COMPONENTS_SRC = $(COMPONENTS_DIR)/listview.cpp $(COMPONENTS_DIR)/scrollview.cpp $(COMPONENTS_DIR)/customsearchentry.cpp $(COMPONENTS_DIR)/itemdetails.cpp
ENGINE_SRC = $(ENGINE_DIR)/app_searcher.cpp $(ENGINE_DIR)/file_searcher.cpp $(ENGINE_DIR)/hybrid_file_searcher.cpp
MODEL_SRC = $(MODEL_DIR)/applicationobject.cpp $(MODEL_DIR)/applicationlistmodel.cpp $(MODEL_DIR)/fileobject.cpp $(MODEL_DIR)/combinedlistmodel.cpp
TEST_SRC = $(ENGINE_DIR)/test_app_searcher.cpp
FILE_SEARCHER_TEST_SRC = $(ENGINE_DIR)/test_file_searcher.cpp
HYBRID_FILE_SEARCHER_TEST_SRC = $(ENGINE_DIR)/test_hybrid_file_searcher.cpp
CUSTOM_SEARCH_TEST_SRC = tests/main.cpp tests/test_window.cpp $(COMPONENTS_DIR)/customsearchentry.cpp
ITEM_DETAILS_TEST_SRC = tests/test_itemdetails.cpp $(COMPONENTS_DIR)/itemdetails.cpp

# Object files
CORE_OBJ = $(CORE_SRC:%.cpp=$(BUILD_DIR)/%.o)
COMPONENTS_OBJ = $(COMPONENTS_SRC:%.cpp=$(BUILD_DIR)/%.o)
ENGINE_OBJ = $(ENGINE_SRC:%.cpp=$(BUILD_DIR)/%.o)
MODEL_OBJ = $(MODEL_SRC:%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJ = $(TEST_SRC:%.cpp=$(BUILD_DIR)/%.o)
FILE_SEARCHER_TEST_OBJ = $(FILE_SEARCHER_TEST_SRC:%.cpp=$(BUILD_DIR)/%.o)
HYBRID_FILE_SEARCHER_TEST_OBJ = $(HYBRID_FILE_SEARCHER_TEST_SRC:%.cpp=$(BUILD_DIR)/%.o)
CUSTOM_SEARCH_TEST_OBJ = $(CUSTOM_SEARCH_TEST_SRC:%.cpp=$(BUILD_DIR)/%.o)
ITEM_DETAILS_TEST_OBJ = $(ITEM_DETAILS_TEST_SRC:%.cpp=$(BUILD_DIR)/%.o)

# Target executable
TARGET = serene
TEST_BIN = $(BUILD_DIR)/test_app_searcher
FILE_SEARCHER_TEST_BIN = $(BUILD_DIR)/test_file_searcher
HYBRID_FILE_SEARCHER_TEST_BIN = $(BUILD_DIR)/test_hybrid_file_searcher
CUSTOM_SEARCH_TEST_BIN = $(BUILD_DIR)/custom_search_test
ITEM_DETAILS_TEST_BIN = $(BUILD_DIR)/test_itemdetails

# All target
all: $(TARGET)

# Main target
$(TARGET): $(CORE_OBJ) $(COMPONENTS_OBJ) $(ENGINE_OBJ) $(MODEL_OBJ)
	@echo "Linking..."
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(GTK_LDFLAGS)

# Test targets
test: $(TEST_BIN)
	@echo "Running application searcher test..."
	@$(TEST_BIN)

test-file-searcher: $(FILE_SEARCHER_TEST_BIN)
	@echo "Running file searcher test..."
	@$(FILE_SEARCHER_TEST_BIN)

test-hybrid-file-searcher: $(HYBRID_FILE_SEARCHER_TEST_BIN)
	@echo "Running hybrid file searcher test..."
	@$(HYBRID_FILE_SEARCHER_TEST_BIN)

test-item-details: $(ITEM_DETAILS_TEST_BIN)
	@echo "Running item details test..."
	@$(ITEM_DETAILS_TEST_BIN)

$(TEST_BIN): $(TEST_OBJ) $(ENGINE_OBJ)
	@echo "Linking test program..."
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -o $@ $^

$(FILE_SEARCHER_TEST_BIN): $(FILE_SEARCHER_TEST_OBJ) $(ENGINE_OBJ)
	@echo "Linking file searcher test program..."
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -o $@ $^

$(HYBRID_FILE_SEARCHER_TEST_BIN): $(HYBRID_FILE_SEARCHER_TEST_OBJ) $(ENGINE_OBJ)
	@echo "Linking hybrid file searcher test program..."
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -o $@ $^

$(ITEM_DETAILS_TEST_BIN): $(ITEM_DETAILS_TEST_OBJ) $(MODEL_OBJ)
	@echo "Linking item details test program..."
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(GTK_LDFLAGS)

# Pattern rule for object files
$(BUILD_DIR)/%.o: %.cpp
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

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

.PHONY: all clean test test-file-searcher test-hybrid-file-searcher custom-search-test test-item-details
