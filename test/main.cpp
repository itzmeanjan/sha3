#include "test/test_shake128.hpp"
#include "test/test_shake256.hpp"
#include <iostream>

int
main()
{
  constexpr size_t MIN_MSG_LEN = 0;
  constexpr size_t MAX_MSG_LEN = 1025;

  for (size_t i = MIN_MSG_LEN; i < MAX_MSG_LEN; i++) {
    test_sha3::test_shake128(i);
    test_sha3::test_shake256(i);
  }
  std::cout << "[test] SHAKE{128, 256} incremental message absorption\n";

  return EXIT_SUCCESS;
}
