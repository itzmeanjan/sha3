#include "sha3/sha3_512.hpp"
#include "test_conf.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

// Ensure that SHA3-512 implementation is compile-time evaluable.
TEST(Sha3Hashing, CompileTimeEvalSha3_512)
{
  constexpr auto input = sha3_test_utils::from_hex<sha3_512::DIGEST_LEN * 2>(
    "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c"
    "4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f");
  constexpr auto digest = sha3_512::sha3_512_t::hash(input);

  static_assert(digest == sha3_test_utils::from_hex<sha3_512::DIGEST_LEN>(
                            "989c1995da9d2d341f993c2e2ca695f3477075061bfbd2cdf0be75cf7ba99fbe33d8d2c4dcc31fa89917786b883e6c9d5b02ed81b7483a4cb3ea98671588f745"),
                "Must be able to compute Sha3-512 hash during compile-time !");
}

// Test that absorbing same input message bytes using both incremental and one-shot hashing, should yield same output bytes, for SHA3-512 hasher.
TEST(Sha3Hashing, Sha3_512IncrementalAbsorption)
{
  for (size_t mlen = MIN_MSG_LEN; mlen < MAX_MSG_LEN; mlen++) {
    std::vector<uint8_t> msg(mlen);
    auto msg_span = std::span(msg);

    sha3_test_utils::random_data(msg_span);

    // Oneshot Hashing
    auto oneshot_out = sha3_512::sha3_512_t::hash(msg_span);

    // Incremental Hashing
    std::array<uint8_t, sha3_512::DIGEST_LEN> multishot_out{ 0 };
    sha3_512::sha3_512_t hasher;

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
 * Ensure that SHA3-512 implementation is conformant with FIPS 202 standard, by using KAT file generated following
 * https://gist.github.com/itzmeanjan/448f97f9c49d781a5eb3ddd6ea6e7364.
 */
TEST(Sha3Hashing, Sha3_512KnownAnswerTests)
{
  using namespace std::literals;

  const std::string kat_file = "./kats/sha3_512.kat";
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

      auto expected_md = sha3_test_utils::from_hex<sha3_512::DIGEST_LEN>(md2);
      auto computed_md = sha3_512::sha3_512_t::hash(msg);

      EXPECT_EQ(computed_md, expected_md);

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}
