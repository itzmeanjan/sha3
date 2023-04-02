#include "shake256.hpp"
#include "utils.hpp"
#include <iostream>

// Compile it using
//
// g++ -std=c++20 -Wall -O3 -march=native -I include
// example/incremental_shake256.cpp
int
main()
{
  constexpr size_t ilen0 = 15;
  constexpr size_t ilen1 = 17;
  constexpr size_t olen = 40;

  uint8_t* msg0 = static_cast<uint8_t*>(std::malloc(ilen0));
  uint8_t* msg1 = static_cast<uint8_t*>(std::malloc(ilen1));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(olen));

  sha3_utils::random_data<uint8_t>(msg0, ilen0);
  sha3_utils::random_data<uint8_t>(msg1, ilen1);
  std::memset(dig, 0, olen);

  // Create Shake256 object, enabling incremental API ( see template argument
  // true ) and start consuming message bytes, using absorb API. Calling
  // absorb API N -many times, each time with M -bytes data and then finalizing
  // it, should yield same digest as it would in case when one uses one-shot API
  // ( default shake256 object creation ) and consumes whole N*M -bytes message
  // ( concatenated in order ) at a time.
  shake256::shake256<true> hasher{};
  hasher.absorb(msg0, ilen0);
  hasher.absorb(msg1, ilen1);
  hasher.finalize();

  // squeeze total `olen` -bytes out of sponge, a single byte at a time
  for (size_t i = 0; i < olen; i++) {
    hasher.read(dig + i, 1);
  }

  std::cout << "Incremental SHAKE-256" << std::endl << std::endl;
  std::cout << "Input 0  : " << sha3_utils::to_hex(msg0, ilen0) << std::endl;
  std::cout << "Input 1  : " << sha3_utils::to_hex(msg1, ilen1) << std::endl;
  std::cout << "Output   : " << sha3_utils::to_hex(dig, olen) << std::endl;

  std::free(msg0);
  std::free(msg1);
  std::free(dig);

  return EXIT_SUCCESS;
}
