#include "experiment5.hpp"

// ============================================================
// Part D configuration
// ============================================================

constexpr int AUDIT_THREADS[] = {
    1, 2, 4, 8, 12
};

constexpr int NUM_THREAD_TESTS = 5;

// ============================================================
// IMPORTANT:
//
// Change this schedule after looking at Part B.
//
// Currently using:
//     dynamic,100
//
// If Part B says guided/static/dynamic,1 is better,
// change the kernel accordingly.
// ============================================================

// ============================================================
// Combined audit kernel
//
// Best schedule + reduction
// ============================================================

long audit_combined(
    const std::vector<std::string>& dictionary,
    unsigned long target_hash
) {
    long matches = 0;

    #pragma omp parallel for schedule(dynamic,100) reduction(+:matches)
    for (long i = 0;
         i < static_cast<long>(dictionary.size());
         i++) {

        if (toy_hash(dictionary[i]) == target_hash) {
            matches++;
        }
    }

    return matches;
}

// ============================================================
// Serial audit
// ============================================================

long audit_serial(
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

// ============================================================
// Main
// ============================================================

int main()
{
    printf("============================================\n");
    printf(" Experiment 5 - Part D\n");
    printf(" Scaling Case Study\n");
    printf("============================================\n\n");

    // --------------------------------------------------------
    // Load dictionary
    // --------------------------------------------------------

    std::vector<std::string> dictionary;

    load_dictionary(
        dictionary,
        DICTIONARY_SIZE,
        DICTIONARY_SEED
    );

    const std::string target_password =
        "target_password";

    unsigned long target_hash =
        toy_hash(target_password);

    // --------------------------------------------------------
    // Serial baseline
    // --------------------------------------------------------

    double serial_start =
        omp_get_wtime();

    long serial_result =
        audit_serial(
            dictionary,
            target_hash
        );

    double serial_end =
        omp_get_wtime();

    double serial_time =
        serial_end - serial_start;

    printf(
        "Serial matches : %ld\n",
        serial_result
    );

    printf(
        "Serial time    : %.9f s\n\n",
        serial_time
    );

    printf(
        "Schedule       : dynamic,100\n"
    );

    printf(
        "Reduction      : enabled\n\n"
    );

    // --------------------------------------------------------
    // Table header
    // --------------------------------------------------------

    printf(
        "%-10s %-15s %-15s %-15s\n",
        "Threads",
        "Time (s)",
        "Speedup",
        "Efficiency"
    );

    printf(
        "------------------------------------------------------------\n"
    );

    // --------------------------------------------------------
    // Thread sweep
    // --------------------------------------------------------

    for (int i = 0;
         i < NUM_THREAD_TESTS;
         i++) {

        int threads =
            AUDIT_THREADS[i];

        omp_set_num_threads(threads);

        // Warm-up
        audit_combined(
            dictionary,
            target_hash
        );

        double start =
            omp_get_wtime();

        long result =
            audit_combined(
                dictionary,
                target_hash
            );

        double end =
            omp_get_wtime();

        double parallel_time =
            end - start;

        // Speedup:
        //
        // S = T_serial / T_parallel

        double speedup =
            serial_time / parallel_time;

        // Efficiency:
        //
        // E = speedup / number_of_threads

        double efficiency =
            speedup / threads;

        printf(
            "%-10d %-15.9f %-15.6f %-15.6f\n",
            threads,
            parallel_time,
            speedup,
            efficiency
        );

        // Correctness check
        if (result != serial_result) {

            printf(
                "WARNING: incorrect result at %d threads! "
                "Expected %ld, got %ld\n",
                threads,
                serial_result,
                result
            );
        }
    }

    printf(
        "\nPart D complete.\n"
    );

    return 0;
}