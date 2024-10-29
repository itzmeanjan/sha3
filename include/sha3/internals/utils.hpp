#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

// Utility ( or commonly used ) functions for SHA3 implementation
namespace sha3_utils {

// Given a 64 -bit unsigned integer word, this routine swaps byte order and
// returns byte swapped 64 -bit word.
//
// Collects inspiration from
// https://github.com/itzmeanjan/ascon/blob/6160fee18814c7c313262e365c53de96ab6602b4/include/utils.hpp#L17-L43.
static inline constexpr uint64_t
bswap(const uint64_t a)
{
#if defined __GNUG__
  return __builtin_bswap64(a);
#else
  return ((a & 0x00000000000000fful) << 56) |
         ((a & 0x000000000000ff00ul) << 40) |
         ((a & 0x0000000000ff0000ul) << 24) |
         ((a & 0x00000000ff000000ul) << 0x8) |
         ((a & 0x000000ff00000000ul) >> 0x8) |
         ((a & 0x0000ff0000000000ul) >> 24) |
         ((a & 0x00ff000000000000ul) >> 40) |
         ((a & 0xff00000000000000ul) >> 56);
#endif
}

// Given a byte array of length 8, this routine can be used for interpreting
// those 8 -bytes in little-endian order, as a 64 -bit unsigned integer.
static inline constexpr uint64_t
le_bytes_to_u64(std::span<const uint8_t> bytes)
{
  const uint64_t word = (static_cast<uint64_t>(bytes[7]) << 56) |
                        (static_cast<uint64_t>(bytes[6]) << 48) |
                        (static_cast<uint64_t>(bytes[5]) << 40) |
                        (static_cast<uint64_t>(bytes[4]) << 32) |
                        (static_cast<uint64_t>(bytes[3]) << 24) |
                        (static_cast<uint64_t>(bytes[2]) << 16) |
                        (static_cast<uint64_t>(bytes[1]) << 8) |
                        (static_cast<uint64_t>(bytes[0]) << 0);

  if constexpr (std::endian::native == std::endian::big) {
    return bswap(word);
  } else {
    return word;
  }
}

// Given a byte array holding rate/8 -many bytes, this routine can be invoked
// for interpreting those bytes as rate/ 64 -many words ( each word is 64 -bit
// unsigned interger ) s.t. bytes in a word are placed in little-endian order.
template<size_t rate>
static inline constexpr void
le_bytes_to_u64_words(std::span<const uint8_t, rate / 8> bytes,
                      std::span<uint64_t, rate / 64> words)
{
  size_t off = 0;
  while (off < bytes.size()) {
    words[off / 8] = le_bytes_to_u64(bytes.subspan(off, 8));
    off += 8;
  }
}

// Given a 64 -bit unsigned integer as input, this routine can be used for
// interpreting those 8 -bytes in little-endian byte order.
static inline constexpr void
u64_to_le_bytes(uint64_t word, std::span<uint8_t> bytes)
{
  if constexpr (std::endian::native == std::endian::big) {
    word = bswap(word);
  }

  bytes[0] = static_cast<uint8_t>(word >> 0);
  bytes[1] = static_cast<uint8_t>(word >> 8);
  bytes[2] = static_cast<uint8_t>(word >> 16);
  bytes[3] = static_cast<uint8_t>(word >> 24);
  bytes[4] = static_cast<uint8_t>(word >> 32);
  bytes[5] = static_cast<uint8_t>(word >> 40);
  bytes[6] = static_cast<uint8_t>(word >> 48);
  bytes[7] = static_cast<uint8_t>(word >> 56);
}

// Given an array of rate/64 -many 64 -bit unsigned integer words, this routine
// can be used for interpreting words in little-endian byte-order, computing
// rate/8 -many bytes output.
template<size_t rate>
static inline constexpr void
u64_words_to_le_bytes(std::span<const uint64_t, rate / 64> words,
                      std::span<uint8_t, rate / 8> bytes)
{
  size_t off = 0;
  while (off < words.size()) {
    u64_to_le_bytes(words[off], bytes.subspan(off * 8, 8));
    off++;
  }
}

}
