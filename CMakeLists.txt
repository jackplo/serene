cmake_minimum_required(VERSION 3.16)
project(Serene LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings and debugging
add_compile_options(-Wall -Wextra -g)

# Use pkg-config to find gtkmm-4.0
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

add_subdirectory(thirdparty/SQLiteCpp)

# Include paths
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/lib/include
    ${PROJECT_SOURCE_DIR}
)

# Group source files
file(GLOB_RECURSE CORE_SRC src/core/*.cpp)
file(GLOB_RECURSE COMPONENTS_SRC src/components/*.cpp)
file(GLOB_RECURSE ENGINE_SRC lib/engine/*.cpp)
file(GLOB_RECURSE MODEL_SRC src/models/*.cpp)

# Exclude main.cpp from CORE_SRC
list(REMOVE_ITEM CORE_SRC ${PROJECT_SOURCE_DIR}/src/core/main.cpp)

# Create common library (shared between app and test)
add_library(common STATIC
    ${CORE_SRC}
    ${COMPONENTS_SRC}
    ${ENGINE_SRC}
    ${MODEL_SRC}
)

target_include_directories(common PRIVATE
    ${GTKMM_INCLUDE_DIRS}
)

target_link_libraries(common
    ${GTKMM_LIBRARIES}
    SQLiteCpp
    sqlite3
    pthread
    dl
)

# Main executable
add_executable(serene ${PROJECT_SOURCE_DIR}/src/core/main.cpp)
target_link_libraries(serene common)

add_custom_command(TARGET serene POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:serene>/assets
)

# ========================
# Test Executable (test_runner)
# ========================

# Gather test sources
file(GLOB_RECURSE TEST_SRC tests/*.cpp)

# Optional: Exclude specific test files if needed
# list(REMOVE_ITEM TEST_SRC ${PROJECT_SOURCE_DIR}/tests/some_file.cpp)

# Create test executable
add_executable(test_runner ${TEST_SRC})

# Link only lib/engine (ENGINE_SRC) to tests, not full app
target_sources(test_runner PRIVATE ${ENGINE_SRC})

target_include_directories(test_runner PRIVATE
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/lib/include
    ${GTKMM_INCLUDE_DIRS}
)

target_link_libraries(test_runner
    SQLiteCpp
    sqlite3
    pthread
    dl
)
