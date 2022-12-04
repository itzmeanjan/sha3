#include "bench/bench_keccak.hpp"
#include "bench/bench_sha3.hpp"

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
BENCHMARK(bench_sha3::shake128)->Args({ 32, 64 });
BENCHMARK(bench_sha3::shake128)->Args({ 32, 128 });
BENCHMARK(bench_sha3::shake128)->Args({ 64, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 64, 64 });
BENCHMARK(bench_sha3::shake128)->Args({ 64, 128 });
BENCHMARK(bench_sha3::shake128)->Args({ 128, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 128, 64 });
BENCHMARK(bench_sha3::shake128)->Args({ 128, 128 });
BENCHMARK(bench_sha3::shake128)->Args({ 256, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 256, 64 });
BENCHMARK(bench_sha3::shake128)->Args({ 256, 128 });
BENCHMARK(bench_sha3::shake128)->Args({ 512, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 512, 64 });
BENCHMARK(bench_sha3::shake128)->Args({ 512, 128 });
BENCHMARK(bench_sha3::shake128)->Args({ 1024, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 1024, 64 });
BENCHMARK(bench_sha3::shake128)->Args({ 1024, 128 });
BENCHMARK(bench_sha3::shake128)->Args({ 2048, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 2048, 64 });
BENCHMARK(bench_sha3::shake128)->Args({ 2048, 128 });
BENCHMARK(bench_sha3::shake128)->Args({ 4096, 32 });
BENCHMARK(bench_sha3::shake128)->Args({ 4096, 64 });
BENCHMARK(bench_sha3::shake128)->Args({ 4096, 128 });

// register shake-256 extendable output function for benchmark
BENCHMARK(bench_sha3::shake256)->Args({ 32, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 32, 64 });
BENCHMARK(bench_sha3::shake256)->Args({ 32, 128 });
BENCHMARK(bench_sha3::shake256)->Args({ 64, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 64, 64 });
BENCHMARK(bench_sha3::shake256)->Args({ 64, 128 });
BENCHMARK(bench_sha3::shake256)->Args({ 128, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 128, 64 });
BENCHMARK(bench_sha3::shake256)->Args({ 128, 128 });
BENCHMARK(bench_sha3::shake256)->Args({ 256, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 256, 64 });
BENCHMARK(bench_sha3::shake256)->Args({ 256, 128 });
BENCHMARK(bench_sha3::shake256)->Args({ 512, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 512, 64 });
BENCHMARK(bench_sha3::shake256)->Args({ 512, 128 });
BENCHMARK(bench_sha3::shake256)->Args({ 1024, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 1024, 64 });
BENCHMARK(bench_sha3::shake256)->Args({ 1024, 128 });
BENCHMARK(bench_sha3::shake256)->Args({ 2048, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 2048, 64 });
BENCHMARK(bench_sha3::shake256)->Args({ 2048, 128 });
BENCHMARK(bench_sha3::shake256)->Args({ 4096, 32 });
BENCHMARK(bench_sha3::shake256)->Args({ 4096, 64 });
BENCHMARK(bench_sha3::shake256)->Args({ 4096, 128 });

// benchmark runner main routine
BENCHMARK_MAIN();
