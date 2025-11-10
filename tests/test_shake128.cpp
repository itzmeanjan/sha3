#include "sha3/shake128.hpp"
#include "test_conf.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <numeric>
#include <vector>

// Eval SHAKE128 XOF on statically defined input message during compilation-time.
constexpr std::array<uint8_t, 256>
eval_shake128()
{
  // Statically defined input.
  std::array<uint8_t, 256> data{};
  std::iota(data.begin(), data.end(), 0);

  // To be computed output.
  std::array<uint8_t, 256> md{};

  shake128::shake128_t hasher;
  hasher.absorb(data);
  hasher.finalize();
  hasher.squeeze(md);

  return md;
}

// Ensure that SHAKE128 XOF implementation is compile-time evaluable.
TEST(Sha3XOF, CompileTimeEvalSHAKE128)
{
  // Input  =
  // 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff
  // Output =
  // 9d32ba2aa8f40b0cdf108376d77abfd5c97f149e6ba0c9efe3499c7b3c039b0afac641a978ef435b3d83b9712da8ea826bb38078899b3efaec77d44a0460b220225d1b0b11a1d1c5cb0acb5aca92c6fb95f64a992eee6b6de24434aae4fba9d496bd8bd90624391f79c0db7d20eef1ddbfe8d771b4123e97ad7664012188590eb0b43c7073b7a9ab8af27229bc7246296ac0e172fca7314b8f100dc247d51c949bc4977c345d7c1d5536c96825f3650b7f80b5981b252ce4a858e54f9833cceaf38c12a91a8c6b341e197eb894553ca6f100f731f00f43b854098aace7a4e0ed8252782523f561dd994c291229eaf70185c98ed0026be1bd39c17dd817424009

  constexpr auto md = eval_shake128();
  static_assert(md == std::array<uint8_t, 256>{ 157, 50,  186, 42,  168, 244, 11,  12,  223, 16,  131, 118, 215, 122, 191, 213, 201, 127, 20,  158, 107, 160,
                                                201, 239, 227, 73,  156, 123, 60,  3,   155, 10,  250, 198, 65,  169, 120, 239, 67,  91,  61,  131, 185, 113,
                                                45,  168, 234, 130, 107, 179, 128, 120, 137, 155, 62,  250, 236, 119, 212, 74,  4,   96,  178, 32,  34,  93,
                                                27,  11,  17,  161, 209, 197, 203, 10,  203, 90,  202, 146, 198, 251, 149, 246, 74,  153, 46,  238, 107, 109,
                                                226, 68,  52,  170, 228, 251, 169, 212, 150, 189, 139, 217, 6,   36,  57,  31,  121, 192, 219, 125, 32,  238,
                                                241, 221, 191, 232, 215, 113, 180, 18,  62,  151, 173, 118, 100, 1,   33,  136, 89,  14,  176, 180, 60,  112,
                                                115, 183, 169, 171, 138, 242, 114, 41,  188, 114, 70,  41,  106, 192, 225, 114, 252, 167, 49,  75,  143, 16,
                                                13,  194, 71,  213, 28,  148, 155, 196, 151, 124, 52,  93,  124, 29,  85,  54,  201, 104, 37,  243, 101, 11,
                                                127, 128, 181, 152, 27,  37,  44,  228, 168, 88,  229, 79,  152, 51,  204, 234, 243, 140, 18,  169, 26,  140,
                                                107, 52,  30,  25,  126, 184, 148, 85,  60,  166, 241, 0,   247, 49,  240, 15,  67,  184, 84,  9,   138, 172,
                                                231, 164, 224, 237, 130, 82,  120, 37,  35,  245, 97,  221, 153, 76,  41,  18,  41,  234, 247, 1,   133, 201,
                                                142, 208, 2,   107, 225, 189, 57,  193, 125, 216, 23,  66,  64,  9 },
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
