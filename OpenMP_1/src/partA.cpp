#include "experiment5.hpp"

// ============================================================
// Part A
//
// 1. parallel for + reduction
// 2. single banner
// 3. dictionary loading before/inside parallel region
// ============================================================

void run_parallel_reduction(
    const std::vector<double>& a,
    double& total
) {
    total = 0.0;

    #pragma omp parallel for reduction(+:total)
    for (long i = 0;
         i < static_cast<long>(a.size());
         i++) {

        total += a[i];
    }
}

// ============================================================
// Single banner
// ============================================================

void run_single_banner()
{
    #pragma omp parallel
    {
        #pragma omp single
        {
            printf(
                "Banner: OpenMP team size = %d "
                "(printed by thread %d)\n",
                omp_get_num_threads(),
                omp_get_thread_num()
            );
        }
    }
}

// ============================================================
// Dictionary loading before parallel region
// ============================================================

double time_dictionary_before_parallel()
{
    std::vector<std::string> dictionary;

    double start = omp_get_wtime();

    load_dictionary(
        dictionary,
        DICTIONARY_SIZE,
        DICTIONARY_SEED
    );

    double end = omp_get_wtime();

    return end - start;
}

// ============================================================
// Dictionary loading inside parallel region
// ============================================================

double time_dictionary_inside_parallel()
{
    std::vector<std::string> dictionary;

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            load_dictionary(
                dictionary,
                DICTIONARY_SIZE,
                DICTIONARY_SEED
            );
        }
    }

    double end = omp_get_wtime();

    return end - start;
}

// ============================================================
// Main
// ============================================================

int main()
{
    printf("============================================\n");
    printf(" Experiment 5 - Part A\n");
    printf("============================================\n\n");

    // --------------------------------------------------------
    // Generate array
    // --------------------------------------------------------

    std::vector<double> a(ARRAY_N);

    generate_array(a, ARRAY_SEED);

    // --------------------------------------------------------
    // Serial trusted reference
    // --------------------------------------------------------

    double reference_start = omp_get_wtime();

    double reference = serial_array_sum(a);

    double reference_end = omp_get_wtime();

    double reference_time =
        reference_end - reference_start;

    // --------------------------------------------------------
    // Parallel for reduction
    // --------------------------------------------------------

    double parallel_total = 0.0;

    double parallel_start = omp_get_wtime();

    run_parallel_reduction(
        a,
        parallel_total
    );

    double parallel_end = omp_get_wtime();

    double parallel_time =
        parallel_end - parallel_start;

    // --------------------------------------------------------
    // Results
    // --------------------------------------------------------

    printf("Array size        : %ld\n", ARRAY_N);
    printf("Array seed        : %u\n\n", ARRAY_SEED);

    printf(
        "Serial reference  : %.15f\n",
        reference
    );

    printf(
        "Parallel reduction: %.15f\n",
        parallel_total
    );

    printf(
        "Difference         : %.15e\n",
        parallel_total - reference
    );

    printf(
        "Serial time        : %.9f s\n",
        reference_time
    );

    printf(
        "Parallel time      : %.9f s\n",
        parallel_time
    );

    // Floating point sums can differ very slightly because
    // parallel reduction may change the addition order.

    double tolerance = 1e-9;

    if (std::fabs(parallel_total - reference)
        <= tolerance) {

        printf("Verification       : PASS\n");

    } else {

        printf("Verification       : FAIL\n");
    }

    // --------------------------------------------------------
    // Single banner tests
    // --------------------------------------------------------

    printf("\n============================================\n");
    printf(" Single Banner Test\n");
    printf("============================================\n");

    int thread_counts[] = {2, 4, 8};

    for (int i = 0; i < 3; i++) {

        int threads = thread_counts[i];

        omp_set_num_threads(threads);

        printf(
            "\nRequested threads = %d\n",
            threads
        );

        run_single_banner();
    }

    // --------------------------------------------------------
    // Dictionary loading comparison
    // --------------------------------------------------------

    printf("\n============================================\n");
    printf(" Dictionary Loading Comparison\n");
    printf("============================================\n");

    omp_set_num_threads(4);

    double before_time =
        time_dictionary_before_parallel();

    double inside_time =
        time_dictionary_inside_parallel();

    printf(
        "Dictionary size                 : %ld\n",
        DICTIONARY_SIZE
    );

    printf(
        "Load before parallel region    : %.9f s\n",
        before_time
    );

    printf(
        "Load inside single/parallel    : %.9f s\n",
        inside_time
    );

    printf(
        "Difference                     : %.9f s\n",
        inside_time - before_time
    );

    printf("\nPart A complete.\n");

    return 0;
}