#include "experiment5.hpp"

// ============================================================
// Generate the same type of array used in Experiment 4
// ============================================================

void generate_array(
    std::vector<double>& a,
    unsigned int seed
) {
    std::mt19937 rng(seed);

    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (long i = 0; i < static_cast<long>(a.size()); i++) {
        a[i] = dist(rng);
    }
}

// ============================================================
// Trusted serial reference sum
// ============================================================

double serial_array_sum(
    const std::vector<double>& a
) {
    double total = 0.0;

    for (long i = 0; i < static_cast<long>(a.size()); i++) {
        total += a[i];
    }

    return total;
}

// ============================================================
// Dictionary loader
// ============================================================
//
// Creates synthetic candidate strings.
//
// One planted password is:
//
//     target_password
//
// The target is inserted at dictionary index 0.
//
// ============================================================

void load_dictionary(
    std::vector<std::string>& dictionary,
    long n,
    unsigned int seed
) {
    dictionary.clear();
    dictionary.resize(n);

    std::mt19937 rng(seed);

    std::uniform_int_distribution<int> letter_dist(0, 25);

    // Plant the target password at index 0.
    if (n > 0) {
        dictionary[0] = "target_password";
    }

    // Generate the remaining candidates.
    for (long i = 1; i < n; i++) {

        std::string word;

        word.reserve(16);

        for (int j = 0; j < 16; j++) {
            char c =
                static_cast<char>('a' + letter_dist(rng));

            word += c;
        }

        dictionary[i] = word;
    }
}

// ============================================================
// Toy hash function
// ============================================================

unsigned long toy_hash(
    const std::string& s
) {
    unsigned long hash = 5381;

    for (char c : s) {
        hash = ((hash << 5) + hash) ^
               static_cast<unsigned long>(c);
    }

    return hash;
}

// ============================================================
// Trusted serial password audit
// ============================================================

long serial_password_audit(
    const std::vector<std::string>& dictionary,
    unsigned long target_hash
) {
    long matches = 0;

    for (long i = 0;
         i < static_cast<long>(dictionary.size());
         i++) {

        if (toy_hash(dictionary[i]) == target_hash) {
            matches++;
        }
    }

    return matches;
}