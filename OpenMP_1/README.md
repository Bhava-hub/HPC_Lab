# Experiment 5 — OpenMP Work-Sharing, Scheduling, Reduction & Scaling

## Overview

This experiment explores higher-level OpenMP constructs for parallel loop execution, workload scheduling, safe accumulation, and performance scaling.

The experiment builds on **Experiment 4**, where loop iterations were manually divided among threads using each thread's ID and team size.

In this experiment, OpenMP is used to automate iteration distribution and synchronization through:

* `#pragma omp parallel for`
* `reduction`
* `schedule(static)`
* `schedule(dynamic, chunk)`
* `schedule(guided)`
* `#pragma omp single`

The experiment consists of four parts:

* **Part A:** Replacing manual loop chunking with `parallel for` and using `single` for one-time work.
* **Part B:** Comparing OpenMP scheduling policies on an imbalanced workload.
* **Part C:** Reproducing a race condition and fixing it using `reduction`.
* **Part D:** Combining the best scheduling policy with reduction and measuring scalability.

---

## Objectives

The main objectives are to:

1. Understand how `parallel for` replaces manual iteration splitting.
2. Use OpenMP reduction to safely combine per-thread results.
3. Understand the purpose of the `single` construct.
4. Compare static, dynamic, and guided scheduling.
5. Observe the effect of scheduling overhead on performance.
6. Reproduce a race condition involving a shared counter.
7. Fix the race using `reduction`.
8. Compare reduction with a manually implemented private-then-combine solution.
9. Measure speed-up and parallel efficiency as thread count increases.

---

## Project Structure

```text
Experiment5/
├── CMakeLists.txt
├── include/
│   └── experiment5.hpp
├── src/
│   ├── common.cpp
│   ├── partA.cpp
│   ├── partB.cpp
│   ├── partC.cpp
│   └── partD.cpp
├── build/
└── README.md
```

### File Description

| File                      | Description                                                            |
| ------------------------- | ---------------------------------------------------------------------- |
| `CMakeLists.txt`          | CMake configuration for building all experiment parts                  |
| `include/experiment5.hpp` | Shared declarations, constants, and function prototypes                |
| `src/common.cpp`          | Shared array, dictionary, hash, and serial-audit utilities             |
| `src/partA.cpp`           | Parallel reduction, `single` banner, and dictionary-loading experiment |
| `src/partB.cpp`           | Scheduling policy comparison                                           |
| `src/partC.cpp`           | Race reproduction and reduction-based fix                              |
| `src/partD.cpp`           | Scaling experiment using the best schedule and reduction               |
| `build/`                  | CMake-generated files and executables                                  |
| `README.md`               | Experiment documentation                                               |

---

# Requirements

The experiment requires:

* C++17 or later
* OpenMP
* CMake 3.16 or later
* A compiler with OpenMP support

For Ubuntu:

```bash
sudo apt update
sudo apt install build-essential cmake
```

---

# Building the Project

From the project root:

```bash
mkdir -p build
cd build
cmake ..
make -j
```

This generates four executables:

```text
partA
partB
partC
partD
```

---

# Running the Experiments

From the `build/` directory:

### Part A

```bash
./partA
```

### Part B

```bash
./partB
```

### Part C

```bash
./partC
```

### Part D

```bash
./partD
```

---

# Part A — `parallel for`, Reduction and `single`

## 1. Parallel Array Sum

Experiment 4 manually calculated the iteration range for every thread:

```cpp
int T = omp_get_num_threads();
int tid = omp_get_thread_num();

long lo = tid * N / T;
long hi = (tid + 1) * N / T;
```

Experiment 5 replaces this manual work with:

```cpp
double total = 0.0;

#pragma omp parallel for reduction(+:total)
for (long i = 0; i < N; i++) {
    total += a[i];
}
```

OpenMP automatically creates the team of threads and distributes the loop iterations.

The reduction creates a private `total` for each thread and safely combines the partial sums at the end.

### Result

Observed values:

```text
Serial reference  : 5000015.808513110503554
Parallel reduction: 5000015.808514459058642
Difference         : 1.348555088043213e-06
```

The small difference is caused by floating-point rounding because the additions are performed in a different order during the parallel reduction.

---

## 2. `single` Banner

A separate parallel region was used:

```cpp
#pragma omp parallel
{
    #pragma omp single
    {
        printf("Banner: OpenMP team size = %d\n",
               omp_get_num_threads());
    }
}
```

The banner was tested at:

* 2 threads
* 4 threads
* 8 threads

### Result

| Threads | Banner Prints |
| ------: | ------------: |
|       2 |             1 |
|       4 |             1 |
|       8 |             1 |

The banner printed exactly once for every team size.

The thread executing the `single` block does not have to be thread 0.

---

## 3. Dictionary Loading

Dictionary loading was measured in two situations:

1. Before entering the parallel region.
2. Inside a `single` block within a parallel region.

Observed results:

```text
Load before parallel region : 0.506210690 s
Load inside single/parallel  : 0.508385816 s
Difference                   : 0.002175126 s
```

The difference was approximately **0.43%**, indicating that there was no significant difference in the dictionary-loading operation itself. The small additional cost is consistent with entering the parallel region and synchronization associated with `single`.

---

# Part B — Scheduling Policies

A synthetic log-scan workload containing:

```text
200,000 lines
```

was generated.

Most lines had:

```text
20 units of work
```

while rare expensive lines had:

```text
20,000 units of work
```

The same line-cost array was reused for every scheduling experiment.

The fixed seed was:

```text
12345
```

The experiment used:

```text
8 threads
```

There were:

```text
186 expensive lines
```

## Results

Median execution times:

| Schedule      | Median Time (s) |
| ------------- | --------------: |
| `static`      |     0.003385971 |
| `dynamic,1`   |     0.004344463 |
| `dynamic,100` |     0.002247164 |
| `guided`      | **0.002185421** |

### Ranking

| Rank | Schedule      | Median Time (s) |
| ---: | ------------- | --------------: |
|    1 | `guided`      | **0.002185421** |
|    2 | `dynamic,100` |     0.002247164 |
|    3 | `static`      |     0.003385971 |
|    4 | `dynamic,1`   |     0.004344463 |

### Observation

`guided` performed best because it provides dynamic load balancing while gradually reducing chunk sizes, which limits scheduling overhead.

`dynamic,1` performed worst because assigning one iteration at a time creates significant scheduling overhead for 200,000 iterations.

Therefore, **`guided` was selected as the best scheduling policy for Part D**.

---

# Part C — Reduction and Race Condition

A synthetic dictionary containing:

```text
1,000,000 candidates
```

was used.

One known password was planted:

```text
target_password
```

The target hash was:

```text
4914591545138680372
```

The trusted serial result was:

```text
1 match
```

The experiment used:

```text
8 threads
```

---

## 1. Unprotected Counter

The intentionally incorrect version used:

```cpp
#pragma omp parallel for
for (...) {
    if (...) {
        matches++;
    }
}
```

The shared `matches++` operation is a read-modify-write operation and is vulnerable to a race condition.

### Observed Results

```text
Run 1: matches = 1
Run 2: matches = 1
Run 3: matches = 1
Run 4: matches = 1
Run 5: matches = 1
```

Although all five runs produced the correct result, this does **not** prove that the program is race-free.

The result can be correct by chance because the race does not necessarily manifest visibly in every execution.

---

## 2. Reduction Fix

The race was fixed using:

```cpp
#pragma omp parallel for reduction(+:matches)
```

All five reduction runs produced:

```text
matches = 1
```

and passed the correctness check.

This demonstrates that OpenMP reduction safely creates private copies of the counter and combines them after the parallel loop.

---

## 3. Timing Comparison

The measured results were:

| Implementation                | Correct |    Time (s) |
| ----------------------------- | :-----: | ----------: |
| Experiment 4 `critical-smart` |   Yes   | 0.004770111 |
| Reduction                     |   Yes   | 0.030005266 |

In this measurement, `critical-smart` was faster than reduction.

Both approaches use the same basic private-then-combine concept:

```text
Private work
     ↓
Per-thread result
     ↓
Safe combination
     ↓
Final result
```

The difference is that OpenMP performs the private-copy creation and final combination automatically for a reduction.

---

# Part D — Scaling

Part D combines:

```text
Best scheduling policy from Part B
+
Reduction from Part C
```

The best scheduling policy identified in Part B was:

```text
guided
```

Therefore, the final kernel should use:

```cpp
#pragma omp parallel for schedule(guided) reduction(+:matches)
```

The dictionary size was:

```text
n = 1,000,000
```

The serial audit time was:

```text
0.139425009 s
```

## Scaling Metrics

Speed-up:

```text
Speed-up = Serial Time / Parallel Time
```

Efficiency:

```text
Efficiency = Speed-up / Number of Threads
```

The thread sweep used:

```text
1, 2, 4, 8, 12
```

where 12 was the machine's logical core count.

## Results

| Threads | Execution Time (s) | Speed-up | Efficiency |
| ------: | -----------------: | -------: | ---------: |
|       1 |        0.139248644 | 1.001267 |   1.001267 |
|       2 |        0.071023278 | 1.963089 |   0.981544 |
|       4 |        0.036763194 | 3.792516 |   0.948129 |
|       8 |        0.023837225 | 5.849045 |   0.731131 |
|      12 |        0.020107976 | 6.933816 |   0.577818 |

### Observation

Performance improved as the number of threads increased, but efficiency decreased at higher thread counts.

At 2 threads, efficiency was approximately:

```text
98.15%
```

At 12 threads, efficiency decreased to:

```text
57.78%
```

This demonstrates diminishing returns as the number of threads approaches the available logical cores.

Scheduling overhead, reduction overhead, thread-management overhead, and hardware resource contention become increasingly significant at higher thread counts.

---

# Key OpenMP Concepts Demonstrated

## `parallel for`

`parallel for` automatically:

1. Creates an OpenMP team.
2. Divides loop iterations among the threads.
3. Assigns iterations to the threads.
4. Synchronizes the threads at the end of the loop.

It replaces manual calculations involving:

```cpp
omp_get_thread_num()
omp_get_num_threads()
lo
hi
```

---

## `reduction`

A reduction gives each thread a private copy of a variable.

For:

```cpp
reduction(+:matches)
```

each thread updates its own `matches`, and OpenMP combines the private values into the final result.

This prevents the race caused by:

```cpp
matches++;
```

---

## `single`

`single` ensures that only one thread executes a block:

```cpp
#pragma omp single
{
    // executed once
}
```

By default, the other threads wait at the implicit barrier at the end of the block.

---

# Scheduling Policies

## Static

```cpp
schedule(static)
```

Iterations are divided among threads in predetermined chunks.

### Advantages

* Very low scheduling overhead.
* Predictable iteration distribution.
* Good for balanced workloads.

### Disadvantage

* Poor load balancing when iteration costs vary significantly.

---

## Dynamic

```cpp
schedule(dynamic, chunk)
```

Threads receive chunks of iterations dynamically as they finish their previous work.

### Advantages

* Better load balancing.
* Useful for irregular workloads.

### Disadvantages

* Higher scheduling overhead.
* Very small chunks can significantly increase overhead.

---

## Guided

```cpp
schedule(guided)
```

Guided scheduling starts with larger chunks and gradually decreases the chunk size.

### Advantages

* Good load balancing.
* Lower overhead than extremely fine-grained dynamic scheduling.
* Effective for workloads with irregular iteration costs.

For this experiment, `guided` produced the best performance.

---

# Key Results Summary

| Part   | Main Finding                                                          |
| ------ | --------------------------------------------------------------------- |
| Part A | `parallel for` successfully replaced manual chunk calculation         |
| Part A | Reduction produced the correct sum within floating-point tolerance    |
| Part A | `single` printed the banner exactly once at 2, 4, and 8 threads       |
| Part A | Dictionary loading times were almost identical                        |
| Part B | `guided` was the fastest scheduling policy                            |
| Part B | `dynamic,1` was the slowest due to scheduling overhead                |
| Part C | Unprotected `matches++` produced the correct result by chance         |
| Part C | Reduction consistently produced the correct result                    |
| Part C | `critical-smart` was faster than reduction in the measured comparison |
| Part D | Performance improved with increasing thread count                     |
| Part D | Efficiency decreased at higher thread counts                          |
| Part D | 12 threads achieved a speed-up of 6.933816×                           |

---

# Final Conclusion

This experiment demonstrated how OpenMP provides higher-level mechanisms for parallel programming while addressing common correctness and performance issues.

The main conclusions are:

1. `parallel for` eliminates the need to manually calculate thread-specific loop ranges.
2. `reduction` safely handles shared accumulation without requiring explicit synchronization for every update.
3. `single` allows one thread to perform one-time work inside an existing parallel team.
4. Scheduling policy has a significant effect on irregular workloads.
5. `guided` was the best-performing schedule for the synthetic imbalanced workload.
6. Very small dynamic chunks can introduce enough scheduling overhead to reduce performance.
7. A race condition can produce the correct answer by chance, so repeated correct output does not prove race freedom.
8. Reduction and hand-written private-then-combine solutions use the same fundamental strategy.
9. Increasing the number of threads improved execution time, but parallel efficiency decreased at higher thread counts.
10. The 12-thread experiment achieved a speed-up of approximately **6.93×** relative to the serial audit.

---

# Rebuild From Scratch

To remove the existing build and rebuild the complete project:

```bash
rm -rf build
mkdir build
cd build
cmake ..
make -j
```

Then run:

```bash
./partA
./partB
./partC
./partD
```

> **Important:** Part D should use the best schedule identified in Part B. In this experiment, that schedule was `guided`.
