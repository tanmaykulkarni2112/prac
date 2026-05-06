#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void bubblesort(vector<int> &arr) {
    bool swapped;
    
    for(int i = 0; i < arr.size() - 1; i++) {
        swapped = false;

        #pragma omp parallel for shared(arr, swapped)
        for(int j = 0; j < arr.size() - i - 1; j++) {
            if(arr[j] > arr[j+1]) {
                #pragma omp critical
                {
                    if(arr[j] > arr[j+1]) { // recheck inside critical
                        swap(arr[j], arr[j+1]);
                        swapped = true;
                    }
                }
            }
        }

        if(!swapped) break;
    }
}

int main() {
    vector<int> arr = {5, 3, 8, 4, 2};

    bubblesort(arr);

    for(int x : arr) {
        cout << x << " ";
    }

    return 0;
}