#include "sha3/sha3_384.hpp"
#include "test_conf.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

// Ensure that SHA3-384 implementation is compile-time evaluable.
TEST(Sha3Hashing, CompileTimeEvalSha3_384)
{
  constexpr auto input =
    sha3_test_utils::from_hex<sha3_384::DIGEST_LEN * 2>("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132"
                                                        "333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f");
  constexpr auto digest = sha3_384::sha3_384_t::hash(input);

  static_assert(digest == sha3_test_utils::from_hex<sha3_384::DIGEST_LEN>(
                            "d6e266970a3fdcd4a833da861599179a060b576959e993b4698529304ee38c23c7102a7084c4d568b1d95523d14077e7"),
                "Must be able to compute Sha3-384 hash during compile-time !");
}

// Test that absorbing same input message bytes using both incremental and one-shot hashing, should yield same output bytes, for SHA3-384 hasher.
TEST(Sha3Hashing, Sha3_384IncrementalAbsorption)
{
  for (size_t mlen = MIN_MSG_LEN; mlen < MAX_MSG_LEN; mlen++) {
    std::vector<uint8_t> msg(mlen);
    auto msg_span = std::span(msg);

    sha3_test_utils::random_data(msg_span);

    // Oneshot Hashing
    auto oneshot_out = sha3_384::sha3_384_t::hash(msg_span);

    // Incremental Hashing
    std::array<uint8_t, sha3_384::DIGEST_LEN> multishot_out{ 0 };
    sha3_384::sha3_384_t hasher;

    size_t off = 0;
    while (off < mlen) {
      // because we don't want to be stuck in an infinite loop if msg[off] = 0 !
      auto elen = std::min<size_t>(std::max<uint8_t>(msg[off], 1), mlen - off);

      hasher.absorb(msg_span.subspan(off, elen));
      off += elen;
    }

    hasher.finalize();
    hasher.digest(multishot_out);

    EXPECT_EQ(oneshot_out, multishot_out);
  }
}

/**
 * Ensure that SHA3-384 implementation is conformant with FIPS 202 standard, by using KAT file generated following
 * https://gist.github.com/itzmeanjan/448f97f9c49d781a5eb3ddd6ea6e7364.
 */
TEST(Sha3Hashing, Sha3_384KnownAnswerTests)
{
  using namespace std::literals;

  const std::string kat_file = "./kats/sha3_384.kat";
  std::fstream file(kat_file);

  while (true) {
    std::string len0;

    if (!std::getline(file, len0).eof()) {
      std::string msg0;
      std::string md0;

      std::getline(file, msg0);
      std::getline(file, md0);

      auto msg1 = std::string_view(msg0);
      auto md1 = std::string_view(md0);

      auto msg2 = msg1.substr(msg1.find("="sv) + 2, msg1.size());
      auto md2 = md1.substr(md1.find("="sv) + 2, md1.size());

      auto msg = sha3_test_utils::parse_dynamic_sized_hex_string(msg2);

      auto expected_md = sha3_test_utils::from_hex<sha3_384::DIGEST_LEN>(md2);
      auto computed_md = sha3_384::sha3_384_t::hash(msg);

      EXPECT_EQ(computed_md, expected_md);

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}
