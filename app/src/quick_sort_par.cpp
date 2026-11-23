#include "quick_sort_par.h"
#include "quick_sort_seq.h"
#include "primitives.h"

#include <parlay/parallel.h>

BlockInfo count_block_info(std::vector<int>&a, int l, int r, int pivot) {
    BlockInfo info;
    for (int i = l; i < r; ++i) {
        info.less += a[i] < pivot;
        info.equal += a[i] == pivot;
    }
    return info;
}

void write_block(
    std::vector<int>&a, 
    std::vector<int>&buff, 
    int l, 
    int r, 
    int pivot, 
    BlockInfo&info, 
    BlockInfo& total, 
    int block_id
) {
    int s = l + block_id * MIN_SEGMENT_SIZE;
    int t = std::min(s + MIN_SEGMENT_SIZE, r);
    
    int less = l + info.less;
    int equals = l + total.less + info.equal;
    int greater = l + total.less + total.equal + (block_id * MIN_SEGMENT_SIZE - info.less - info.equal);
    for (int i = s; i < t; i++) {
        if (a[i] == pivot) {
            buff[equals++] = a[i];
        } else if (a[i] < pivot) {
            buff[less++] = a[i];
        } else {
            buff[greater++] = a[i];
        }
    }
}

BlockInfo partition_blocks(
    std::vector<int>& a, 
    std::vector<BlockInfo>& tmp, 
    std::vector<int>& buff, 
    int l, 
    int r, 
    int pivot
) {
    int blocks = (r - l) / MIN_SEGMENT_SIZE;
    int tmp_begin = l / MIN_SEGMENT_SIZE;

    pfor(0, blocks, [&](int i) {
        tmp[tmp_begin + i] = count_block_info(a, l + i * MIN_SEGMENT_SIZE, l + (i+1) * MIN_SEGMENT_SIZE, pivot);
    });

    BlockInfo blocks_res = scan(tmp, tmp_begin, tmp_begin + blocks);
    BlockInfo total = blocks_res + count_block_info(a, l + blocks * MIN_SEGMENT_SIZE, r, pivot);

    pfor(0, blocks, [&](int i) {
        write_block(a, buff, l, r, pivot, tmp[tmp_begin + i], total, i);
    });

    write_block(a, buff, l, r, pivot, blocks_res, total, blocks);

    pfor(l, r, [&](int i) { 
        a[i] = buff[i]; 
    });
    return total;
}

void quick_sort_par(std::vector<int>& a, std::vector<BlockInfo>& tmp, std::vector<int>& buff, int l, int r) {
    if (r - l <= 100'000) {
        quick_sort_seq(a, l, r);
        return;
    }

    int pivot = a[(r + l) / 2];
    BlockInfo totals = partition_blocks(a, tmp, buff, l, r, pivot);

    parlay::par_do(
        [&] { quick_sort_par(a, tmp, buff, l, l + totals.less); },
        [&] { quick_sort_par(a, tmp, buff, l + totals.less + totals.equal, r); }
    );
}

void quick_sort_par(std::vector<int>&a) {
    int size = static_cast<int>(a.size());
    std::vector<BlockInfo> tmp(a.size() / MIN_SEGMENT_SIZE);
    std::vector<int> buff(a.size());
    quick_sort_par(a, tmp, buff, 0, a.size());
}
