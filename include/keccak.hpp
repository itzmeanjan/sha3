#pragma once
#include <cmath>
#include <cstddef>
#include <cstdint>

// Keccak-p[1600, 24] permutation
namespace keccak {

// Bit width of each lane of Keccak-p[1600, 24] state
constexpr size_t LANE_SIZE = 64;

// Keccak-p[b, nr] permutation to be applied `nr` ( = 24 ) rounds
// s.t. b = 1600, w = b/ 25, l = log2(w), nr = 12 + 2l
constexpr size_t ROUNDS = 12 + 2 * 6;

// Leftwards circular rotation offset of 25 lanes ( each lane is 64 -bit wide )
// of state array, as provided in table 2 below algorithm 2 in section 3.2.2 of
// https://dx.doi.org/10.6028/NIST.FIPS.202
//
// Note, following offsets are obtained by performing % 64 ( bit width of lane )
// on offsets provided in above mentioned link
constexpr size_t ROT[]{ 0 % LANE_SIZE,   1 % LANE_SIZE,   190 % LANE_SIZE,
                        28 % LANE_SIZE,  91 % LANE_SIZE,  36 % LANE_SIZE,
                        300 % LANE_SIZE, 6 % LANE_SIZE,   55 % LANE_SIZE,
                        276 % LANE_SIZE, 3 % LANE_SIZE,   10 % LANE_SIZE,
                        171 % LANE_SIZE, 153 % LANE_SIZE, 231 % LANE_SIZE,
                        105 % LANE_SIZE, 45 % LANE_SIZE,  15 % LANE_SIZE,
                        21 % LANE_SIZE,  136 % LANE_SIZE, 210 % LANE_SIZE,
                        66 % LANE_SIZE,  253 % LANE_SIZE, 120 % LANE_SIZE,
                        78 % LANE_SIZE };

// Computes single bit of Keccak-p[1600, 24] round constant ( at compile-time ),
// using binary LFSR, defined by primitive polynomial x^8 + x^6 + x^5 + x^4 + 1
//
// See algorithm 5 in section 3.2.5 of http://dx.doi.org/10.6028/NIST.FIPS.202
//
// Taken from
// https://github.com/itzmeanjan/elephant/blob/2a21c7e/include/keccak.hpp#L24-L59
consteval static bool
rc(const size_t t)
{
  // step 1 of algorithm 5
  if (t % 255 == 0) {
    return 1;
  }

  // step 2 of algorithm 5
  //
  // note, step 3.a of algorithm 5 is also being
  // executed in this statement ( for first iteration, with i = 1 ) !
  uint16_t r = 0b10000000;

  // step 3 of algorithm 5
  for (size_t i = 1; i <= t % 255; i++) {
    const uint16_t b0 = r & 1;

    r = (r & 0b011111111) ^ ((((r >> 8) & 1) ^ b0) << 8);
    r = (r & 0b111101111) ^ ((((r >> 4) & 1) ^ b0) << 4);
    r = (r & 0b111110111) ^ ((((r >> 3) & 1) ^ b0) << 3);
    r = (r & 0b111111011) ^ ((((r >> 2) & 1) ^ b0) << 2);

    // step 3.f of algorithm 5
    //
    // note, this statement also executes step 3.a for upcoming
    // iterations ( i.e. when i > 1 )
    r >>= 1;
  }

  return static_cast<bool>((r >> 7) & 1);
}

}
