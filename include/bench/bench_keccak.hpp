#pragma once
#include "keccak.hpp"
#include "utils.hpp"
#include "x86_64_cpu_cycles.hpp"
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

#if defined __x86_64__
  uint64_t total_cycles = 0ul;
#endif

  for (auto _ : state) {
#if defined __x86_64__
    const uint64_t start = cpu_cycles();
#endif

    keccak::permute(st);

#if defined __x86_64__
    const uint64_t end = cpu_cycles();
    total_cycles += (end - start);
#endif

    benchmark::DoNotOptimize(st);
    benchmark::ClobberMemory();
  }

  const size_t tot_bytes = state.iterations() * sizeof(st);
  state.SetBytesProcessed(static_cast<int64_t>(tot_bytes));

#if defined __x86_64__
  total_cycles /= static_cast<uint64_t>(state.iterations());
  state.counters["average_cpu_cycles"] = static_cast<double>(total_cycles);
#endif
}

}
