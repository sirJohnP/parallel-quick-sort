#include "quick_sort_par.h"
#include "quick_sort_seq.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <string_view>
#include <vector>

int main(int argc, char* argv[]) {
    std::vector<int> a(100'000'000);
    for (int& value : a) {
        value = std::rand();
    }

    auto start = std::chrono::steady_clock::now();
    if (argv[1][0] == 'p') {
        quick_sort_par(a);
    } else {
        quick_sort_seq(a);
    }
    std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;

    std::printf("%.2f\n", elapsed.count());
    return 0;
}
