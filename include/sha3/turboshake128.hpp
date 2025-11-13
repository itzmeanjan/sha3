#pragma once
#include "sha3/internals/keccak.hpp"
#include "sha3/internals/sponge.hpp"
#include <algorithm>
#include <bit>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <limits>

// TurboSHAKE128 eXtendable Output Function
namespace turboshake128 {

// 12 -rounds Keccak-p[1600, 12] is applied.
static constexpr size_t NUM_KECCAK_ROUNDS = keccak::MAX_NUM_ROUNDS / 2;

// TurboSHAKE128 XOF offers at max 128-bits of security.
static constexpr size_t TARGET_BIT_SECURITY_LEVEL = 128;

// Width of capacity portion of the sponge, in bits.
static constexpr size_t CAPACITY = 2 * TARGET_BIT_SECURITY_LEVEL;

// Width of rate portion of the sponge, in bits.
static constexpr size_t RATE = 1600 - CAPACITY;

/**
 * TurboSHAKE128 eXtendable Output Function (XOF).
 *
 * See TurboSHAKE extendable output function definition in section 2 of RFC 9861 https://datatracker.ietf.org/doc/rfc9861.
 */
struct turboshake128_t
{
private:
  uint64_t state[keccak::LANE_CNT]{};
  size_t offset = 0;
  alignas(4) bool finalized = false; // all message bytes absorbed ?
  size_t squeezable = 0;

public:
  forceinline constexpr turboshake128_t() = default;
  forceinline constexpr size_t squeezable_num_bytes() const { return squeezable; }

  /**
   * Given N -many bytes input message, this routine consumes those into keccak[256] sponge state.
   *
   * Note, this routine can be called arbitrary number of times, each time with arbitrary bytes of input message, until
   * keccak[256] state is finalized ( by calling routine with similar name ). Once the sponge is finalized, it can't
   * absorb any more message bytes.
   */
  forceinline constexpr void absorb(std::span<const uint8_t> msg)
  {
    if (!finalized) {
      sponge::absorb<RATE, NUM_KECCAK_ROUNDS>(state, offset, msg);
    }
  }

  /**
   * After consuming arbitrary many input bytes, this routine is invoked when no more input bytes remaining to be consumed by keccak[256] state.
   * This function expects a domain separator byte ∈ [0x01, 0x7f]. If not supplied, it will take 0x1f as default value.
   *
   * Note, once this routine is called, calling absorb() or finalize() again, on same TurboSHAKE128 object, doesn't do
   * anything. After finalization, one might intend to read arbitrary many bytes by squeezing sponge, which is done by
   * calling squeeze() function, as many times required.
   */
  template<uint8_t dom_sep = 0x1f>
  forceinline constexpr void finalize()
    requires((dom_sep >= 0x01) && (dom_sep <= 0x7f))
  {
    if (!finalized) {
      sponge::finalize<dom_sep, std::bit_width(dom_sep) - 1, RATE, NUM_KECCAK_ROUNDS>(state, offset);

      finalized = true;
      squeezable = RATE / std::numeric_limits<uint8_t>::digits;
    }
  }

  /**
   * After sponge state is finalized, arbitrary many output bytes can be squeezed by calling this function
   * any number of times required.
   */
  forceinline constexpr void squeeze(std::span<uint8_t> dig)
  {
    if (finalized) {
      sponge::squeeze<RATE, NUM_KECCAK_ROUNDS>(state, squeezable, dig);
    }
  }

  /**
   * Reset the internal state of the TurboSHAKE128 XOF hasher.
   * Now it can again be used for another `absorb() -> finalize() -> squeeze()` cycle.
   */
  forceinline constexpr void reset()
  {
    std::fill(std::begin(state), std::end(state), 0);
    offset = 0;
    finalized = false;
    squeezable = 0;
  }

  /**
   * Given that sponge is already finalized, this routine can be used for zeroizing first n -bytes of
   * permutation state s.t. n <= 200 and applying permutation.
   */
  forceinline void ratchet(const size_t byte_len)
  {
    if (finalized) {
      const auto ratchetable_portion_byte_len = std::min(byte_len, keccak::STATE_BYTE_LEN);

      auto state_as_bytes = reinterpret_cast<uint8_t*>(state);
      std::memset(state_as_bytes, 0, ratchetable_portion_byte_len);

      keccak::permute<NUM_KECCAK_ROUNDS>(state);
    }
  }
};

}
