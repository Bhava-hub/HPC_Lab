cmake_minimum_required(VERSION 3.16)

project(Experiment5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Common source
add_library(experiment5_common
    src/common.cpp
)

target_link_libraries(experiment5_common
    PUBLIC OpenMP::OpenMP_CXX
)

target_include_directories(experiment5_common
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# -------------------------
# Part A
# -------------------------
add_executable(partA
    src/partA.cpp
)

target_link_libraries(partA
    PRIVATE experiment5_common
    PRIVATE OpenMP::OpenMP_CXX
)

# -------------------------
# Part B
# -------------------------
add_executable(partB
    src/partB.cpp
)

target_link_libraries(partB
    PRIVATE experiment5_common
    PRIVATE OpenMP::OpenMP_CXX
)

# -------------------------
# Part C
# -------------------------
add_executable(partC
    src/partC.cpp
)

target_link_libraries(partC
    PRIVATE experiment5_common
    PRIVATE OpenMP::OpenMP_CXX
)

# -------------------------
# Part D
# -------------------------
add_executable(partD
    src/partD.cpp
)

target_link_libraries(partD
    PRIVATE experiment5_common
    PRIVATE OpenMP::OpenMP_CXX
)