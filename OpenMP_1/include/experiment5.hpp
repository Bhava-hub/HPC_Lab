#ifndef EXPERIMENT5_HPP
#define EXPERIMENT5_HPP

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <random>
#include <omp.h>

// ============================================================
// Experiment 4-compatible array configuration
// ============================================================

// IMPORTANT:
// Replace these with the exact N and seed from Experiment 4.
constexpr long ARRAY_N = 10000000;
constexpr unsigned int ARRAY_SEED = 12345;

// ============================================================
// Part C dictionary configuration
// ============================================================

constexpr long DICTIONARY_SIZE = 1000000;

constexpr unsigned int DICTIONARY_SEED = 54321;

// ============================================================
// Shared array utilities
// ============================================================

void generate_array(
    std::vector<double>& a,
    unsigned int seed
);

double serial_array_sum(
    const std::vector<double>& a
);

// ============================================================
// Dictionary utilities
// ============================================================

void load_dictionary(
    std::vector<std::string>& dictionary,
    long n,
    unsigned int seed
);

unsigned long toy_hash(
    const std::string& s
);

// ============================================================
// Password audit utilities
// ============================================================

long serial_password_audit(
    const std::vector<std::string>& dictionary,
    unsigned long target_hash
);

#endif