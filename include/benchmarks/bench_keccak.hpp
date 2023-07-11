#pragma once
#include "keccak.hpp"
#include "utils.hpp"
#include <benchmark/benchmark.h>

// Benchmarks SHA3 functions targeting CPU systems, using google-benchmark.
namespace bench_sha3 {

// Benchmarks Keccak-p[1600, 24] permutation.
inline void
keccakf1600(benchmark::State& state)
{
  uint64_t st[keccak::LANE_CNT];
  sha3_utils::random_data(st, keccak::LANE_CNT);

  for (auto _ : state) {
    keccak::permute(st);

    benchmark::DoNotOptimize(st);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * sizeof(st);
  state.SetBytesProcessed(bytes_processed);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_processed;
#endif

#ifdef INSTRUCTIONS_PER_CYCLE
  const double ipc = state.counters["INSTRUCTIONS"] / state.counters["CYCLES"];
  state.counters["INSTRUCTIONS/ CYCLE"] = ipc;
#endif
}

}
