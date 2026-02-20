#include "sha3/turboshake128.hpp"
#include "test_conf.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <span>
#include <vector>

namespace {

// Eval TurboSHAKE128 XOF on given input message during compilation-time.
template<size_t OLEN>
constexpr std::array<uint8_t, OLEN>
eval_turboshake128(std::span<const uint8_t> data)
{
  std::array<uint8_t, OLEN> out{};

  turboshake128::turboshake128_t hasher;
  hasher.absorb(data);
  hasher.finalize();
  hasher.squeeze(out);

  return out;
}

}

// Ensure that TurboSHAKE128 XOF implementation is compile-time evaluable.
TEST(Sha3XOF, CompileTimeEvalTurboSHAKE128)
{
  constexpr auto input = sha3_test_utils::from_hex<256>(
    "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c"
    "4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f90919293949596979899"
    "9a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6"
    "e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
  constexpr auto digest = eval_turboshake128<256>(input);

  static_assert(digest == sha3_test_utils::from_hex<256>(
                            "5d6efc5eb4b62f82fe835511ad0ac01f55d47366f99d1476d99069c2f43997989263aacacabbe3c80b376c8403424b714b20a5d4a389aec7c72fd0efcf25ba3c9a"
                            "ac118bc557bd1d1f691db24eea33b6a569dfc2c13b0d4f4ed7b5860f5af9b1a014249dec67df97538904d48926a323d462908be20d56fbae9d34f7912e35d14406"
                            "b9bfabb8f0bda7b7f2e54f8f6d16d34fed442129a281e2f5fa80d22b2ce90e5229bc31e5138cc7075e8b4b912d5d6147e02a37777a93feb819dd9d07953f00c29b"
                            "fb38f719774d7f0130c6d984d6728c296ba094057d64b27abfd32e50a1982ad8cc0ebe220e458ac29bf127d3e827aea4f33efe253efc315a2caacd5cff"),
                "Must be able to compute TurboSHAKE128 XOF during compile-time !");
}

/**
 * Test that absorbing same message bytes using both incremental and one-shot hashing, should yield same output bytes, for TurboSHAKE128 XOF.
 *
 * This test collects inspiration from https://github.com/itzmeanjan/turboshake/blob/e1a6b950c5374aff49f04f6d51d807e68077ab25/src/tests.rs#L372-L415.
 */
TEST(Sha3XOF, TurboSHAKE128IncrementalAbsorptionAndSqueezing)
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

namespace {

template<uint8_t domain_separator, size_t OLEN>
std::array<uint8_t, OLEN>
compute_turboshake128_output(const std::vector<uint8_t>& msg)
{
  std::array<uint8_t, OLEN> out_bytes{};

  turboshake128::turboshake128_t hasher;

  hasher.absorb(msg);
  hasher.finalize<domain_separator>();
  hasher.squeeze(out_bytes);

  return out_bytes;
}

}

// Ensure that TurboSHAKE128 XOF implementation is conformant with RFC 9861 https://datatracker.ietf.org/doc/rfc9861, by using test vectors defined there.
TEST(Sha3XOF, TurboSHAKE128KnownAnswerTests)
{
  // clang-format off
  EXPECT_EQ((compute_turboshake128_output<0x01, 32>({})), sha3_test_utils::from_hex<32>("868cbd53b078205abb85815d941f7d0376bff5b8888a6a2d03483afbaf83967f"));
  EXPECT_EQ((compute_turboshake128_output<0x02, 32>({})), sha3_test_utils::from_hex<32>("8bcf8b0266eb3ef49e2b1df2eb627021d86281801116761f44efc976444f021b"));
  EXPECT_EQ((compute_turboshake128_output<0x03, 32>({})), sha3_test_utils::from_hex<32>("a0347b35a7fa3d2f8561b3a4648de357be6762a6b76d5b2c1119cda104688192"));
  EXPECT_EQ((compute_turboshake128_output<0x0c, 32>({})), sha3_test_utils::from_hex<32>("2c6462e826d1d5fa989b91ae4d8b3a3b63df64141e0ac0f9a1fbdf653b4ccf13"));
  EXPECT_EQ((compute_turboshake128_output<0x1f, 32>({})), sha3_test_utils::from_hex<32>("1e415f1c5983aff2169217277d17bb538cd945a397ddec541f1ce41af2c1b74c"));
  EXPECT_EQ((compute_turboshake128_output<0x23, 32>({})), sha3_test_utils::from_hex<32>("82d2b02713285b0dc2e8d1f2b40848ee62589b5b11262867e610e15ee62e1835"));
  EXPECT_EQ((compute_turboshake128_output<0x3a, 32>({})), sha3_test_utils::from_hex<32>("55c63f13a040da7034f67d7b7b9a173426970419232209c01ca176e08b5acf5c"));
  EXPECT_EQ((compute_turboshake128_output<0x51, 32>({})), sha3_test_utils::from_hex<32>("4e2695cf70d7c6c87e80a9f383b7aa6f0f8a4b0727f5cd2951c6947dffab6425"));
  EXPECT_EQ((compute_turboshake128_output<0x68, 32>({})), sha3_test_utils::from_hex<32>("2e1c136a8af2e8b4c4cf9a7bca593d798f61bd1f153cd08483447a5de4369b1e"));
  EXPECT_EQ((compute_turboshake128_output<0x7f, 32>({})), sha3_test_utils::from_hex<32>("e4e1fd449c36ef25256c896e1907af3f458253d4a0bd820a6fef83377ae031f9"));

  EXPECT_EQ((compute_turboshake128_output<0x01, 64>({})), sha3_test_utils::from_hex<64>("868cbd53b078205abb85815d941f7d0376bff5b8888a6a2d03483afbaf83967f226e2cad5e7b1ec4ca72236f076462199fea48c93438ad4c49c767f9417be7c5"));
  EXPECT_EQ((compute_turboshake128_output<0x1f, 64>({})), sha3_test_utils::from_hex<64>("1E415F1C5983AFF2169217277D17BB538CD945A397DDEC541F1CE41AF2C1B74C3E8CCAE2A4DAE56C84A04C2385C03C15E8193BDF58737363321691C05462C8DF"));

  EXPECT_EQ((compute_turboshake128_output<0x1f, 32>(sha3_test_utils::ptn(1))), sha3_test_utils::from_hex<32>("55cedd6f60af7bb29a4042ae832ef3f58db7299f893ebb9247247d856958daa9"));
  EXPECT_EQ((compute_turboshake128_output<0x1f, 32>(sha3_test_utils::ptn(17UL))), sha3_test_utils::from_hex<32>("9c97d036a3bac819db70ede0ca554ec6e4c2a1a4ffbfd9ec269ca6a111161233"));
  EXPECT_EQ((compute_turboshake128_output<0x1f, 32>(sha3_test_utils::ptn(17UL * 17UL))), sha3_test_utils::from_hex<32>("96c77c279e0126f7fc07c9b07f5cdae1e0be60bdbe10620040e75d7223a624d2"));
  EXPECT_EQ((compute_turboshake128_output<0x1f, 32>(sha3_test_utils::ptn(17UL * 17UL * 17UL))), sha3_test_utils::from_hex<32>("d4976eb56bcf118520582b709f73e1d6853e001fdaf80e1b13e0d0599d5fb372"));
  EXPECT_EQ((compute_turboshake128_output<0x1f, 32>(sha3_test_utils::ptn(17UL * 17UL * 17UL * 17UL))), sha3_test_utils::from_hex<32>("DA67C7039E98BF530CF7A37830C6664E14CBAB7F540F58403B1B82951318EE5C"));
  EXPECT_EQ((compute_turboshake128_output<0x1f, 32>(sha3_test_utils::ptn(17UL * 17UL * 17UL * 17UL * 17UL))), sha3_test_utils::from_hex<32>("B97A906FBF83EF7C812517ABF3B2D0AEA0C4F60318CE11CF103925127F59EECD"));
  EXPECT_EQ((compute_turboshake128_output<0x1f, 32>(sha3_test_utils::ptn(17UL * 17UL * 17UL * 17UL * 17UL * 17UL))), sha3_test_utils::from_hex<32>("35CD494ADEDED2F25239AF09A7B8EF0C4D1CA4FE2D1AC370FA63216FE7B4C2B1"));

  EXPECT_EQ((compute_turboshake128_output<0x01, 32>(sha3_test_utils::ptn(1))), sha3_test_utils::from_hex<32>("0fc5bb1616bfd8121beb8cd6cde167ffbe4b11e51d9bc9a6a92c34ed3e46f4e1"));
  EXPECT_EQ((compute_turboshake128_output<0x01, 32>(sha3_test_utils::ptn(17UL))), sha3_test_utils::from_hex<32>("6f0f5f330a7114ed345b97d012f8a8bac5ba32f1c0aafab22ef880737bf0c103"));
  EXPECT_EQ((compute_turboshake128_output<0x01, 32>(sha3_test_utils::ptn(17UL * 17UL))), sha3_test_utils::from_hex<32>("6232caa37353b5adb0e16e5beb97928110c5b837531339a2c9eb08014faa8ef6"));
  EXPECT_EQ((compute_turboshake128_output<0x01, 32>(sha3_test_utils::ptn(17UL * 17UL * 17UL))), sha3_test_utils::from_hex<32>("668105870786e2aa80718487563aa06824eabc1d3a8e8b642f6d9996244fe8cf"));
  EXPECT_EQ((compute_turboshake128_output<0x01, 32>(sha3_test_utils::ptn(17UL * 17UL * 17UL * 17UL))), sha3_test_utils::from_hex<32>("795de7dd0ec596c20145d1784ac2acd625b4f62653872a06d8a8b9a0543aa863"));
  EXPECT_EQ((compute_turboshake128_output<0x01, 32>(sha3_test_utils::ptn(17UL * 17UL * 17UL * 17UL * 17UL))), sha3_test_utils::from_hex<32>("4185e05262bcbcf7f74f50f08a710791ea0a12fba13c3a23ff07c33c0110bd20"));

  EXPECT_EQ((compute_turboshake128_output<0x01, 32>({ 0xff, 0xff, 0xff })), sha3_test_utils::from_hex<32>("BF323F940494E88EE1C540FE660BE8A0C93F43D15EC006998462FA994EED5DAB"));
  EXPECT_EQ((compute_turboshake128_output<0x06, 32>({ 0xff })), sha3_test_utils::from_hex<32>("8EC9C66465ED0D4A6C35D13506718D687A25CB05C74CCA1E42501ABD83874A67"));
  EXPECT_EQ((compute_turboshake128_output<0x07, 32>({ 0xff, 0xff, 0xff })), sha3_test_utils::from_hex<32>("B658576001CAD9B1E5F399A9F77723BBA05458042D68206F7252682DBA3663ED"));
  EXPECT_EQ((compute_turboshake128_output<0x0b, 32>({ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff })), sha3_test_utils::from_hex<32>("8DEEAA1AEC47CCEE569F659C21DFA8E112DB3CEE37B18178B2ACD805B799CC37"));
  EXPECT_EQ((compute_turboshake128_output<0x30, 32>({ 0xff })), sha3_test_utils::from_hex<32>("553122E2135E363C3292BED2C6421FA232BAB03DAA07C7D6636603286506325B"));
  EXPECT_EQ((compute_turboshake128_output<0x7f, 32>({ 0xff, 0xff, 0xff })), sha3_test_utils::from_hex<32>("16274CC656D44CEFD422395D0F9053BDA6D28E122ABA15C765E5AD0E6EAF26F9"));

  {
    auto out = compute_turboshake128_output<0x01, 10032>({});
    auto out_span = std::span(out);
    EXPECT_TRUE(std::equal(out_span.last<32>().begin(), out_span.last<32>().end(), sha3_test_utils::from_hex<32>("fa09df77a17a33fe098328ba02786ac770301386f77d0731f2b866bd0140b412").begin()));
  }

  {
    auto out = compute_turboshake128_output<0x1f, 10032>({});
    auto out_span = std::span(out);
    EXPECT_TRUE(std::equal(out_span.last<32>().begin(), out_span.last<32>().end(), sha3_test_utils::from_hex<32>("A3B9B0385900CE761F22AED548E754DA10A5242D62E8C658E3F3A923A7555607").begin()));
  }
  // clang-format on
}
