#pragma once

#include <parlay/parallel.h>

#include <vector>

inline constexpr int MIN_SEGMENT_SIZE = 10'000;

template <typename Body>
void pfor(int l, int r, const Body& body) {
    if (r - l <= MIN_SEGMENT_SIZE) {
        for (int i = l; i < r; ++i) {
            body(i);
        }
        return;
    }

    int m = (r + l) / 2;
    parlay::par_do(
        [&] { pfor(l, m, body); },
        [&] { pfor(m, r, body); }
    );
}

template <typename T>
void up_sweep(std::vector<T>& a, int s, int t) {
    if (t - s <= 1) {
        return;
    }

    int m = (t + s) / 2;
    parlay::par_do(
        [&] { up_sweep(a, s, m); },
        [&] { up_sweep(a, m, t); }
    );

    a[t - 1] = a[m - 1] + a[t - 1];
}

template <typename T>
void down_sweep(std::vector<T>& a, int s, int t, T& p) {
    if (s >= t) {
        return;
    }

    if (t - s == 1) {
        a[s] = p;
        return;
    }

    int m = (t + s) / 2;
    T right_prefix = p + a[m - 1];
    parlay::par_do(
        [&] { down_sweep(a, s, m, p); },
        [&] { down_sweep(a, m, t, right_prefix); }
    );
}

template <typename T>
T scan(std::vector<T>& a, int l, int r) {
    if (r <= l) {
        return T{};
    }
    up_sweep(a, l, r);
    T total = a[r - 1];
    T zero{};
    down_sweep(a, l, r, zero);
    return total;
}
