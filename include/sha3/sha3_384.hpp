#pragma once
#include "sha3/internals/sponge.hpp"
#include <bit>
#include <cstdint>
#include <limits>

// SHA3-384 Hash Function : Keccak[768](M || 01, 384)
namespace sha3_384 {

// Number of rounds keccak-p[1600] is applied.
static constexpr size_t NUM_KECCAK_ROUNDS = keccak::MAX_NUM_ROUNDS;

// Bit length of SHA3-384 message digest.
static constexpr size_t DIGEST_BIT_LEN = 384;

// Byte length of SHA3-384 message digest.
static constexpr size_t DIGEST_LEN = DIGEST_BIT_LEN / std::numeric_limits<uint8_t>::digits;

// Width of capacity portion of the sponge, in bits.
static constexpr size_t CAPACITY = 2 * DIGEST_BIT_LEN;

// Width of rate portion of the sponge, in bits.
static constexpr size_t RATE = 1600 - CAPACITY;

// Domain separator bits, used for finalization.
static constexpr uint8_t DOM_SEP = 0b00000010;

// Bit-width of domain separator, starting from least significant bit.
static constexpr size_t DOM_SEP_BW = std::bit_width(DOM_SEP);

/**
 * Given arbitrary many input message bytes, this routine consumes it into keccak[768] sponge state and squeezes out 48 -bytes digest.
 *
 * See SHA3 hash function definition in section 6.1 of SHA3 specification https://dx.doi.org/10.6028/NIST.FIPS.202.
 */
struct sha3_384_t
{
private:
  std::array<uint64_t, keccak::LANE_CNT> state{};
  size_t offset = 0;
  alignas(4) bool finalized = false;
  alignas(4) bool squeezed = false;

public:
  // Constructor
  forceinline constexpr sha3_384_t() = default;

  /**
   * Given an arbitrary length message, absorbs it all in the SHA3_384 hasher and returns a 48 -bytes message digest.
   * This is the oneshot hashing API. For working with longer message stream, prefer using `absorb() -> finalize() -> digest()`.
   */
  forceinline static constexpr std::array<uint8_t, DIGEST_LEN> hash(std::span<const uint8_t> msg)
  {
    std::array<uint8_t, DIGEST_LEN> md{ 0 };

    sha3_384_t hasher;
    hasher.absorb(msg);
    hasher.finalize();
    hasher.digest(md);

    return md;
  }

  /**
   * Given N (>=0) -bytes message as input, this routine can be invoked arbitrary many times ( until the sponge is
   * finalized ), each time absorbing arbitrary many message bytes into RATE portion of the sponge.
   */
  forceinline constexpr void absorb(std::span<const uint8_t> msg)
  {
    if (!finalized) {
      sponge::absorb<RATE, NUM_KECCAK_ROUNDS>(state, offset, msg);
    }
  }

  /**
   * Finalizes the sponge after all message bytes are absorbed into it, now it should be ready for squeezing message
   * digest bytes. Once finalized, you can't absorb any message bytes into sponge. After finalization, calling this
   * function again and again doesn't mutate anything.
   */
  forceinline constexpr void finalize()
  {
    if (!finalized) {
      sponge::finalize<DOM_SEP, DOM_SEP_BW, RATE, NUM_KECCAK_ROUNDS>(state, offset);
      finalized = true;
    }
  }

  /**
   * After sponge state is finalized, 48 message digest (MD) bytes can be squeezed by calling this function.
   * Once digest bytes are squeezed, calling this function again and again returns nothing.
   */
  forceinline constexpr void digest(std::span<uint8_t, DIGEST_LEN> md)
  {
    if (finalized && !squeezed) {
      size_t squeezable = RATE / std::numeric_limits<uint8_t>::digits;
      sponge::squeeze<RATE, NUM_KECCAK_ROUNDS>(state, squeezable, md);

      squeezed = true;
    }
  }

  /**
   * Reset the internal state of the SHA3-384 hasher.
   * Now it can again be used for another `absorb() -> finalize() -> digest()` cycle.
   */
  forceinline constexpr void reset()
  {
    state.fill(0);
    offset = 0;
    finalized = false;
    squeezed = false;
  }
};

}
