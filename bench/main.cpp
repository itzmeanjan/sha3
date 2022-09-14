#include "bench_keccak.hpp"
#include "bench_sha3.hpp"

// register keccak-f[1600] for benchmark
BENCHMARK(bench_sha3::keccakf1600);

// register sha3-224 hash function for benchmark
BENCHMARK(bench_sha3::sha3_224)->Arg(32);
BENCHMARK(bench_sha3::sha3_224)->Arg(64);
BENCHMARK(bench_sha3::sha3_224)->Arg(128);
BENCHMARK(bench_sha3::sha3_224)->Arg(256);
BENCHMARK(bench_sha3::sha3_224)->Arg(512);
BENCHMARK(bench_sha3::sha3_224)->Arg(1024);
BENCHMARK(bench_sha3::sha3_224)->Arg(2048);
BENCHMARK(bench_sha3::sha3_224)->Arg(4096);

// benchmark runner main routine
BENCHMARK_MAIN();
