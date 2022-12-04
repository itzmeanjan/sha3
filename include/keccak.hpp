#pragma once
#include <bit>
#include <cstddef>
#include <cstdint>

#if !defined __AVX2__
#include <immintrin.h>
#endif

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
constexpr size_t ROT[]{ 0 % LANE_SIZE,   1 % LANE_SIZE,   190 % LANE_SIZE, 28 % LANE_SIZE,
                        91 % LANE_SIZE,  36 % LANE_SIZE,  300 % LANE_SIZE, 6 % LANE_SIZE,
                        55 % LANE_SIZE,  276 % LANE_SIZE, 3 % LANE_SIZE,   10 % LANE_SIZE,
                        171 % LANE_SIZE, 153 % LANE_SIZE, 231 % LANE_SIZE, 105 % LANE_SIZE,
                        45 % LANE_SIZE,  15 % LANE_SIZE,  21 % LANE_SIZE,  136 % LANE_SIZE,
                        210 % LANE_SIZE, 66 % LANE_SIZE,  253 % LANE_SIZE, 120 % LANE_SIZE,
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
constexpr uint64_t RC[ROUNDS]{ compute_rc(0),  compute_rc(1),  compute_rc(2),  compute_rc(3),
                               compute_rc(4),  compute_rc(5),  compute_rc(6),  compute_rc(7),
                               compute_rc(8),  compute_rc(9),  compute_rc(10), compute_rc(11),
                               compute_rc(12), compute_rc(13), compute_rc(14), compute_rc(15),
                               compute_rc(16), compute_rc(17), compute_rc(18), compute_rc(19),
                               compute_rc(20), compute_rc(21), compute_rc(22), compute_rc(23) };

// Keccak-p[1600, 24] step mapping function θ, see section 3.2.1 of SHA3
// specification https://dx.doi.org/10.6028/NIST.FIPS.202
inline static void
theta(uint64_t* const state)
{
  uint64_t c[5]{};
  uint64_t d[5];

  for (size_t i = 0; i < 25; i += 5) {
    c[0] ^= state[i + 0];
    c[1] ^= state[i + 1];
    c[2] ^= state[i + 2];
    c[3] ^= state[i + 3];
    c[4] ^= state[i + 4];
  }

  for (size_t i = 0; i < 5; i++) {
    const size_t pidx = (i + 4) % 5;
    const size_t nidx = (i + 1) % 5;

    d[i] = c[pidx] ^ std::rotl(c[nidx], 1);
  }

  for (size_t i = 0; i < 25; i += 5) {
    state[i + 0] ^= d[0];
    state[i + 1] ^= d[1];
    state[i + 2] ^= d[2];
    state[i + 3] ^= d[3];
    state[i + 4] ^= d[4];
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

// Keccak-p[1600, 24] round function, which applies all five
// step mapping functions in order, updates state array
//
// See section 3.3 of https://dx.doi.org/10.6028/NIST.FIPS.202
inline static void
round(uint64_t* const state, const size_t r_idx)
{
  uint64_t tmp[25]{};

  theta(state);
  rho(state);
  pi(state, tmp);
  chi(tmp, state);
  iota(state, r_idx);
}

// Keccak-p[1600, 24] permutation, applying 24 rounds of permutation
// on state of dimension 5 x 5 x 64 ( = 1600 ) -bits, using algorithm 7 defined
// in section 3.3 of SHA3 specification https://dx.doi.org/10.6028/NIST.FIPS.202
inline static void
permute(uint64_t* const state)
{

#if !defined __AVX2__

  alignas(32) constexpr uint64_t bit_widths[]{ 64, 64, 64, 64 };
  const auto bw = _mm256_load_si256((__m256i*)bit_widths);

  alignas(32) uint64_t rot[28]{};
  std::memcpy(rot, ROT, 25 * sizeof(uint64_t));

  const auto shl0 = _mm256_loadu_si256((__m256i*)(rot + 0));
  const auto shl4 = _mm256_loadu_si256((__m256i*)(rot + 4));
  const auto shl5 = _mm256_loadu_si256((__m256i*)(rot + 5));
  const auto shl9 = _mm256_loadu_si256((__m256i*)(rot + 9));
  const auto shl10 = _mm256_loadu_si256((__m256i*)(rot + 10));
  const auto shl14 = _mm256_loadu_si256((__m256i*)(rot + 14));
  const auto shl15 = _mm256_loadu_si256((__m256i*)(rot + 15));
  const auto shl19 = _mm256_loadu_si256((__m256i*)(rot + 19));
  const auto shl20 = _mm256_loadu_si256((__m256i*)(rot + 20));
  const auto shl24 = _mm256_loadu_si256((__m256i*)(rot + 24));

  const auto shr0 = _mm256_sub_epi64(bw, shl0);
  const auto shr4 = _mm256_sub_epi64(bw, shl4);
  const auto shr5 = _mm256_sub_epi64(bw, shl5);
  const auto shr9 = _mm256_sub_epi64(bw, shl9);
  const auto shr10 = _mm256_sub_epi64(bw, shl10);
  const auto shr14 = _mm256_sub_epi64(bw, shl14);
  const auto shr15 = _mm256_sub_epi64(bw, shl15);
  const auto shr19 = _mm256_sub_epi64(bw, shl19);
  const auto shr20 = _mm256_sub_epi64(bw, shl20);
  const auto shr24 = _mm256_sub_epi64(bw, shl24);

  alignas(32) uint64_t tmp[28]{};
  std::memcpy(tmp, state, 25 * sizeof(uint64_t));

  auto s0 = _mm256_load_si256((__m256i*)(tmp + 0));
  auto s4 = _mm256_load_si256((__m256i*)(tmp + 4));
  auto s8 = _mm256_load_si256((__m256i*)(tmp + 8));
  auto s12 = _mm256_load_si256((__m256i*)(tmp + 12));
  auto s16 = _mm256_load_si256((__m256i*)(tmp + 16));
  auto s20 = _mm256_load_si256((__m256i*)(tmp + 20));
  auto s24 = _mm256_load_si256((__m256i*)(tmp + 24));

  for (size_t i = 0; i < ROUNDS; i++) {
    // θ step mapping

    const auto t0 = _mm256_blend_epi32(s4, s8, 0b00000011u);
    const auto s5 = _mm256_permute4x64_epi64(t0, 0b00111001);
    const auto t1 = _mm256_blend_epi32(s8, s12, 0b00001111u);
    const auto s10 = _mm256_permute4x64_epi64(t1, 0b01001110u);
    const auto t2 = _mm256_blend_epi32(s12, s16, 0b00111111u);
    const auto s15 = _mm256_permute4x64_epi64(t2, 0b10010011u);

    const auto t3 = _mm256_xor_si256(s0, s5);
    const auto t4 = _mm256_xor_si256(t3, s10);
    const auto t5 = _mm256_xor_si256(t4, s15);
    const auto c0_4 = _mm256_xor_si256(t5, s20); // c[0], c[1], c[2], c[3]

    const auto s19 = _mm256_permute4x64_epi64(s16, 0b10010011u); // s[19], s[16], s[17], s[18]
    const auto s14 = _mm256_permute4x64_epi64(s12, 0b11010010u); // s[14], s[12], s[13], s[15]
    const auto s9 = _mm256_permute4x64_epi64(s8, 0b11100001u);   // s[9], s[8], s[10], s[11]

    const auto t6 = _mm256_xor_si256(s4, s9);
    const auto t7 = _mm256_xor_si256(t6, s14);
    const auto t8 = _mm256_xor_si256(t7, s19);
    const auto c4 = _mm256_xor_si256(t8, s24); // c[4], _, _, _

    const auto t9 = _mm256_slli_epi64(c0_4, 1);
    const auto t10 = _mm256_srli_epi64(c0_4, 63);
    const auto c0_4_ = _mm256_xor_si256(t9, t10); // c'[0], c'[1], c'[2], c'[3]

    const auto t11 = _mm256_slli_epi64(c4, 1);
    const auto t12 = _mm256_srli_epi64(c4, 63);
    const auto c4_ = _mm256_xor_si256(t11, t12); // c'[4], _, _, _

    const auto t13 = _mm256_permute4x64_epi64(c4_, 0b11100001u); // _, c'[4], _, _
    const auto t14 = _mm256_blend_epi32(c0_4_, t13, 0b00001100u);
    const auto t15 = _mm256_permute4x64_epi64(t14, 0b00011110u);

    const auto d1_5 = _mm256_xor_si256(c0_4, t15); // d[1], d[2], d[3], d[4]

    const auto t16 = _mm256_permute4x64_epi64(c0_4_, 0b00111001u); // c'[1], c'[2], c'[3], c'[0]
    const auto d0 = _mm256_xor_si256(c4, t16);                     // d[0], _, _, _

    const auto t17 = _mm256_permute4x64_epi64(d0, 0b00111001u);   // _, _, _, d[0]
    const auto t18 = _mm256_blend_epi32(t17, d1_5, 0b00111111u);  // d[1], d[2], d[3], d[0]
    const auto d0_4 = _mm256_permute4x64_epi64(t18, 0b10010011u); // d[0], d[1], d[2], d[3]

    const auto s0_ = _mm256_xor_si256(s0, d0_4);   // s'[0], s'[1], s'[2], s'[3]
    const auto s5_ = _mm256_xor_si256(s5, d0_4);   // s'[5], s'[6], s'[7], s'[8]
    const auto s10_ = _mm256_xor_si256(s10, d0_4); // s'[10], s'[11], s'[12], s'[13]
    const auto s15_ = _mm256_xor_si256(s15, d0_4); // s'[15], s'[16], s'[17], s'[18]
    const auto s20_ = _mm256_xor_si256(s20, d0_4); // s'[20], s'[21], s'[22], s'[23]

    const auto d4 = _mm256_permute4x64_epi64(d1_5, 0b10010011u); // d[4], d[1], d[2], d[3]
    const auto s4_ = _mm256_xor_si256(s4, d4);                   // s'[4], _, _, _
    const auto s9_ = _mm256_xor_si256(s9, d4);                   // s'[9], _, _, _
    const auto s14_ = _mm256_xor_si256(s14, d4);                 // s'[14], _, _, _
    const auto s19_ = _mm256_xor_si256(s19, d4);                 // s'[19], _, _, _
    const auto s24_ = _mm256_xor_si256(s24, d4);                 // s'[24], _, _, _
  }

  _mm256_store_si256((__m256i*)(tmp + 0), s0);
  _mm256_store_si256((__m256i*)(tmp + 4), s4);
  _mm256_store_si256((__m256i*)(tmp + 8), s8);
  _mm256_store_si256((__m256i*)(tmp + 12), s12);
  _mm256_store_si256((__m256i*)(tmp + 16), s16);
  _mm256_store_si256((__m256i*)(tmp + 20), s20);
  _mm256_store_si256((__m256i*)(tmp + 24), s24);

  std::memcpy(state, tmp, 25 * sizeof(uint64_t));

#else

  for (size_t i = 0; i < ROUNDS; i++) {
    round(state, i);
  }

#endif
}

}
