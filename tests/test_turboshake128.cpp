#include "sha3/turboshake128.hpp"
#include "test_conf.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <numeric>
#include <vector>

// Eval TurboSHAKE128 XOF on statically defined input message during compilation-time.
constexpr std::array<uint8_t, 256>
eval_turboshake128()
{
  // Statically defined input.
  std::array<uint8_t, 256> data{};
  std::iota(data.begin(), data.end(), 0);

  // To be computed output.
  std::array<uint8_t, 256> md{};

  turboshake128::turboshake128_t hasher;
  hasher.absorb(data);
  hasher.finalize();
  hasher.squeeze(md);

  return md;
}

// Ensure that TurboSHAKE128 XOF implementation is compile-time evaluable.
TEST(Sha3XOF, CompileTimeEvalTurboSHAKE128)
{
  // Input  =
  // 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff
  // Output =
  // 5d6efc5eb4b62f82fe835511ad0ac01f55d47366f99d1476d99069c2f43997989263aacacabbe3c80b376c8403424b714b20a5d4a389aec7c72fd0efcf25ba3c9aac118bc557bd1d1f691db24eea33b6a569dfc2c13b0d4f4ed7b5860f5af9b1a014249dec67df97538904d48926a323d462908be20d56fbae9d34f7912e35d14406b9bfabb8f0bda7b7f2e54f8f6d16d34fed442129a281e2f5fa80d22b2ce90e5229bc31e5138cc7075e8b4b912d5d6147e02a37777a93feb819dd9d07953f00c29bfb38f719774d7f0130c6d984d6728c296ba094057d64b27abfd32e50a1982ad8cc0ebe220e458ac29bf127d3e827aea4f33efe253efc315a2caacd5cff

  constexpr auto md = eval_turboshake128();
  static_assert(md == std::array<uint8_t, 256>{ 93,  110, 252, 94,  180, 182, 47,  130, 254, 131, 85,  17,  173, 10,  192, 31,  85,  212, 115, 102, 249, 157,
                                                20,  118, 217, 144, 105, 194, 244, 57,  151, 152, 146, 99,  170, 202, 202, 187, 227, 200, 11,  55,  108, 132,
                                                3,   66,  75,  113, 75,  32,  165, 212, 163, 137, 174, 199, 199, 47,  208, 239, 207, 37,  186, 60,  154, 172,
                                                17,  139, 197, 87,  189, 29,  31,  105, 29,  178, 78,  234, 51,  182, 165, 105, 223, 194, 193, 59,  13,  79,
                                                78,  215, 181, 134, 15,  90,  249, 177, 160, 20,  36,  157, 236, 103, 223, 151, 83,  137, 4,   212, 137, 38,
                                                163, 35,  212, 98,  144, 139, 226, 13,  86,  251, 174, 157, 52,  247, 145, 46,  53,  209, 68,  6,   185, 191,
                                                171, 184, 240, 189, 167, 183, 242, 229, 79,  143, 109, 22,  211, 79,  237, 68,  33,  41,  162, 129, 226, 245,
                                                250, 128, 210, 43,  44,  233, 14,  82,  41,  188, 49,  229, 19,  140, 199, 7,   94,  139, 75,  145, 45,  93,
                                                97,  71,  224, 42,  55,  119, 122, 147, 254, 184, 25,  221, 157, 7,   149, 63,  0,   194, 155, 251, 56,  247,
                                                25,  119, 77,  127, 1,   48,  198, 217, 132, 214, 114, 140, 41,  107, 160, 148, 5,   125, 100, 178, 122, 191,
                                                211, 46,  80,  161, 152, 42,  216, 204, 14,  190, 34,  14,  69,  138, 194, 155, 241, 39,  211, 232, 39,  174,
                                                164, 243, 62,  254, 37,  62,  252, 49,  90,  44,  170, 205, 92,  255 },
                "Must be able to compute TurboSHAKE128 XOF during compile-time !");
}

// Test that absorbing same message bytes using both incremental and one-shot hashing, should yield same output bytes, for TurboSHAKE128 XOF.
//
// This test collects inspiration from https://github.com/itzmeanjan/turboshake/blob/e1a6b950c5374aff49f04f6d51d807e68077ab25/src/tests.rs#L372-L415.
TEST(Sha3XOF, Shake128IncrementalAbsorptionAndSqueezing)
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

      turboshake128::turboshake128_t hasher;

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
