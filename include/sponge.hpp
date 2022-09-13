#pragma once
#include "keccak.hpp"
#include <cstring>

// Keccak family of sponge functions
namespace sponge {

// Pad10*1 --- generates padding string P = 1 || 0^j || 1 such that when padded
// to input message M of bit length `mlen`, padded string M || P has bit length
// which is evenly divisible by rate ( parameterizable )
//
// Returns how many bits of padding ( = len(P) ) is stored in `pad`, staring
// from bit index 0.
//
// See specification of this function in section 5.1 of SHA3 document
// https://dx.doi.org/10.6028/NIST.FIPS.202
template<const size_t rate>
static size_t
pad101(const size_t mlen, uint8_t* const pad)
{
  const size_t j = rate - (mlen + 2ul) % rate;
  const bool flg = j <= 6ul;

  const size_t rm_bits = !flg * (j - 7ul);
  const size_t rm_bytes = rm_bits >> 3;
  const size_t last_bits = !flg * (j - ((rm_bytes << 3) + 7ul));
  const size_t boff = rm_bytes + 1ul;

  pad[0] = (flg * (0b1 << (j + 1))) | 0b1;
  std::memset(pad + 1ul, 0, rm_bytes);
  pad[boff] = 0b1 << last_bits;

  return j + 2ul;
}

}
