cmake_minimum_required(VERSION 3.10)

project(SortingComparison)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    SortingComparison
    src/main.cpp
    src/bubble.cpp
    src/merge.cpp
    src/count.cpp
)

target_include_directories(
    SortingComparison
    PRIVATE
    include
)


# Testing
enable_testing()

add_executable(
    SortingTests
    tests/test_sorting.cpp
    src/bubble.cpp
    src/merge.cpp
    src/count.cpp
)

target_include_directories(
    SortingTests
    PRIVATE
    include
)


add_test(
    NAME SortingAlgorithmTests
    COMMAND SortingTests
)