#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int maxNum(vector<int> &arr) {
    int maxN = arr[0];
    #pragma omp parallel for reduction(max:maxN)
    for(int i = 1; i < arr.size(); i++) {
        if (maxN < arr[i]) {
            maxN = arr[i];
        }
    }
    return maxN;
}

int minNum(vector<int> &arr) {
    int minN = arr[0];
    #pragma omp parallel for reduction(min:minN)
    for(int i = 1; i < arr.size(); i++) {
        if (minN > arr[i]) {
            minN = arr[i];
        }
    }
    return minN;
}

long long sumNum(vector<int> &arr) {
    long long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < arr.size(); i++) {
        sum += arr[i];
    }
    return sum;
}

double avgNum(vector<int> &arr) {
    long long sum = sumNum(arr);
    return (double)sum / arr.size();
}

int main() {
    vector<int> arr{241, 62351, 12354767, 232, 1};
    
    cout << "Max: " << maxNum(arr) << endl;
    cout << "Min: " << minNum(arr) << endl;
    cout << "Sum: " << sumNum(arr) << endl;
    cout << "Average: " << avgNum(arr) << endl;

    return 0;
}