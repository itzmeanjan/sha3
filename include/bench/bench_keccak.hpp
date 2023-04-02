#pragma once
#include "keccak.hpp"
#include "utils.hpp"
#include <benchmark/benchmark.h>

// Benchmarks SHA3 functions on CPU systems, using google-benchmark
namespace bench_sha3 {

// Benchmarks Keccak-p[1600, 24] permutation
inline void
keccakf1600(benchmark::State& state)
{
  uint64_t st[25];
  sha3_utils::random_data(st, keccak::LANE_CNT);

  for (auto _ : state) {
    keccak::permute(st);

    benchmark::DoNotOptimize(st);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * sizeof(st);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));
}

}
