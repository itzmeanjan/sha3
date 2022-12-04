#pragma once
#include "keccak.hpp"
#include "utils.hpp"
#include <benchmark/benchmark.h>

// Benchmarks SHA3 functions on CPU systems, using google-benchmark
namespace bench_sha3 {

// Benchmarks Keccak-p[1600, 24] permutation
static void
keccakf1600(benchmark::State& state)
{
  uint64_t st[25]{};

  constexpr size_t len = sizeof(st) / sizeof(st[0]);
  random_data<uint64_t>(st, len);

  for (auto _ : state) {
    keccak::permute(st);

    benchmark::DoNotOptimize(st);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * sizeof(st);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));
}

}
