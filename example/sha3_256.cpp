#include "sha3_256.hpp"
#include "utils.hpp"
#include <iostream>

// Compile it using
//
// g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_256.cpp
int
main()
{
  constexpr size_t ilen = 32;
  constexpr size_t olen = 32;

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(ilen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(olen));

  sha3_utils::random_data<uint8_t>(msg, ilen);
  std::memset(dig, 0, olen);

  sha3_256::hash(msg, ilen, dig);

  std::cout << "SHA3-256" << std::endl << std::endl;
  std::cout << "Input  : " << sha3_utils::to_hex(msg, ilen) << std::endl;
  std::cout << "Output : " << sha3_utils::to_hex(dig, olen) << std::endl;

  std::free(msg);
  std::free(dig);

  return EXIT_SUCCESS;
}
