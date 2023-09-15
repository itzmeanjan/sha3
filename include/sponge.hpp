#pragma once
#include "keccak.hpp"
#include "utils.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <span>

// Keccak family of sponge functions
namespace sponge {

// Compile-time check to ensure that domain separator can only be 2/ 4 -bits
// wide.
//
// When used in context of extendable output functions ( SHAKE{128, 256} )
// domain separator bits are 4 -bit wide.
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
static inline constexpr std::array<uint8_t, rate / 8>
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
// - `offset` must ∈ [0, `rbytes`).
//
// This function implementation collects inspiration from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950/src/sponge.rs#L4-L56
template<const size_t rate>
static inline constexpr void
absorb(uint64_t state[keccak::LANE_CNT],
       size_t& offset,
       std::span<const uint8_t> msg)
{
  constexpr size_t rbytes = rate >> 3;   // # -of bytes
  constexpr size_t rwords = rbytes >> 3; // # -of 64 -bit words

  std::array<uint8_t, rbytes> blk_bytes{};
  std::array<uint64_t, rwords> blk_words{};

  auto _blk_bytes = std::span(blk_bytes);
  auto _blk_words = std::span(blk_words);

  const size_t mlen = msg.size();
  const size_t blk_cnt = (offset + mlen) / rbytes;

  size_t moff = 0;

  for (size_t i = 0; i < blk_cnt; i++) {
    const size_t readable = rbytes - offset;

    auto _msg = msg.subspan(moff, readable);
    auto _blk = _blk_bytes.subspan(offset, readable);

    std::ranges::copy(_msg.begin(), _msg.end(), _blk.begin());
    sha3_utils::le_bytes_to_u64_words<rate>(_blk_bytes, _blk_words);

    for (size_t j = 0; j < rwords; j++) {
      state[j] ^= _blk_words[j];
    }

    keccak::permute(state);

    moff += readable;
    offset = 0;
  }

  const size_t rm_bytes = mlen - moff;

  auto _msg = msg.subspan(moff, rm_bytes);
  auto _blk = _blk_bytes.subspan(offset, rm_bytes);

  blk_bytes.fill(0x00);
  std::ranges::copy(_msg.begin(), _msg.end(), _blk.begin());
  sha3_utils::le_bytes_to_u64_words<rate>(_blk_bytes, _blk_words);

  for (size_t j = 0; j < rwords; j++) {
    state[j] ^= _blk_words[j];
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
static inline constexpr void
finalize(uint64_t state[keccak::LANE_CNT], size_t& offset)
  requires(check_domain_separator(ds_bits))
{
  constexpr size_t rbytes = rate >> 3;   // # -of bytes
  constexpr size_t rwords = rbytes >> 3; // # -of 64 -bit words

  const auto padb = pad10x1<domain_separator, ds_bits, rate>(offset);
  std::array<uint64_t, rwords> padw{};

  auto _padb = std::span(padb);
  auto _padw = std::span(padw);

  sha3_utils::le_bytes_to_u64_words<rate>(_padb, _padw);

  for (size_t j = 0; j < rwords; j++) {
    state[j] ^= _padw[j];
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
static inline constexpr void
squeeze(uint64_t state[keccak::LANE_CNT],
        size_t& squeezable,
        std::span<uint8_t> out)
{
  constexpr size_t rbytes = rate >> 3;   // # -of bytes
  constexpr size_t rwords = rbytes >> 3; // # -of 64 -bit words

  std::array<uint8_t, rbytes> blk_bytes{};
  auto _blk_bytes = std::span(blk_bytes);

  auto swords = std::span{ state, keccak::LANE_CNT };
  auto _swords = swords.template subspan<0, rwords>();

  const size_t olen = out.size();
  size_t off = 0;

  while (off < olen) {
    const size_t read = std::min(squeezable, olen - off);
    const size_t soff = rbytes - squeezable;

    sha3_utils::u64_words_to_le_bytes<rate>(_swords, _blk_bytes);

    auto _blk = _blk_bytes.subspan(soff, read);
    auto _out = out.subspan(off, read);

    std::ranges::copy(_blk.begin(), _blk.end(), _out.begin());

    squeezable -= read;
    off += read;

    if (squeezable == 0) {
      keccak::permute(state);
      squeezable = rbytes;
    }
  }
}

}
