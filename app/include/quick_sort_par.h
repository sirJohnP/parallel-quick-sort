#pragma once

#include <vector>

struct BlockInfo {
    int less = 0;
    int equal = 0;

    BlockInfo operator+(BlockInfo other) {
        return {less + other.less, equal + other.equal};
    }
};

void quick_sort_par(std::vector<int>& a);
