#pragma once
#include "sha3/internals/force_inline.hpp"
#include "sha3/internals/keccak.hpp"
#include "sha3/internals/utils.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <span>

// Keccak family of sponge functions
namespace sponge {

static constexpr size_t KECCAK_WORD_BYTE_LEN = keccak::LANE_BW / std::numeric_limits<uint8_t>::digits;

// Compile-time check to ensure that domain separator can only be 2/ 4 -bits wide.
//
// When used in context of extendable output functions ( SHAKE{128, 256} ) domain separator bits are 4 -bit wide.
//
// See section 6.{1, 2} of SHA3 specification https://dx.doi.org/10.6028/NIST.FIPS.202
constexpr bool
check_domain_separator(const size_t dom_sep_bit_len)
{
  return (dom_sep_bit_len == 2u) | (dom_sep_bit_len == 4u);
}

// Pad10*1 - generates a padding, while also considering domain separator bits ( which are either 2 or 4 -bit wide ),
// such that when both domain separator bits and 10*1 padding is appended ( in order ) to actual message, total byte
// length of message consumed into keccak-p[1600, 24] permutation becomes multiple of `rate` -bits. The only parameter
// `offset` denotes how many bytes are already mixed with rate portion of permutation state meaning `offset` must ∈ [0,
// `rate/ 8`). This routine returns a byte array of length `rate/ 8` -bytes which can safely be mixed into permutation
// state duing sponge finalization phase.
//
// This function implementation collects motivation from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950/src/sponge.rs#L70-L72
template<uint8_t domain_separator, size_t ds_bits, size_t rate>
static forceinline constexpr std::array<uint8_t, rate / std::numeric_limits<uint8_t>::digits>
pad10x1(const size_t offset)
  requires(check_domain_separator(ds_bits))
{
  std::array<uint8_t, rate / std::numeric_limits<uint8_t>::digits> res{};

  constexpr uint8_t mask = (1u << ds_bits) - 1u;
  constexpr uint8_t pad_byte = (1u << ds_bits) | (domain_separator & mask);

  res[offset] = pad_byte;
  res[res.size() - 1] ^= 0x80u;

  return res;
}

// Given `mlen` (>=0) -bytes message, this routine consumes it into Keccak[c] permutation state s.t. `offset` ( second
// parameter ) denotes how many bytes are already consumed into rate portion of the state.
//
// - `num_bits_in_rate` portion of sponge will have bitwidth of 1600 - c.
// - `offset` must ∈ [0, `num_bytes_in_rate`).
//
// This function implementation collects inspiration from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950/src/sponge.rs#L4-L56
template<size_t num_bits_in_rate>
static forceinline constexpr void
absorb(uint64_t state[keccak::LANE_CNT], size_t& offset, std::span<const uint8_t> msg)
{
  constexpr size_t num_bytes_in_rate = num_bits_in_rate / std::numeric_limits<uint8_t>::digits;

  std::array<uint8_t, num_bytes_in_rate> block{};
  auto block_span = std::span(block);

  size_t msg_offset = 0;
  while (msg_offset < msg.size()) {
    const size_t remaining_num_bytes = msg.size() - msg_offset;
    const size_t absorbable_num_bytes = std::min(remaining_num_bytes, num_bytes_in_rate - offset);
    const size_t effective_block_byte_len = offset + absorbable_num_bytes;
    const size_t padded_effective_block_byte_len =
      (effective_block_byte_len + (KECCAK_WORD_BYTE_LEN - 1)) & (-KECCAK_WORD_BYTE_LEN);
    const size_t padded_effective_block_begins_at = offset & (-KECCAK_WORD_BYTE_LEN);

    std::fill_n(block_span.subspan(padded_effective_block_begins_at).begin(),
                padded_effective_block_byte_len - padded_effective_block_begins_at,
                0x00);
    std::copy_n(msg.subspan(msg_offset).begin(), absorbable_num_bytes, block_span.subspan(offset).begin());

    size_t state_word_index = padded_effective_block_begins_at / KECCAK_WORD_BYTE_LEN;
    for (size_t i = padded_effective_block_begins_at; i < padded_effective_block_byte_len; i += KECCAK_WORD_BYTE_LEN) {
      auto msg_chunk = std::span<const uint8_t, KECCAK_WORD_BYTE_LEN>(block_span.subspan(i, KECCAK_WORD_BYTE_LEN));
      auto msg_word = sha3_utils::le_bytes_to_u64(msg_chunk);

      state[state_word_index] ^= msg_word;
      state_word_index++;
    }

    offset += absorbable_num_bytes;
    msg_offset += absorbable_num_bytes;

    if (offset == num_bytes_in_rate) {
      keccak::permute(state);
      offset = 0;
    }
  }
}

// Given that N message bytes are already consumed into Keccak[c] permutation state, this routine finalizes sponge state
// and makes it ready for squeezing, by appending ( along with domain separation bits ) 10*1 padding bits to input
// message s.t. total absorbed message byte length becomes multiple of `rate/ 8` -bytes.
//
// - `rate` portion of sponge will have bitwidth of 1600 - c.
// - `offset` must ∈ [0, `rbytes`)
//
// This function implementation collects some motivation from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950/src/sponge.rs#L58-L81
template<uint8_t domain_separator, size_t ds_bits, size_t rate>
static forceinline constexpr void
finalize(uint64_t state[keccak::LANE_CNT], size_t& offset)
  requires(check_domain_separator(ds_bits))
{
  constexpr size_t rbytes = rate >> 3u;   // # -of bytes
  constexpr size_t rwords = rbytes >> 3u; // # -of 64 -bit words

  const auto padb = pad10x1<domain_separator, ds_bits, rate>(offset);
  std::array<uint64_t, rwords> padw{};

  auto padb_span = std::span(padb);
  auto padw_span = std::span(padw);

  sha3_utils::le_bytes_to_u64_words<rate>(padb_span, padw_span);

  for (size_t j = 0; j < rwords; j++) {
    state[j] ^= padw_span[j];
  }

  keccak::permute(state);
  offset = 0;
}

// Given that Keccak[c] permutation state is finalized, this routine can be invoked for squeezing `olen` -bytes out of
// rate portion of the state.
//
// - `rate` portion of sponge will have bitwidth of 1600 - c.
// - `squeezable` denotes how many bytes can be squeezed without permutating the
// sponge state.
// - When `squeezable` becomes 0, state needs to be permutated again, after
// which `rbytes` can again be squeezed from rate portion of the state.
//
// This function implementation collects motivation from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950/src/sponge.rs#L83-L118
template<size_t rate>
static forceinline constexpr void
squeeze(uint64_t state[keccak::LANE_CNT], size_t& squeezable, std::span<uint8_t> out)
{
  constexpr size_t rbytes = rate >> 3u;   // # -of bytes
  constexpr size_t rwords = rbytes >> 3u; // # -of 64 -bit words

  std::array<uint8_t, rbytes> blk_bytes{};
  auto blk_bytes_span = std::span(blk_bytes);

  auto swords = std::span{ state, keccak::LANE_CNT };
  auto swords_span = swords.template subspan<0, rwords>();

  const size_t olen = out.size();
  size_t off = 0;

  while (off < olen) {
    const size_t read = std::min(squeezable, olen - off);
    const size_t soff = rbytes - squeezable;

    sha3_utils::u64_words_to_le_bytes<rate>(swords_span, blk_bytes_span);

    auto blk_span = blk_bytes_span.subspan(soff, read);
    auto out_span = out.subspan(off, read);

    std::copy(blk_span.begin(), blk_span.end(), out_span.begin());

    squeezable -= read;
    off += read;

    if (squeezable == 0) {
      keccak::permute(state);
      squeezable = rbytes;
    }
  }
}

}
