#pragma once
#include "keccak.hpp"
#include <algorithm>
#include <cstring>

// Keccak family of sponge functions
namespace sponge {

// Compile-time check to ensure that domain seperator can only be 2 -bits wide
static constexpr bool
check_domain_seperator(const size_t dom_sep_bit_len)
{
  return dom_sep_bit_len == 2;
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
pad101(const size_t mlen,
       uint8_t* const pad) requires(check_domain_seperator(bits))
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
get_msg_blk(
  const uint8_t* const __restrict msg, // message ( except domain seperator )
  const size_t mlen,                   // in bytes
  const uint8_t* const __restrict pad, // padding ( includes domain seperator )
  const size_t plen,                   // in bits
  uint8_t* const __restrict blk,       // extracted block
  const size_t blk_idx                 // index of block to extract
  ) requires(check_domain_seperator(bits))
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

}
