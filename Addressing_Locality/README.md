
```markdown
# Matrix Multiplication Cache Locality Analysis using C++ and perf

## Overview

This project studies the effect of **loop ordering on matrix multiplication performance**.

The goal is to analyze:

- Temporal locality
- Spatial locality
- Cache behavior
- CPU cycles
- Instructions per cycle (IPC)
- Cache misses

A `2500 x 2500` matrix multiplication is implemented using all six possible loop permutations:

1. `ijk`
2. `ikj`
3. `jik`
4. `jki`
5. `kij`
6. `kji`

The performance of each version is measured using Linux `perf`.

---

# Project Structure

```

lab-2/
│
├── CMakeLists.txt
├── README.md
│
├── include/
│   ├── matrix.h
│   ├── ijk.h
│   ├── ikj.h
│   ├── jik.h
│   ├── jki.h
│   ├── kij.h
│   └── kji.h
│
├── src/
│   ├── main.cpp
│   ├── matrix.cpp
│   ├── ijk.cpp
│   ├── ikj.cpp
│   ├── jik.cpp
│   ├── jki.cpp
│   ├── kij.cpp
│   └── kji.cpp
│
└── build/

```

---

# Matrix Multiplication

The multiplication performed is:

```

C = A × B

```

where:

```

C[i][j] = Σ A[i][k] * B[k][j]

````

The matrix size is:

```cpp
#define SIZE 2500
````

Matrices are stored in row-major order using:

```cpp
vector<vector<double>>
```

---

# Loop Permutations

## 1. ijk

```cpp
for(i)
    for(j)
        for(k)
            C[i][j] += A[i][k] * B[k][j];
```

Access pattern:

* A: good spatial locality
* B: poor locality because columns are accessed
* C: good temporal locality

---

## 2. ikj

```cpp
for(i)
    for(k)
        for(j)
            C[i][j] += A[i][k] * B[k][j];
```

Access pattern:

* A: reused
* B: sequential row access
* C: sequential row access

Usually gives better cache performance for row-major matrices.

---

## 3. jik

```cpp
for(j)
    for(i)
        for(k)
            C[i][j] += A[i][k] * B[k][j];
```

---

## 4. jki

```cpp
for(j)
    for(k)
        for(i)
            C[i][j] += A[i][k] * B[k][j];
```

---

## 5. kij

```cpp
for(k)
    for(i)
        for(j)
            C[i][j] += A[i][k] * B[k][j];
```

---

## 6. kji

```cpp
for(k)
    for(j)
        for(i)
            C[i][j] += A[i][k] * B[k][j];
```

---

# Building Using CMake

Create build directory:

```bash
mkdir build
cd build
```

Configure:

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

Compile:

```bash
make
```

Executable generated:

```
build/matrix_mul
```

---

# Running Matrix Multiplication

Run a particular loop ordering:

```bash
./matrix_mul ijk
```

Examples:

```bash
./matrix_mul ikj

./matrix_mul jik

./matrix_mul jki

./matrix_mul kij

./matrix_mul kji
```

All six versions should produce the same matrix result.

Only the execution time and cache behavior change.

---

# Installing perf

Check perf:

```bash
perf --version
```

If required:

```bash
sudo apt install linux-tools-generic-hwe-24.04
```

For systems where the kernel-specific perf is not found, run:

```bash
/usr/lib/linux-tools/<kernel-version>/perf
```

Example:

```bash
/usr/lib/linux-tools/6.17.0-40-generic/perf
```

---

# Allowing Performance Counters

If you get:

```
Access to performance monitoring operations is limited
```

set:

```bash
sudo sysctl kernel.perf_event_paranoid=1
```

For permanent configuration:

Create:

```bash
sudo nano /etc/sysctl.d/99-perf.conf
```

Add:

```
kernel.perf_event_paranoid=1
```

Apply:

```bash
sudo sysctl --system
```

---

# Running perf

Basic profiling:

```bash
perf stat ./matrix_mul ijk
```

Detailed cache profiling:

```bash
perf stat \
-e cache-references,cache-misses,cycles,instructions \
./matrix_mul ijk
```

Repeat for all versions:

```bash
perf stat \
-e cache-references,cache-misses,cycles,instructions \
./matrix_mul ikj
```

---

# Saving Results

Save output:

```bash
perf stat \
-e cache-references,cache-misses,cycles,instructions \
./matrix_mul ijk 2> ijk.txt
```

Files:

```
ijk.txt
ikj.txt
jik.txt
jki.txt
kij.txt
kji.txt
```

---

# Automating All Tests

Run:

```bash
for x in ijk ikj jik jki kij kji
do
echo "===== $x ====="

perf stat \
-e cache-references,cache-misses,cycles,instructions \
./matrix_mul $x

done
```

---

# Example Results

Example:

```
===== ijk =====

62,282,632,097 cache-references
2,959,707,571 cache-misses
300,741,999,305 cycles
142,498,246,739 instructions

Time:
69.83 seconds
```

```
===== ikj =====

3,256,098,351 cache-references
1,815,762,718 cache-misses
38,450,057,120 cycles
63,889,486,390 instructions

Time:
8.90 seconds
```

---

# Analysis

The output matrix remains mathematically identical for all loop orders.

The loop ordering only changes:

* Memory access pattern
* Cache utilization
* CPU stalls
* Execution time

For row-major matrices:

`ikj` generally performs better because:

* Matrix rows are accessed sequentially
* Cache lines are used efficiently
* Temporal locality is improved

Performance should be evaluated using:

* Execution time
* Total cache misses
* CPU cycles
* Instructions per cycle (IPC)

Not only cache miss percentage.

---

# Conclusion

This experiment demonstrates how changing loop order in matrix multiplication affects hardware performance.

Even though all six implementations compute the same mathematical result, their execution speed differs significantly because of differences in:

* Spatial locality
* Temporal locality
* Cache behavior
* Memory access patterns

The best loop ordering depends on the matrix storage layout and hardware architecture.

```

This README matches your actual setup: **CMake + header files + 2500×2500 matrix + six permutations + Linux perf analysis**.
```
