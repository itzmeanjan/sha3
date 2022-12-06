#pragma once
#include "keccak.hpp"
#include <algorithm>
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
static constexpr bool
check_domain_seperator(const size_t dom_sep_bit_len)
{
  return (dom_sep_bit_len == 2) | (dom_sep_bit_len == 4);
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
static size_t
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
static void
get_msg_blk(const uint8_t* const __restrict msg, // message ( except domain seperator )
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

// Sequentially absorbs N -message blocks into keccak[c] sponge state
//
// See step (1 - 6) of algorithm 8 defined in section 4 of SHA3 specification
// https://dx.doi.org/10.6028/NIST.FIPS.202
template<const uint8_t dom_sep, const size_t bits, const size_t rate>
static void
absorb(uint64_t* const __restrict state, const uint8_t* const __restrict msg, const size_t mlen)
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

    if constexpr (std::endian::native == std::endian::little) {
      std::memcpy(blk_words, blk_bytes, rbytes);
    } else {
      for (size_t j = 0; j < rwords; j++) {
        const size_t off = j << 3;
        blk_words[j] = (static_cast<uint64_t>(blk_bytes[off ^ 7]) << 56) |
                       (static_cast<uint64_t>(blk_bytes[off ^ 6]) << 48) |
                       (static_cast<uint64_t>(blk_bytes[off ^ 5]) << 40) |
                       (static_cast<uint64_t>(blk_bytes[off ^ 4]) << 32) |
                       (static_cast<uint64_t>(blk_bytes[off ^ 3]) << 24) |
                       (static_cast<uint64_t>(blk_bytes[off ^ 2]) << 16) |
                       (static_cast<uint64_t>(blk_bytes[off ^ 1]) << 8) |
                       (static_cast<uint64_t>(blk_bytes[off ^ 0]) << 0);
      }
    }

    for (size_t j = 0; j < rwords; j++) {
      state[j] ^= blk_words[j];
    }

    keccak::permute(state);
  }
}

// Squeezes N -bytes output from sponge state which has consumed input message
//
// See step (7 - 10) of algorithm 8 defined in section 4 of SHA3 specification
// https://dx.doi.org/10.6028/NIST.FIPS.202
template<const size_t rate>
static void
squeeze(uint64_t* const __restrict state, uint8_t* const __restrict dig, const size_t dlen)
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
