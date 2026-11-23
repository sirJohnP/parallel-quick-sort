#pragma once

#include <vector>

struct BlockInfo {
    int less = 0;
    int equal = 0;

    BlockInfo operator+(BlockInfo other) {
        return {less + other.less, equal + other.equal};
    }
};

BlockInfo partition_blocks(std::vector<int>& a, std::vector<BlockInfo>& tmp,
                           std::vector<int>& buff, int l, int r, int pivot);

void quick_sort_par(std::vector<int>& a, std::vector<BlockInfo>& tmp, std::vector<int>& buff, int l, int r);
