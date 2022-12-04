#pragma once
#include "sha3_224.hpp"
#include "sha3_256.hpp"
#include "sha3_384.hpp"
#include "sha3_512.hpp"
#include "shake128.hpp"
#include "shake256.hpp"
#include "utils.hpp"
#include "x86_64_cpu_cycles.hpp"
#include <benchmark/benchmark.h>

// Benchmarks SHA3 functions on CPU systems, using google-benchmark
namespace bench_sha3 {

// Benchmarks SHA3-224 hash function with specified number of random input bytes
static void
sha3_224(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  assert(mlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(28));

  random_data<uint8_t>(msg, mlen);
  std::memset(dig, 0, 28);

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    sha3_224::hash(msg, mlen, dig);

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(dig);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * mlen;
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(msg);
  std::free(dig);
}

// Benchmarks SHA3-256 hash function with specified number of random input bytes
static void
sha3_256(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  assert(mlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(32));

  random_data<uint8_t>(msg, mlen);
  std::memset(dig, 0, 32);

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    sha3_256::hash(msg, mlen, dig);

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(dig);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * mlen;
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(msg);
  std::free(dig);
}

// Benchmarks SHA3-384 hash function with specified number of random input bytes
static void
sha3_384(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  assert(mlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(48));

  random_data<uint8_t>(msg, mlen);
  std::memset(dig, 0, 48);

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    sha3_384::hash(msg, mlen, dig);

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(dig);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * mlen;
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(msg);
  std::free(dig);
}

// Benchmarks SHA3-512 hash function with specified number of random input bytes
static void
sha3_512(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  assert(mlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(64));

  random_data<uint8_t>(msg, mlen);
  std::memset(dig, 0, 64);

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    sha3_512::hash(msg, mlen, dig);

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(dig);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * mlen;
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(msg);
  std::free(dig);
}

// Benchmarks SHAKE-128 extendable output function with specified number of
// input bytes & output digest byte length
//
// Note, each byte of output digest is read at a time.
static void
shake128(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  const size_t dlen = static_cast<size_t>(state.range(1));

  assert(mlen > 0);
  assert(dlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(dlen));

  random_data<uint8_t>(msg, mlen);
  std::memset(dig, 0, dlen);

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    shake128::shake128 hasher{};
    hasher.hash(msg, mlen);

    for (size_t i = 0; i < dlen; i++) {
      hasher.read(dig + i, 1);
    }

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(dig);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * (mlen + dlen);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(msg);
  std::free(dig);
}

// Benchmarks SHAKE-256 extendable output function with specified number of
// input bytes & output digest byte length
//
// Note, each byte of output digest is read at a time.
static void
shake256(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  const size_t dlen = static_cast<size_t>(state.range(1));

  assert(mlen > 0);
  assert(dlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(dlen));

  random_data<uint8_t>(msg, mlen);
  std::memset(dig, 0, dlen);

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    shake256::shake256 hasher{};
    hasher.hash(msg, mlen);

    for (size_t i = 0; i < dlen; i++) {
      hasher.read(dig + i, 1);
    }

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(dig);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * (mlen + dlen);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif

  std::free(msg);
  std::free(dig);
}

}
