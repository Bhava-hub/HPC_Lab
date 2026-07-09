#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <algorithm>

#include "sorting.hpp"

using namespace std;
using namespace chrono;

int main() {

    int n;

    cout << "Enter number of inputs: ";
    cin >> n;

    vector<int> original(n);

    // Generate random input values
    for (int i = 0; i < n; i++) {
        original[i] = rand() % 10000;
    }

    vector<int> bubbleArr = original;
    vector<int> mergeArr = original;
    vector<int> countArr = original;


    // Bubble Sort timing
    auto start = high_resolution_clock::now();

    bubbleSort(bubbleArr);

    auto end = high_resolution_clock::now();

    double bubbleTime =
        duration<double, milli>(end - start).count();



    // Merge Sort timing
    start = high_resolution_clock::now();

    mergeSort(mergeArr, 0, mergeArr.size() - 1);

    end = high_resolution_clock::now();

    double mergeTime =
        duration<double, milli>(end - start).count();



    // Counting Sort timing
    start = high_resolution_clock::now();

    countingSort(countArr);

    end = high_resolution_clock::now();

    double countTime =
        duration<double, milli>(end - start).count();



    // Display results
    cout << "\nExecution Time:\n";

    cout << "Bubble Sort   : "
         << bubbleTime << " ms\n";

    cout << "Merge Sort    : "
         << mergeTime << " ms\n";

    cout << "Counting Sort : "
         << countTime << " ms\n";


    // Find fastest algorithm
    double fastest = min({bubbleTime, mergeTime, countTime});

    cout << "\nFastest Algorithm: ";

    if (fastest == bubbleTime)
        cout << "Bubble Sort";

    else if (fastest == mergeTime)
        cout << "Merge Sort";

    else
        cout << "Counting Sort";


    cout << "\nTime: " << fastest << " ms\n";


    return 0;
}