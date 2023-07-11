#pragma once
#include "sha3_224.hpp"
#include "sha3_256.hpp"
#include "sha3_384.hpp"
#include "sha3_512.hpp"
#include <benchmark/benchmark.h>
#include <vector>

// Benchmarks SHA3 functions targeting CPU systems, using google-benchmark.
namespace bench_sha3 {

// Benchmarks SHA3-224 hash function with variable length input message.
inline void
sha3_224(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range());

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> md(sha3_224::DIGEST_LEN);

  sha3_utils::random_data(msg.data(), msg.size());

  for (auto _ : state) {
    sha3_224::sha3_224 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.digest(md.data());

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(md);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + md.size());
  state.SetBytesProcessed(bytes_processed);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_processed;
#endif

#ifdef INSTRUCTIONS_PER_CYCLE
  const double ipc = state.counters["INSTRUCTIONS"] / state.counters["CYCLES"];
  state.counters["INSTRUCTIONS/ CYCLE"] = ipc;
#endif
}

// Benchmarks SHA3-256 hash function with variable length input message.
inline void
sha3_256(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range());

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> md(sha3_256::DIGEST_LEN);

  sha3_utils::random_data(msg.data(), msg.size());

  for (auto _ : state) {
    sha3_256::sha3_256 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.digest(md.data());

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(md);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + md.size());
  state.SetBytesProcessed(bytes_processed);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_processed;
#endif

#ifdef INSTRUCTIONS_PER_CYCLE
  const double ipc = state.counters["INSTRUCTIONS"] / state.counters["CYCLES"];
  state.counters["INSTRUCTIONS/ CYCLE"] = ipc;
#endif
}

// Benchmarks SHA3-384 hash function with variable length input message.
inline void
sha3_384(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range());

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> md(sha3_384::DIGEST_LEN);

  sha3_utils::random_data(msg.data(), msg.size());

  for (auto _ : state) {
    sha3_384::sha3_384 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.digest(md.data());

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(md);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + md.size());
  state.SetBytesProcessed(bytes_processed);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_processed;
#endif

#ifdef INSTRUCTIONS_PER_CYCLE
  const double ipc = state.counters["INSTRUCTIONS"] / state.counters["CYCLES"];
  state.counters["INSTRUCTIONS/ CYCLE"] = ipc;
#endif
}

// Benchmarks SHA3-512 hash function with variable length input message.
inline void
sha3_512(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range());

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> md(sha3_512::DIGEST_LEN);

  sha3_utils::random_data(msg.data(), msg.size());

  for (auto _ : state) {
    sha3_512::sha3_512 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.digest(md.data());

    benchmark::DoNotOptimize(hasher);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(md);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + md.size());
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
