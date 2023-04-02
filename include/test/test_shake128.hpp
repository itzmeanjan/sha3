#pragma once
#include "shake128.hpp"
#include <cassert>

// Test SHA3 Extendable-Output Functions
namespace test_sha3 {

// Test that absorbing same message bytes using both incremental and one-shot
// hashing APIs, should yield same output bytes, for SHAKE128 XOF.
//
// This test collects inspiration from
// https://github.com/itzmeanjan/turboshake/blob/e1a6b950c5374aff49f04f6d51d807e68077ab25/src/tests.rs#L372-L415
inline void
test_shake128(const size_t mlen)
{
  constexpr size_t dlen = 32;

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig0 = static_cast<uint8_t*>(std::malloc(dlen));
  uint8_t* dig1 = static_cast<uint8_t*>(std::malloc(dlen));

  sha3_utils::random_data(msg, mlen);

  // Oneshot Hashing
  shake128::shake128 hasher0{};
  hasher0.hash(msg, mlen);
  hasher0.read(dig0, dlen);

  // Incremental Hashing
  shake128::shake128<true> hasher1{};

  size_t off = 0;
  while (off < mlen) {
    // because we don't want to be stuck in an infinite loop if msg[off] = 0 !
    auto elen = std::min<size_t>(std::max<uint8_t>(msg[off], 1), mlen - off);

    hasher1.absorb(msg + off, elen);
    off += elen;
  }

  hasher1.finalize();
  hasher1.read(dig1, dlen);

  bool flg = false;
  for (size_t i = 0; i < dlen; i++) {
    flg |= static_cast<bool>(dig0[i] ^ dig1[i]);
  }

  std::free(msg);
  std::free(dig0);
  std::free(dig1);

  assert(!flg);
}

}
