#include "shake256.hpp"
#include "utils.hpp"
#include <iostream>
#include <vector>

// Compile it using
//
// g++ -std=c++20 -Wall -O3 -march=native -I include example/shake256.cpp
int
main()
{
  constexpr size_t ilen = 32;
  constexpr size_t olen = 40;

  std::vector<uint8_t> msg(ilen, 0);
  std::vector<uint8_t> dig(olen, 0);

  sha3_utils::random_data<uint8_t>(msg.data(), msg.size());

  // Create shake256 hasher
  shake256::shake256 hasher;

  // Absorb message bytes into sponge state
  hasher.absorb(msg.data(), msg.size());
  // Finalize sponge state
  hasher.finalize();

  // Squeeze total `olen` -bytes out of sponge, a single byte at a time.
  // One can request arbitrary many bytes of output, by calling `squeeze`
  // arbitrary many times.
  for (size_t i = 0; i < olen; i++) {
    hasher.squeeze(dig.data() + i, 1);
  }

  std::cout << "SHAKE-256" << std::endl << std::endl;
  std::cout << "Input  : " << sha3_utils::to_hex(msg.data(), ilen) << "\n";
  std::cout << "Output : " << sha3_utils::to_hex(dig.data(), olen) << "\n";

  return EXIT_SUCCESS;
}
