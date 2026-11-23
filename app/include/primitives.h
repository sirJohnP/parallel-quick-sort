#pragma once

#include <vector>

inline constexpr int MIN_SEGMENT_SIZE = 10'000;

template <typename Body>
void pfor(int start, int end, Body&& body);

template <typename T>
T scan(std::vector<T>& a, int l, int r);

#include "primitives.tpp"
