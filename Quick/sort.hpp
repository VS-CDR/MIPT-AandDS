#pragma once

#include "select.hpp"

template<std::random_access_iterator It, class T=std::iterator_traits<It>::value_type>
requires std::is_convertible_v<T, typename std::iterator_traits<It>::value_type>
void SortPartition(T pivot, It& left_ptr, It& right_ptr) {
  while (left_ptr <= right_ptr) {
    while (*left_ptr < pivot) {
      ++left_ptr;
    }
    while (*right_ptr > pivot) {
      --right_ptr;
    }
    if (left_ptr <= right_ptr) {
      std::iter_swap(left_ptr++, right_ptr--);
    }
  }
}

template<std::random_access_iterator It>
void QuickSort(It left, It right) {
  if (left < right) {
    auto left_ptr(left);
    auto right_ptr(right);
    SortPartition(
        deterministic::QuickSelect((right - left) / 2, left, right),
        left_ptr,
        right_ptr
    );
    QuickSort(left, right_ptr);
    QuickSort(left_ptr, right);
  }
}
