#!/bin/bash

set -euo pipefail

project_dir="$(dirname "${BASH_SOURCE[0]}")"
build_dir="$project_dir/out"

cmake -S "$project_dir" -B "$build_dir" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build "$build_dir" --config Release --target sort_tests --parallel
ctest --test-dir "$build_dir" --build-config Release --output-on-failure
