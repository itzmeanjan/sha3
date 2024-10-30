#pragma once
#include <algorithm>
#include <random>
#include <span>
#include <vector>

const auto compute_min = [](const std::vector<double>& v) -> double { return *std::min_element(v.begin(), v.end()); };
const auto compute_max = [](const std::vector<double>& v) -> double { return *std::max_element(v.begin(), v.end()); };

// Generates N -many random values of type T | N >= 0
template<typename T>
static inline void
random_data(std::span<T> data)
  requires(std::is_unsigned_v<T>)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<T> dis;

  const size_t len = data.size();
  for (size_t i = 0; i < len; i++) {
    data[i] = dis(gen);
  }
}
