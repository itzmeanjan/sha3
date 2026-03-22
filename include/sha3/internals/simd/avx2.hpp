#pragma once

#if defined(__AVX2__)

#include "sha3/internals/force_inline.hpp"
#include <cstdint>
#include <cstring>
#include <immintrin.h>
#include <span>

// AVX2 SIMD vector wrapper for 4x64-bit lanes.
// Zero-cost abstraction: every method is forceinline and wraps a single intrinsic.
namespace sha3_simd::avx2 {

struct vec
{
private:
  __m256i v;

public:
  forceinline vec()
    : v(_mm256_setzero_si256())
  {
  }

  forceinline explicit vec(__m256i x)
    : v(x)
  {
  }

  forceinline explicit operator __m256i() const { return v; }

  // --- Factories ---

  static forceinline vec broadcast(uint64_t x) { return vec(_mm256_set1_epi64x(static_cast<long long>(x))); }

  // e0 = bits[63:0], e1 = bits[127:64], e2 = bits[191:128], e3 = bits[255:192].
  static forceinline vec set(uint64_t e0, uint64_t e1, uint64_t e2, uint64_t e3)
  {
    return vec(_mm256_set_epi64x(static_cast<long long>(e3), static_cast<long long>(e2), static_cast<long long>(e1), static_cast<long long>(e0)));
  }

  static forceinline vec load(std::span<const uint64_t, 4> data)
  {
    __m256i result;
    std::memcpy(&result, data.data(), sizeof(__m256i));

    return vec(result);
  }

  // --- Bitwise operators ---

  forceinline vec operator^(vec rhs) const { return vec(_mm256_xor_si256(v, rhs.v)); }
  forceinline vec& operator^=(vec rhs)
  {
    *this = *this ^ rhs;
    return *this;
  }

  forceinline vec operator|(vec rhs) const { return vec(_mm256_or_si256(v, rhs.v)); }
  forceinline vec& operator|=(vec rhs)
  {
    *this = *this | rhs;
    return *this;
  }

  forceinline vec operator&(vec rhs) const { return vec(_mm256_and_si256(v, rhs.v)); }
  forceinline vec& operator&=(vec rhs)
  {
    *this = *this & rhs;
    return *this;
  }

  // Per-lane 64-bit shifts.
  forceinline vec operator<<(int n) const { return vec(_mm256_slli_epi64(v, n)); }
  forceinline vec operator>>(int n) const { return vec(_mm256_srli_epi64(v, n)); }

  // --- Lane extraction ---

  template<int lane>
  [[nodiscard]] forceinline uint64_t extract() const
  {
    return static_cast<uint64_t>(_mm256_extract_epi64(v, lane));
  }

  // ~a & b (note: _mm256_andnot_si256 computes ~first & second).
  friend forceinline vec andnot(vec a, vec b) { return vec(_mm256_andnot_si256(a.v, b.v)); }

  // Rotate left each 64-bit lane by n bits.
  friend forceinline vec rotl64(vec x, int n) { return (x << n) | (x >> (64 - n)); }
};

}

#endif
