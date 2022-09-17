#include "shake128.hpp"
#include "utils.hpp"
#include <iostream>

// Compile it using
//
// g++ -std=c++20 -Wall -O3 -I include example/shake128.cpp
int
main()
{
  constexpr size_t ilen = 32;
  constexpr size_t olen = 40;

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(ilen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(olen));

  random_data<uint8_t>(msg, ilen);
  std::memset(dig, 0, olen);

  // create shake128 hasher
  shake128::shake128 hasher{};
  // absorb message bytes into state
  hasher.hash(msg, ilen);

  // squeeze total `olen` -bytes out of sponge, a single byte at a time
  for (size_t i = 0; i < olen; i++) {
    hasher.read(dig + i, 1);
  }

  std::cout << "SHAKE-128" << std::endl << std::endl;
  std::cout << "Input  : " << to_hex(msg, ilen) << std::endl;
  std::cout << "Output : " << to_hex(dig, olen) << std::endl;

  std::free(msg);
  std::free(dig);

  return EXIT_SUCCESS;
}
