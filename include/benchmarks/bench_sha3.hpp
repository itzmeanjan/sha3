#pragma once
#include "sha3_224.hpp"
#include "sha3_256.hpp"
#include "sha3_384.hpp"
#include "sha3_512.hpp"
#include "shake128.hpp"
#include "shake256.hpp"
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

  const size_t bytes_per_iter = state.iterations() * (msg.size() + md.size());
  state.SetBytesProcessed(bytes_per_iter);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_per_iter;
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

  const size_t bytes_per_iter = state.iterations() * (msg.size() + md.size());
  state.SetBytesProcessed(bytes_per_iter);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_per_iter;
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

  const size_t bytes_per_iter = state.iterations() * (msg.size() + md.size());
  state.SetBytesProcessed(bytes_per_iter);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_per_iter;
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

  const size_t bytes_per_iter = state.iterations() * (msg.size() + md.size());
  state.SetBytesProcessed(bytes_per_iter);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_per_iter;
#endif
}

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

  const size_t bytes_per_iter = state.iterations() * (msg.size() + out.size());
  state.SetBytesProcessed(bytes_per_iter);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_per_iter;
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

  const size_t bytes_per_iter = state.iterations() * (msg.size() + out.size());
  state.SetBytesProcessed(bytes_per_iter);

#ifdef CYCLES_PER_BYTE
  state.counters["CYCLES/ BYTE"] = state.counters["CYCLES"] / bytes_per_iter;
#endif
}

}
