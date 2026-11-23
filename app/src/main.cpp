#include "quick_sort_par.h"
#include "primitives.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <vector>

int main() {
    std::vector<int> a(100'000'000);
    for (int& value : a) {
        value = std::rand();
    }

    auto start = std::chrono::steady_clock::now();
    int size = static_cast<int>(a.size());
    std::vector<BlockInfo> tmp(size / MIN_SEGMENT_SIZE);
    std::vector<int> buff(a.size());
    quick_sort_par(a, tmp, buff, 0, size);

    std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
    std::printf("%.2f\n", elapsed.count());
    return 0;
}
