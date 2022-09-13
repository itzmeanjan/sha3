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

}
