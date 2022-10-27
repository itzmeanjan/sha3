#pragma once
#include "shake128.hpp"
#include "utils.hpp"
#include <cassert>

// Test SHA3 Extendable-Output Functions
namespace test_sha3 {

// Test that hashing same message bytes using both incremental and one-shot
// hashing APIs, should yield same output bytes.
template<const size_t m0len, const size_t m1len>
static void
test_shake128()
{
  constexpr size_t mlen = m0len + m1len;
  constexpr size_t dlen = 32;

  uint8_t* msg0 = static_cast<uint8_t*>(std::malloc(m0len));
  uint8_t* msg1 = static_cast<uint8_t*>(std::malloc(m1len));
  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig0 = static_cast<uint8_t*>(std::malloc(dlen));
  uint8_t* dig1 = static_cast<uint8_t*>(std::malloc(dlen));

  random_data<uint8_t>(msg0, m0len);
  random_data<uint8_t>(msg1, m1len);
  std::memcpy(msg + 0, msg0, m0len);
  std::memcpy(msg + m0len, msg1, m1len);

  // Oneshot Hashing
  shake128::shake128 hasher0{};
  hasher0.hash(msg, mlen);
  hasher0.read(dig0, dlen);

  // Incremental Hashing
  shake128::shake128<true> hasher1{};
  hasher1.absorb(msg0, m0len);
  hasher1.absorb(msg1, m1len);
  hasher1.finalize();
  hasher1.read(dig1, dlen);

  bool flg = false;
  for (size_t i = 0; i < dlen; i++) {
    flg |= static_cast<bool>(dig0[i] ^ dig1[i]);
  }

  std::free(msg0);
  std::free(msg1);
  std::free(msg);
  std::free(dig0);
  std::free(dig1);

  assert(!flg);
}

}
