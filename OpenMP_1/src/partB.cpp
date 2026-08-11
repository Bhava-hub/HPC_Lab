#include "experiment5.hpp"

// ============================================================
// Part B configuration
// ============================================================

constexpr int NUM_LINES = 200000;

constexpr unsigned int LINE_SEED = 12345;

constexpr int CHEAP_COST = 20;

constexpr int EXPENSIVE_COST = 20000;

constexpr int PART_B_THREADS = 8;

// ============================================================
// Generate line costs ONCE
// ============================================================

void generate_line_costs(
    std::vector<int>& line_cost
) {
    std::mt19937 rng(LINE_SEED);

    std::uniform_int_distribution<int> rare(
        0,
        999
    );

    for (int i = 0; i < NUM_LINES; i++) {

        line_cost[i] =
            (rare(rng) == 0)
            ? EXPENSIVE_COST
            : CHEAP_COST;
    }
}

// ============================================================
// Dummy workload
// ============================================================

inline void scan_line(
    int cost
) {
    volatile long acc = 0;

    for (int k = 0; k < cost; k++) {
        acc += k;
    }

    (void)acc;
}

// ============================================================
// Static
// ============================================================

double run_static(
    const std::vector<int>& line_cost
) {
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < NUM_LINES; i++) {

        scan_line(line_cost[i]);
    }

    double end = omp_get_wtime();

    return end - start;
}

// ============================================================
// Dynamic
// ============================================================

double run_dynamic(
    const std::vector<int>& line_cost,
    int chunk
) {
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < NUM_LINES; i++) {

        scan_line(line_cost[i]);
    }

    double end = omp_get_wtime();

    return end - start;
}

// ============================================================
// Dynamic with runtime chunk
//
// OpenMP schedule clause needs the chunk to be known at
// compile time in the pragma, so use separate functions.
// ============================================================

double run_dynamic_1(
    const std::vector<int>& line_cost
) {
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < NUM_LINES; i++) {

        scan_line(line_cost[i]);
    }

    double end = omp_get_wtime();

    return end - start;
}

double run_dynamic_100(
    const std::vector<int>& line_cost
) {
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, 100)
    for (int i = 0; i < NUM_LINES; i++) {

        scan_line(line_cost[i]);
    }

    double end = omp_get_wtime();

    return end - start;
}

// ============================================================
// Guided
// ============================================================

double run_guided(
    const std::vector<int>& line_cost
) {
    double start = omp_get_wtime();

    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < NUM_LINES; i++) {

        scan_line(line_cost[i]);
    }

    double end = omp_get_wtime();

    return end - start;
}

// ============================================================
// Three-run helper
// ============================================================

template <typename Function>
void measure_three_runs(
    const char* name,
    Function function
) {
    double times[3];

    for (int i = 0; i < 3; i++) {

        times[i] = function();

        printf(
            "%s Run %d: %.9f s\n",
            name,
            i + 1,
            times[i]
        );
    }

    double sorted[3] = {
        times[0],
        times[1],
        times[2]
    };

    // Simple sorting for median
    for (int i = 0; i < 2; i++) {
        for (int j = i + 1; j < 3; j++) {

            if (sorted[j] < sorted[i]) {

                double temp = sorted[i];

                sorted[i] = sorted[j];

                sorted[j] = temp;
            }
        }
    }

    printf(
        "%s Median: %.9f s\n\n",
        name,
        sorted[1]
    );
}

// ============================================================
// Main
// ============================================================

int main()
{
    printf("============================================\n");
    printf(" Experiment 5 - Part B\n");
    printf(" Scheduling Policies\n");
    printf("============================================\n\n");

    omp_set_num_threads(PART_B_THREADS);

    // --------------------------------------------------------
    // Generate workload once
    // --------------------------------------------------------

    std::vector<int> line_cost(NUM_LINES);

    generate_line_costs(line_cost);

    // Count expensive lines
    int expensive_count = 0;

    for (int i = 0; i < NUM_LINES; i++) {

        if (line_cost[i] == EXPENSIVE_COST) {
            expensive_count++;
        }
    }

    printf("Number of lines     : %d\n", NUM_LINES);
    printf("Thread count        : %d\n", PART_B_THREADS);
    printf("Seed                : %u\n", LINE_SEED);
    printf("Cheap cost          : %d\n", CHEAP_COST);
    printf("Expensive cost      : %d\n", EXPENSIVE_COST);
    printf(
        "Expensive lines     : %d\n\n",
        expensive_count
    );

    printf("IMPORTANT: Same line-cost array is reused.\n\n");

    // --------------------------------------------------------
    // Static
    // --------------------------------------------------------

    measure_three_runs(
        "static",
        [&]() {
            return run_static(line_cost);
        }
    );

    // --------------------------------------------------------
    // Dynamic, chunk 1
    // --------------------------------------------------------

    measure_three_runs(
        "dynamic,1",
        [&]() {
            return run_dynamic_1(line_cost);
        }
    );

    // --------------------------------------------------------
    // Dynamic, chunk 100
    // --------------------------------------------------------

    measure_three_runs(
        "dynamic,100",
        [&]() {
            return run_dynamic_100(line_cost);
        }
    );

    // --------------------------------------------------------
    // Guided
    // --------------------------------------------------------

    measure_three_runs(
        "guided",
        [&]() {
            return run_guided(line_cost);
        }
    );

    printf("Part B complete.\n");

    return 0;
}