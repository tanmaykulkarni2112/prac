#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// ---------------- NORMAL BUBBLE SORT ----------------
void bubbleSortNormal(vector<int> res) {
    for (int i = 0; i < res.size() - 1; i++) {
        for (int j = 0; j < res.size() - 1 - i; j++) {
            if (res[j] > res[j + 1]) {
                swap(res[j], res[j + 1]);
            }
        }
    }
}

// ---------------- OPENMP BUBBLE SORT ----------------
void bubbleSortOMP(vector<int> &res) {
    for (int i = 0; i < res.size() - 1; i++) {
        bool swapped = false;

        #pragma omp parallel for
        for (int j = 0; j < res.size() - 1 - i; j++) {
            if (res[j] > res[j + 1]) {
                swap(res[j], res[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) return;
    }
}

// ---------------- MAIN ----------------
int main() {

    vector<int> arr = {123,54,6523,5135,1231,45,1,234,5};

    // -------- NORMAL SORT TIMING --------
    vector<int> arr1 = arr;

    auto start1 = high_resolution_clock::now();
    bubbleSortNormal(arr1);
    auto end1 = high_resolution_clock::now();

    auto time1 = duration_cast<microseconds>(end1 - start1);

    // -------- OMP SORT TIMING --------
    vector<int> arr2 = arr;

    auto start2 = high_resolution_clock::now();
    bubbleSortOMP(arr2);
    auto end2 = high_resolution_clock::now();

    auto time2 = duration_cast<microseconds>(end2 - start2);

    // -------- OUTPUT --------
    cout << "Normal Bubble Sort Time: " << time1.count() << " microseconds" << endl;
    cout << "OpenMP Bubble Sort Time: " << time2.count() << " microseconds" << endl;

    return 0;
}
