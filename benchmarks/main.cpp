#include "benchmarks/bench_hashing.hpp"
#include "benchmarks/bench_keccak.hpp"
#include "benchmarks/bench_xof.hpp"

// register keccak-p[1600, 24] permutation for benchmark
BENCHMARK(bench_sha3::keccakf1600);

// register sha3-{224, 256, 384, 512} hash function for benchmark
BENCHMARK(bench_sha3::sha3_224)->RangeMultiplier(2)->Range(32, 4096);
BENCHMARK(bench_sha3::sha3_256)->RangeMultiplier(2)->Range(32, 4096);
BENCHMARK(bench_sha3::sha3_384)->RangeMultiplier(2)->Range(32, 4096);
BENCHMARK(bench_sha3::sha3_512)->RangeMultiplier(2)->Range(32, 4096);

// register shake-{128, 256} extendable output function for benchmark
BENCHMARK(bench_sha3::shake128)
  ->ArgsProduct({ benchmark::CreateRange(32, 4096, 2), { 32, 64 } });
BENCHMARK(bench_sha3::shake256)
  ->ArgsProduct({ benchmark::CreateRange(32, 4096, 2), { 32, 64 } });

// benchmark runner main routine
BENCHMARK_MAIN();
