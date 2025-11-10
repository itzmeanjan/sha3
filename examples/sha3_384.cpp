#include "sha3/sha3_384.hpp"
#include "example_helper.hpp"
#include <iostream>
#include <numeric>
#include <vector>

// Compile it using
//
// g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_384.cpp
int
main()
{
  constexpr size_t msg_len = 32;

  std::vector<uint8_t> msg(msg_len, 0);
  std::iota(msg.begin(), msg.end(), 0);

  auto md = sha3_384::sha3_384_t::hash(msg);

  // Or do following, if you want to absorb message in multiple calls.
  //
  // std::array<uint8_t, sha3_384::DIGEST_LEN> md{};
  // sha3_384::sha3_384_t hasher;
  //
  // hasher.absorb(msg);
  // hasher.finalize();
  // hasher.digest(md);

  std::cout << "SHA3-384" << std::endl << std::endl;
  std::cout << "Message        : " << to_hex(msg) << "\n";
  std::cout << "Message Digest : " << to_hex(md) << "\n";

  return EXIT_SUCCESS;
}
