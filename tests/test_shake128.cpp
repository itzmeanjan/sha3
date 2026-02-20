#include "sha3/shake128.hpp"
#include "test_conf.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

namespace {

// Eval SHAKE128 XOF on given input message during compilation-time.
template<size_t OLEN>
constexpr std::array<uint8_t, OLEN>
eval_shake128(std::span<const uint8_t> data)
{
  std::array<uint8_t, OLEN> out{};

  shake128::shake128_t hasher;
  hasher.absorb(data);
  hasher.finalize();
  hasher.squeeze(out);

  return out;
}

}

// Ensure that SHAKE128 XOF implementation is compile-time evaluable.
TEST(Sha3XOF, CompileTimeEvalSHAKE128)
{
  constexpr auto input = sha3_test_utils::from_hex<256>(
    "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c"
    "4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f90919293949596979899"
    "9a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6"
    "e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
  constexpr auto digest = eval_shake128<256>(input);

  static_assert(digest == sha3_test_utils::from_hex<256>(
                            "9d32ba2aa8f40b0cdf108376d77abfd5c97f149e6ba0c9efe3499c7b3c039b0afac641a978ef435b3d83b9712da8ea826bb38078899b3efaec77d44a0460b22022"
                            "5d1b0b11a1d1c5cb0acb5aca92c6fb95f64a992eee6b6de24434aae4fba9d496bd8bd90624391f79c0db7d20eef1ddbfe8d771b4123e97ad7664012188590eb0b4"
                            "3c7073b7a9ab8af27229bc7246296ac0e172fca7314b8f100dc247d51c949bc4977c345d7c1d5536c96825f3650b7f80b5981b252ce4a858e54f9833cceaf38c12"
                            "a91a8c6b341e197eb894553ca6f100f731f00f43b854098aace7a4e0ed8252782523f561dd994c291229eaf70185c98ed0026be1bd39c17dd817424009"),
                "Must be able to compute Shake128 Xof during compile-time !");
}

/**
 * Test that absorbing same message bytes using both incremental and one-shot hashing, should yield same output bytes, for SHAKE128 XOF.
 *
 * This test collects inspiration from https://github.com/itzmeanjan/turboshake/blob/e1a6b950c5374aff49f04f6d51d807e68077ab25/src/tests.rs#L372-L415
 */
TEST(Sha3XOF, SHAKE128IncrementalAbsorptionAndSqueezing)
{
  for (size_t mlen = MIN_MSG_LEN; mlen < MAX_MSG_LEN; mlen++) {
    for (size_t olen = MIN_OUT_LEN; olen < MAX_OUT_LEN; olen++) {
      std::vector<uint8_t> msg(mlen);
      std::vector<uint8_t> oneshot_out(olen);
      std::vector<uint8_t> multishot_out(olen);

      auto msg_span = std::span(msg);
      auto oneshot_out_span = std::span(oneshot_out);
      auto multishot_out_span = std::span(multishot_out);

      sha3_test_utils::random_data(msg_span);

      shake128::shake128_t hasher;

      // Oneshot absorption and squeezing
      hasher.absorb(msg_span);
      hasher.finalize();
      hasher.squeeze(oneshot_out_span);

      hasher.reset();

      // Incremental absorption and squeezing
      size_t off = 0;
      while (off < mlen) {
        // because we don't want to be stuck in an infinite loop if msg[off] = 0
        auto tmp = std::max<uint8_t>(msg[off], 1);
        auto elen = std::min<size_t>(tmp, mlen - off);

        hasher.absorb(msg_span.subspan(off, elen));
        off += elen;
      }

      hasher.finalize();

      // squeeze message bytes in many iterations
      off = 0;
      while (off < olen) {
        hasher.squeeze(multishot_out_span.subspan(off, 1));

        auto elen = std::min<size_t>(multishot_out[off], olen - (off + 1));

        off += 1;
        hasher.squeeze(multishot_out_span.subspan(off, elen));
        off += elen;
      }

      EXPECT_EQ(oneshot_out, multishot_out);
    }
  }
}

/**
 * Ensure that SHAKE128 XOF implementation is conformant with FIPS 202 standard, by using KAT file generated following
 * https://gist.github.com/itzmeanjan/448f97f9c49d781a5eb3ddd6ea6e7364.
 */
TEST(Sha3XOF, SHAKE128KnownAnswerTests)
{
  using namespace std::literals;

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

      auto msg = sha3_test_utils::parse_dynamic_sized_hex_string(msg2);
      auto expected_out = sha3_test_utils::parse_dynamic_sized_hex_string(out2);

      std::vector<uint8_t> computed_out(expected_out.size());
      shake128::shake128_t hasher;

      hasher.absorb(msg);
      hasher.finalize();
      hasher.squeeze(computed_out);

      EXPECT_EQ(computed_out, expected_out);

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}
