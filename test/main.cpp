#include "test/test_shake128.hpp"
#include "test/test_shake256.hpp"
#include <iostream>

int
main()
{
  constexpr size_t shake128_rate = shake128::rate >> 3;
  constexpr size_t shake256_rate = shake256::rate >> 3;

  test_sha3::test_shake128<0, shake128_rate>();
  test_sha3::test_shake128<1, shake128_rate>();
  test_sha3::test_shake128<1, shake128_rate - 1>();
  test_sha3::test_shake128<2, shake128_rate - 2>();
  test_sha3::test_shake128<2, shake128_rate - 1>();
  test_sha3::test_shake128<shake128_rate - 1, 1>();
  test_sha3::test_shake128<shake128_rate - 2, 2>();
  test_sha3::test_shake128<shake128_rate - 1, 2>();
  std::cout << "[test] SHAKE128 incremental hashing\n";

  test_sha3::test_shake256<0, shake256_rate>();
  test_sha3::test_shake256<1, shake256_rate>();
  test_sha3::test_shake256<1, shake256_rate - 1>();
  test_sha3::test_shake256<2, shake256_rate - 2>();
  test_sha3::test_shake256<2, shake256_rate - 1>();
  test_sha3::test_shake256<shake256_rate - 1, 1>();
  test_sha3::test_shake256<shake256_rate - 2, 2>();
  test_sha3::test_shake256<shake256_rate - 1, 2>();
  std::cout << "[test] SHAKE256 incremental hashing\n";

  return EXIT_SUCCESS;
}
