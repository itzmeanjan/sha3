#pragma once

#if defined(__AVX2__)

#include "sha3/internals/keccak.hpp"
#include "sha3/internals/simd/avx2.hpp"
#include "sha3/internals/sponge_x4.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>

// 4-way parallel SHAKE256 Extendable Output Function using AVX2.
namespace shake256_x4 {

using vec = sha3_simd::avx2::vec;

static constexpr size_t NUM_KECCAK_ROUNDS = keccak::MAX_NUM_ROUNDS;
static constexpr size_t TARGET_BIT_SECURITY_LEVEL = 256;
static constexpr size_t CAPACITY = 2 * TARGET_BIT_SECURITY_LEVEL;
static constexpr size_t RATE = 1600 - CAPACITY;
static constexpr uint8_t DOM_SEP = 0b00001111;
static constexpr size_t DOM_SEP_BW = std::bit_width(DOM_SEP);

/**
 * 4-way parallel SHAKE256 XOF using AVX2.
 *
 * Operates on 4 independent SHAKE256 instances simultaneously, sharing the same
 * absorb/finalize/squeeze schedule. All 4 instances must absorb same-length messages
 * and squeeze the same number of bytes.
 */
struct shake256_x4_t
{
private:
  std::array<vec, keccak::LANE_CNT> state{};
  size_t offset = 0;
  alignas(4) bool finalized = false;
  size_t squeezable = 0;

public:
  forceinline shake256_x4_t() = default;

  forceinline void absorb(std::span<const uint8_t> msg0, std::span<const uint8_t> msg1, std::span<const uint8_t> msg2, std::span<const uint8_t> msg3)
  {
    if (!finalized) {
      sponge_x4::absorb_x4<RATE, NUM_KECCAK_ROUNDS>(state, offset, msg0, msg1, msg2, msg3);
    }
  }

  forceinline void finalize()
  {
    if (!finalized) {
      sponge_x4::finalize_x4<DOM_SEP, DOM_SEP_BW, RATE, NUM_KECCAK_ROUNDS>(state, offset);
      finalized = true;
      squeezable = RATE / std::numeric_limits<uint8_t>::digits;
    }
  }

  forceinline void squeeze(std::span<uint8_t> out0, std::span<uint8_t> out1, std::span<uint8_t> out2, std::span<uint8_t> out3)
  {
    if (finalized) {
      sponge_x4::squeeze_x4<RATE, NUM_KECCAK_ROUNDS>(state, squeezable, out0, out1, out2, out3);
    }
  }

  forceinline void reset()
  {
    std::fill(state.begin(), state.end(), vec{});
    offset = 0;
    finalized = false;
    squeezable = 0;
  }
};

}

#endif
