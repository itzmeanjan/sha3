#pragma once

#if defined(__AVX2__)

#include "sha3/internals/force_inline.hpp"
#include "sha3/internals/keccak.hpp"
#include "sha3/internals/keccak_x4.hpp"
#include "sha3/internals/simd/avx2.hpp"
#include "sha3/internals/utils.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>

// 4-way parallel Keccak sponge functions using AVX2.
// All 4 instances must absorb same-length messages and squeeze same-length outputs.
namespace sponge_x4 {

using vec = sha3_simd::avx2::vec;

static constexpr size_t KECCAK_WORD_BYTE_LEN = keccak::LANE_BW / std::numeric_limits<uint8_t>::digits;

/**
 * Absorb 4 same-length messages into 4 parallel Keccak[c] sponge states.
 * All 4 messages must have the same length.
 */
template<size_t num_bits_in_rate, size_t num_rounds>
static forceinline void
absorb_x4(std::array<vec, keccak::LANE_CNT>& state,
          size_t& offset,
          std::span<const uint8_t> msg0,
          std::span<const uint8_t> msg1,
          std::span<const uint8_t> msg2,
          std::span<const uint8_t> msg3)
{
  constexpr size_t num_bytes_in_rate = num_bits_in_rate / std::numeric_limits<uint8_t>::digits;

  std::array<uint8_t, num_bytes_in_rate> blk0{};
  std::array<uint8_t, num_bytes_in_rate> blk1{};
  std::array<uint8_t, num_bytes_in_rate> blk2{};
  std::array<uint8_t, num_bytes_in_rate> blk3{};

  auto blk0_span = std::span(blk0);
  auto blk1_span = std::span(blk1);
  auto blk2_span = std::span(blk2);
  auto blk3_span = std::span(blk3);

  const size_t mlen = msg0.size();
  size_t msg_offset = 0;

  while (msg_offset < mlen) {
    const size_t remaining_num_bytes = mlen - msg_offset;
    const size_t absorbable_num_bytes = std::min(remaining_num_bytes, num_bytes_in_rate - offset);
    const size_t effective_block_byte_len = offset + absorbable_num_bytes;
    const size_t padded_effective_block_byte_len = (effective_block_byte_len + (KECCAK_WORD_BYTE_LEN - 1)) & (-KECCAK_WORD_BYTE_LEN);
    const size_t padded_effective_block_begins_at = offset & (-KECCAK_WORD_BYTE_LEN);
    const size_t fill_len = padded_effective_block_byte_len - padded_effective_block_begins_at;

    std::fill_n(blk0_span.subspan(padded_effective_block_begins_at).begin(), fill_len, 0);
    std::fill_n(blk1_span.subspan(padded_effective_block_begins_at).begin(), fill_len, 0);
    std::fill_n(blk2_span.subspan(padded_effective_block_begins_at).begin(), fill_len, 0);
    std::fill_n(blk3_span.subspan(padded_effective_block_begins_at).begin(), fill_len, 0);

    std::copy_n(msg0.subspan(msg_offset).begin(), absorbable_num_bytes, blk0_span.subspan(offset).begin());
    std::copy_n(msg1.subspan(msg_offset).begin(), absorbable_num_bytes, blk1_span.subspan(offset).begin());
    std::copy_n(msg2.subspan(msg_offset).begin(), absorbable_num_bytes, blk2_span.subspan(offset).begin());
    std::copy_n(msg3.subspan(msg_offset).begin(), absorbable_num_bytes, blk3_span.subspan(offset).begin());

    size_t state_word_index = padded_effective_block_begins_at / KECCAK_WORD_BYTE_LEN;
    for (size_t i = padded_effective_block_begins_at; i < padded_effective_block_byte_len; i += KECCAK_WORD_BYTE_LEN) {
      const auto w0 = sha3_utils::le_bytes_to_u64(std::span<const uint8_t, KECCAK_WORD_BYTE_LEN>(blk0_span.subspan(i, KECCAK_WORD_BYTE_LEN)));
      const auto w1 = sha3_utils::le_bytes_to_u64(std::span<const uint8_t, KECCAK_WORD_BYTE_LEN>(blk1_span.subspan(i, KECCAK_WORD_BYTE_LEN)));
      const auto w2 = sha3_utils::le_bytes_to_u64(std::span<const uint8_t, KECCAK_WORD_BYTE_LEN>(blk2_span.subspan(i, KECCAK_WORD_BYTE_LEN)));
      const auto w3 = sha3_utils::le_bytes_to_u64(std::span<const uint8_t, KECCAK_WORD_BYTE_LEN>(blk3_span.subspan(i, KECCAK_WORD_BYTE_LEN)));

      state[state_word_index] ^= vec::set(w0, w1, w2, w3);
      state_word_index++;
    }

    offset += absorbable_num_bytes;
    msg_offset += absorbable_num_bytes;

    if (offset == num_bytes_in_rate) [[unlikely]] {
      keccak_x4::permute<num_rounds>(state);
      offset = 0;
    }
  }
}

/**
 * Finalize 4 parallel Keccak[c] sponge states with domain separator and 10*1 padding.
 * All 4 instances share the same offset (since they absorbed same-length messages).
 */
template<uint8_t domain_separator, size_t ds_bit_len, size_t num_bits_in_rate, size_t num_rounds>
static forceinline void
finalize_x4(std::array<vec, keccak::LANE_CNT>& state, size_t& offset)
  requires(ds_bit_len <= 6U)
{
  constexpr size_t num_bytes_in_rate = num_bits_in_rate / std::numeric_limits<uint8_t>::digits;
  constexpr size_t num_words_in_rate = num_bytes_in_rate / KECCAK_WORD_BYTE_LEN;

  const auto state_word_index = offset / KECCAK_WORD_BYTE_LEN;
  const auto byte_index_in_state_word = offset % KECCAK_WORD_BYTE_LEN;
  const auto shl_bit_offset = byte_index_in_state_word * std::numeric_limits<uint8_t>::digits;

  constexpr uint8_t mask = (1U << ds_bit_len) - 1U;
  constexpr uint8_t pad_byte = (1U << ds_bit_len) | (domain_separator & mask);

  state[state_word_index] ^= vec::broadcast(static_cast<uint64_t>(pad_byte) << shl_bit_offset);
  state[num_words_in_rate - 1] ^= vec::broadcast(UINT64_C(0x80) << 56);

  keccak_x4::permute<num_rounds>(state);
  offset = 0;
}

/**
 * Squeeze from 4 parallel Keccak[c] sponge states into 4 same-length output buffers.
 * All 4 output buffers must have the same length.
 */
template<size_t num_bits_in_rate, size_t num_rounds>
static forceinline void
squeeze_x4(std::array<vec, keccak::LANE_CNT>& state,
           size_t& squeezable,
           std::span<uint8_t> out0,
           std::span<uint8_t> out1,
           std::span<uint8_t> out2,
           std::span<uint8_t> out3)
{
  constexpr size_t num_bytes_in_rate = num_bits_in_rate / std::numeric_limits<uint8_t>::digits;

  std::array<uint8_t, num_bytes_in_rate> blk0{};
  std::array<uint8_t, num_bytes_in_rate> blk1{};
  std::array<uint8_t, num_bytes_in_rate> blk2{};
  std::array<uint8_t, num_bytes_in_rate> blk3{};

  auto blk0_span = std::span(blk0);
  auto blk1_span = std::span(blk1);
  auto blk2_span = std::span(blk2);
  auto blk3_span = std::span(blk3);

  const size_t olen = out0.size();
  size_t out_offset = 0;

  while (out_offset < olen) {
    const size_t state_byte_offset = num_bytes_in_rate - squeezable;
    const size_t remaining_num_bytes = olen - out_offset;
    const size_t squeezable_num_bytes = std::min(remaining_num_bytes, squeezable);
    const size_t effective_block_byte_len = state_byte_offset + squeezable_num_bytes;
    const size_t padded_effective_block_byte_len = (effective_block_byte_len + (KECCAK_WORD_BYTE_LEN - 1)) & (-KECCAK_WORD_BYTE_LEN);
    const size_t padded_effective_block_begins_at = state_byte_offset & (-KECCAK_WORD_BYTE_LEN);

    size_t state_word_index = padded_effective_block_begins_at / KECCAK_WORD_BYTE_LEN;
    for (size_t i = padded_effective_block_begins_at; i < padded_effective_block_byte_len; i += KECCAK_WORD_BYTE_LEN) {
      sha3_utils::u64_to_le_bytes(state[state_word_index].extract<0>(), std::span<uint8_t, KECCAK_WORD_BYTE_LEN>(blk0_span.subspan(i, KECCAK_WORD_BYTE_LEN)));
      sha3_utils::u64_to_le_bytes(state[state_word_index].extract<1>(), std::span<uint8_t, KECCAK_WORD_BYTE_LEN>(blk1_span.subspan(i, KECCAK_WORD_BYTE_LEN)));
      sha3_utils::u64_to_le_bytes(state[state_word_index].extract<2>(), std::span<uint8_t, KECCAK_WORD_BYTE_LEN>(blk2_span.subspan(i, KECCAK_WORD_BYTE_LEN)));
      sha3_utils::u64_to_le_bytes(state[state_word_index].extract<3>(), std::span<uint8_t, KECCAK_WORD_BYTE_LEN>(blk3_span.subspan(i, KECCAK_WORD_BYTE_LEN)));

      state_word_index++;
    }

    std::copy_n(blk0_span.subspan(state_byte_offset).begin(), squeezable_num_bytes, out0.subspan(out_offset).begin());
    std::copy_n(blk1_span.subspan(state_byte_offset).begin(), squeezable_num_bytes, out1.subspan(out_offset).begin());
    std::copy_n(blk2_span.subspan(state_byte_offset).begin(), squeezable_num_bytes, out2.subspan(out_offset).begin());
    std::copy_n(blk3_span.subspan(state_byte_offset).begin(), squeezable_num_bytes, out3.subspan(out_offset).begin());

    squeezable -= squeezable_num_bytes;
    out_offset += squeezable_num_bytes;

    if (squeezable == 0) [[unlikely]] {
      keccak_x4::permute<num_rounds>(state);
      squeezable = num_bytes_in_rate;
    }
  }
}

}

#endif
