#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

#include "sha3/turboshake128.hpp"
#include <benchmark/benchmark.h>

extern "C"
{
#include "TurboSHAKE.h"
}

static const auto compute_min = [](const std::vector<double>& v) -> double { return *std::min_element(v.begin(), v.end()); };
static const auto compute_max = [](const std::vector<double>& v) -> double { return *std::max_element(v.begin(), v.end()); };

static void
generate_random_data(std::vector<uint8_t>& data)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint8_t> dis;

  for (auto& byte : data) {
    byte = dis(gen);
  }
}

void
bench_xkcp_turboshake128(benchmark::State& state)
{
  const auto mlen = static_cast<size_t>(state.range(0));
  const auto olen = static_cast<size_t>(state.range(1));

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out(olen);

  generate_random_data(msg);

  for (auto _ : state) {
    TurboSHAKE_Instance instance;
    TurboSHAKE128_Initialize(&instance);
    TurboSHAKE_Absorb(&instance, msg.data(), msg.size());
    TurboSHAKE_AbsorbDomainSeparationByte(&instance, 0x07);
    TurboSHAKE_Squeeze(&instance, out.data(), out.size());

    benchmark::DoNotOptimize(instance);
    benchmark::DoNotOptimize(msg);
    benchmark::DoNotOptimize(out);
    benchmark::ClobberMemory();
  }

  const size_t bytes_processed = state.iterations() * (msg.size() + out.size());
  state.SetBytesProcessed(static_cast<int64_t>(bytes_processed));
}

void
bench_portable_turboshake128(benchmark::State& state)
{
  const auto mlen = static_cast<size_t>(state.range(0));
  const auto olen = static_cast<size_t>(state.range(1));

  std::vector<uint8_t> msg(mlen);
  std::vector<uint8_t> out(olen);

  generate_random_data(msg);

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
}

// Message sizes: 32B to 1GB, multiplicative factor 32. Output: 64B.
BENCHMARK(bench_xkcp_turboshake128)
  ->ArgsProduct({
    { 32, 1024, 32768, 1048576, 33554432, 1073741824 },
    { 64 },
  })
  ->Name(XKCP_BENCH_NAME)
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);

BENCHMARK(bench_portable_turboshake128)
  ->ArgsProduct({
    { 32, 1024, 32768, 1048576, 33554432, 1073741824 },
    { 64 },
  })
  ->Name("portable_turboshake128")
  ->ComputeStatistics("min", compute_min)
  ->ComputeStatistics("max", compute_max);
