#include "sha3_256.hpp"
#include "test_conf.hpp"
#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

// Test that absorbing same input message bytes using both incremental and
// one-shot hashing, should yield same output bytes, for SHA3-256 hasher.
TEST(Sha3Hashing, Sha3_256IncrementalAbsorption)
{
  for (size_t mlen = MIN_MSG_LEN; mlen < MAX_MSG_LEN; mlen++) {
    std::vector<uint8_t> msg(mlen);
    std::vector<uint8_t> out0(sha3_256::DIGEST_LEN);
    std::vector<uint8_t> out1(sha3_256::DIGEST_LEN);

    sha3_utils::random_data(msg.data(), msg.size());

    sha3_256::sha3_256 hasher;

    // Oneshot Hashing
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.digest(out0.data());

    hasher.reset();

    // Incremental Hashing
    size_t off = 0;
    while (off < mlen) {
      // because we don't want to be stuck in an infinite loop if msg[off] = 0 !
      auto elen = std::min<size_t>(std::max<uint8_t>(msg[off], 1), mlen - off);

      hasher.absorb(msg.data() + off, elen);
      off += elen;
    }

    hasher.finalize();
    hasher.digest(out1.data());

    ASSERT_TRUE(std::ranges::equal(out0, out1));
  }
}

// Ensure that SHA3-256 implementation is conformant with FIPS 202 standard, by
// using KAT file generated following
// https://gist.github.com/itzmeanjan/448f97f9c49d781a5eb3ddd6ea6e7364.
TEST(Sha3Hashing, Sha3_256KnownAnswerTests)
{
  using namespace std::literals;

  const std::string kat_file = "./kats/sha3_256.kat";
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

      auto msg = sha3_utils::from_hex(msg2);
      auto md = sha3_utils::from_hex(md2);

      std::vector<uint8_t> digest(sha3_256::DIGEST_LEN);

      sha3_256::sha3_256 hasher;

      hasher.absorb(msg.data(), msg.size());
      hasher.finalize();
      hasher.digest(digest.data());

      ASSERT_TRUE(std::ranges::equal(digest, md));

      std::string empty_line;
      std::getline(file, empty_line);
    } else {
      break;
    }
  }

  file.close();
}
