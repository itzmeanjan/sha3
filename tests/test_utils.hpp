#pragma once
#include <algorithm>
#include <cassert>
#include <charconv>
#include <cstdint>
#include <numeric>
#include <random>
#include <span>
#include <string_view>
#include <type_traits>
#include <vector>

namespace sha3_test_utils {

// Compile-time hex character to nibble conversion.
constexpr uint8_t
hex_digit(char chr)
{
  if (chr >= '0' && chr <= '9') {
    return static_cast<uint8_t>(chr - '0');
  }
  if (chr >= 'a' && chr <= 'f') {
    return static_cast<uint8_t>(chr - 'a' + 10);
  }
  if (chr >= 'A' && chr <= 'F') {
    return static_cast<uint8_t>(chr - 'A' + 10);
  }
  return 0;
}

// Given a hex encoded string of length 2*L, this routine can be used for parsing it as a byte array of length L.
// This is constexpr and can be used in compile-time contexts (e.g. static_assert).
template<size_t L>
constexpr std::array<uint8_t, L>
from_hex(std::string_view hex)
{
  if (!std::is_constant_evaluated()) {
    assert(hex.length() % 2 == 0);
    assert(hex.length() / 2 == L);
  }

  std::array<uint8_t, L> res{};

  for (size_t i = 0; i < L; i++) {
    res[i] = static_cast<uint8_t>((hex_digit(hex[2 * i]) << 4) | hex_digit(hex[(2 * i) + 1]));
  }

  return res;
}

// Generates N -many random values of type T | N >= 0.
template<typename T>
static inline void
random_data(std::span<T> data)
  requires(std::is_unsigned_v<T>)
{
  std::random_device randev;
  std::mt19937_64 gen(randev());
  std::uniform_int_distribution<T> dis;

  const size_t len = data.size();
  for (size_t i = 0; i < len; i++) {
    data[i] = dis(gen);
  }
}

// Given a dynamic sized hex encoded string of length 2*L, this routine can be used for parsing it as a byte array of length L.
static inline std::vector<uint8_t>
parse_dynamic_sized_hex_string(std::string_view hex)
{
  const size_t hlen = hex.length();
  assert(hlen % 2 == 0);

  const size_t blen = hlen / 2;
  std::vector<uint8_t> res(blen, 0);

  for (size_t i = 0; i < blen; i++) {
    const size_t off = i * 2;

    uint8_t byte = 0;
    auto sstr = hex.substr(off, 2);
    std::from_chars(sstr.data(), sstr.data() + sstr.size(), byte, 16);

    res[i] = byte;
  }

  return res;
}

/**
 * Generates bytearray of length n by repeating static byte pattern returned by `pattern()`,
 * following https://www.ietf.org/archive/id/draft-irtf-cfrg-kangarootwelve-09.html#name-test-vectors
 */
static inline std::vector<uint8_t>
ptn(const size_t n)
{
  // Generates static byte pattern of length 251, following https://www.rfc-editor.org/rfc/rfc9861.html#name-test-vectors.
  auto pattern = []() -> auto {
    std::array<uint8_t, 251> bytes{};
    std::iota(bytes.begin(), bytes.end(), 0);

    return bytes;
  };

  std::vector<uint8_t> res(n, 0);
  auto res_span = std::span(res);

  size_t off = 0;
  while (off < n) {
    const auto read = std::min<size_t>(n - off, 251);

    auto static_pattern = pattern();
    auto static_pattern_span = std::span(static_pattern);

    std::copy_n(static_pattern_span.subspan(0, read).begin(), read, res_span.subspan(off, read).begin());
    off += read;
  }

  return res;
}

}
