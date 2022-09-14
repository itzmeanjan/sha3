#pragma once
#include "sha3_224.hpp"
#include "utils.hpp"
#include <benchmark/benchmark.h>

// Benchmarks SHA3 functions on CPU systems, using google-benchmark
namespace bench_sha3 {

// Benchmarks SHA3-224 hash function for specified number of random input bytes
static void
sha3_224(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  assert(mlen > 0);

  uint8_t* msg = static_cast<uint8_t*>(std::malloc(mlen));
  uint8_t* dig = static_cast<uint8_t*>(std::malloc(28));

  random_data<uint8_t>(msg, mlen);
  std::memset(dig, 0, 28);

  for (auto _ : state) {
    sha3_224::hash(msg, mlen, dig);

    benchmark::DoNotOptimize(dig);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * mlen;
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));
}

}
