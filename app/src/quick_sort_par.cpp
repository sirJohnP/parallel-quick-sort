#include "quick_sort_par.h"

#include <parlay/parallel.h>

#include "quick_sort_seq.h"
#include "primitives.h"

BlockInfo count_block_info(std::vector<int>&a, int l, int r, int pivot) {
    BlockInfo info;
    for (int i = l; i < r; ++i) {
        info.less += a[i] < pivot;
        info.equal += a[i] == pivot;
    }
    return info;
}

BlockInfo partition_blocks(std::vector<int>& a, std::vector<BlockInfo>& tmp, std::vector<int>& buff, int l, int r, int pivot) {
    int blocks = (r - l) / MIN_SEGMENT_SIZE;
    int tail_begin = l + blocks * MIN_SEGMENT_SIZE;
    int tmp_begin = l / MIN_SEGMENT_SIZE;

    pfor(0, blocks, [&](int i) {
        int begin = l + i * MIN_SEGMENT_SIZE;
        int end = begin + MIN_SEGMENT_SIZE;
        tmp[tmp_begin + i] = count_block_info(a, begin, end, pivot);
    });

    BlockInfo full_total = scan(tmp, tmp_begin, tmp_begin + blocks);
    BlockInfo total = full_total + count_block_info(a, tail_begin, r, pivot);
    int equal_begin = l + total.less;
    int greater_begin = equal_begin + total.equal;

    pfor(0, blocks, [&](int i) {
        int begin = l + i * MIN_SEGMENT_SIZE;
        int end = begin + MIN_SEGMENT_SIZE;
        BlockInfo prefix = tmp[tmp_begin + i];
        int less = l + prefix.less;
        int equals = equal_begin + prefix.equal;
        int greater = greater_begin + ((begin - l) - prefix.less - prefix.equal);

        for (int j = begin; j < end; ++j) {
            int value = a[j];
            if (value == pivot) {
                buff[equals++] = value;
            } else {
                int is_less = value < pivot;
                int index = is_less ? less : greater;
                buff[index] = value;
                less += is_less;
                greater += 1 - is_less;
            }
        }
    });

    int less = l + full_total.less;
    int equals = equal_begin + full_total.equal;
    int greater = greater_begin + ((tail_begin - l) - full_total.less - full_total.equal);
    for (int i = tail_begin; i < r; ++i) {
        int value = a[i];
        if (value == pivot) {
            buff[equals++] = value;
        } else {
            int is_less = value < pivot;
            int index = is_less ? less : greater;
            buff[index] = value;
            less += is_less;
            greater += 1 - is_less;
        }
    }

    pfor(l, tail_begin, [&](int i) { a[i] = buff[i]; });
    for (int i = tail_begin; i < r; ++i) {
        a[i] = buff[i];
    }
    return total;
}

void quick_sort_par(std::vector<int>& a, std::vector<BlockInfo>& tmp, std::vector<int>& buff, int l, int r) {
    if (r - l <= 100'000) {
        quick_sort_seq(a, l, r);
        return;
    }

    int pivot = a[l + (r - l) / 2];
    BlockInfo totals = partition_blocks(a, tmp, buff, l, r, pivot);

    parlay::par_do(
        [&] { quick_sort_par(a, tmp, buff, l, l + totals.less); },
        [&] { quick_sort_par(a, tmp, buff, l + totals.less + totals.equal, r); }
    );
}
