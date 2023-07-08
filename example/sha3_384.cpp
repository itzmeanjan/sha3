#include "sha3_384.hpp"
#include "utils.hpp"
#include <iostream>
#include <vector>

// Compile it using
//
// g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_384.cpp
int
main()
{
  constexpr size_t ilen = 32;
  constexpr size_t olen = sha3_384::DIGEST_LEN;

  std::vector<uint8_t> msg(ilen, 0);
  std::vector<uint8_t> dig(olen, 0);

  sha3_utils::random_data<uint8_t>(msg.data(), msg.size());

  sha3_384::sha3_384 hasher;
  hasher.absorb(msg.data(), msg.size());
  hasher.finalize();
  hasher.digest(dig.data());

  std::cout << "SHA3-384" << std::endl << std::endl;
  std::cout << "Input  : " << sha3_utils::to_hex(msg.data(), ilen) << "\n";
  std::cout << "Output : " << sha3_utils::to_hex(dig.data(), olen) << "\n";

  return EXIT_SUCCESS;
}
