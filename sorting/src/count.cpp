#include "sorting.hpp"
#include <algorithm>

using namespace std;

void countingSort(vector<int>& arr) {
    if (arr.empty())
        return;

    int maxVal = *max_element(arr.begin(), arr.end());

    vector<int> count(maxVal + 1, 0);

    for (int num : arr)
        count[num]++;

    int index = 0;

    for (int i = 0; i <= maxVal; i++) {
        while (count[i] > 0) {
            arr[index++] = i;
            count[i]--;
        }
    }
}