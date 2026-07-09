#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>

#include "sorting.hpp"

using namespace std;
using namespace chrono;


double measureTime(void (*sortFunction)(vector<int>&), vector<int> arr) {

    auto start = high_resolution_clock::now();

    sortFunction(arr);

    auto end = high_resolution_clock::now();

    return duration<double, milli>(end - start).count();
}


void mergeWrapper(vector<int>& arr) {
    mergeSort(arr, 0, arr.size() - 1);
}


int main() {

    vector<int> sizes = {
        100,
        500,
        1000,
        10000,
        
    };


    for (int size : sizes) {

        vector<int> data(size);

        for (int i = 0; i < size; i++) {
            data[i] = rand() % 100000;
        }


        double bubbleTime = measureTime(bubbleSort, data);
        double mergeTime = measureTime(mergeWrapper, data);
        double countTime = measureTime(countingSort, data);


        cout << "\nInput Size: " << size << endl;

        cout << "Bubble Sort   : "
             << bubbleTime << " ms" << endl;

        cout << "Merge Sort    : "
             << mergeTime << " ms" << endl;

        cout << "Counting Sort : "
             << countTime << " ms" << endl;


        double fastest = min({
            bubbleTime,
            mergeTime,
            countTime
        });


        cout << "Fastest: ";

        if (fastest == bubbleTime)
            cout << "Bubble Sort";

        else if (fastest == mergeTime)
            cout << "Merge Sort";

        else
            cout << "Counting Sort";


        cout << endl;
    }


    return 0;
}