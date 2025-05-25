################################################################################
# Build Configuration
################################################################################
CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
GTK_FLAGS := $(shell pkg-config gtkmm-4.0 --cflags)
GTK_LIBS := $(shell pkg-config gtkmm-4.0 --libs)

################################################################################
# Directory Structure
################################################################################
SRC_DIR     := src
LIB_DIR     := lib
INCLUDE_DIR := include
BUILD_DIR   := build
TEST_DIR    := tests

# Source code subdirectories
CORE_DIR       := $(SRC_DIR)/core
COMPONENTS_DIR := $(SRC_DIR)/components
ENGINE_DIR     := $(LIB_DIR)/engine
MODEL_DIR      := $(SRC_DIR)/models

# Include directories
INCLUDES := \
    -I$(SRC_DIR) \
    -I$(SRC_DIR)/include \
    -I$(LIB_DIR) \
    -I$(LIB_DIR)/include \
    -I.

################################################################################
# Source Files
################################################################################
# Core application sources (excluding main)
CORE_SRC := \
    $(CORE_DIR)/serene.cpp

# UI Component sources
COMPONENTS_SRC := \
    $(COMPONENTS_DIR)/listview.cpp \
    $(COMPONENTS_DIR)/scrollview.cpp \
    $(COMPONENTS_DIR)/customsearchentry.cpp \
    $(COMPONENTS_DIR)/itemdetails.cpp

# Search engine sources
ENGINE_SRC := \
    $(ENGINE_DIR)/engine.cpp \
    $(ENGINE_DIR)/app_searcher.cpp \
    $(ENGINE_DIR)/file_searcher.cpp \
    $(ENGINE_DIR)/hybrid_file_searcher.cpp

# Data model sources
MODEL_SRC := \
    $(MODEL_DIR)/applicationobject.cpp \
    $(MODEL_DIR)/fileobject.cpp \
    $(MODEL_DIR)/combinedlistmodel.cpp

# Main entry point
MAIN_SRC := $(CORE_DIR)/main.cpp

# Test sources
TEST_SRC := \
    $(TEST_DIR)/main.cpp \
    $(TEST_DIR)/test_window.cpp

################################################################################
# Object Files
################################################################################
CORE_OBJ       := $(CORE_SRC:%.cpp=$(BUILD_DIR)/%.o)
COMPONENTS_OBJ := $(COMPONENTS_SRC:%.cpp=$(BUILD_DIR)/%.o)
ENGINE_OBJ     := $(ENGINE_SRC:%.cpp=$(BUILD_DIR)/%.o)
MODEL_OBJ      := $(MODEL_SRC:%.cpp=$(BUILD_DIR)/%.o)
MAIN_OBJ       := $(MAIN_SRC:%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJ       := $(TEST_SRC:%.cpp=$(BUILD_DIR)/%.o)

# Common objects used by both main app and tests
COMMON_OBJ := $(CORE_OBJ) $(COMPONENTS_OBJ) $(ENGINE_OBJ) $(MODEL_OBJ)

################################################################################
# Targets
################################################################################
TARGET      := serene
TEST_TARGET := $(BUILD_DIR)/test_runner

# Default target
.PHONY: all
all: $(TARGET)

# Main application build
$(TARGET): $(COMMON_OBJ) $(MAIN_OBJ)
	@echo "==> Linking main application"
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) $(GTK_FLAGS) -o $@ $^ $(GTK_LIBS)
	@./$(TARGET)

# Test build
.PHONY: test
test: $(TEST_TARGET)
	@echo "==> Running tests"
	@./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(COMMON_OBJ)
	@echo "==> Linking test runner"
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) $(GTK_FLAGS) -o $@ $^ $(GTK_LIBS)

################################################################################
# Build Rules
################################################################################
# Pattern rule for object files
$(BUILD_DIR)/%.o: %.cpp
	@echo "==> Compiling $<"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(GTK_FLAGS) $(INCLUDES) -c -o $@ $<

################################################################################
# Utility Targets
################################################################################
.PHONY: clean
clean:
	@echo "==> Cleaning build artifacts"
	@rm -rf $(BUILD_DIR) $(TARGET)
