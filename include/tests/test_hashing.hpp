#pragma once
#include "sha3_224.hpp"
#include "sha3_256.hpp"
#include "sha3_384.hpp"
#include "sha3_512.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

// Test SHA3 Hash and Extendable-Output Functions.
namespace test_sha3 {

// Test that absorbing same message bytes using both incremental and one-shot
// hashing, should yield same output bytes, for SHA3-224 hasher.
inline void
test_sha3_224_absorption(const size_t mlen)
{
  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out0(sha3_224::DIGEST_LEN);
  std::vector<uint8_t> out1(sha3_224::DIGEST_LEN);

  sha3_utils::random_data(msg.data(), msg.size());

  // Oneshot Hashing
  {
    sha3_224::sha3_224 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.digest(out0.data());
  }

  // Incremental Hashing
  {
    sha3_224::sha3_224 hasher{};

    size_t off = 0;
    while (off < mlen) {
      // because we don't want to be stuck in an infinite loop if msg[off] = 0 !
      auto elen = std::min<size_t>(std::max<uint8_t>(msg[off], 1), mlen - off);

      hasher.absorb(msg.data() + off, elen);
      off += elen;
    }

    hasher.finalize();
    hasher.digest(out1.data());
  }

  assert(std::ranges::equal(out0, out1));
}

// Test that absorbing same message bytes using both incremental and one-shot
// hashing, should yield same output bytes, for SHA3-256 hasher.
inline void
test_sha3_256_absorption(const size_t mlen)
{
  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out0(sha3_256::DIGEST_LEN);
  std::vector<uint8_t> out1(sha3_256::DIGEST_LEN);

  sha3_utils::random_data(msg.data(), msg.size());

  // Oneshot Hashing
  {
    sha3_256::sha3_256 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.digest(out0.data());
  }

  // Incremental Hashing
  {
    sha3_256::sha3_256 hasher{};

    size_t off = 0;
    while (off < mlen) {
      // because we don't want to be stuck in an infinite loop if msg[off] = 0 !
      auto elen = std::min<size_t>(std::max<uint8_t>(msg[off], 1), mlen - off);

      hasher.absorb(msg.data() + off, elen);
      off += elen;
    }

    hasher.finalize();
    hasher.digest(out1.data());
  }

  assert(std::ranges::equal(out0, out1));
}

// Test that absorbing same message bytes using both incremental and one-shot
// hashing, should yield same output bytes, for SHA3-384 hasher.
inline void
test_sha3_384_absorption(const size_t mlen)
{
  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out0(sha3_384::DIGEST_LEN);
  std::vector<uint8_t> out1(sha3_384::DIGEST_LEN);

  sha3_utils::random_data(msg.data(), msg.size());

  // Oneshot Hashing
  {
    sha3_384::sha3_384 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.digest(out0.data());
  }

  // Incremental Hashing
  {
    sha3_384::sha3_384 hasher{};

    size_t off = 0;
    while (off < mlen) {
      // because we don't want to be stuck in an infinite loop if msg[off] = 0 !
      auto elen = std::min<size_t>(std::max<uint8_t>(msg[off], 1), mlen - off);

      hasher.absorb(msg.data() + off, elen);
      off += elen;
    }

    hasher.finalize();
    hasher.digest(out1.data());
  }

  assert(std::ranges::equal(out0, out1));
}

// Test that absorbing same message bytes using both incremental and one-shot
// hashing, should yield same output bytes, for SHA3-512 hasher.
inline void
test_sha3_512_absorption(const size_t mlen)
{
  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out0(sha3_512::DIGEST_LEN);
  std::vector<uint8_t> out1(sha3_512::DIGEST_LEN);

  sha3_utils::random_data(msg.data(), msg.size());

  // Oneshot Hashing
  {
    sha3_512::sha3_512 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.digest(out0.data());
  }

  // Incremental Hashing
  {
    sha3_512::sha3_512 hasher{};

    size_t off = 0;
    while (off < mlen) {
      // because we don't want to be stuck in an infinite loop if msg[off] = 0 !
      auto elen = std::min<size_t>(std::max<uint8_t>(msg[off], 1), mlen - off);

      hasher.absorb(msg.data() + off, elen);
      off += elen;
    }

    hasher.finalize();
    hasher.digest(out1.data());
  }

  assert(std::ranges::equal(out0, out1));
}

}
