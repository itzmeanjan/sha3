#include "sha3/sha3_512.hpp"
#include "example_helper.hpp"
#include <iostream>
#include <numeric>
#include <vector>

// Compile it using
//
// g++ -std=c++20 -Wall -O3 -march=native -I include examples/sha3_512.cpp
int
main()
{
  constexpr size_t msg_len = 32;

  std::vector<uint8_t> msg(msg_len, 0);
  std::iota(msg.begin(), msg.end(), 0);

  auto digest = sha3_512::sha3_512_t::hash(msg);

  // Or do following, if you want to absorb message in multiple calls.
  //
  // std::array<uint8_t, sha3_512::DIGEST_LEN> md{};
  // sha3_512::sha3_512_t hasher;
  //
  // hasher.absorb(msg);
  // hasher.finalize();
  // hasher.digest(md);

  std::cout << "SHA3-512\n\n";
  std::cout << "Message        : " << to_hex(msg) << "\n";
  std::cout << "Message Digest : " << to_hex(digest) << "\n";

  return EXIT_SUCCESS;
}
