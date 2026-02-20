#include "sha3/turboshake128.hpp"
#include "example_helper.hpp"
#include <iostream>
#include <numeric>
#include <vector>

// Compile it using
//
// g++ -std=c++20 -Wall -O3 -march=native -I include examples/turboshake128.cpp
int
main()
{
  constexpr size_t msg_len = 32;
  constexpr size_t out_len = 40;

  std::vector<uint8_t> msg(msg_len, 0);
  std::iota(msg.begin(), msg.end(), 0);

  std::vector<uint8_t> out(out_len, 0);
  auto out_span = std::span(out);

  // Create TurboSHAKE128 hasher
  turboshake128::turboshake128_t hasher;

  // Absorb message bytes into sponge state
  hasher.absorb(msg);
  // Finalize sponge state
  hasher.finalize();

  // Squeeze total `out_len` -bytes out of sponge, a single byte at a time.
  // One can request arbitrary many bytes of output, by calling `squeeze` arbitrary
  // many times, after it has been finalized.
  for (size_t i = 0; i < out_len; i++) {
    hasher.squeeze(out_span.subspan(i, 1));
  }

  std::cout << "TurboSHAKE128\n\n";
  std::cout << "Message  : " << to_hex(msg) << "\n";
  std::cout << "Output   : " << to_hex(out) << "\n";

  return EXIT_SUCCESS;
}
