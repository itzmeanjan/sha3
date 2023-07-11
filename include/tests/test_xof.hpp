#pragma once
#include "shake128.hpp"
#include "shake256.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <vector>

// Test SHA3 Hash and Extendable-Output Functions.
namespace test_sha3 {

using namespace std::literals;

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

// Ensure that Shake128 Xof implementation is conformant with FIPS 202 standard,
// by using KAT file generated following
// https://gist.github.com/itzmeanjan/448f97f9c49d781a5eb3ddd6ea6e7364.
inline void
test_shake128_kat()
{
  const std::string kat_file = "./kats/shake128.kat";
  std::fstream file(kat_file);

  while (true) {
    std::string len0;

    if (!std::getline(file, len0).eof()) {
      std::string msg0;
      std::string out0;

      std::getline(file, msg0);
      std::getline(file, out0);

      auto msg1 = std::string_view(msg0);
      auto out1 = std::string_view(out0);

      auto msg2 = msg1.substr(msg1.find("="sv) + 2, msg1.size());
      auto out2 = out1.substr(out1.find("="sv) + 2, out1.size());

      auto msg = sha3_utils::from_hex(msg2);
      auto out = sha3_utils::from_hex(out2);

      std::vector<uint8_t> squeezed(out.size());

      shake128::shake128 hasher;

      hasher.absorb(msg.data(), msg.size());
      hasher.finalize();
      hasher.squeeze(squeezed.data(), squeezed.size());

      assert(std::ranges::equal(squeezed, out));

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
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

// Ensure that Shake256 Xof implementation is conformant with FIPS 202 standard,
// by using KAT file generated following
// https://gist.github.com/itzmeanjan/448f97f9c49d781a5eb3ddd6ea6e7364.
inline void
test_shake256_kat()
{
  const std::string kat_file = "./kats/shake256.kat";
  std::fstream file(kat_file);

  while (true) {
    std::string len0;

    if (!std::getline(file, len0).eof()) {
      std::string msg0;
      std::string out0;

      std::getline(file, msg0);
      std::getline(file, out0);

      auto msg1 = std::string_view(msg0);
      auto out1 = std::string_view(out0);

      auto msg2 = msg1.substr(msg1.find("="sv) + 2, msg1.size());
      auto out2 = out1.substr(out1.find("="sv) + 2, out1.size());

      auto msg = sha3_utils::from_hex(msg2);
      auto out = sha3_utils::from_hex(out2);

      std::vector<uint8_t> squeezed(out.size());

      shake256::shake256 hasher;

      hasher.absorb(msg.data(), msg.size());
      hasher.finalize();
      hasher.squeeze(squeezed.data(), squeezed.size());

      assert(std::ranges::equal(squeezed, out));

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}

}
