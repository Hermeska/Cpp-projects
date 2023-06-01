#pragma once

int* LowerBound(const int* first, const int* end, int value) {
  if (first == nullptr && end == nullptr) {
    return nullptr;
  }
  int distance = static_cast<int>(end - first);
  const int* middle = first + (end - first) / 2;
  while (distance > 0) {
    middle = first + (end - first) / 2;
    if (*(middle) >= value) {
      end = middle;
    } else {
      first = middle + 1;
    }
    distance = static_cast<int>(end - first);
  }
  return const_cast<int*>(first);
}
