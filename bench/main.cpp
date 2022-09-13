#include "bench_keccak.hpp"

// register keccak-f[1600] for benchmark
BENCHMARK(bench_sha3::keccakf1600);

// benchmark runner main routine
BENCHMARK_MAIN();
