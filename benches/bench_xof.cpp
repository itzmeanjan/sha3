#include "bench_common.hpp"
#include "shake128.hpp"
#include "shake256.hpp"
#include <benchmark/benchmark.h>

// Benchmarks SHAKE-128 extendable output function with variable length input
// and squeezed output.
//
// Note, all input bytes are absorbed in a single call to `absorb` function.
// And all output bytes are squeezed in a single call to `squeeze` function.
void
bench_shake128(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  const size_t olen = static_cast<size_t>(state.range(1));

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out(olen);

  sha3_utils::random_data<uint8_t>(msg);

  for (auto _ : state) {
    shake128::shake128_t hasher;
    hasher.absorb(msg);
    hasher.finalize();
    hasher.squeeze(out);

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(out);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + out.size());
  state.SetBytesProcessed(bytes_processed);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_processed;
#endif
}

// Benchmarks SHAKE-256 extendable output function with variable length input
// and squeezed output.
//
// Note, all input bytes are absorbed in a single call to `absorb` function.
// And all output bytes are squeezed in a single call to `squeeze` function.
void
bench_shake256(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  const size_t olen = static_cast<size_t>(state.range(1));

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out(olen);

  sha3_utils::random_data<uint8_t>(msg);

  for (auto _ : state) {
    shake256::shake256_t hasher;
    hasher.absorb(msg);
    hasher.finalize();
    hasher.squeeze(out);

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(out);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + out.size());
  state.SetBytesProcessed(bytes_processed);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_processed;
#endif
}

BENCHMARK(bench_shake128)
  ->ArgsProduct({ benchmark::CreateRange(64, 16384, 4), { 64 } })
  ->Name("shake128")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_shake256)
  ->ArgsProduct({ benchmark::CreateRange(64, 16384, 4), { 64 } })
  ->Name("shake256")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
