#pragma once
#include <algorithm>
#include <cassert>
#include <charconv>
#include <numeric>
#include <random>
#include <span>
#include <type_traits>
#include <vector>

namespace sha3_test_utils {

// Generates N -many random values of type T | N >= 0.
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

// Given a static sized hex encoded string of length 2*L, this routine can be used for parsing it as a byte array of length L.
template<size_t L>
static inline std::array<uint8_t, L>
parse_static_sized_hex_string(std::string_view hex)
{
  const size_t hlen = hex.length();
  assert(hlen == 2 * L);

  const size_t blen = hlen / 2;
  std::array<uint8_t, L> res{ 0 };

  for (size_t i = 0; i < blen; i++) {
    const size_t off = i * 2;

    uint8_t byte = 0;
    auto sstr = hex.substr(off, 2);
    std::from_chars(sstr.data(), sstr.data() + 2, byte, 16);

    res[i] = byte;
  }

  return res;
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
    std::from_chars(sstr.data(), sstr.data() + 2, byte, 16);

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
    std::array<uint8_t, 251> pattern{};
    std::iota(pattern.begin(), pattern.end(), 0);

    return pattern;
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
