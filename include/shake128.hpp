#pragma once
#include "sponge.hpp"

// SHAKE128 Extendable Output Function : Keccak[256](M || 1111, d)
namespace shake128 {

// Width of capacity portion of the sponge, in bits.
constexpr size_t CAPACITY = 256;

// Width of rate portion of the sponge, in bits.
constexpr size_t RATE = 1600 - CAPACITY;

// Domain separator bits, used for finalization.
constexpr uint8_t DOM_SEP = 0b00001111;

// Bit-width of domain separator, starting from least significant bit.
constexpr size_t DOM_SEP_BW = 4;

// SHAKE128 Extendable Output Function (Xof)
//
// See SHA3 extendable output function definition in section 6.2 of SHA3
// specification https://dx.doi.org/10.6028/NIST.FIPS.202
struct shake128
{
private:
  uint64_t state[keccak::LANE_CNT]{};
  size_t offset = 0;
  alignas(4) bool finalized = false; // all message bytes absorbed ?
  size_t squeezable = 0;

public:
  inline shake128() = default;

  // Given N -many bytes input message, this routine consumes those into
  // keccak[256] sponge state.
  //
  // Note, this routine can be called arbitrary number of times, each time with
  // arbitrary bytes of input message, until keccak[256] state is finalized ( by
  // calling routine with similar name ). Once the sponge is finalized, it can't
  // absorb any more message bytes.
  inline void absorb(const uint8_t* const msg, const size_t mlen)
  {
    if (!finalized) {
      sponge::absorb<RATE>(state, offset, msg, mlen);
    }
  }

  // After consuming arbitrary many input bytes, this routine is invoked when
  // no more input bytes remaining to be consumed by keccak[256] state.
  //
  // Note, once this routine is called, calling absorb() or finalize() again, on
  // same SHAKE128 object, doesn't do anything. After finalization, one might
  // intend to read arbitrary many bytes by squeezing sponge, which is done by
  // calling read() function, as many times required.
  inline void finalize()
  {
    if (!finalized) {
      sponge::finalize<DOM_SEP, DOM_SEP_BW, RATE>(state, offset);

      finalized = true;
      squeezable = RATE / 8;
    }
  }

  // After sponge state is finalized, arbitrary many output bytes can be
  // squeezed by calling this function any number of times required.
  inline void squeeze(uint8_t* const dig, const size_t dlen)
  {
    if (finalized) {
      sponge::squeeze<RATE>(state, squeezable, dig, dlen);
    }
  }

  // Reset the internal state of the Shake128-Xof hasher, now it can again be
  // used for another absorb->finalize->squeeze cycle.
  inline void reset() { std::memset(this, 0, sizeof(*this)); }
};

}
