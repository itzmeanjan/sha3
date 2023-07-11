#pragma once
#include "keccak.hpp"
#include "utils.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cstring>

// Keccak family of sponge functions
namespace sponge {

// Compile-time check to ensure that domain separator can only be 2/ 4 -bits
// wide
//
// When used in context of extendable output functions ( SHAKE{128, 256} )
// domain separator bits are 4 -bit wide
//
// See section 6.{1, 2} of SHA3 specification
// https://dx.doi.org/10.6028/NIST.FIPS.202
constexpr bool
check_domain_separator(const size_t dom_sep_bit_len)
{
  return (dom_sep_bit_len == 2) | (dom_sep_bit_len == 4);
}

// Pad10*1 - generates a padding, while also considering domain separator bits (
// which are either 2 or 4 -bit wide ), such that when both domain separator
// bits and 10*1 padding is appended ( in order ) to actual message, total byte
// length of message consumed into keccak-p[1600, 24] permutation becomes
// multiple of `rate` -bits. The only parameter `offset` denotes how many bytes
// are already mixed with rate portion of permutation state meaning `offset`
// must ∈ [0, `rate/ 8`). This routine returns a byte array of length `rate/ 8`
// -bytes which can safely be mixed into permutation state duing sponge
// finalization phase.
//
// This function implementation collects motivation from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950/src/sponge.rs#L70-L72
template<const uint8_t domain_separator,
         const size_t ds_bits,
         const size_t rate>
static inline std::array<uint8_t, rate / 8>
pad10x1(const size_t offset)
  requires(check_domain_separator(ds_bits))
{
  std::array<uint8_t, rate / 8> res{};

  constexpr uint8_t mask = (1 << ds_bits) - 1;
  constexpr uint8_t pad_byte = (1 << ds_bits) | (domain_separator & mask);

  res[offset] = pad_byte;
  res[(rate / 8) - 1] ^= 0x80;

  return res;
}

// Given `mlen` (>=0) -bytes message, this routine consumes it into Keccak[c]
// permutation state s.t. `offset` ( second parameter ) denotes how many bytes
// are already consumed into rate portion of the state.
//
// - `rate` portion of sponge will have bitwidth of 1600 - c.
// - `offset` must ∈ [0, `rbytes`)
//
// This function implementation collects inspiration from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950/src/sponge.rs#L4-L56
template<const size_t rate>
static inline void
absorb(uint64_t* const __restrict state,
       size_t& offset,
       const uint8_t* const __restrict msg,
       const size_t mlen)
{
  constexpr size_t rbytes = rate >> 3;   // # -of bytes
  constexpr size_t rwords = rbytes >> 3; // # -of 64 -bit words

  uint8_t blk_bytes[rbytes]{};
  uint64_t blk_words[rwords]{};

  const size_t blk_cnt = (offset + mlen) / rbytes;
  size_t moff = 0;

  for (size_t i = 0; i < blk_cnt; i++) {
    std::memcpy(blk_bytes + offset, msg + moff, rbytes - offset);

    if constexpr (std::endian::native == std::endian::little) {
      auto words = reinterpret_cast<const uint64_t*>(blk_bytes);

      for (size_t j = 0; j < rwords; j++) {
        state[j] ^= words[j];
      }
    } else {
      sha3_utils::bytes_to_le_words<rate>(blk_bytes, blk_words);

      for (size_t j = 0; j < rwords; j++) {
        state[j] ^= blk_words[j];
      }
    }

    keccak::permute(state);

    moff += (rbytes - offset);
    offset = 0;
  }

  const size_t rm_bytes = mlen - moff;

  std::memset(blk_bytes, 0, rbytes);
  std::memcpy(blk_bytes + offset, msg + moff, rm_bytes);

  if constexpr (std::endian::native == std::endian::little) {
    auto words = reinterpret_cast<const uint64_t*>(blk_bytes);

    for (size_t j = 0; j < rwords; j++) {
      state[j] ^= words[j];
    }
  } else {
    sha3_utils::bytes_to_le_words<rate>(blk_bytes, blk_words);

    for (size_t j = 0; j < rwords; j++) {
      state[j] ^= blk_words[j];
    }
  }

  offset += rm_bytes;
}

// Given that N message bytes are already consumed into Keccak[c] permutation
// state, this routine finalizes sponge state and makes it ready for squeezing,
// by appending ( along with domain separation bits ) 10*1 padding bits to input
// message s.t. total absorbed message byte length becomes multiple of
// `rate/ 8` -bytes.
//
// - `rate` portion of sponge will have bitwidth of 1600 - c.
// - `offset` must ∈ [0, `rbytes`)
//
// This function implementation collects some motivation from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950/src/sponge.rs#L58-L81
template<const uint8_t domain_separator,
         const size_t ds_bits,
         const size_t rate>
static inline void
finalize(uint64_t* const __restrict state, size_t& offset)
  requires(check_domain_separator(ds_bits))
{
  constexpr size_t rbytes = rate >> 3;   // # -of bytes
  constexpr size_t rwords = rbytes >> 3; // # -of 64 -bit words

  const auto pad = pad10x1<domain_separator, ds_bits, rate>(offset);

  if constexpr (std::endian::native == std::endian::little) {
    auto words = reinterpret_cast<const uint64_t*>(pad.data());

    for (size_t j = 0; j < rwords; j++) {
      state[j] ^= words[j];
    }
  } else {
    uint64_t words[rwords];
    sha3_utils::bytes_to_le_words<rate>(pad.data(), words);

    for (size_t j = 0; j < rwords; j++) {
      state[j] ^= words[j];
    }
  }

  keccak::permute(state);
  offset = 0;
}

// Given that Keccak[c] permutation state is finalized, this routine can be
// invoked for squeezing `olen` -bytes out of rate portion of the state.
//
// - `rate` portion of sponge will have bitwidth of 1600 - c.
// - `squeezable` denotes how many bytes can be squeezed without permutating the
// sponge state.
// - When `squeezable` becomes 0, state needs to be permutated again, after
// which `rbytes` can again be squeezed from rate portion of the state.
//
// This function implementation collects motivation from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950/src/sponge.rs#L83-L118
template<const size_t rate>
static inline void
squeeze(uint64_t* const __restrict state,
        size_t& squeezable,
        uint8_t* const __restrict out,
        const size_t olen)
{
  constexpr size_t rbytes = rate >> 3;

  uint8_t rate_blk[rbytes];
  size_t off = 0;

  while (off < olen) {
    const size_t read = std::min(squeezable, olen - off);
    const size_t soff = rbytes - squeezable;

    if constexpr (std::endian::native == std::endian::little) {
      std::memcpy(out + off, reinterpret_cast<uint8_t*>(state) + soff, read);
    } else {
      sha3_utils::words_to_le_bytes<rate>(state, rate_blk);
      std::memcpy(out + off, rate_blk + soff, read);
    }

    squeezable -= read;
    off += read;

    if (squeezable == 0) {
      keccak::permute(state);
      squeezable = rbytes;
    }
  }
}

}
