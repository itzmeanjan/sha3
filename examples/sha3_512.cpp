#include "sha3/sha3_512.hpp"
#include "example_helper.hpp"
#include <iostream>
#include <vector>

// Compile it using
//
// g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_512.cpp
int
main()
{
  constexpr size_t ilen = 32;
  constexpr size_t olen = sha3_512::DIGEST_LEN;

  std::vector<uint8_t> msg(ilen, 0);
  std::vector<uint8_t> dig(olen, 0);
  auto _dig = std::span<uint8_t, olen>(dig);

  random_data<uint8_t>(msg);

  sha3_512::sha3_512_t hasher;
  hasher.absorb(msg);
  hasher.finalize();
  hasher.digest(_dig);

  std::cout << "SHA3-512" << std::endl << std::endl;
  std::cout << "Input  : " << to_hex(msg) << "\n";
  std::cout << "Output : " << to_hex(dig) << "\n";

  return EXIT_SUCCESS;
}
