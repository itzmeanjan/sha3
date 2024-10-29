#include "bench_common.hpp"
#include "keccak.hpp"
#include "utils.hpp"
#include <benchmark/benchmark.h>

// Benchmarks Keccak-p[1600, 24] permutation.
void
bench_keccak_permutation(benchmark::State& state)
{
  uint64_t st[keccak::LANE_CNT]{};
  sha3_utils::random_data<uint64_t>(st);

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
}

BENCHMARK(bench_keccak_permutation)
  ->Name("keccak-p[1600, 24]")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
