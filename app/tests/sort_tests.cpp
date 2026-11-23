#include "quick_sort_par.h"
#include "quick_sort_seq.h"

#include <algorithm>
#include <cstdio>
#include <limits>
#include <random>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using Sort = void (*)(std::vector<int>&);

bool check(Sort sort, const std::string& name, std::vector<int> values) {
    auto expected = values;
    std::sort(expected.begin(), expected.end());
    sort(values);
    if (values != expected) {
        std::fprintf(stderr, "FAIL: %s (size=%zu)\n", name.c_str(), values.size());
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2 || (std::string_view(argv[1]) != "seq" &&
                      std::string_view(argv[1]) != "par")) {
        std::fprintf(stderr, "Usage: %s <seq|par>\n", argv[0]);
        return 1;
    }
    Sort sort = quick_sort_seq;
    if (std::string_view(argv[1]) == "par") {
        sort = quick_sort_par;
    }
    int passed = 0;
    auto test = [&](const std::string& name, std::vector<int> values) {
        bool ok = check(sort, name, std::move(values));
        passed += ok;
        return ok;
    };

    if (!test("empty", {}) || !test("single", {42}) ||
        !test("pair", {2, 1}) || !test("equal pair", {7, 7}) ||
        !test("mixed signs", {0, -1, 5, -10, 5, 0, 3})) {
        return 1;
    }

    std::mt19937 random(42);
    std::uniform_int_distribution<int> values(-1'000'000, 1'000'000);
    for (int size : {9'999, 10'000, 10'001, 99'999, 100'000, 100'001, 400'003}) {
        std::vector<int> a(size);
        for (int& value : a) {
            value = values(random);
        }
        if (!test("random " + std::to_string(size), a)) {
            return 1;
        }
        std::sort(a.begin(), a.end());
        if (!test("sorted " + std::to_string(size), a)) {
            return 1;
        }
        std::reverse(a.begin(), a.end());
        if (!test("reverse " + std::to_string(size), a)) {
            return 1;
        }
    }

    if (!test("all equal, full blocks", std::vector<int>(120'000, 7)) ||
        !test("all equal, with tail", std::vector<int>(120'003, 7))) {
        return 1;
    }
    std::vector<int> duplicates(400'003);
    for (int& value : duplicates) {
        value = static_cast<int>(random() % 5) - 2;
    }
    if (!test("many duplicates", duplicates)) {
        return 1;
    }
    for (int i = 0; i < 100; ++i) {
        std::vector<int> a(random() % 1000);
        for (int& value : a) {
            value = values(random);
        }
        if (!test("small random " + std::to_string(i), a)) {
            return 1;
        }
    }

    const int min = std::numeric_limits<int>::min();
    const int max = std::numeric_limits<int>::max();
    if (!test("INT_MIN and INT_MAX", {min, max, 0, max, min, -1, 1}) ||
        !test("INT_MAX pair", {max, max}) ||
        !test("INT_MIN pair", {min, min}) ||
        !test("all INT_MAX, parallel range", std::vector<int>(100'001, max))) {
        return 1;
    }

    std::printf("%s: %d cases passed\n", argv[1], passed);
    return 0;
}
