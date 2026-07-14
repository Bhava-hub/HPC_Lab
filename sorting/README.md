# CMake: Complete Explanation

## 1. What is CMake?

**CMake** is a **cross-platform build system generator**. It does not directly compile your code. Instead, it creates build files for the compiler and build tools available on your system.

For example:

* On Linux → CMake generates **Makefiles** or **Ninja build files**
* On Windows → CMake can generate **Visual Studio projects**
* On macOS → CMake can generate **Xcode projects** or Makefiles

The main purpose of CMake is to manage the process of:

* Finding source files
* Finding header files
* Linking libraries
* Setting compiler options
* Managing dependencies
* Building large projects consistently

Instead of manually writing long compiler commands like:

```bash
g++ main.cpp src/bubble_sort.cpp src/merge_sort.cpp src/counting_sort.cpp -Iinclude -o SortingComparison
```

CMake allows you to describe your project once using a `CMakeLists.txt` file.

---

# 2. Why use CMake?

Without CMake, for a small project you might compile using:

```bash
g++ main.cpp bubble_sort.cpp merge_sort.cpp counting_sort.cpp -o program
```

For a larger project, this becomes difficult:

* Hundreds of `.cpp` files
* Many libraries
* Different operating systems
* Different compilers
* Different compiler options

CMake solves this by automating the build process.

You only write:

```cmake
add_executable(
    SortingComparison
    src/main.cpp
    src/bubble_sort.cpp
    src/merge_sort.cpp
    src/counting_sort.cpp
)
```

CMake understands:

* These are source files
* They belong to one executable
* They need to be compiled and linked together

---

# 3. CMake Project Structure

Your current project:

```
SortingProject/
│
├── CMakeLists.txt
│
├── include/
│   └── sorting.h
│
├── src/
│   ├── main.cpp
│   ├── bubble_sort.cpp
│   ├── merge_sort.cpp
│   └── counting_sort.cpp
│
├── tests/
│   └── test_sorting.cpp
│
└── build/
```

Each folder has a purpose.

---

## `src/` Folder

Contains implementation files.

Example:

```
src/
├── main.cpp
├── bubble_sort.cpp
├── merge_sort.cpp
└── counting_sort.cpp
```

These contain the actual algorithms.

Example:

```cpp
void bubbleSort(vector<int>& arr)
{
    ...
}
```

---

## `include/` Folder

Contains header files.

Example:

```
include/
└── sorting.h
```

The header contains declarations:

```cpp
void bubbleSort(vector<int>& arr);

void mergeSort(vector<int>& arr, int left, int right);

void countingSort(vector<int>& arr);
```

It tells other files:

> These functions exist somewhere. You can use them.

---

## `tests/` Folder

Contains automated testing code.

Example:

```
tests/
└── test_sorting.cpp
```

It checks:

* Are algorithms working?
* Are outputs correct?
* Which algorithm is faster?

---

# 4. Understanding CMakeLists.txt

Your CMake file:

```cmake
cmake_minimum_required(VERSION 3.10)
```

Means:

> The project requires at least CMake version 3.10.

---

```cmake
project(SortingComparison)
```

Creates a project named:

```
SortingComparison
```

---

```cmake
set(CMAKE_CXX_STANDARD 17)
```

Sets the C++ version.

Equivalent to:

```bash
g++ -std=c++17
```

Your compiler will use C++17 features.

---

```cmake
add_executable(
    SortingComparison
    src/main.cpp
    src/bubble_sort.cpp
    src/merge_sort.cpp
    src/counting_sort.cpp
)
```

Creates an executable.

The final program becomes:

```
SortingComparison
```

It combines:

```
main.cpp
+
bubble_sort.cpp
+
merge_sort.cpp
+
counting_sort.cpp
```

into one executable.

---

```cmake
target_include_directories(
    SortingComparison
    PRIVATE
    include
)
```

Tells CMake:

> Look inside the include folder when searching for header files.

So this:

```cpp
#include "sorting.h"
```

will search:

```
include/sorting.h
```

---

# 5. Important CMake Commands

## Create build directory

Command:

```bash
mkdir build
```

Creates:

```
build/
```

This separates generated files from your source code.

---

## Enter build directory

```bash
cd build
```

Now you are working inside the build environment.

---

## Generate build files

```bash
cmake ..
```

Meaning:

```
cmake [location of CMakeLists.txt]
```

`..` means:

> Go one directory up

Your structure:

```
build/
    |
    |
    └── CMakeLists.txt  <-- parent folder
```

CMake reads:

```
../CMakeLists.txt
```

and generates build instructions.

---

## Compile the project

```bash
make
```

This executes the generated instructions.

It:

1. Compiles `.cpp` files

Example:

```
main.cpp
        |
        v
main.o
```

```
bubble_sort.cpp
        |
        v
bubble_sort.o
```

2. Links object files:

```
main.o
bubble_sort.o
merge_sort.o
counting_sort.o

        |
        v

SortingComparison
```

---

## Run program

```bash
./SortingComparison
```

The executable starts.

---

# 6. Why is the Build Folder Important?

The build folder contains **generated files**.

Example:

```
build/
│
├── CMakeCache.txt
├── Makefile
├── CMakeFiles/
│
└── SortingComparison
```

---

## CMakeCache.txt

Stores configuration information:

Example:

```
Compiler = g++
C++ standard = 17
Build type = Debug
```

---

## Makefile

Contains instructions:

Example:

```
Compile main.cpp

Compile bubble_sort.cpp

Link everything
```

---

## CMakeFiles/

Contains temporary compilation information:

* Object files
* Dependency information
* Compiler settings

---

## Executable

Example:

```
SortingComparison
```

This is the final program.

---

# 7. How CMake Makes Coding Easier

## Without CMake:

Every time you add a file:

```
new_algorithm.cpp
```

you manually update:

```bash
g++ main.cpp old.cpp new_algorithm.cpp
```

You also manually specify:

* Include paths
* Libraries
* Compiler flags

---

## With CMake:

Add one line:

```cmake
src/new_algorithm.cpp
```

Run:

```bash
cmake ..
make
```

Done.

---

# 8. How CMake Improves Efficient Computing

Efficient computing is not only about faster algorithms. It also includes efficient development and resource usage.

## 1. Incremental Compilation

Suppose you modify:

```
bubble_sort.cpp
```

CMake + Make only recompiles:

```
bubble_sort.cpp
```

It does not rebuild everything.

This saves:

* CPU time
* Compilation time
* Developer time

---

## 2. Dependency Management

CMake understands relationships:

```
main.cpp
   |
   |
sorting.h
   |
   |
bubble_sort.cpp
```

If the header changes, CMake knows which files need rebuilding.

---

## 3. Portability

The same project can run on:

Linux:

```bash
cmake ..
make
```

Windows:

```
cmake ..
Visual Studio Build
```

No rewriting required.

---

## 4. Automated Testing

With CMake and CTest:

```
Code
 |
 |
Build
 |
 |
Test
 |
 |
Performance Analysis
```

Your sorting project can automatically check:

* Correctness
* Runtime
* Performance differences

---

# 9. Overall Workflow

The complete workflow is:

```
Write Code
    |
    |
CMakeLists.txt
    |
    |
cmake ..
    |
    |
Generate Build Files
    |
    |
make
    |
    |
Executable Created
    |
    |
Run Program
    |
    |
Measure Performance
```

---

# 10. In Your Sorting Project

Your system now demonstrates efficient computing because it:

* Separates interface (`include`) from implementation (`src`)
* Uses automated builds through CMake
* Uses automated testing
* Measures algorithm performance
* Compares computational complexity

The algorithms show:

| Algorithm     | Complexity | Purpose                           |
| ------------- | ---------- | --------------------------------- |
| Bubble Sort   | O(n²)      | Demonstrates slow scaling         |
| Merge Sort    | O(n log n) | Efficient comparison sorting      |
| Counting Sort | O(n+k)     | Faster for limited integer ranges |

CMake provides the infrastructure to build, test, and evaluate these algorithms efficiently.
