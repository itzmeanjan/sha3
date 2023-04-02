#pragma once
#include "sha3_224.hpp"
#include "sha3_256.hpp"
#include "sha3_384.hpp"
#include "sha3_512.hpp"
#include "shake128.hpp"
#include "shake256.hpp"
#include <benchmark/benchmark.h>

// Benchmarks SHA3 functions on CPU systems, using google-benchmark
namespace bench_sha3 {

// Benchmarks SHA3-224 hash function with specified number of random input bytes
inline void
sha3_224(benchmark::State& state)
{
  constexpr size_t dlen = 28;
  const size_t mlen = static_cast<size_t>(state.range(0));
  assert(mlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(dlen));

  sha3_utils::random_data(msg, mlen);

  for (auto _ : state) {
    sha3_224::hash(msg, mlen, dig);

    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(dig);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * (mlen + dlen);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

  std::free(msg);
  std::free(dig);
}

// Benchmarks SHA3-256 hash function with specified number of random input bytes
inline void
sha3_256(benchmark::State& state)
{
  constexpr size_t dlen = 32;
  const size_t mlen = static_cast<size_t>(state.range(0));
  assert(mlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(dlen));

  sha3_utils::random_data(msg, mlen);

  for (auto _ : state) {
    sha3_256::hash(msg, mlen, dig);

    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(dig);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * (mlen + dlen);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

  std::free(msg);
  std::free(dig);
}

// Benchmarks SHA3-384 hash function with specified number of random input bytes
inline void
sha3_384(benchmark::State& state)
{
  constexpr size_t dlen = 48;
  const size_t mlen = static_cast<size_t>(state.range(0));
  assert(mlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(dlen));

  sha3_utils::random_data(msg, mlen);

  for (auto _ : state) {
    sha3_384::hash(msg, mlen, dig);

    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(dig);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * (mlen + dlen);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

  std::free(msg);
  std::free(dig);
}

// Benchmarks SHA3-512 hash function with specified number of random input bytes
inline void
sha3_512(benchmark::State& state)
{
  constexpr size_t dlen = 64;
  const size_t mlen = static_cast<size_t>(state.range(0));
  assert(mlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(dlen));

  sha3_utils::random_data(msg, mlen);

  for (auto _ : state) {
    sha3_512::hash(msg, mlen, dig);

    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(dig);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * (mlen + dlen);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

  std::free(msg);
  std::free(dig);
}

// Benchmarks SHAKE-128 extendable output function with specified number of
// input message bytes & output digest bytes.
//
// Note, all input bytes are absorbed in a single call to `hash` function.
// And all output bytes are squeezed in a single call to `read` function.
inline void
shake128(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  const size_t dlen = static_cast<size_t>(state.range(1));

  assert(mlen > 0);
  assert(dlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(dlen));

  sha3_utils::random_data(msg, mlen);

  for (auto _ : state) {
    shake128::shake128 hasher;
    benchmark::DoNotOptimize(hasher);

    hasher.hash(msg, mlen);
    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);

    hasher.read(dig, dlen);
    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(dig);

    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * (mlen + dlen);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

  std::free(msg);
  std::free(dig);
}

// Benchmarks SHAKE-256 extendable output function with specified number of
// input message bytes & output digest bytes.
//
// Note, all input bytes are absorbed in a single call to `hash` function.
// And all output bytes are squeezed in a single call to `read` function.
inline void
shake256(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  const size_t dlen = static_cast<size_t>(state.range(1));

  assert(mlen > 0);
  assert(dlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(dlen));

  sha3_utils::random_data(msg, mlen);

  for (auto _ : state) {
    shake256::shake256 hasher;
    benchmark::DoNotOptimize(hasher);

    hasher.hash(msg, mlen);
    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);

    hasher.read(dig, dlen);
    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(dig);

    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * (mlen + dlen);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

  std::free(msg);
  std::free(dig);
}

}
