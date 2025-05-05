#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

int quickSelect(vector<int>& nums, int k) {
    if (nums.size() == 1) return nums[0];

    int pivot = nums[rand() % nums.size()];
    vector<int> lows, highs, pivots;

    for (int num : nums) {
        if (num < pivot) lows.push_back(num);
        else if (num > pivot) highs.push_back(num);
        else pivots.push_back(num);
    }

    if (k < lows.size()) return quickSelect(lows, k);
    else if (k < lows.size() + pivots.size()) return pivot;
    else return quickSelect(highs, k - lows.size() - pivots.size());
}

void testQuickSelect(int n) {
    vector<int> arr(n);
    generate(arr.begin(), arr.end(), [](){ return rand(); });

    double total_qs = 0;
    for (int i = 0; i < 1000; ++i) {
        int k = rand() % n;
        vector<int> temp = arr;
        auto start = high_resolution_clock::now();
        quickSelect(temp, k);
        auto end = high_resolution_clock::now();
        total_qs += duration<double>(end - start).count();
    }

    auto start = high_resolution_clock::now();
    vector<int> temp = arr;
    sort(temp.begin(), temp.end());
    auto end = high_resolution_clock::now();
    double sort_time = duration<double>(end - start).count();

    cout << "n = " << n
         << " | QuickSelect avg time: " << total_qs / 1000.0
         << " s | QuickSort time: " << sort_time << " s" << endl;
}

int main() {
    srand(time(0));
    cout << " QuickSelect vs QuickSort\n";
    for (int n : {10'000'000, 20'000'000, 40'000'000}) {
        testQuickSelect(n);
    }
    return 0;
}
