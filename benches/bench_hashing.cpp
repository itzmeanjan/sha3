#include "bench_common.hpp"
#include "sha3/sha3_224.hpp"
#include "sha3/sha3_256.hpp"
#include "sha3/sha3_384.hpp"
#include "sha3/sha3_512.hpp"
#include <benchmark/benchmark.h>

// Benchmarks SHA3-224 hash function with variable length input message.
void
bench_sha3_224(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range());

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> md(sha3_224::DIGEST_LEN);
  auto _md = std::span<uint8_t, sha3_224::DIGEST_LEN>(md);

  random_data<uint8_t>(msg);

  for (auto _ : state) {
    sha3_224::sha3_224_t hasher;
    hasher.absorb(msg);
    hasher.finalize();
    hasher.digest(_md);

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(_md);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + md.size());
  state.SetBytesProcessed(bytes_processed);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_processed;
#endif
}

// Benchmarks SHA3-256 hash function with variable length input message.
void
bench_sha3_256(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range());

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> md(sha3_256::DIGEST_LEN);
  auto _md = std::span<uint8_t, sha3_256::DIGEST_LEN>(md);

  random_data<uint8_t>(msg);

  for (auto _ : state) {
    sha3_256::sha3_256_t hasher;
    hasher.absorb(msg);
    hasher.finalize();
    hasher.digest(_md);

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(_md);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + md.size());
  state.SetBytesProcessed(bytes_processed);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_processed;
#endif
}

// Benchmarks SHA3-384 hash function with variable length input message.
void
bench_sha3_384(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range());

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> md(sha3_384::DIGEST_LEN);
  auto _md = std::span<uint8_t, sha3_384::DIGEST_LEN>(md);

  random_data<uint8_t>(msg);

  for (auto _ : state) {
    sha3_384::sha3_384_t hasher;
    hasher.absorb(msg);
    hasher.finalize();
    hasher.digest(_md);

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(_md);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + md.size());
  state.SetBytesProcessed(bytes_processed);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_processed;
#endif
}

// Benchmarks SHA3-512 hash function with variable length input message.
void
bench_sha3_512(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range());

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> md(sha3_512::DIGEST_LEN);
  auto _md = std::span<uint8_t, sha3_512::DIGEST_LEN>(md);

  random_data<uint8_t>(msg);

  for (auto _ : state) {
    sha3_512::sha3_512_t hasher;
    hasher.absorb(msg);
    hasher.finalize();
    hasher.digest(_md);

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(_md);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + md.size());
  state.SetBytesProcessed(bytes_processed);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_processed;
#endif
}

BENCHMARK(bench_sha3_224)->RangeMultiplier(4)->Range(64, 16384)->Name("sha3_224")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_sha3_256)->RangeMultiplier(4)->Range(64, 16384)->Name("sha3_256")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_sha3_384)->RangeMultiplier(4)->Range(64, 16384)->Name("sha3_384")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
BENCHMARK(bench_sha3_512)->RangeMultiplier(4)->Range(64, 16384)->Name("sha3_512")->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
