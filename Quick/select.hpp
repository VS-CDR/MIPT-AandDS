#pragma once

#include <array>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

namespace deterministic {

constexpr ssize_t kBlockSz = 5;

template<std::random_access_iterator It, std::integral T=It::value_type>
void Merge(It left, It mid, It right) {
  static std::array<T, kBlockSz> merged;
  std::random_access_iterator auto left_ptr = left;
  std::random_access_iterator auto right_ptr = mid + 1;

  auto* merged_ptr = merged.begin();
  while (left_ptr <= mid && right_ptr <= right) {
    *merged_ptr++ = *left_ptr < *right_ptr ? *left_ptr++ : *right_ptr++;
  }
  merged_ptr = std::copy(left_ptr, mid + 1, merged_ptr);
  merged_ptr = std::copy(right_ptr, right + 1, merged_ptr);

  std::copy(merged.begin(), merged_ptr, left);
}

template<std::random_access_iterator It>
void MergeSort(It left, It right) {
  if (left < right) {
    const auto kMid = left + (right - left) / 2;
    MergeSort(left, kMid);
    MergeSort(kMid + 1, right);
    Merge(left, kMid, right);
  }
}

template<std::random_access_iterator It, std::integral T=It::value_type>
std::pair<T, T> CountLessEqual(T pivot, It left, It right) {
  ssize_t count_less = 0, count_eq = 0;
  for (; left <= right; ++left) {
    if (*left < pivot) {
      ++count_less;
    } else if (*left == pivot) {
      ++count_eq;
    } else {
      break;
    }
  }
  return {count_less, count_eq};
}

template<std::random_access_iterator It, std::integral T=It::value_type>
T QuickSelect(ssize_t kth, It begin, It end) {
  if (end - begin < kBlockSz) {
    MergeSort(begin, end);
    return begin[kth];
  }

  std::vector<T> medians;
  for (auto it = begin; it < end; it += 5) {
    auto sub_right = std::min(it + 4, end);
    medians.emplace_back(QuickSelect((sub_right - it) / 2, it, sub_right));
  }

  auto median = QuickSelect(medians.size() / 2, medians.begin(), medians.end() - 1);
  auto eq_greater_begin = std::partition(begin, end + 1, [median](const auto& elem) {
    return elem < median;
  });
  auto greater_begin = std::partition(eq_greater_begin, end + 1, [median](const auto& elem) {
    return median == elem;
  });

  auto [sz_less, sz_eq] = CountLessEqual(median, begin, end);

  if (kth <= sz_less - 1) {
    return QuickSelect(kth, begin, eq_greater_begin - 1);
  }
  if (kth <= sz_less + sz_eq - 1) {
    return median;
  }
  return QuickSelect(kth - (sz_less + sz_eq), greater_begin, end);
}

}

