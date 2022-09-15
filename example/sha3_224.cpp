#include "sha3_224.hpp"
#include "utils.hpp"
#include <iostream>

// Compile it using
//
// g++ -std=c++20 -Wall -O3 -I include example/sha3_224.cpp
int
main()
{
  constexpr size_t ilen = 32;
  constexpr size_t olen = 28;

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(ilen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(olen));

  random_data<uint8_t>(msg, ilen);
  std::memset(dig, 0, olen);

  sha3_224::hash(msg, ilen, dig);

  std::cout << "SHA3-224" << std::endl << std::endl;
  std::cout << "Input  : " << to_hex(msg, ilen) << std::endl;
  std::cout << "Output : " << to_hex(dig, olen) << std::endl;

  std::free(msg);
  std::free(dig);

  return EXIT_SUCCESS;
}
