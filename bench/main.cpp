#include "bench/bench_keccak.hpp"
#include "bench/bench_sha3.hpp"

// register keccak-p[1600, 24] permutation for benchmark
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

// register sha3-256 hash function for benchmark
BENCHMARK(bench_sha3::sha3_256)->Arg(32);
BENCHMARK(bench_sha3::sha3_256)->Arg(64);
BENCHMARK(bench_sha3::sha3_256)->Arg(128);
BENCHMARK(bench_sha3::sha3_256)->Arg(256);
BENCHMARK(bench_sha3::sha3_256)->Arg(512);
BENCHMARK(bench_sha3::sha3_256)->Arg(1024);
BENCHMARK(bench_sha3::sha3_256)->Arg(2048);
BENCHMARK(bench_sha3::sha3_256)->Arg(4096);

// register sha3-384 hash function for benchmark
BENCHMARK(bench_sha3::sha3_384)->Arg(32);
BENCHMARK(bench_sha3::sha3_384)->Arg(64);
BENCHMARK(bench_sha3::sha3_384)->Arg(128);
BENCHMARK(bench_sha3::sha3_384)->Arg(256);
BENCHMARK(bench_sha3::sha3_384)->Arg(512);
BENCHMARK(bench_sha3::sha3_384)->Arg(1024);
BENCHMARK(bench_sha3::sha3_384)->Arg(2048);
BENCHMARK(bench_sha3::sha3_384)->Arg(4096);

// register sha3-512 hash function for benchmark
BENCHMARK(bench_sha3::sha3_512)->Arg(32);
BENCHMARK(bench_sha3::sha3_512)->Arg(64);
BENCHMARK(bench_sha3::sha3_512)->Arg(128);
BENCHMARK(bench_sha3::sha3_512)->Arg(256);
BENCHMARK(bench_sha3::sha3_512)->Arg(512);
BENCHMARK(bench_sha3::sha3_512)->Arg(1024);
BENCHMARK(bench_sha3::sha3_512)->Arg(2048);
BENCHMARK(bench_sha3::sha3_512)->Arg(4096);

// register shake-128 extendable output function for benchmark
BENCHMARK(bench_sha3::shake128)->Args({ 32, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 64, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 128, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 256, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 512, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 1024, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 2048, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 4096, 32 });

// register shake-256 extendable output function for benchmark
BENCHMARK(bench_sha3::shake256)->Args({ 32, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 64, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 128, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 256, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 512, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 1024, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 2048, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 4096, 32 });

// benchmark runner main routine
BENCHMARK_MAIN();
