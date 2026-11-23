#include "quick_sort_seq.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>

int main() {
    std::vector<int> a(100'000'000);
    for (int& value : a) {
        value = std::rand();
    }

    auto start = std::chrono::steady_clock::now();
    quick_sort_seq(a, 0, a.size());
    std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
    std::printf("%.2f\n", elapsed.count());
    return 0;
}
