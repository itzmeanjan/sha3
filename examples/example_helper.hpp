#pragma once
#include <cstdint>
#include <iomanip>
#include <random>
#include <span>
#include <sstream>

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

// Given a bytearray of length N, this function converts it to human readable
// hex string of length N << 1 | N >= 0
static inline std::string
to_hex(std::span<const uint8_t> bytes)
{
  std::stringstream ss;
  ss << std::hex;

  for (size_t i = 0; i < bytes.size(); i++) {
    ss << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(bytes[i]);
  }

  return ss.str();
}
