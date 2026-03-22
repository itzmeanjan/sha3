#pragma once

#if defined(__AVX2__)

#include "sha3/internals/force_inline.hpp"
#include "sha3/internals/keccak.hpp"
#include "sha3/internals/simd/avx2.hpp"
#include <array>
#include <cstddef>

// 4-way parallel Keccak-p[1600, 12] and Keccak-p[1600, 24] permutation using AVX2.
// Each vec lane holds one of 4 independent Keccak-f[1600] states.
namespace keccak_x4 {

using vec = sha3_simd::avx2::vec;

/**
 * 4-way parallel Keccak-f[1600] round function, applying all five step mapping functions, updating 4 states simultaneously.
 * Applies 4 consecutive rounds (ridx, ridx+1, ridx+2, ridx+3) in a single call.
 *
 * This is a direct AVX2 translation of keccak::roundx4() from keccak.hpp.
 */
static forceinline void
roundx4(std::array<vec, keccak::LANE_CNT>& state, const size_t ridx)
{
  std::array<vec, 5> bc{};
  std::array<vec, 5> d{};
  vec t;

  // Round ridx + 0
  bc[0] = bc[1] = bc[2] = bc[3] = bc[4] = vec{};

  for (size_t i = 0; i < keccak::LANE_CNT; i += 5) {
    bc[0] ^= state[i + 0];
    bc[1] ^= state[i + 1];
    bc[2] ^= state[i + 2];
    bc[3] ^= state[i + 3];
    bc[4] ^= state[i + 4];
  }

  d[0] = bc[4] ^ rotl64(bc[1], 1);
  d[1] = bc[0] ^ rotl64(bc[2], 1);
  d[2] = bc[1] ^ rotl64(bc[3], 1);
  d[3] = bc[2] ^ rotl64(bc[4], 1);
  d[4] = bc[3] ^ rotl64(bc[0], 1);

  bc[0] = state[0] ^ d[0];
  t = state[6] ^ d[1];
  bc[1] = rotl64(t, keccak::ROT[6]);
  t = state[12] ^ d[2];
  bc[2] = rotl64(t, keccak::ROT[12]);
  t = state[18] ^ d[3];
  bc[3] = rotl64(t, keccak::ROT[18]);
  t = state[24] ^ d[4];
  bc[4] = rotl64(t, keccak::ROT[24]);

  state[0] = (bc[0] ^ andnot(bc[1], bc[2])) ^ vec::broadcast(keccak::RC[ridx]);
  state[6] = bc[1] ^ andnot(bc[2], bc[3]);
  state[12] = bc[2] ^ andnot(bc[3], bc[4]);
  state[18] = bc[3] ^ andnot(bc[4], bc[0]);
  state[24] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[10] ^ d[0];
  bc[2] = rotl64(t, keccak::ROT[10]);
  t = state[16] ^ d[1];
  bc[3] = rotl64(t, keccak::ROT[16]);
  t = state[22] ^ d[2];
  bc[4] = rotl64(t, keccak::ROT[22]);
  t = state[3] ^ d[3];
  bc[0] = rotl64(t, keccak::ROT[3]);
  t = state[9] ^ d[4];
  bc[1] = rotl64(t, keccak::ROT[9]);

  state[10] = bc[0] ^ andnot(bc[1], bc[2]);
  state[16] = bc[1] ^ andnot(bc[2], bc[3]);
  state[22] = bc[2] ^ andnot(bc[3], bc[4]);
  state[3] = bc[3] ^ andnot(bc[4], bc[0]);
  state[9] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[20] ^ d[0];
  bc[4] = rotl64(t, keccak::ROT[20]);
  t = state[1] ^ d[1];
  bc[0] = rotl64(t, keccak::ROT[1]);
  t = state[7] ^ d[2];
  bc[1] = rotl64(t, keccak::ROT[7]);
  t = state[13] ^ d[3];
  bc[2] = rotl64(t, keccak::ROT[13]);
  t = state[19] ^ d[4];
  bc[3] = rotl64(t, keccak::ROT[19]);

  state[20] = bc[0] ^ andnot(bc[1], bc[2]);
  state[1] = bc[1] ^ andnot(bc[2], bc[3]);
  state[7] = bc[2] ^ andnot(bc[3], bc[4]);
  state[13] = bc[3] ^ andnot(bc[4], bc[0]);
  state[19] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[5] ^ d[0];
  bc[1] = rotl64(t, keccak::ROT[5]);
  t = state[11] ^ d[1];
  bc[2] = rotl64(t, keccak::ROT[11]);
  t = state[17] ^ d[2];
  bc[3] = rotl64(t, keccak::ROT[17]);
  t = state[23] ^ d[3];
  bc[4] = rotl64(t, keccak::ROT[23]);
  t = state[4] ^ d[4];
  bc[0] = rotl64(t, keccak::ROT[4]);

  state[5] = bc[0] ^ andnot(bc[1], bc[2]);
  state[11] = bc[1] ^ andnot(bc[2], bc[3]);
  state[17] = bc[2] ^ andnot(bc[3], bc[4]);
  state[23] = bc[3] ^ andnot(bc[4], bc[0]);
  state[4] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[15] ^ d[0];
  bc[3] = rotl64(t, keccak::ROT[15]);
  t = state[21] ^ d[1];
  bc[4] = rotl64(t, keccak::ROT[21]);
  t = state[2] ^ d[2];
  bc[0] = rotl64(t, keccak::ROT[2]);
  t = state[8] ^ d[3];
  bc[1] = rotl64(t, keccak::ROT[8]);
  t = state[14] ^ d[4];
  bc[2] = rotl64(t, keccak::ROT[14]);

  state[15] = bc[0] ^ andnot(bc[1], bc[2]);
  state[21] = bc[1] ^ andnot(bc[2], bc[3]);
  state[2] = bc[2] ^ andnot(bc[3], bc[4]);
  state[8] = bc[3] ^ andnot(bc[4], bc[0]);
  state[14] = bc[4] ^ andnot(bc[0], bc[1]);

  // Round ridx + 1
  bc[0] = bc[1] = bc[2] = bc[3] = bc[4] = vec{};

  for (size_t i = 0; i < keccak::LANE_CNT; i += 5) {
    bc[0] ^= state[i + 0];
    bc[1] ^= state[i + 1];
    bc[2] ^= state[i + 2];
    bc[3] ^= state[i + 3];
    bc[4] ^= state[i + 4];
  }

  d[0] = bc[4] ^ rotl64(bc[1], 1);
  d[1] = bc[0] ^ rotl64(bc[2], 1);
  d[2] = bc[1] ^ rotl64(bc[3], 1);
  d[3] = bc[2] ^ rotl64(bc[4], 1);
  d[4] = bc[3] ^ rotl64(bc[0], 1);

  bc[0] = state[0] ^ d[0];
  t = state[16] ^ d[1];
  bc[1] = rotl64(t, keccak::ROT[6]);
  t = state[7] ^ d[2];
  bc[2] = rotl64(t, keccak::ROT[12]);
  t = state[23] ^ d[3];
  bc[3] = rotl64(t, keccak::ROT[18]);
  t = state[14] ^ d[4];
  bc[4] = rotl64(t, keccak::ROT[24]);

  state[0] = (bc[0] ^ andnot(bc[1], bc[2])) ^ vec::broadcast(keccak::RC[ridx + 1]);
  state[16] = bc[1] ^ andnot(bc[2], bc[3]);
  state[7] = bc[2] ^ andnot(bc[3], bc[4]);
  state[23] = bc[3] ^ andnot(bc[4], bc[0]);
  state[14] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[20] ^ d[0];
  bc[2] = rotl64(t, keccak::ROT[10]);
  t = state[11] ^ d[1];
  bc[3] = rotl64(t, keccak::ROT[16]);
  t = state[2] ^ d[2];
  bc[4] = rotl64(t, keccak::ROT[22]);
  t = state[18] ^ d[3];
  bc[0] = rotl64(t, keccak::ROT[3]);
  t = state[9] ^ d[4];
  bc[1] = rotl64(t, keccak::ROT[9]);

  state[20] = bc[0] ^ andnot(bc[1], bc[2]);
  state[11] = bc[1] ^ andnot(bc[2], bc[3]);
  state[2] = bc[2] ^ andnot(bc[3], bc[4]);
  state[18] = bc[3] ^ andnot(bc[4], bc[0]);
  state[9] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[15] ^ d[0];
  bc[4] = rotl64(t, keccak::ROT[20]);
  t = state[6] ^ d[1];
  bc[0] = rotl64(t, keccak::ROT[1]);
  t = state[22] ^ d[2];
  bc[1] = rotl64(t, keccak::ROT[7]);
  t = state[13] ^ d[3];
  bc[2] = rotl64(t, keccak::ROT[13]);
  t = state[4] ^ d[4];
  bc[3] = rotl64(t, keccak::ROT[19]);

  state[15] = bc[0] ^ andnot(bc[1], bc[2]);
  state[6] = bc[1] ^ andnot(bc[2], bc[3]);
  state[22] = bc[2] ^ andnot(bc[3], bc[4]);
  state[13] = bc[3] ^ andnot(bc[4], bc[0]);
  state[4] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[10] ^ d[0];
  bc[1] = rotl64(t, keccak::ROT[5]);
  t = state[1] ^ d[1];
  bc[2] = rotl64(t, keccak::ROT[11]);
  t = state[17] ^ d[2];
  bc[3] = rotl64(t, keccak::ROT[17]);
  t = state[8] ^ d[3];
  bc[4] = rotl64(t, keccak::ROT[23]);
  t = state[24] ^ d[4];
  bc[0] = rotl64(t, keccak::ROT[4]);

  state[10] = bc[0] ^ andnot(bc[1], bc[2]);
  state[1] = bc[1] ^ andnot(bc[2], bc[3]);
  state[17] = bc[2] ^ andnot(bc[3], bc[4]);
  state[8] = bc[3] ^ andnot(bc[4], bc[0]);
  state[24] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[5] ^ d[0];
  bc[3] = rotl64(t, keccak::ROT[15]);
  t = state[21] ^ d[1];
  bc[4] = rotl64(t, keccak::ROT[21]);
  t = state[12] ^ d[2];
  bc[0] = rotl64(t, keccak::ROT[2]);
  t = state[3] ^ d[3];
  bc[1] = rotl64(t, keccak::ROT[8]);
  t = state[19] ^ d[4];
  bc[2] = rotl64(t, keccak::ROT[14]);

  state[5] = bc[0] ^ andnot(bc[1], bc[2]);
  state[21] = bc[1] ^ andnot(bc[2], bc[3]);
  state[12] = bc[2] ^ andnot(bc[3], bc[4]);
  state[3] = bc[3] ^ andnot(bc[4], bc[0]);
  state[19] = bc[4] ^ andnot(bc[0], bc[1]);

  // Round ridx + 2
  bc[0] = bc[1] = bc[2] = bc[3] = bc[4] = vec{};

  for (size_t i = 0; i < keccak::LANE_CNT; i += 5) {
    bc[0] ^= state[i + 0];
    bc[1] ^= state[i + 1];
    bc[2] ^= state[i + 2];
    bc[3] ^= state[i + 3];
    bc[4] ^= state[i + 4];
  }

  d[0] = bc[4] ^ rotl64(bc[1], 1);
  d[1] = bc[0] ^ rotl64(bc[2], 1);
  d[2] = bc[1] ^ rotl64(bc[3], 1);
  d[3] = bc[2] ^ rotl64(bc[4], 1);
  d[4] = bc[3] ^ rotl64(bc[0], 1);

  bc[0] = state[0] ^ d[0];
  t = state[11] ^ d[1];
  bc[1] = rotl64(t, keccak::ROT[6]);
  t = state[22] ^ d[2];
  bc[2] = rotl64(t, keccak::ROT[12]);
  t = state[8] ^ d[3];
  bc[3] = rotl64(t, keccak::ROT[18]);
  t = state[19] ^ d[4];
  bc[4] = rotl64(t, keccak::ROT[24]);

  state[0] = (bc[0] ^ andnot(bc[1], bc[2])) ^ vec::broadcast(keccak::RC[ridx + 2]);
  state[11] = bc[1] ^ andnot(bc[2], bc[3]);
  state[22] = bc[2] ^ andnot(bc[3], bc[4]);
  state[8] = bc[3] ^ andnot(bc[4], bc[0]);
  state[19] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[15] ^ d[0];
  bc[2] = rotl64(t, keccak::ROT[10]);
  t = state[1] ^ d[1];
  bc[3] = rotl64(t, keccak::ROT[16]);
  t = state[12] ^ d[2];
  bc[4] = rotl64(t, keccak::ROT[22]);
  t = state[23] ^ d[3];
  bc[0] = rotl64(t, keccak::ROT[3]);
  t = state[9] ^ d[4];
  bc[1] = rotl64(t, keccak::ROT[9]);

  state[15] = bc[0] ^ andnot(bc[1], bc[2]);
  state[1] = bc[1] ^ andnot(bc[2], bc[3]);
  state[12] = bc[2] ^ andnot(bc[3], bc[4]);
  state[23] = bc[3] ^ andnot(bc[4], bc[0]);
  state[9] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[5] ^ d[0];
  bc[4] = rotl64(t, keccak::ROT[20]);
  t = state[16] ^ d[1];
  bc[0] = rotl64(t, keccak::ROT[1]);
  t = state[2] ^ d[2];
  bc[1] = rotl64(t, keccak::ROT[7]);
  t = state[13] ^ d[3];
  bc[2] = rotl64(t, keccak::ROT[13]);
  t = state[24] ^ d[4];
  bc[3] = rotl64(t, keccak::ROT[19]);

  state[5] = bc[0] ^ andnot(bc[1], bc[2]);
  state[16] = bc[1] ^ andnot(bc[2], bc[3]);
  state[2] = bc[2] ^ andnot(bc[3], bc[4]);
  state[13] = bc[3] ^ andnot(bc[4], bc[0]);
  state[24] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[20] ^ d[0];
  bc[1] = rotl64(t, keccak::ROT[5]);
  t = state[6] ^ d[1];
  bc[2] = rotl64(t, keccak::ROT[11]);
  t = state[17] ^ d[2];
  bc[3] = rotl64(t, keccak::ROT[17]);
  t = state[3] ^ d[3];
  bc[4] = rotl64(t, keccak::ROT[23]);
  t = state[14] ^ d[4];
  bc[0] = rotl64(t, keccak::ROT[4]);

  state[20] = bc[0] ^ andnot(bc[1], bc[2]);
  state[6] = bc[1] ^ andnot(bc[2], bc[3]);
  state[17] = bc[2] ^ andnot(bc[3], bc[4]);
  state[3] = bc[3] ^ andnot(bc[4], bc[0]);
  state[14] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[10] ^ d[0];
  bc[3] = rotl64(t, keccak::ROT[15]);
  t = state[21] ^ d[1];
  bc[4] = rotl64(t, keccak::ROT[21]);
  t = state[7] ^ d[2];
  bc[0] = rotl64(t, keccak::ROT[2]);
  t = state[18] ^ d[3];
  bc[1] = rotl64(t, keccak::ROT[8]);
  t = state[4] ^ d[4];
  bc[2] = rotl64(t, keccak::ROT[14]);

  state[10] = bc[0] ^ andnot(bc[1], bc[2]);
  state[21] = bc[1] ^ andnot(bc[2], bc[3]);
  state[7] = bc[2] ^ andnot(bc[3], bc[4]);
  state[18] = bc[3] ^ andnot(bc[4], bc[0]);
  state[4] = bc[4] ^ andnot(bc[0], bc[1]);

  // Round ridx + 3
  bc[0] = bc[1] = bc[2] = bc[3] = bc[4] = vec{};

  for (size_t i = 0; i < keccak::LANE_CNT; i += 5) {
    bc[0] ^= state[i + 0];
    bc[1] ^= state[i + 1];
    bc[2] ^= state[i + 2];
    bc[3] ^= state[i + 3];
    bc[4] ^= state[i + 4];
  }

  d[0] = bc[4] ^ rotl64(bc[1], 1);
  d[1] = bc[0] ^ rotl64(bc[2], 1);
  d[2] = bc[1] ^ rotl64(bc[3], 1);
  d[3] = bc[2] ^ rotl64(bc[4], 1);
  d[4] = bc[3] ^ rotl64(bc[0], 1);

  bc[0] = state[0] ^ d[0];
  t = state[1] ^ d[1];
  bc[1] = rotl64(t, keccak::ROT[6]);
  t = state[2] ^ d[2];
  bc[2] = rotl64(t, keccak::ROT[12]);
  t = state[3] ^ d[3];
  bc[3] = rotl64(t, keccak::ROT[18]);
  t = state[4] ^ d[4];
  bc[4] = rotl64(t, keccak::ROT[24]);

  state[0] = (bc[0] ^ andnot(bc[1], bc[2])) ^ vec::broadcast(keccak::RC[ridx + 3]);
  state[1] = bc[1] ^ andnot(bc[2], bc[3]);
  state[2] = bc[2] ^ andnot(bc[3], bc[4]);
  state[3] = bc[3] ^ andnot(bc[4], bc[0]);
  state[4] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[5] ^ d[0];
  bc[2] = rotl64(t, keccak::ROT[10]);
  t = state[6] ^ d[1];
  bc[3] = rotl64(t, keccak::ROT[16]);
  t = state[7] ^ d[2];
  bc[4] = rotl64(t, keccak::ROT[22]);
  t = state[8] ^ d[3];
  bc[0] = rotl64(t, keccak::ROT[3]);
  t = state[9] ^ d[4];
  bc[1] = rotl64(t, keccak::ROT[9]);

  state[5] = bc[0] ^ andnot(bc[1], bc[2]);
  state[6] = bc[1] ^ andnot(bc[2], bc[3]);
  state[7] = bc[2] ^ andnot(bc[3], bc[4]);
  state[8] = bc[3] ^ andnot(bc[4], bc[0]);
  state[9] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[10] ^ d[0];
  bc[4] = rotl64(t, keccak::ROT[20]);
  t = state[11] ^ d[1];
  bc[0] = rotl64(t, keccak::ROT[1]);
  t = state[12] ^ d[2];
  bc[1] = rotl64(t, keccak::ROT[7]);
  t = state[13] ^ d[3];
  bc[2] = rotl64(t, keccak::ROT[13]);
  t = state[14] ^ d[4];
  bc[3] = rotl64(t, keccak::ROT[19]);

  state[10] = bc[0] ^ andnot(bc[1], bc[2]);
  state[11] = bc[1] ^ andnot(bc[2], bc[3]);
  state[12] = bc[2] ^ andnot(bc[3], bc[4]);
  state[13] = bc[3] ^ andnot(bc[4], bc[0]);
  state[14] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[15] ^ d[0];
  bc[1] = rotl64(t, keccak::ROT[5]);
  t = state[16] ^ d[1];
  bc[2] = rotl64(t, keccak::ROT[11]);
  t = state[17] ^ d[2];
  bc[3] = rotl64(t, keccak::ROT[17]);
  t = state[18] ^ d[3];
  bc[4] = rotl64(t, keccak::ROT[23]);
  t = state[19] ^ d[4];
  bc[0] = rotl64(t, keccak::ROT[4]);

  state[15] = bc[0] ^ andnot(bc[1], bc[2]);
  state[16] = bc[1] ^ andnot(bc[2], bc[3]);
  state[17] = bc[2] ^ andnot(bc[3], bc[4]);
  state[18] = bc[3] ^ andnot(bc[4], bc[0]);
  state[19] = bc[4] ^ andnot(bc[0], bc[1]);

  t = state[20] ^ d[0];
  bc[3] = rotl64(t, keccak::ROT[15]);
  t = state[21] ^ d[1];
  bc[4] = rotl64(t, keccak::ROT[21]);
  t = state[22] ^ d[2];
  bc[0] = rotl64(t, keccak::ROT[2]);
  t = state[23] ^ d[3];
  bc[1] = rotl64(t, keccak::ROT[8]);
  t = state[24] ^ d[4];
  bc[2] = rotl64(t, keccak::ROT[14]);

  state[20] = bc[0] ^ andnot(bc[1], bc[2]);
  state[21] = bc[1] ^ andnot(bc[2], bc[3]);
  state[22] = bc[2] ^ andnot(bc[3], bc[4]);
  state[23] = bc[3] ^ andnot(bc[4], bc[0]);
  state[24] = bc[4] ^ andnot(bc[0], bc[1]);
}

/**
 * 4-way parallel Keccak-f[1600] permutation, applying either 12 or 24 rounds on 4 independent states simultaneously.
 */
template<size_t num_rounds>
forceinline void
permute(std::array<vec, keccak::LANE_CNT>& state)
  requires((num_rounds == 12) || (num_rounds == keccak::MAX_NUM_ROUNDS))
{
  constexpr size_t start_at_round = keccak::MAX_NUM_ROUNDS - num_rounds;
  constexpr size_t STEP_BY = 4;

  static_assert(num_rounds % STEP_BY == 0, "Requested number of keccak-p[1600] rounds need to be a multiple of 4 for manual unrolling to work.");

  for (size_t i = start_at_round; i < keccak::MAX_NUM_ROUNDS; i += STEP_BY) {
    roundx4(state, i);
  }
}

}

#endif
