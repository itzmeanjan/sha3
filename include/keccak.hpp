#pragma once
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>

// Keccak-p[1600, 24] permutation
namespace keccak {

// Bit width of each lane of Keccak-p[1600, 24] state
constexpr size_t LANE_SIZE = 64;

// Logarithmic base 2 of bit width of lane i.e. log2(LANE_SIZE)
constexpr size_t L = 6;

// Keccak-p[b, nr] permutation to be applied `nr` ( = 24 ) rounds
// s.t. b = 1600, w = b/ 25, l = log2(w), nr = 12 + 2l
constexpr size_t ROUNDS = 12 + 2 * L;

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

// Computes 64 -bit round constant ( at compile-time ), which is XOR-ed into
// very first lane ( = lane(0, 0) ) of Keccak-p[1600, 24] permutation state
//
// Taken from
// https://github.com/itzmeanjan/elephant/blob/2a21c7e/include/keccak.hpp#L61-L74
consteval static uint64_t
compute_rc(const size_t r_idx)
{
  uint64_t tmp = 0;

  for (size_t j = 0; j < (L + 1); j++) {
    const size_t boff = (1 << j) - 1;
    tmp |= static_cast<uint64_t>(rc(j + 7 * r_idx)) << boff;
  }

  return tmp;
}

// Round constants to be XORed with lane (0, 0) of keccak-p[1600, 24]
// permutation state, see section 3.2.5 of
// https://dx.doi.org/10.s6028/NIST.FIPS.202
constexpr uint64_t RC[ROUNDS]{ compute_rc(0),  compute_rc(1),  compute_rc(2),
                               compute_rc(3),  compute_rc(4),  compute_rc(5),
                               compute_rc(6),  compute_rc(7),  compute_rc(8),
                               compute_rc(9),  compute_rc(10), compute_rc(11),
                               compute_rc(12), compute_rc(13), compute_rc(14),
                               compute_rc(15), compute_rc(16), compute_rc(17),
                               compute_rc(18), compute_rc(19), compute_rc(20),
                               compute_rc(21), compute_rc(22), compute_rc(23) };

// Keccak-p[1600, 24] step mapping function θ, see section 3.2.1 of SHA3
// specification https://dx.doi.org/10.6028/NIST.FIPS.202
inline static void
theta(uint64_t* const state)
{
  uint64_t c[5];
  uint64_t d[5];

  for (size_t i = 0; i < 5; i++) {
    const uint64_t t0 = state[i] ^ state[i + 5];
    const uint64_t t1 = state[i + 10] ^ state[i + 15];
    const uint64_t t2 = t0 ^ t1 ^ state[i + 20];

    c[i] = t2;
  }

  for (size_t i = 0; i < 5; i++) {
    const size_t pidx = (i + 4) % 5;
    const size_t nidx = (i + 1) % 5;

    d[i] = c[pidx] ^ std::rotl(c[nidx], 1);
  }

  for (size_t i = 0; i < 5; i++) {
    state[i + 0] ^= d[i];
    state[i + 5] ^= d[i];
    state[i + 10] ^= d[i];
    state[i + 15] ^= d[i];
    state[i + 20] ^= d[i];
  }
}

// Keccak-p[1600, 24] step mapping function ρ, see section 3.2.2 of SHA3
// specification https://dx.doi.org/10.6028/NIST.FIPS.202
inline static void
rho(uint64_t* const state)
{
  for (size_t i = 0; i < 25; i++) {
    state[i] = std::rotl(state[i], ROT[i]);
  }
}

// Keccak-p[1600, 24] step mapping function π, see section 3.2.3 of SHA3
// specification https://dx.doi.org/10.6028/NIST.FIPS.202
inline static void
pi(const uint64_t* const __restrict istate, // input permutation state
   uint64_t* const __restrict ostate        // output permutation state
)
{
  for (size_t i = 0; i < 5; i++) {
    const size_t ix3 = i * 3;
    const size_t ix5 = i * 5;

    for (size_t j = 0; j < 5; j++) {
      ostate[ix5 + j] = istate[5 * j + (ix3 + j) % 5];
    }
  }
}

// Keccak-p[1600, 24] step mapping function χ, see section 3.2.4 of SHA3
// specification https://dx.doi.org/10.6028/NIST.FIPS.202
inline static void
chi(const uint64_t* const __restrict istate, // input permutation state
    uint64_t* const __restrict ostate        // output permutation state
)
{
  for (size_t i = 0; i < 5; i++) {
    const size_t ix5 = i * 5;

    for (size_t j = 0; j < 5; j++) {
      const size_t j0 = (j + 1) % 5;
      const size_t j1 = (j + 2) % 5;

      const uint64_t rhs = ~istate[ix5 + j0] & istate[ix5 + j1];
      ostate[ix5 + j] = istate[ix5 + j] ^ rhs;
    }
  }
}

// Keccak-p[1600, 24] step mapping function ι, see section 3.2.5 of SHA3
// specification https://dx.doi.org/10.6028/NIST.FIPS.202
inline static void
iota(uint64_t* const state, const size_t r_idx)
{
  state[0] ^= RC[r_idx];
}

}
