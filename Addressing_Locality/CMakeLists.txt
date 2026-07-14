cmake_minimum_required(VERSION 3.10)

project(MatrixLocality)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(matrix_mul
    src/main.cpp
    src/matrix.cpp
    src/ijk.cpp
    src/ikj.cpp
    src/jik.cpp
    src/jki.cpp
    src/kij.cpp
    src/kji.cpp
)