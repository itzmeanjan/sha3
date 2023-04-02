#pragma once
#include "keccak.hpp"
#include "utils.hpp"
#include <algorithm>
#include <array>
#include <cstring>

// Keccak family of sponge functions
namespace sponge {

// Compile-time check to ensure that domain seperator can only be 2/ 4 -bits
// wide
//
// When used in context of extendable output functions ( SHAKE{128, 256} )
// domain seperator bits are 4 -bit wide
//
// See section 6.{1, 2} of SHA3 specification
// https://dx.doi.org/10.6028/NIST.FIPS.202
constexpr bool
check_domain_seperator(const size_t dom_sep_bit_len)
{
  return (dom_sep_bit_len == 2) | (dom_sep_bit_len == 4);
}

// Pad10*1 - generates a padding, while also considering domain seperator bits (
// which are either 2 or 4 -bit wide ), such that when both domain seperator
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
template<const uint8_t domain_seperator,
         const size_t ds_bits,
         const size_t rate>
static inline std::array<uint8_t, rate / 8>
pad10x1(const size_t offset)
  requires(check_domain_seperator(ds_bits))
{
  std::array<uint8_t, rate / 8> res{};

  constexpr uint8_t mask = (1 << ds_bits) - 1;
  constexpr uint8_t pad_byte = (1 << ds_bits) | (domain_seperator & mask);

  res[offset] = pad_byte;
  res[(rate / 8) - 1] = 0x80;

  return res;
}

// Pad10*1 --- generates padding string P = 1 || 0^j || 1 such that when padded
// to input message M of bit length `mlen`, padded string M || P has bit length
// which is evenly divisible by rate ( parameterizable )
//
// Returns how many bits of padding ( = len(P) ) is stored in `pad`, starting
// from bit index 0.
//
// See specification of this function in section 5.1 of SHA3 document
// https://dx.doi.org/10.6028/NIST.FIPS.202
template<const uint8_t dom_sep, const size_t bits, const size_t rate>
static inline size_t
pad101(const size_t mlen, uint8_t* const pad)
  requires(check_domain_seperator(bits))
{
  const size_t j = rate - (mlen + 2ul) % rate;
  const bool flg = j <= (6ul - bits);
  constexpr uint8_t mask = (1 << bits) - 1;

  const size_t rm_bits = !flg * (j - (7ul - bits));
  const size_t rm_bytes = rm_bits >> 3;
  const size_t last_bits = !flg * (j - ((rm_bytes << 3) + (7ul - bits)));
  const size_t boff = rm_bytes + 1ul;

  pad[0] = (flg * (0b1 << (j + 1 + bits))) | (0b1 << bits) | (dom_sep & mask);
  std::memset(pad + 1ul, 0, rm_bytes);
  pad[boff] = 0b1 << last_bits;

  return j + 2ul;
}

// Extracts out N -th ( zero -based indexing ) message block from byte array
// P = M || pad10*1(rate, len(M))
//
// This function helps when absorbing padded message bytes into sponge.
template<const uint8_t dom_sep, const size_t bits, const size_t rate>
static inline void
get_msg_blk(
  const uint8_t* const __restrict msg, // message ( except domain seperator )
  const size_t mlen,                   // in bytes
  const uint8_t* const __restrict pad, // padding ( includes domain seperator )
  const size_t plen,                   // in bits
  uint8_t* const __restrict blk,       // extracted block
  const size_t blk_idx                 // index of block to extract
  )
  requires(check_domain_seperator(bits))
{
  const size_t mblen = mlen << 3;             // in bits | < first segment >
  const size_t tot_mblen = mblen + bits;      // in bits | < middle segment >
  const size_t padded_len = tot_mblen + plen; // in bits | < final segment >

  size_t soff = blk_idx * rate;
  size_t doff = 0;

  if (soff < mblen) {
    const size_t read = std::min(rate, mblen - soff);
    const size_t read_bytes = read >> 3;

    std::memcpy(blk + (doff >> 3), msg + (soff >> 3), read_bytes);

    soff += read;
    doff += read;
  }

  if ((doff < rate) && (soff < padded_len)) {
    const size_t read = padded_len - soff;
    const size_t read_bytes = read >> 3;

    std::memcpy(blk + (doff >> 3), pad, read_bytes);

    soff += read;
    doff += read;
  }
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
_absorb(uint64_t* const __restrict state,
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
    sha3_utils::bytes_to_le_words<rate>(blk_bytes, blk_words);

    for (size_t j = 0; j < rwords; j++) {
      state[j] ^= blk_words[j];
    }

    keccak::permute(state);

    moff += (rbytes - offset);
    offset = 0;
  }

  const size_t rm_bytes = mlen - moff;

  std::memset(blk_bytes, 0, rbytes);
  std::memcpy(blk_bytes + offset, msg + moff, rm_bytes);
  sha3_utils::bytes_to_le_words<rate>(blk_bytes, blk_words);

  for (size_t i = 0; i < rwords; i++) {
    state[i] ^= blk_words[i];
  }

  offset += rm_bytes;
}

// Sequentially absorbs N -message blocks into keccak[c] sponge state
//
// See step (1 - 6) of algorithm 8 defined in section 4 of SHA3 specification
// https://dx.doi.org/10.6028/NIST.FIPS.202
template<const uint8_t dom_sep, const size_t bits, const size_t rate>
static inline void
absorb(uint64_t* const __restrict state,
       const uint8_t* const __restrict msg,
       const size_t mlen)
  requires(check_domain_seperator(bits))
{
  const size_t mblen = mlen << 3;        // in bits
  const size_t tot_mblen = mblen + bits; // in bits

  constexpr size_t rbytes = rate >> 3;   // # -of bytes
  constexpr size_t rwords = rbytes >> 3; // # -of 64 -bit words

  uint8_t pad[rbytes]{};
  uint8_t blk_bytes[rbytes]{};
  uint64_t blk_words[rwords]{};

  const size_t plen = pad101<dom_sep, bits, rate>(tot_mblen, pad); // in bits
  const size_t padded_len = tot_mblen + plen;                      // in bits
  const size_t blk_cnt = padded_len / rate;

  std::memset(state, 0, 200);

  for (size_t i = 0; i < blk_cnt; i++) {
    get_msg_blk<dom_sep, bits, rate>(msg, mlen, pad, plen, blk_bytes, i);
    sha3_utils::bytes_to_le_words<rate>(blk_bytes, blk_words);

    for (size_t j = 0; j < rwords; j++) {
      state[j] ^= blk_words[j];
    }

    keccak::permute(state);
  }
}

// Given that N message bytes are already consumed into Keccak[c] permutation
// state, this routine finalizes sponge state and makes it ready for squeezing,
// by appending ( along with domain seperation bits ) 10*1 padding bits to input
// message s.t. total absorbed message byte length becomes multiple of
// `rate/ 8` -bytes.
//
// - `rate` portion of sponge will have bitwidth of 1600 - c.
// - `offset` must ∈ [0, `rbytes`)
//
// This function implementation collects some motivation from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950/src/sponge.rs#L58-L81
template<const uint8_t domain_seperator,
         const size_t ds_bits,
         const size_t rate>
static inline void
finalize(uint64_t* const __restrict state, size_t& offset)
  requires(check_domain_seperator(ds_bits))
{
  constexpr size_t rbytes = rate >> 3;   // # -of bytes
  constexpr size_t rwords = rbytes >> 3; // # -of 64 -bit words

  uint64_t words[rwords];

  const auto pad = pad10x1<domain_seperator, ds_bits, rate>(offset);
  sha3_utils::bytes_to_le_words<rate>(pad.data(), words);

  for (size_t j = 0; j < rwords; j++) {
    state[j] ^= words[j];
  }

  keccak::permute(state);
  offset = 0;
}

// Squeezes N -bytes output from sponge state which has consumed input message
//
// See step (7 - 10) of algorithm 8 defined in section 4 of SHA3 specification
// https://dx.doi.org/10.6028/NIST.FIPS.202
template<const size_t rate>
static inline void
squeeze(uint64_t* const __restrict state,
        uint8_t* const __restrict dig,
        const size_t dlen)
{
  constexpr size_t rbytes = rate >> 3;

  size_t off = 0;
  while (off < dlen) {
    const size_t read = std::min(rbytes, dlen - off);

    if constexpr (std::endian::native == std::endian::little) {
      std::memcpy(dig + off, state, read);
    } else {
      for (size_t i = 0; i < read; i++) {
        const size_t woff = i >> 3;
        const size_t boff = (i & 7ul) << 3;

        dig[off + i] = static_cast<uint8_t>(state[woff] >> boff);
      }
    }

    if (read == rbytes) {
      keccak::permute(state);
    }

    off += read;
  }
}

}
