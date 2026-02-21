#include "bench_common.hpp"
#include "sha3/shake128.hpp"
#include "sha3/shake256.hpp"
#include "sha3/turboshake128.hpp"
#include "sha3/turboshake256.hpp"
#include <benchmark/benchmark.h>
#include <cstdint>

namespace {

/**
 * Benchmarks SHAKE128 extendable output function with variable length input and squeezed output.
 *
 * Note, all input bytes are absorbed in a single call to `absorb` function.
 * And all output bytes are squeezed in a single call to `squeeze` function.
 */
void
bench_shake128(benchmark::State& state)
{
  const auto mlen = static_cast<size_t>(state.range(0));
  const auto olen = static_cast<size_t>(state.range(1));

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out(olen);

  generate_random_data<uint8_t>(msg);

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
  state.SetBytesProcessed(static_cast<int64_t>(bytes_processed));

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / static_cast<double>(bytes_processed);
#endif
}

/**
 * Benchmarks SHAKE256 extendable output function with variable length input and squeezed output.
 *
 * Note, all input bytes are absorbed in a single call to `absorb` function.
 * And all output bytes are squeezed in a single call to `squeeze` function.
 */
void
bench_shake256(benchmark::State& state)
{
  const auto mlen = static_cast<size_t>(state.range(0));
  const auto olen = static_cast<size_t>(state.range(1));

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out(olen);

  generate_random_data<uint8_t>(msg);

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
  state.SetBytesProcessed(static_cast<int64_t>(bytes_processed));

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / static_cast<double>(bytes_processed);
#endif
}

/**
 * Benchmarks TurboSHAKE128 extendable output function with variable length input and squeezed output.
 *
 * Note, all input bytes are absorbed in a single call to `absorb` function.
 * And all output bytes are squeezed in a single call to `squeeze` function.
 */
void
bench_turboshake128(benchmark::State& state)
{
  const auto mlen = static_cast<size_t>(state.range(0));
  const auto olen = static_cast<size_t>(state.range(1));

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out(olen);

  generate_random_data<uint8_t>(msg);

  for (auto _ : state) {
    turboshake128::turboshake128_t hasher;
    hasher.absorb(msg);
    hasher.finalize();
    hasher.squeeze(out);

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(out);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + out.size());
  state.SetBytesProcessed(static_cast<int64_t>(bytes_processed));

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / static_cast<double>(bytes_processed);
#endif
}

/**
 * Benchmarks TurboSHAKE256 extendable output function with variable length input and squeezed output.
 *
 * Note, all input bytes are absorbed in a single call to `absorb` function.
 * And all output bytes are squeezed in a single call to `squeeze` function.
 */
void
bench_turboshake256(benchmark::State& state)
{
  const auto mlen = static_cast<size_t>(state.range(0));
  const auto olen = static_cast<size_t>(state.range(1));

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out(olen);

  generate_random_data<uint8_t>(msg);

  for (auto _ : state) {
    turboshake256::turboshake256_t hasher;
    hasher.absorb(msg);
    hasher.finalize();
    hasher.squeeze(out);

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(out);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + out.size());
  state.SetBytesProcessed(static_cast<int64_t>(bytes_processed));

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / static_cast<double>(bytes_processed);
#endif
}

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
BENCHMARK(bench_turboshake128)
  ->ArgsProduct({ benchmark::CreateRange(64, 16384, 4), { 64 } })
  ->Name("turboshake128")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
BENCHMARK(bench_turboshake256)
  ->ArgsProduct({ benchmark::CreateRange(64, 16384, 4), { 64 } })
  ->Name("turboshake256")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
