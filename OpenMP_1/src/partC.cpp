#include "experiment5.hpp"

// ============================================================
// Part C configuration
// ============================================================

constexpr int PART_C_THREADS = 8;

// ============================================================
// Unprotected password audit
//
// INTENTIONALLY INCORRECT.
// This is the race-condition version.
// ============================================================

long audit_unprotected(
    const std::vector<std::string>& dictionary,
    unsigned long target_hash
) {
    long matches = 0;

    #pragma omp parallel for
    for (long i = 0;
         i < static_cast<long>(dictionary.size());
         i++) {

        if (toy_hash(dictionary[i]) == target_hash) {

            // INTENTIONAL RACE
            matches++;
        }
    }

    return matches;
}

// ============================================================
// Reduction version
// ============================================================

long audit_reduction(
    const std::vector<std::string>& dictionary,
    unsigned long target_hash
) {
    long matches = 0;

    #pragma omp parallel for reduction(+:matches)
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
// Critical-smart version
//
// Each thread maintains a local counter.
// Only the final combination is protected.
//
// This is the "smart" pattern from Experiment 4.
// ============================================================

long audit_critical_smart(
    const std::vector<std::string>& dictionary,
    unsigned long target_hash
) {
    long matches = 0;

    #pragma omp parallel
    {
        long local_matches = 0;

        #pragma omp for
        for (long i = 0;
             i < static_cast<long>(dictionary.size());
             i++) {

            if (toy_hash(dictionary[i]) == target_hash) {

                local_matches++;
            }
        }

        #pragma omp critical
        {
            matches += local_matches;
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
    printf(" Experiment 5 - Part C\n");
    printf(" Reduction: The Third Fix\n");
    printf("============================================\n\n");

    omp_set_num_threads(PART_C_THREADS);

    // --------------------------------------------------------
    // Load dictionary
    // --------------------------------------------------------

    std::vector<std::string> dictionary;

    printf(
        "Loading dictionary (%ld candidates)...\n",
        DICTIONARY_SIZE
    );

    load_dictionary(
        dictionary,
        DICTIONARY_SIZE,
        DICTIONARY_SEED
    );

    // --------------------------------------------------------
    // Target password
    // --------------------------------------------------------

    const std::string target_password =
        "target_password";

    unsigned long target_hash =
        toy_hash(target_password);

    printf(
        "Target password    : %s\n",
        target_password.c_str()
    );

    printf(
        "Target hash        : %lu\n",
        target_hash
    );

    printf(
        "Threads            : %d\n\n",
        PART_C_THREADS
    );

    // --------------------------------------------------------
    // Trusted serial result
    // --------------------------------------------------------

    long reference =
        serial_password_audit(
            dictionary,
            target_hash
        );

    printf(
        "Trusted serial matches = %ld\n\n",
        reference
    );

    // ========================================================
    // C1 - Reproduce race
    // ========================================================

    printf("============================================\n");
    printf(" C1 - Unprotected matches++\n");
    printf("============================================\n");

    printf(
        "Running 5 times without recompiling...\n\n"
    );

    for (int run = 1; run <= 5; run++) {

        long result =
            audit_unprotected(
                dictionary,
                target_hash
            );

        printf(
            "Run %d: matches = %ld",
            run,
            result
        );

        if (result == reference) {
            printf("  <-- correct by chance");
        } else {
            printf("  <-- WRONG");
        }

        printf("\n");
    }

    // ========================================================
    // C2 - Reduction correctness
    // ========================================================

    printf("\n============================================\n");
    printf(" C2 - Reduction\n");
    printf("============================================\n");

    bool all_correct = true;

    for (int run = 1; run <= 5; run++) {

        long result =
            audit_reduction(
                dictionary,
                target_hash
            );

        printf(
            "Run %d: matches = %ld",
            run,
            result
        );

        if (result == reference) {

            printf("  PASS");

        } else {

            printf("  FAIL");

            all_correct = false;
        }

        printf("\n");
    }

    if (all_correct) {
        printf(
            "\nReduction verification: PASS\n"
        );
    } else {
        printf(
            "\nReduction verification: FAIL\n"
        );
    }

    // ========================================================
    // C3 - Timing comparison
    // ========================================================

    printf("\n============================================\n");
    printf(" C3 - Timing\n");
    printf("============================================\n");

    // Warm-up
    audit_reduction(
        dictionary,
        target_hash
    );

    audit_critical_smart(
        dictionary,
        target_hash
    );

    // --------------------------------------------------------
    // Reduction timing
    // --------------------------------------------------------

    double start_reduction =
        omp_get_wtime();

    long reduction_result =
        audit_reduction(
            dictionary,
            target_hash
        );

    double end_reduction =
        omp_get_wtime();

    double reduction_time =
        end_reduction - start_reduction;

    // --------------------------------------------------------
    // Critical-smart timing
    // --------------------------------------------------------

    double start_critical =
        omp_get_wtime();

    long critical_result =
        audit_critical_smart(
            dictionary,
            target_hash
        );

    double end_critical =
        omp_get_wtime();

    double critical_time =
        end_critical - start_critical;

    // --------------------------------------------------------
    // Results
    // --------------------------------------------------------

    printf(
        "Reduction result       : %ld\n",
        reduction_result
    );

    printf(
        "Critical-smart result  : %ld\n",
        critical_result
    );

    printf(
        "Reduction time         : %.9f s\n",
        reduction_time
    );

    printf(
        "Critical-smart time    : %.9f s\n",
        critical_time
    );

    if (reduction_time < critical_time) {

        printf(
            "Faster implementation  : reduction\n"
        );

    } else {

        printf(
            "Faster implementation  : critical-smart\n"
        );
    }

    printf("\nPart C complete.\n");

    return 0;
}