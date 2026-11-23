#include <parlay/parallel.h>

template <typename Body>
void pfor(int start, int end, Body&& body) {
    int size = end - start;
    if (size <= MIN_SEGMENT_SIZE) {
        for (int i = start; i < end; ++i) {
            body(i);
        }
        return;
    }

    int mid = start + size / 2;
    parlay::par_do(
        [&] { pfor(start, mid, body); },
        [&] { pfor(mid, end, body); }
    );
}

template <typename T>
void up_sweep(std::vector<T>& a, int s, int t) {
    if (t - s <= MIN_SEGMENT_SIZE) {
        for (int i = s + 1; i < t; ++i) {
            a[i] = a[i - 1] + a[i];
        }
        return;
    }

    int m = s + (t - s) / 2;
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

    if (t - s <= MIN_SEGMENT_SIZE) {
        for (int i = t - 1; i > s; --i) {
            a[i] = p + a[i - 1];
        }
        a[s] = p;
        return;
    }

    int m = s + (t - s) / 2;
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
