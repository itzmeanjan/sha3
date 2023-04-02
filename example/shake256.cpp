#include "shake256.hpp"
#include "utils.hpp"
#include <iostream>

// Compile it using
//
// g++ -std=c++20 -Wall -O3 -march=native -I include example/shake256.cpp
int
main()
{
  constexpr size_t ilen = 32;
  constexpr size_t olen = 40;

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(ilen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(olen));

  sha3_utils::random_data<uint8_t>(msg, ilen);
  std::memset(dig, 0, olen);

  // create shake256 hasher
  shake256::shake256 hasher{};
  // absorb message bytes into state
  hasher.hash(msg, ilen);

  // squeeze total `olen` -bytes out of sponge, a single byte at a time
  for (size_t i = 0; i < olen; i++) {
    hasher.read(dig + i, 1);
  }

  std::cout << "SHAKE-256" << std::endl << std::endl;
  std::cout << "Input  : " << sha3_utils::to_hex(msg, ilen) << std::endl;
  std::cout << "Output : " << sha3_utils::to_hex(dig, olen) << std::endl;

  std::free(msg);
  std::free(dig);

  return EXIT_SUCCESS;
}
