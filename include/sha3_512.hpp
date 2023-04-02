#pragma once
#include "sponge.hpp"

// SHA3-512 Hash Function : Keccak[1024](M || 01, 512)
namespace sha3_512 {

// Given N -bytes input message, this routine consumes it into keccak[1024]
// sponge state and squeezes out 64 -bytes digest | N >= 0
//
// See SHA3 hash function definition in section 6.1 of SHA3 specification
// https://dx.doi.org/10.6028/NIST.FIPS.202
static inline void
hash(const uint8_t* const __restrict msg,
     const size_t mlen,
     uint8_t* const __restrict dig)
{
  constexpr size_t dlen = 512;
  constexpr size_t capacity = 2 * dlen;
  constexpr size_t rate = 1600 - capacity;

  uint64_t state[25]{};
  size_t offset = 0;
  size_t squeezable = rate >> 3;

  sponge::absorb<rate>(state, offset, msg, mlen);
  sponge::finalize<0b00000010, 2, rate>(state, offset);
  sponge::squeeze<rate>(state, squeezable, dig, dlen >> 3);
}

}
