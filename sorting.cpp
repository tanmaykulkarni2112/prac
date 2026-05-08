#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

void bubbleSort(vector<int> &res) {
    for (int i = 0; i < res.size() - 1; i++) {
        bool swapped = false;

        #pragma omp parallel for
        for (int j = 0; j < res.size() - 1 - i; j++) {
            if (res[j] > res[j + 1]) {
                #pragma omp critical
                {
                    if (res[j] > res[j + 1]) { 
                        swap(res[j], res[j + 1]);
                        swapped = true;
                    }
                }
            }
        }

        if (!swapped) return;
    }
}

void merge(int left, int mid, int right, vector<int> &arr) {
    vector<int> temp;
    int i = left;
    int j = mid + 1;
    while(i <= mid && j <= right) {
        if(arr[i] > arr[j]) {
            temp.push_back(arr[j++]);
        } else {
            temp.push_back(arr[i++]);
        }
    }
    for(int x = i; x <= mid; x++) {
        temp.push_back(arr[x]);
    }
    for(int x = j; x <= right; x++) {
        temp.push_back(arr[x]);
    }

    for(int i = 0; i < temp.size(); i++) {
        arr[left + i] = temp[i];
    }
}

void mergeSort(int left, int right, vector<int> &arr) {
    if (left >= right) {
        return;
    }
    int mid = (left + right) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            mergeSort(left, mid, arr);
        }

        #pragma omp section
        {
            mergeSort(mid + 1, right, arr);
        }

    }
    
    merge(left, mid, right, arr);
}

int main() {

    vector<int> arr = {123,54,6523,5135,1231,45,1,234,5};

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;

    auto start1 = high_resolution_clock::now();
    bubbleSort(arr1);
    auto end1 = high_resolution_clock::now();
    auto time1 = duration_cast<microseconds>(end1 - start1);
    cout << "Bubble Sort Time: " << time1.count() << " microseconds" << endl;

    auto start2 = high_resolution_clock::now();
    mergeSort(0, arr2.size() -1, arr2);
    auto end2 = high_resolution_clock::now();
    auto elapsed = duration_cast<microseconds>(end2 - start2);
    cout << "Mergesort time: " << elapsed.count() << "ms" << endl;

    return 0;
}
