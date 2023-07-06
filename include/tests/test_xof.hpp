#pragma once
#include "shake128.hpp"
#include "shake256.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

// Test SHA3 Hash and Extendable-Output Functions.
namespace test_sha3 {

// Test that absorbing same message bytes using both incremental and one-shot
// hashing, should yield same output bytes, for SHAKE128 XOF.
//
// This test collects inspiration from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950c5374aff49f04f6d51d807e68077ab25/src/tests.rs#L372-L415
inline void
test_shake128_absorb_squeeze(const size_t mlen, const size_t olen)
{
  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out0(olen);
  std::vector<uint8_t> out1(olen);

  sha3_utils::random_data(msg.data(), msg.size());

  // Oneshot absorption and squeezing
  {
    shake128::shake128 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.squeeze(out0.data(), out0.size());
  }

  // Incremental absorption and squeezing
  {
    shake128::shake128 hasher{};

    size_t off = 0;
    while (off < mlen) {
      // because we don't want to be stuck in an infinite loop if msg[off] = 0 !
      auto elen = std::min<size_t>(std::max<uint8_t>(msg[off], 1), mlen - off);

      hasher.absorb(msg.data() + off, elen);
      off += elen;
    }

    hasher.finalize();

    // squeeze message bytes in many iterations
    off = 0;
    while (off < olen) {
      hasher.squeeze(out1.data() + off, 1);

      auto elen = std::min<size_t>(out1[off], olen - (off + 1));

      off += 1;
      hasher.squeeze(out1.data() + off, elen);
      off += elen;
    }
  }

  assert(std::ranges::equal(out0, out1));
}

// Test that absorbing same message bytes using both incremental and one-shot
// hashing, should yield same output bytes, for SHAKE256 XOF.
//
// This test collects inspiration from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950c5374aff49f04f6d51d807e68077ab25/src/tests.rs#L372-L415
inline void
test_shake256_absorb_squeeze(const size_t mlen, const size_t olen)
{
  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out0(olen);
  std::vector<uint8_t> out1(olen);

  sha3_utils::random_data(msg.data(), msg.size());

  // Oneshot absorption and squeezing
  {
    shake256::shake256 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.squeeze(out0.data(), out0.size());
  }

  // Incremental absorption and squeezing
  {
    shake256::shake256 hasher{};

    size_t off = 0;
    while (off < mlen) {
      // because we don't want to be stuck in an infinite loop if msg[off] = 0 !
      auto elen = std::min<size_t>(std::max<uint8_t>(msg[off], 1), mlen - off);

      hasher.absorb(msg.data() + off, elen);
      off += elen;
    }

    hasher.finalize();

    // squeeze message bytes in many iterations
    off = 0;
    while (off < olen) {
      hasher.squeeze(out1.data() + off, 1);

      auto elen = std::min<size_t>(out1[off], olen - (off + 1));

      off += 1;
      hasher.squeeze(out1.data() + off, elen);
      off += elen;
    }
  }

  assert(std::ranges::equal(out0, out1));
}

}
