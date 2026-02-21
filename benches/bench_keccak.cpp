#include "bench_common.hpp"
#include "sha3/internals/keccak.hpp"
#include <benchmark/benchmark.h>
#include <cstdint>

namespace {

// Benchmarks Keccak-p[1600, 12] or Keccak-p[1600, 24] permutation.
template<size_t num_rounds>
void
bench_keccak_permutation(benchmark::State& state)
{
  std::array<uint64_t, keccak::LANE_CNT> st{};
  generate_random_data<uint64_t>(st);

  for (auto _ : state) {
    keccak::permute<num_rounds>(st);

    benchmark::DoNotOptimize(st);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * sizeof(st);
  state.SetBytesProcessed(static_cast<int64_t>(bytes_processed));

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / static_cast<double>(bytes_processed);
#endif
}

}

BENCHMARK(bench_keccak_permutation<12>)->Name("keccak-p[1600, 12]")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_keccak_permutation<24>)->Name("keccak-p[1600, 24]")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
