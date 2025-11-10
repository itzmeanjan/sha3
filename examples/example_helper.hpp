#pragma once
#include <cstdint>
#include <iomanip>
#include <span>
#include <sstream>

// Given a bytearray of length N, this function converts it to human readable hex string of length N << 1 | N >= 0
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
