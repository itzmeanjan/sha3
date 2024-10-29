#include "sha3/sha3_512.hpp"
#include "test_conf.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <numeric>
#include <vector>

// Eval SHA3-512 hash on statically defined input message during
// compilation-time.
constexpr std::array<uint8_t, sha3_512::DIGEST_LEN>
eval_sha3_512()
{
  // Statically defined input.
  std::array<uint8_t, sha3_512::DIGEST_LEN * 2> data{};
  std::iota(data.begin(), data.end(), 0);

  // To be computed output.
  std::array<uint8_t, sha3_512::DIGEST_LEN> md{};

  sha3_512::sha3_512_t hasher;
  hasher.absorb(data);
  hasher.finalize();
  hasher.digest(md);

  return md;
}

// Ensure that SHA3-512 implementation is compile-time evaluable.
TEST(Sha3Hashing, CompileTimeEvalSha3_512)
{
  // Input  =
  // 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f
  // Output =
  // 989c1995da9d2d341f993c2e2ca695f3477075061bfbd2cdf0be75cf7ba99fbe33d8d2c4dcc31fa89917786b883e6c9d5b02ed81b7483a4cb3ea98671588f745

  constexpr auto md = eval_sha3_512();
  static_assert(md ==
                  std::array<uint8_t, sha3_512::DIGEST_LEN>{
                    152, 156, 25,  149, 218, 157, 45,  52,  31,  153, 60,
                    46,  44,  166, 149, 243, 71,  112, 117, 6,   27,  251,
                    210, 205, 240, 190, 117, 207, 123, 169, 159, 190, 51,
                    216, 210, 196, 220, 195, 31,  168, 153, 23,  120, 107,
                    136, 62,  108, 157, 91,  2,   237, 129, 183, 72,  58,
                    76,  179, 234, 152, 103, 21,  136, 247, 69 },
                "Must be able to compute Sha3-512 hash during compile-time !");
}

// Test that absorbing same input message bytes using both incremental and
// one-shot hashing, should yield same output bytes, for SHA3-512 hasher.
TEST(Sha3Hashing, Sha3_512IncrementalAbsorption)
{
  for (size_t mlen = MIN_MSG_LEN; mlen < MAX_MSG_LEN; mlen++) {
    std::vector<uint8_t> msg(mlen);
    std::vector<uint8_t> out0(sha3_512::DIGEST_LEN);
    std::vector<uint8_t> out1(sha3_512::DIGEST_LEN);

    auto _msg = std::span(msg);
    auto _out0 = std::span<uint8_t, sha3_512::DIGEST_LEN>(out0);
    auto _out1 = std::span<uint8_t, sha3_512::DIGEST_LEN>(out1);

    sha3_test_utils::random_data(_msg);

    sha3_512::sha3_512_t hasher;

    // Oneshot Hashing
    hasher.absorb(_msg);
    hasher.finalize();
    hasher.digest(_out0);

    hasher.reset();

    // Incremental Hashing
    size_t off = 0;
    while (off < mlen) {
      // because we don't want to be stuck in an infinite loop if msg[off] = 0 !
      auto elen = std::min<size_t>(std::max<uint8_t>(msg[off], 1), mlen - off);

      hasher.absorb(_msg.subspan(off, elen));
      off += elen;
    }

    hasher.finalize();
    hasher.digest(_out1);

    EXPECT_EQ(out0, out1);
  }
}

// Ensure that SHA3-512 implementation is conformant with FIPS 202 standard, by
// using KAT file generated following
// https://gist.github.com/itzmeanjan/448f97f9c49d781a5eb3ddd6ea6e7364.
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

      auto msg = sha3_test_utils::from_hex(msg2);
      auto md = sha3_test_utils::from_hex(md2);

      std::vector<uint8_t> digest(sha3_512::DIGEST_LEN);
      auto _digest = std::span<uint8_t, sha3_512::DIGEST_LEN>(digest);

      sha3_512::sha3_512_t hasher;

      hasher.absorb(msg);
      hasher.finalize();
      hasher.digest(_digest);

      EXPECT_EQ(digest, md);

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}
