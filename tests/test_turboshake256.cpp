#include "sha3/turboshake256.hpp"
#include "test_conf.hpp"
#include "test_utils.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <numeric>
#include <span>
#include <vector>

// Eval TurboSHAKE256 XOF on statically defined input message during compilation-time.
constexpr std::array<uint8_t, 256>
eval_turboshake256()
{
  // Statically defined input.
  std::array<uint8_t, 256> data{};
  std::iota(data.begin(), data.end(), 0);

  // To be computed output.
  std::array<uint8_t, 256> md{};

  turboshake256::turboshake256_t hasher;
  hasher.absorb(data);
  hasher.finalize();
  hasher.squeeze(md);

  return md;
}

// Ensure that TurboSHAKE256 XOF implementation is compile-time evaluable.
TEST(Sha3XOF, CompileTimeEvalTurboSHAKE256)
{
  // Input  =
  // 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff
  // Output =
  // 2e1c8e060e12fab9d0f69f476a85832ca52590a8ac1cbc55b4ffc6d128dd10376c62c23ed88d53ec22e65f6026dc846fbbe9c23afa8e40e6f37fcfdbc77cfc64326c9d3afb5580c1eaeef1cdce48ded652ffc6d752d7293078ef60f77839620a20e2810d1c1e5ef3f8dc062798c1f9518b1294786e3d2600c779b819b37b423d3bfa4064ff58de6a0c226417dcc0405e0186c3922979c360694a412a97dba2fa89a41c6720e9dce5681e74c9a2f8eb8fa0cbc09f1cfb180a83315ea040811534eed86907f8f9f098c5d68b2385b3fac242259a1d099a14a984dfce3464cde9ecda311c041c9a44e2566475b6c781a791a88fba3d0169fcf1b27c8d82b27a34e1

  constexpr auto md = eval_turboshake256();
  static_assert(md == std::array<uint8_t, 256>{ 46,  28,  142, 6,   14,  18,  250, 185, 208, 246, 159, 71,  106, 133, 131, 44,  165, 37,  144, 168, 172, 28,
                                                188, 85,  180, 255, 198, 209, 40,  221, 16,  55,  108, 98,  194, 62,  216, 141, 83,  236, 34,  230, 95,  96,
                                                38,  220, 132, 111, 187, 233, 194, 58,  250, 142, 64,  230, 243, 127, 207, 219, 199, 124, 252, 100, 50,  108,
                                                157, 58,  251, 85,  128, 193, 234, 238, 241, 205, 206, 72,  222, 214, 82,  255, 198, 215, 82,  215, 41,  48,
                                                120, 239, 96,  247, 120, 57,  98,  10,  32,  226, 129, 13,  28,  30,  94,  243, 248, 220, 6,   39,  152, 193,
                                                249, 81,  139, 18,  148, 120, 110, 61,  38,  0,   199, 121, 184, 25,  179, 123, 66,  61,  59,  250, 64,  100,
                                                255, 88,  222, 106, 12,  34,  100, 23,  220, 192, 64,  94,  1,   134, 195, 146, 41,  121, 195, 96,  105, 74,
                                                65,  42,  151, 219, 162, 250, 137, 164, 28,  103, 32,  233, 220, 229, 104, 30,  116, 201, 162, 248, 235, 143,
                                                160, 203, 192, 159, 28,  251, 24,  10,  131, 49,  94,  160, 64,  129, 21,  52,  238, 216, 105, 7,   248, 249,
                                                240, 152, 197, 214, 139, 35,  133, 179, 250, 194, 66,  37,  154, 29,  9,   154, 20,  169, 132, 223, 206, 52,
                                                100, 205, 233, 236, 218, 49,  28,  4,   28,  154, 68,  226, 86,  100, 117, 182, 199, 129, 167, 145, 168, 143,
                                                186, 61,  1,   105, 252, 241, 178, 124, 141, 130, 178, 122, 52,  225 },
                "Must be able to compute TurboSHAKE256 XOF during compile-time !");
}

/**
 * Test that absorbing same message bytes using both incremental and one-shot hashing, should yield same output bytes, for TurboSHAKE256 XOF.
 *
 * This test collects inspiration from https://github.com/itzmeanjan/turboshake/blob/e1a6b950c5374aff49f04f6d51d807e68077ab25/src/tests.rs#L372-L415.
 */
TEST(Sha3XOF, TurboSHAKE256IncrementalAbsorptionAndSqueezing)
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

      turboshake256::turboshake256_t hasher;

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

template<uint8_t domain_separator>
std::vector<uint8_t>
compute_turboshake256_output(const std::vector<uint8_t> msg, const size_t out_byte_len)
{
  std::vector<uint8_t> out_bytes(out_byte_len, 0);

  turboshake256::turboshake256_t hasher;

  hasher.absorb(msg);
  hasher.finalize<domain_separator>();
  hasher.squeeze(out_bytes);

  return out_bytes;
}

// Ensure that TurboSHAKE256 XOF implementation is conformant with RFC 9861 https://datatracker.ietf.org/doc/rfc9861, by using test vectors defined there.
TEST(Sha3XOF, TurboSHAKE256KnownAnswerTests)
{
  // clang-format off
  EXPECT_EQ(compute_turboshake256_output<0x01>({}, 32), sha3_test_utils::parse_dynamic_sized_hex_string("e3dd2df0943bde6d82e39ec36059f35cd76720e2df38cc6b10b69fddfcaa3a4a"));
  EXPECT_EQ(compute_turboshake256_output<0x02>({}, 32), sha3_test_utils::parse_dynamic_sized_hex_string("cfdbc69ec2652711dc3013cee68def374948ef09e62d82f2749e3dbc71f04dce"));
  EXPECT_EQ(compute_turboshake256_output<0x03>({}, 32), sha3_test_utils::parse_dynamic_sized_hex_string("1402a1d6bebcf52cdbc7074c3d7b1adc545646458400a63980ebb3dd0ab04c68"));
  EXPECT_EQ(compute_turboshake256_output<0x0c>({}, 32), sha3_test_utils::parse_dynamic_sized_hex_string("3c78a84557f19506a6151985664cf6163c4d4033d6bc310f8e8dde56e232abf4"));
  EXPECT_EQ(compute_turboshake256_output<0x1f>({}, 64), sha3_test_utils::parse_dynamic_sized_hex_string("367A329DAFEA871C7802EC67F905AE13C57695DC2C6663C61035F59A18F8E7DB11EDC0E12E91EA60EB6B32DF06DD7F002FBAFABB6E13EC1CC20D995547600DB0"));
  EXPECT_EQ(compute_turboshake256_output<0x23>({}, 32), sha3_test_utils::parse_dynamic_sized_hex_string("bd8f3f5eae3fb4ba604ad2d9d9431867532ab1e2f773819620b79281e3258bbc"));
  EXPECT_EQ(compute_turboshake256_output<0x3a>({}, 32), sha3_test_utils::parse_dynamic_sized_hex_string("cfa491078479604fd78e967071a081cf357a1244d2999c929c318782a24d7c21"));
  EXPECT_EQ(compute_turboshake256_output<0x51>({}, 32), sha3_test_utils::parse_dynamic_sized_hex_string("b92a11dd21017255a8285bbdf413269dcfae55f79d188a55cc2e04ea667bc047"));
  EXPECT_EQ(compute_turboshake256_output<0x68>({}, 32), sha3_test_utils::parse_dynamic_sized_hex_string("229acb8530b6e700bebb304655a5dfad00f7ac4ab7f582ee909c11b96f6d5fb3"));
  EXPECT_EQ(compute_turboshake256_output<0x7f>({}, 32), sha3_test_utils::parse_dynamic_sized_hex_string("49b38a11204328440c4c40fdaee305629379936d7a31f9474c4f0fb062a2a427"));

  EXPECT_EQ(compute_turboshake256_output<0x01>({}, 64), sha3_test_utils::parse_dynamic_sized_hex_string("e3dd2df0943bde6d82e39ec36059f35cd76720e2df38cc6b10b69fddfcaa3a4a72fbbbe42c00ced7aa88e26d4675dd6e2c43c4413c4ea4d44bb170f03a981cab"));
  EXPECT_EQ(compute_turboshake256_output<0x1f>({}, 64), sha3_test_utils::parse_dynamic_sized_hex_string("367A329DAFEA871C7802EC67F905AE13C57695DC2C6663C61035F59A18F8E7DB11EDC0E12E91EA60EB6B32DF06DD7F002FBAFABB6E13EC1CC20D995547600DB0"));
 
  EXPECT_EQ(compute_turboshake256_output<0x1f>(sha3_test_utils::ptn(1), 64), sha3_test_utils::parse_dynamic_sized_hex_string("3E1712F928F8EAF1054632B2AA0A246ED8B0C378728F60BC970410155C28820E90CC90D8A3006AA2372C5C5EA176B0682BF22BAE7467AC94F74D43D39B0482E2"));
  EXPECT_EQ(compute_turboshake256_output<0x1f>(sha3_test_utils::ptn(17), 64), sha3_test_utils::parse_dynamic_sized_hex_string("B3BAB0300E6A191FBE6137939835923578794EA54843F5011090FA2F3780A9E5CB22C59D78B40A0FBFF9E672C0FBE0970BD2C845091C6044D687054DA5D8E9C7"));
  EXPECT_EQ(compute_turboshake256_output<0x1f>(sha3_test_utils::ptn(17 * 17), 64), sha3_test_utils::parse_dynamic_sized_hex_string("66B810DB8E90780424C0847372FDC95710882FDE31C6DF75BEB9D4CD9305CFCAE35E7B83E8B7E6EB4B78605880116316FE2C078A09B94AD7B8213C0A738B65C0"));
  EXPECT_EQ(compute_turboshake256_output<0x1f>(sha3_test_utils::ptn(17 * 17 * 17), 64), sha3_test_utils::parse_dynamic_sized_hex_string("C74EBC919A5B3B0DD1228185BA02D29EF442D69D3D4276A93EFE0BF9A16A7DC0CD4EABADAB8CD7A5EDD96695F5D360ABE09E2C6511A3EC397DA3B76B9E1674FB"));
  EXPECT_EQ(compute_turboshake256_output<0x1f>(sha3_test_utils::ptn(17 * 17 * 17 * 17), 64), sha3_test_utils::parse_dynamic_sized_hex_string("02CC3A8897E6F4F6CCB6FD46631B1F5207B66C6DE9C7B55B2D1A23134A170AFDAC234EABA9A77CFF88C1F020B73724618C5687B362C430B248CD38647F848A1D"));
  EXPECT_EQ(compute_turboshake256_output<0x1f>(sha3_test_utils::ptn(17 * 17 * 17 * 17 * 17), 64), sha3_test_utils::parse_dynamic_sized_hex_string("ADD53B06543E584B5823F626996AEE50FE45ED15F20243A7165485ACB4AA76B4FFDA75CEDF6D8CDC95C332BD56F4B986B58BB17D1778BFC1B1A97545CDF4EC9F"));
  EXPECT_EQ(compute_turboshake256_output<0x1f>(sha3_test_utils::ptn(17 * 17 * 17 * 17 * 17 * 17), 64), sha3_test_utils::parse_dynamic_sized_hex_string("9E11BC59C24E73993C1484EC66358EF71DB74AEFD84E123F7800BA9C4853E02CFE701D9E6BB765A304F0DC34A4EE3BA82C410F0DA70E86BFBD90EA877C2D6104"));

  EXPECT_EQ(compute_turboshake256_output<0x01>(sha3_test_utils::ptn(1), 32), sha3_test_utils::parse_dynamic_sized_hex_string("73ebf1d543d855a3c5e4be6322f75604c254f70394b396884b6010fcca694722"));
  EXPECT_EQ(compute_turboshake256_output<0x01>(sha3_test_utils::ptn(17), 32), sha3_test_utils::parse_dynamic_sized_hex_string("1da47d188755b75307a242a8f2675bbd76aebf8a13b1d40f587a0732cbb3dc3d"));
  EXPECT_EQ(compute_turboshake256_output<0x01>(sha3_test_utils::ptn(17 * 17), 32), sha3_test_utils::parse_dynamic_sized_hex_string("a48c938770f916b09d764e29e2279b90d5fa3dd0e006ee8d6c2eb0db8893525e"));
  EXPECT_EQ(compute_turboshake256_output<0x01>(sha3_test_utils::ptn(17 * 17 * 17), 32), sha3_test_utils::parse_dynamic_sized_hex_string("75e8668d3a46baa7c75c3ac7d33fc2c218df38cdf0f8d70352a495bd9d5d6dfa"));
  EXPECT_EQ(compute_turboshake256_output<0x01>(sha3_test_utils::ptn(17 * 17 * 17 * 17), 32), sha3_test_utils::parse_dynamic_sized_hex_string("ffa49653e40c7ba33f11c278d99be3010f65446a7bf8a69d70b07feb54e7107c"));
  EXPECT_EQ(compute_turboshake256_output<0x01>(sha3_test_utils::ptn(17 * 17 * 17 * 17 * 17), 32), sha3_test_utils::parse_dynamic_sized_hex_string("2ad2b3beb8671840fa9d5e8f7faf2d1139d99483f3c4e56a6a25553f83c25931"));

  EXPECT_EQ(compute_turboshake256_output<0x01>({ 0xff, 0xff, 0xff }, 64), sha3_test_utils::parse_dynamic_sized_hex_string("D21C6FBBF587FA2282F29AEA620175FB0257413AF78A0B1B2A87419CE031D933AE7A4D383327A8A17641A34F8A1D1003AD7DA6B72DBA84BB62FEF28F62F12424"));
  EXPECT_EQ(compute_turboshake256_output<0x06>({ 0xff },64), sha3_test_utils::parse_dynamic_sized_hex_string("738D7B4E37D18B7F22AD1B5313E357E3DD7D07056A26A303C433FA3533455280F4F5A7D4F700EFB437FE6D281405E07BE32A0A972E22E63ADC1B090DAEFE004B"));
  EXPECT_EQ(compute_turboshake256_output<0x07>({ 0xff, 0xff, 0xff }, 64), sha3_test_utils::parse_dynamic_sized_hex_string("18B3B5B7061C2E67C1753A00E6AD7ED7BA1C906CF93EFB7092EAF27FBEEBB755AE6E292493C110E48D260028492B8E09B5500612B8F2578985DED5357D00EC67"));
  EXPECT_EQ(compute_turboshake256_output<0x0b>({ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }, 64), sha3_test_utils::parse_dynamic_sized_hex_string("BB36764951EC97E9D85F7EE9A67A7718FC005CF42556BE79CE12C0BDE50E5736D6632B0D0DFB202D1BBB8FFE3DD74CB00834FA756CB03471BAB13A1E2C16B3C0"));
  EXPECT_EQ(compute_turboshake256_output<0x30>({ 0xff }, 64), sha3_test_utils::parse_dynamic_sized_hex_string("F3FE12873D34BCBB2E608779D6B70E7F86BEC7E90BF113CBD4FDD0C4E2F4625E148DD7EE1A52776CF77F240514D9CCFC3B5DDAB8EE255E39EE389072962C111A"));
  EXPECT_EQ(compute_turboshake256_output<0x7f>({ 0xff, 0xff, 0xff }, 64), sha3_test_utils::parse_dynamic_sized_hex_string("ABE569C1F77EC340F02705E7D37C9AB7E155516E4A6A150021D70B6FAC0BB40C069F9A9828A0D575CD99F9BAE435AB1ACF7ED9110BA97CE0388D074BAC768776"));
  
  {
    auto out_bytes = compute_turboshake256_output<0x01>({}, 10032);
    auto out_bytes_span = std::span(out_bytes);
    
    EXPECT_TRUE(std::equal(out_bytes_span.last<32>().begin(), out_bytes_span.last<32>().end(), sha3_test_utils::parse_dynamic_sized_hex_string("b021b244dcd9599966d7742225fc7372639233f0ff0863fa79683ebf1f57114f").begin()));
  }
  
  {
    auto out_bytes = compute_turboshake256_output<0x1f>({}, 10032);
    auto out_bytes_span = std::span(out_bytes);
    
    EXPECT_TRUE(std::equal(out_bytes_span.last<32>().begin(), out_bytes_span.last<32>().end(), sha3_test_utils::parse_dynamic_sized_hex_string("ABEFA11630C661269249742685EC082F207265DCCF2F43534E9C61BA0C9D1D75").begin()));
  }
  // clang-format on
}
