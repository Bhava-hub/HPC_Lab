#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

volatile long long checksum = 0;

int main() {
const size_t MIN_SIZE = 4 * 1024;
const size_t MAX_SIZE = 64 * 1024 * 1024;
const int REPEAT = 100;

    random_device rd;
    mt19937 gen(rd());

    cout << "SizeKB,Sequential(ns),Random(ns)\n";

for(size_t bytes = MIN_SIZE; bytes <= MAX_SIZE; bytes *= 2) {

size_t n = bytes / sizeof(int);

        vector<int> data(n,1);

        // ---------- Sequential ----------
auto start = high_resolution_clock::now();

for(int r=0;r<REPEAT;r++)
for(size_t i=0;i<n;i++)
                checksum += data[i];

auto end = high_resolution_clock::now();

double seq_time =
duration<double,nano>(end-start).count()/(REPEAT*n);

        // ---------- Random ----------
        vector<size_t> index(n);

for(size_t i=0;i<n;i++)
index[i]=i;

shuffle(index.begin(),index.end(),gen);

        start = high_resolution_clock::now();

for(int r=0;r<REPEAT;r++)
for(size_t i=0;i<n;i++)
                checksum += data[index[i]];

        end = high_resolution_clock::now();

double rand_time =
duration<double,nano>(end-start).count()/(REPEAT*n);

        cout << bytes/1024 << ","
<< seq_time << ","
<< rand_time << endl;
    }

    cerr << checksum << endl;
}