#pragma once
#include "shake128.hpp"
#include "shake256.hpp"
#include <benchmark/benchmark.h>
#include <vector>

// Benchmarks SHA3 functions targeting CPU systems, using google-benchmark.
namespace bench_sha3 {

// Benchmarks SHAKE-128 extendable output function with variable length input
// and squeezed output.
//
// Note, all input bytes are absorbed in a single call to `absorb` function.
// And all output bytes are squeezed in a single call to `squeeze` function.
inline void
shake128(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  const size_t olen = static_cast<size_t>(state.range(1));

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out(olen);

  sha3_utils::random_data(msg.data(), msg.size());

  for (auto _ : state) {
    shake128::shake128 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.squeeze(out.data(), out.size());

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

#ifdef INSTRUCTIONS_PER_CYCLE
  const double ipc = state.counters["INSTRUCTIONS"] / state.counters["CYCLES"];
  state.counters["INSTRUCTIONS/ CYCLE"] = ipc;
#endif
}

// Benchmarks SHAKE-256 extendable output function with variable length input
// and squeezed output.
//
// Note, all input bytes are absorbed in a single call to `absorb` function.
// And all output bytes are squeezed in a single call to `squeeze` function.
inline void
shake256(benchmark::State& state)
{
  const size_t mlen = static_cast<size_t>(state.range(0));
  const size_t olen = static_cast<size_t>(state.range(1));

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out(olen);

  sha3_utils::random_data(msg.data(), msg.size());

  for (auto _ : state) {
    shake256::shake256 hasher;
    hasher.absorb(msg.data(), msg.size());
    hasher.finalize();
    hasher.squeeze(out.data(), out.size());

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

#ifdef INSTRUCTIONS_PER_CYCLE
  const double ipc = state.counters["INSTRUCTIONS"] / state.counters["CYCLES"];
  state.counters["INSTRUCTIONS/ CYCLE"] = ipc;
#endif
}

}
