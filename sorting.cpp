#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

/* ================= MERGE SORT ================= */

void merge(int left, int mid, int right, vector<int> &arr) {

    vector<int> res;

    int i = left;
    int j = mid + 1;

    while(i <= mid && j <= right) {

        if(arr[i] < arr[j]) {
            res.push_back(arr[i++]);
        }
        else {
            res.push_back(arr[j++]);
        }
    }

    while(i <= mid) {
        res.push_back(arr[i++]);
    }

    while(j <= right) {
        res.push_back(arr[j++]);
    }

    for(int k = 0; k < res.size(); k++) {
        arr[left + k] = res[k];
    }
}

void mergesort(int left, int right, vector<int> &arr) {

    if(left >= right) {
        return;
    }

    int mid = (left + right) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            mergesort(left, mid, arr);
        }

        #pragma omp section
        {
            mergesort(mid + 1, right, arr);
        }
    }

    merge(left, mid, right, arr);
}

/* ================= BUBBLE SORT ================= */

void bubblesort(vector<int> &arr) {

    bool swapped;

    for(int i = 0; i < arr.size() - 1; i++) {

        swapped = false;

        // Parallel comparison only
        #pragma omp parallel for
        for(int j = 0; j < arr.size() - i - 1; j++) {

            // lock only swap section
            if(arr[j] > arr[j + 1]) {

                #pragma omp critical
                {
                    if(arr[j] > arr[j + 1]) {
                        swap(arr[j], arr[j + 1]);
                        swapped = true;
                    }
                }
            }
        }

        if(!swapped) {
            break;
        }
    }
}

/* ================= MAIN ================= */

int main() {

    vector<int> arr1 = {5, 2, 9, 1, 3, 7};
    vector<int> arr2 = {5, 3, 8, 4, 2};

    // Merge Sort
    mergesort(0, arr1.size() - 1, arr1);

    cout << "Merge Sort Result: ";
    for(int x : arr1) {
        cout << x << " ";
    }
    cout << endl;

    // Bubble Sort
    bubblesort(arr2);

    cout << "Bubble Sort Result: ";
    for(int x : arr2) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}