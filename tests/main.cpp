#include "tests/test_hashing.hpp"
#include "tests/test_xof.hpp"
#include <iostream>

int
main()
{
  constexpr size_t MIN_MSG_LEN = 0;
  constexpr size_t MAX_MSG_LEN = 513;
  constexpr size_t MIN_OUT_LEN = 0;
  constexpr size_t MAX_OUT_LEN = 513;

  for (size_t mlen = MIN_MSG_LEN; mlen < MAX_MSG_LEN; mlen++) {
    test_sha3::test_sha3_224_absorption(mlen);
    test_sha3::test_sha3_256_absorption(mlen);
    test_sha3::test_sha3_384_absorption(mlen);
    test_sha3::test_sha3_512_absorption(mlen);
  }
  std::cout << "[test] SHA3-{224,256,384,512} incremental absorption\n";

  for (size_t mlen = MIN_MSG_LEN; mlen < MAX_MSG_LEN; mlen++) {
    for (size_t olen = MIN_OUT_LEN; olen < MAX_OUT_LEN; olen++) {
      test_sha3::test_shake128_absorb_squeeze(mlen, olen);
      test_sha3::test_shake256_absorb_squeeze(mlen, olen);
    }
  }
  std::cout << "[test] SHAKE{128,256} incremental absorption and squeezing\n";

  test_sha3::test_sha3_224_kat();
  std::cout << "[test] SHA3-224 K(nown) A(nswer) T(ests)\n";

  test_sha3::test_sha3_256_kat();
  std::cout << "[test] SHA3-256 K(nown) A(nswer) T(ests)\n";

  test_sha3::test_sha3_384_kat();
  std::cout << "[test] SHA3-384 K(nown) A(nswer) T(ests)\n";

  test_sha3::test_sha3_512_kat();
  std::cout << "[test] SHA3-512 K(nown) A(nswer) T(ests)\n";

  test_sha3::test_shake128_kat();
  std::cout << "[test] Shake128 Xof K(nown) A(nswer) T(ests)\n";

  test_sha3::test_shake256_kat();
  std::cout << "[test] Shake256 Xof K(nown) A(nswer) T(ests)\n";

  return EXIT_SUCCESS;
}
