#!/bin/bash

set -e

cmake -S . -B "out" -DCMAKE_BUILD_TYPE=Release
cmake --build "out" --config Release --parallel

bin_dir="out/bin/Release"
results_dir=$(mktemp -d "out/quick-sort-results.XXXXXX")
trap 'rm -rf -- "$results_dir"' EXIT

for i in 0 1 2 3 4; do
    "$bin_dir/quick_sort" seq >> "$results_dir/seq"
    PARLAY_NUM_THREADS=4 "$bin_dir/quick_sort" par >> "$results_dir/blocks"
done

LC_ALL=C awk '
    NF { sum[FILENAME] += $1; count[FILENAME]++ }
    END {
        if (count[ARGV[1]] == 0 || count[ARGV[2]] == 0) exit 1

        seq = sum[ARGV[1]] / count[ARGV[1]]
        blocks = sum[ARGV[2]] / count[ARGV[2]]
        printf "Sequantial — %.2f s\n", seq
        printf "Parallel — %.2f s\n", blocks

        printf "Speedup — %.2fx\n", seq / blocks
        
    }
' "$results_dir/seq" "$results_dir/blocks"
