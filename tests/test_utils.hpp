#pragma once
#include <cassert>
#include <charconv>
#include <random>
#include <span>
#include <type_traits>
#include <vector>

namespace sha3_test_utils {

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

// Given a hex encoded string of length 2*L, this routine can be used for parsing it as a byte array of length L.
//
// Taken from
// https://github.com/itzmeanjan/ascon/blob/603ba1f223ddd3a46cb0b3d31d014312d96792b5/include/utils.hpp#L120-L145
static inline std::vector<uint8_t>
from_hex(std::string_view hex)
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

}
