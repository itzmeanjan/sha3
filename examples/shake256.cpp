#include "sha3/shake256.hpp"
#include "example_helper.hpp"
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
  auto _dig = std::span(dig);

  random_data<uint8_t>(msg);

  // Create shake256 hasher
  shake256::shake256_t hasher;

  // Absorb message bytes into sponge state
  hasher.absorb(msg);
  // Finalize sponge state
  hasher.finalize();

  // Squeeze total `olen` -bytes out of sponge, a single byte at a time.
  // One can request arbitrary many bytes of output, by calling `squeeze`
  // arbitrary many times.
  for (size_t i = 0; i < olen; i++) {
    hasher.squeeze(_dig.subspan(i, 1));
  }

  std::cout << "SHAKE-256" << std::endl << std::endl;
  std::cout << "Input  : " << to_hex(msg) << "\n";
  std::cout << "Output : " << to_hex(dig) << "\n";

  return EXIT_SUCCESS;
}
