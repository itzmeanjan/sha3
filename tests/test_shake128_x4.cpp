#if defined(__AVX2__)

#include "sha3/shake128.hpp"
#include "sha3/shake128_x4.hpp"
#include "test_conf.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

// Verify each lane of SHAKE128 x4 matches the scalar SHAKE128 output.
TEST(Sha3XOF, SHAKE128x4ParityWithScalar)
{
  for (size_t mlen = MIN_MSG_LEN; mlen < MAX_MSG_LEN; mlen++) {
    for (size_t olen = MIN_OUT_LEN; olen < MAX_OUT_LEN; olen++) {
      std::vector<uint8_t> msg0(mlen);
      std::vector<uint8_t> msg1(mlen);
      std::vector<uint8_t> msg2(mlen);
      std::vector<uint8_t> msg3(mlen);

      sha3_test_utils::random_data<uint8_t>(msg0);
      sha3_test_utils::random_data<uint8_t>(msg1);
      sha3_test_utils::random_data<uint8_t>(msg2);
      sha3_test_utils::random_data<uint8_t>(msg3);

      // x4 path
      std::vector<uint8_t> out0_x4(olen);
      std::vector<uint8_t> out1_x4(olen);
      std::vector<uint8_t> out2_x4(olen);
      std::vector<uint8_t> out3_x4(olen);

      {
        shake128_x4::shake128_x4_t h;

        h.absorb(msg0, msg1, msg2, msg3);
        h.finalize();
        h.squeeze(out0_x4, out1_x4, out2_x4, out3_x4);
      }

      // scalar path
      auto shake128x1 = [&](std::span<const uint8_t> msg, std::span<uint8_t> out) {
        shake128::shake128_t h;

        h.absorb(msg);
        h.finalize();
        h.squeeze(out);
      };

      std::vector<uint8_t> out0_sc(olen);
      std::vector<uint8_t> out1_sc(olen);
      std::vector<uint8_t> out2_sc(olen);
      std::vector<uint8_t> out3_sc(olen);

      shake128x1(msg0, out0_sc);
      shake128x1(msg1, out1_sc);
      shake128x1(msg2, out2_sc);
      shake128x1(msg3, out3_sc);

      EXPECT_EQ(out0_x4, out0_sc);
      EXPECT_EQ(out1_x4, out1_sc);
      EXPECT_EQ(out2_x4, out2_sc);
      EXPECT_EQ(out3_x4, out3_sc);
    }
  }
}

/**
 * Test that absorbing same message bytes using both incremental and one-shot hashing,
 * should yield same output bytes, for SHAKE128 x4 XOF.
 *
 * Uses msg0's content to determine chunk sizes (same across all 4 lanes).
 */
TEST(Sha3XOF, SHAKE128x4IncrementalAbsorptionAndSqueezing)
{
  for (size_t mlen = MIN_MSG_LEN; mlen < MAX_MSG_LEN; mlen++) {
    for (size_t olen = MIN_OUT_LEN; olen < MAX_OUT_LEN; olen++) {
      std::vector<uint8_t> msg0(mlen);
      std::vector<uint8_t> msg1(mlen);
      std::vector<uint8_t> msg2(mlen);
      std::vector<uint8_t> msg3(mlen);

      sha3_test_utils::random_data<uint8_t>(msg0);
      sha3_test_utils::random_data<uint8_t>(msg1);
      sha3_test_utils::random_data<uint8_t>(msg2);
      sha3_test_utils::random_data<uint8_t>(msg3);

      auto msg0_span = std::span<const uint8_t>(msg0);
      auto msg1_span = std::span<const uint8_t>(msg1);
      auto msg2_span = std::span<const uint8_t>(msg2);
      auto msg3_span = std::span<const uint8_t>(msg3);

      std::vector<uint8_t> oneshot_out0(olen);
      std::vector<uint8_t> oneshot_out1(olen);
      std::vector<uint8_t> oneshot_out2(olen);
      std::vector<uint8_t> oneshot_out3(olen);

      std::vector<uint8_t> multishot_out0(olen);
      std::vector<uint8_t> multishot_out1(olen);
      std::vector<uint8_t> multishot_out2(olen);
      std::vector<uint8_t> multishot_out3(olen);

      shake128_x4::shake128_x4_t hasher;

      // Oneshot absorption and squeezing
      hasher.absorb(msg0_span, msg1_span, msg2_span, msg3_span);
      hasher.finalize();
      hasher.squeeze(oneshot_out0, oneshot_out1, oneshot_out2, oneshot_out3);

      hasher.reset();

      // Incremental absorption (chunk sizes driven by msg0 content)
      size_t off = 0;
      while (off < mlen) {
        auto tmp = std::max<uint8_t>(msg0[off], 1);
        auto elen = std::min<size_t>(tmp, mlen - off);

        hasher.absorb(msg0_span.subspan(off, elen), msg1_span.subspan(off, elen), msg2_span.subspan(off, elen), msg3_span.subspan(off, elen));
        off += elen;
      }

      hasher.finalize();

      // Incremental squeezing (chunk sizes driven by multishot_out0 content)
      off = 0;
      while (off < olen) {
        auto s0 = std::span(multishot_out0).subspan(off, 1);
        auto s1 = std::span(multishot_out1).subspan(off, 1);
        auto s2 = std::span(multishot_out2).subspan(off, 1);
        auto s3 = std::span(multishot_out3).subspan(off, 1);
        hasher.squeeze(s0, s1, s2, s3);

        auto elen = std::min<size_t>(multishot_out0[off], olen - (off + 1));

        off += 1;
        hasher.squeeze(std::span(multishot_out0).subspan(off, elen),
                       std::span(multishot_out1).subspan(off, elen),
                       std::span(multishot_out2).subspan(off, elen),
                       std::span(multishot_out3).subspan(off, elen));
        off += elen;
      }

      EXPECT_EQ(oneshot_out0, multishot_out0);
      EXPECT_EQ(oneshot_out1, multishot_out1);
      EXPECT_EQ(oneshot_out2, multishot_out2);
      EXPECT_EQ(oneshot_out3, multishot_out3);
    }
  }
}

// All 4 lanes get identical input; verify all 4 outputs are identical.
TEST(Sha3XOF, SHAKE128x4AllSameInput)
{
  constexpr size_t MLEN = 200;
  constexpr size_t OLEN = 256;

  std::vector<uint8_t> msg(MLEN);
  sha3_test_utils::random_data<uint8_t>(msg);

  std::vector<uint8_t> out0(OLEN);
  std::vector<uint8_t> out1(OLEN);
  std::vector<uint8_t> out2(OLEN);
  std::vector<uint8_t> out3(OLEN);

  {
    shake128_x4::shake128_x4_t h;

    h.absorb(msg, msg, msg, msg);
    h.finalize();
    h.squeeze(out0, out1, out2, out3);
  }

  EXPECT_EQ(out0, out1);
  EXPECT_EQ(out1, out2);
  EXPECT_EQ(out2, out3);

  // Also verify against scalar
  std::vector<uint8_t> out_sc(OLEN);

  {
    shake128::shake128_t h;

    h.absorb(msg);
    h.finalize();
    h.squeeze(out_sc);
  }

  EXPECT_EQ(out0, out_sc);
}

#endif
