#include "quick_sort_seq.h"

#include <limits>

void quick_sort_seq(std::vector<int>& a) {
    quick_sort_seq(a, 0, a.size());
}

static int partition(std::vector<int>& a, int l, int r, int pivot) {
    while (l < r) {
        while (l < r && a[l] < pivot) {
            ++l;
        }
        while (l < r && a[r - 1] >= pivot) {
            --r;
        }
        if (l < r) {
            --r;
            std::swap(a[l], a[r]);
            ++l;
        }
    }
    return l;
}

void quick_sort_seq(std::vector<int>& a, int l, int r) {
    if (r - l <= 1) {
        return;
    }

    int pivot = a[(r + l) / 2];

    int equal_begin = partition(a, l, r, pivot);
    int greater_begin = pivot == std::numeric_limits<int>::max()
        ? r
        : partition(a, equal_begin, r, pivot + 1);
    quick_sort_seq(a, l, equal_begin);
    quick_sort_seq(a, greater_begin, r);
}
