#pragma once
#include <cassert>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <random>
#include <sstream>
#include <type_traits>

// Utility ( or commonly used ) functions for SHA3 implementation
namespace sha3_utils {

// Given a byte array holding rate/8 -many bytes, this routine can be invoked
// for interpreting those bytes as rate/ 64 -many words ( each word is 64 -bit
// unsigned interger ) s.t. bytes in a word are placed in little-endian order.
template<const size_t rate>
static inline void
bytes_to_le_words(const uint8_t* const __restrict bytes,
                  uint64_t* const __restrict words)
{
  constexpr size_t rbytes = rate >> 3;   // # -of bytes
  constexpr size_t rwords = rbytes >> 3; // # -of 64 -bit words

  if constexpr (std::endian::native == std::endian::little) {
    std::memcpy(words, bytes, rbytes);
  } else {
    for (size_t j = 0; j < rwords; j++) {
      const size_t boff = j << 3;
      words[j] = (static_cast<uint64_t>(bytes[boff + 7]) << 56) |
                 (static_cast<uint64_t>(bytes[boff + 6]) << 48) |
                 (static_cast<uint64_t>(bytes[boff + 5]) << 40) |
                 (static_cast<uint64_t>(bytes[boff + 4]) << 32) |
                 (static_cast<uint64_t>(bytes[boff + 3]) << 24) |
                 (static_cast<uint64_t>(bytes[boff + 2]) << 16) |
                 (static_cast<uint64_t>(bytes[boff + 1]) << 8) |
                 (static_cast<uint64_t>(bytes[boff + 0]) << 0);
    }
  }
}

// Given an array of uint64_t words, holding rate/ 8 -many bytes, this routine
// can be used for interpreting words as little-endian bytes.
template<const size_t rate>
static inline void
words_to_le_bytes(const uint64_t* const __restrict words,
                  uint8_t* const __restrict bytes)
{
  constexpr size_t rbytes = rate >> 3;   // # -of bytes
  constexpr size_t rwords = rbytes >> 3; // # -of 64 -bit words

  if constexpr (std::endian::native == std::endian::little) {
    std::memcpy(bytes, words, rbytes);
  } else {
    for (size_t i = 0; i < rwords; i++) {
      const size_t boff = i * 8;

      bytes[boff + 0] = static_cast<uint8_t>(words[i] >> 0);
      bytes[boff + 1] = static_cast<uint8_t>(words[i] >> 8);
      bytes[boff + 2] = static_cast<uint8_t>(words[i] >> 16);
      bytes[boff + 3] = static_cast<uint8_t>(words[i] >> 24);
      bytes[boff + 4] = static_cast<uint8_t>(words[i] >> 32);
      bytes[boff + 5] = static_cast<uint8_t>(words[i] >> 40);
      bytes[boff + 6] = static_cast<uint8_t>(words[i] >> 48);
      bytes[boff + 7] = static_cast<uint8_t>(words[i] >> 56);
    }
  }
}

// Generates N -many random values of type T | N >= 0
template<typename T>
static inline void
random_data(T* const data, const size_t len)
  requires(std::is_unsigned_v<T>)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<T> dis;

  for (size_t i = 0; i < len; i++) {
    data[i] = dis(gen);
  }
}

// Given a bytearray of length N, this function converts it to human readable
// hex string of length N << 1 | N >= 0
inline const std::string
to_hex(const uint8_t* const bytes, const size_t len)
{
  std::stringstream ss;
  ss << std::hex;

  for (size_t i = 0; i < len; i++) {
    ss << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(bytes[i]);
  }

  return ss.str();
}

// Given a hex encoded string of length 2*L, this routine can be used for
// parsing it as a byte array of length L.
//
// Taken from
// https://github.com/itzmeanjan/ascon/blob/603ba1f223ddd3a46cb0b3d31d014312d96792b5/include/utils.hpp#L120-L145
inline std::vector<uint8_t>
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
