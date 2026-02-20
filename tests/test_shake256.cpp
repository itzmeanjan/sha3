#include "sha3/shake256.hpp"
#include "test_conf.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

namespace {

// Eval SHAKE256 XOF on given input message during compilation-time.
template<size_t OLEN>
constexpr std::array<uint8_t, OLEN>
eval_shake256(std::span<const uint8_t> data)
{
  std::array<uint8_t, OLEN> out{};

  shake256::shake256_t hasher;
  hasher.absorb(data);
  hasher.finalize();
  hasher.squeeze(out);

  return out;
}

}

// Ensure that SHAKE256 XOF implementation is compile-time evaluable.
TEST(Sha3XOF, CompileTimeEvalSHAKE256)
{
  constexpr auto input = sha3_test_utils::from_hex<256>(
    "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c"
    "4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f90919293949596979899"
    "9a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6"
    "e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
  constexpr auto digest = eval_shake256<256>(input);

  static_assert(digest == sha3_test_utils::from_hex<256>(
                            "336c8aa7f2b08bda6bd7402cd2ea89760b7728a8b31802b80524756361165366ff8159f2f4568a2bfa286db6387895629938c2868a6421c37f988455763a75e4b9"
                            "259e0a939aaa68295119ccea72c9f0ca7d048aa70eeeb4534c6bd08ecc6163217c790f33b84a89623f8e5538b734967e9490a48b7d0658afb4565364e8b234dfe6"
                            "a2bceb12ce2130eec00bf2113615a276819d7815f5891d07600275f4d8fbc87b056f44bc2b141ca5ed9e4cb6e9a7bf71f520971dca1c8da6140e2af31faef5502e"
                            "84991a2d9e9a80183c174cc105ef178d5f6fa45b0f284eb7bced20a47c3f584aca27eac5558da517af7569fe2e843461b4b65f81f819bf81aae6dfaa3b"),
                "Must be able to compute Shake256 Xof during compile-time !");
}

/**
 * Test that absorbing same message bytes using both incremental and one-shot hashing, should yield same output bytes, for SHAKE256 XOF.
 *
 * This test collects inspiration from https://github.com/itzmeanjan/turboshake/blob/e1a6b950c5374aff49f04f6d51d807e68077ab25/src/tests.rs#L372-L415
 */
TEST(Sha3XOF, SHAKE256IncrementalAbsorptionAndSqueezing)
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

      shake256::shake256_t hasher;

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
 * Ensure that SHAKE256 XOF implementation is conformant with FIPS 202 standard, by using KAT file generated following
 * https://gist.github.com/itzmeanjan/448f97f9c49d781a5eb3ddd6ea6e7364.
 */
TEST(Sha3XOF, SHAKE256KnownAnswerTests)
{
  using namespace std::literals;

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

      auto msg = sha3_test_utils::parse_dynamic_sized_hex_string(msg2);
      auto expected_out = sha3_test_utils::parse_dynamic_sized_hex_string(out2);

      std::vector<uint8_t> computed_out(expected_out.size());
      shake256::shake256_t hasher;

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
