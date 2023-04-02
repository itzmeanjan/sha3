#pragma once
#include "sponge.hpp"

// SHAKE256 Extendable Output Function : Keccak[512](M || 1111, d)
namespace shake256 {

// Capacity of sponge, in bits
constexpr size_t capacity = 512;
// Rate of sponge, in bits
constexpr size_t rate = 1600 - capacity;

// Truth value denoting all message bytes are absorbed.
constexpr size_t ABSORBED_TRUE = -1ul;

// SHAKE256 Extendable Output Function
//
// See SHA3 extendable output function definition in section 6.2 of SHA3
// specification https://dx.doi.org/10.6028/NIST.FIPS.202
template<const bool incremental = false>
struct shake256
{
private:
  uint64_t state[25]{};
  size_t offset = 0;
  size_t absorbed = 0; // all message bytes absorbed ?
  size_t squeezable = 0;

public:
  // Given N -bytes input message, this routine consumes it into keccak[512]
  // sponge state
  //
  // Once you call this function on some object, calling it again doesn't do
  // anything !
  inline void hash(const uint8_t* const __restrict msg, const size_t mlen)
    requires(!incremental)
  {
    if (absorbed == ABSORBED_TRUE) {
      return;
    }

    sponge::absorb<rate>(state, offset, msg, mlen);
    sponge::finalize<0b00001111, 4, rate>(state, offset);

    absorbed = ABSORBED_TRUE;
    squeezable = rate >> 3;
  }

  // Given N -many bytes input message, this routine consumes those into
  // keccak[512] sponge state
  //
  // Note, this routine can be called arbitrary number of times, each time with
  // arbitrary bytes of input message, until keccak[512] state is finalized ( by
  // calling routine with similar name ).
  //
  // This function is only enabled, when you decide to use SHAKE256 in
  // incremental mode ( compile-time decision ). By default one uses SHAKE256
  // API in non-incremental mode.
  inline void absorb(const uint8_t* const __restrict msg, const size_t mlen)
    requires(incremental)
  {
    if (absorbed == ABSORBED_TRUE) {
      return;
    }

    sponge::absorb<rate>(state, offset, msg, mlen);
  }

  // After consuming N -many bytes ( by invoking absorb routine arbitrary many
  // times, each time with arbitrary input bytes ), this routine is invoked when
  // no more input bytes remaining to be consumed by keccak[512] state.
  //
  // Note, once this routine is called, calling absorb() or finalize() again, on
  // same SHAKE256 object, doesn't do anything. After finalization, one might
  // intend to read arbitrary many bytes by squeezing sponge, which is done by
  // calling read() function, as many times required.
  //
  // This function is only enabled, when you decide to use SHAKE256 in
  // incremental mode ( compile-time decision ). By default one uses SHAKE256
  // API in non-incremental mode.
  inline void finalize()
    requires(incremental)
  {
    if (absorbed == ABSORBED_TRUE) {
      return;
    }

    sponge::finalize<0b00001111, 4, rate>(state, offset);

    absorbed = ABSORBED_TRUE;
    squeezable = rate >> 3;
  }

  // Given that N -bytes input message is already absorbed into sponge state
  // using `hash( ... )` routine, this routine is used for squeezing M -bytes
  // out of consumable part of state ( i.e. rate portion of state )
  //
  // This routine can be used for squeezing arbitrary number of bytes from
  // sponge keccak[512]
  //
  // Make sure you absorb ( see hash(...) routine ) message bytes first, then
  // only call this function, otherwise it can't squeeze out anything.
  inline void read(uint8_t* const __restrict dig, const size_t dlen)
  {
    if (absorbed != ABSORBED_TRUE) {
      return;
    }

    sponge::squeeze<rate>(state, squeezable, dig, dlen);
  }
};

}
