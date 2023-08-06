# sha3
`constexpr` ( i.e. **Compile-time Evaluable** ) SHA3: Permutation-Based Hash and Extendable-Output Functions.

## Overview

SHA3 standard by NIST ( i.e. NIST FIPS PUB 202 ) specifies four ( of different digest length ) permutation-based hash functions and two extendable-output functions, which are built on top of keccak-p[1600, 24] permutation.

These hash functions and extendable output functions are pretty commonly used in various post-quantum cryptography algorithms ( those used for public key encryption, key establishment mechanism & digital signature ), some of which are already declared as selected candidates ( e.g. Kyber, Falcon, Dilithium, SPHINCS+ etc. ) of NIST PQC standardization effort - waiting to be standardized or some are still competing. I decided to implement SHA3 specification as a **header-only C++ library**, so that I can make use of it as a modular dependency ( say pinned to specific commit using git submodule ) in libraries where I implement various PQC schemes.

Few of those places, where I've already used `sha3` as ( git submodule based ) dependency

- [Kyber: Post-Quantum Public-key Encryption & Key-establishment Algorithm](https://github.com/itzmeanjan/kyber)
- [Dilithium: Post-Quantum Digital Signature Algorithm](https://github.com/itzmeanjan/dilithium)
- [SPHINCS+: Stateless Hash-based Digital Signature Algorithm](https://github.com/itzmeanjan/sphincs)
- [Falcon: Fast-Fourier Lattice-based Compact Signatures over NTRU](https://github.com/itzmeanjan/falcon)
- [FrodoKEM: Practical Quantum-secure Key Encapsulation from Generic Lattices](https://github.com/itzmeanjan/frodokem)
- [Saber: Post-Quantum Key Encapsulation Mechanism](https://github.com/itzmeanjan/saber)

> **Warning** Above list may not be up-to-date !

Here I'm maintaining a zero-dependency, header-only C++ library, using modern C++ features ( such as C++{>=11} ), which is fairly easy-to-use in your project, implementing SHA3 [specification](https://dx.doi.org/10.6028/NIST.FIPS.202) i.e. NIST FIPS PUB 202. 

> **Note** All Sha3 hash functions and xofs are implemented as `constexpr` functions - meaning for any statically defined input message these functions can be evaluated in compile-time in constant-expression context. See [tests](./tests).

Following algorithms ( with flexible interfaces ) are implemented in `sha3` library.

Algorithm | Input | Output | Behaviour | Namespace + Header
--- | :-: | :-: | :-: | --:
SHA3-224 | N ( >=0 ) -bytes message | 28 -bytes digest | Given N -bytes input message, this routine computes 28 -bytes sha3-224 digest, while *(incrementally)* consuming message into Keccak[448] sponge. | [`sha3_224::sha3_224_t`](./include/sha3_224.hpp)
SHA3-256 | N ( >=0 ) -bytes message | 32 -bytes digest | Given N -bytes input message, this routine computes 32 -bytes sha3-256 digest, while *(incrementally)* consuming message into Keccak[512] sponge. | [`sha3_256::sha3_256_t`](./include/sha3_256.hpp)
SHA3-384 | N ( >=0 ) -bytes message | 48 -bytes digest | Given N -bytes input message, this routine computes 48 -bytes sha3-384 digest, while *(incrementally)* consuming message into Keccak[768] sponge. | [`sha3_384::sha3_384_t`](./include/sha3_384.hpp)
SHA3-512 | N ( >=0 ) -bytes message | 64 -bytes digest | Given N -bytes input message, this routine computes 64 -bytes sha3-512 digest, while *(incrementally)* consuming message into Keccak[1024] sponge. | [`sha3_512::sha3_512_t`](./include/sha3_512.hpp)
SHAKE-128 | N ( >=0 ) -bytes message | M ( >=0 ) -bytes output | Given N -bytes input message, this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[256] sponge, which has already *(incrementally)* absorbed input bytes. | [`shake128::shake128_t`](./include/shake128.hpp)
SHAKE-256 | N ( >=0 ) -bytes message | M ( >=0 ) -bytes digest | Given N -bytes input message, this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[512] sponge, which has already *(incrementally)* absorbed input bytes. | [`shake256::shake256_t`](./include/shake256.hpp)

> **Note** Learn more about NIST PQC standardization effort [here](https://csrc.nist.gov/projects/post-quantum-cryptography/).

## Prerequisites

- A C++ compiler such as `g++`/ `clang++`, with support for C++20 standard library.

```bash
$ g++ --version
g++ (Ubuntu 13.1.0-2ubuntu2~23.04) 13.1.0

$ clang++ --version
Ubuntu clang version 16.0.0 (1~exp5ubuntu3)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

- Build tools such as `cmake` and `make`.

```bash
$ make --version
GNU Make 4.3

$ cmake --version
cmake version 3.25.1
```

- For testing SHA3 algorithms, you need to globally install `google-test` library and headers. Follow [this](https://github.com/google/googletest/tree/main/googletest#standalone-cmake-project) guide if you haven't installed it yet.
- For benchmarking SHA3 algorithms, targeting CPU systems, `google-benchmark` library and headers are required to be installed system-wide. Follow [this](https://github.com/google/benchmark#installation) guide if you don't have it installed yet.
- If you are on a machine running GNU/Linux kernel and you want to obtain following (see list below), when benchmarking SHA3 algorithms, you should consider building `google-benchmark` library yourself with libPFM support, following [this](https://gist.github.com/itzmeanjan/05dc3e946f635d00c5e0b21aae6203a7) step-by-step guide. Find more about libPFM @ https://perfmon2.sourceforge.net.
    1) CPU cycle count.
    2) Retired instruction count.
    3) Cycles/ byte ( aka cpb ).
    4) Retired instructions/ cycle ( aka ipc ).

## Testing

For ensuring that SHA3 hash function and extendable output function implementations are correct & conformant to the NIST standard ( see [this](https://dx.doi.org/10.6028/NIST.FIPS.202) ), I make use of K(nown) A(nswer) T(ests), generated following [this](https://gist.github.com/itzmeanjan/448f97f9c49d781a5eb3ddd6ea6e7364) gist.

I also test correctness of

- Incremental message absorption property of SHA3 hash functions and Xofs.
- Incremental output squeezing property of SHA3 Xofs.

Some compile-time executed tests ( using `static_assert` ) are also implemented, which ensure that all Sha3 hash functions and xofs are `constexpr` - meaning they can be evaluated during compilation-time for any statically defined input message.

Issue following command for running all the test cases.

```bash
$ make -j8

[==========] Running 18 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 12 tests from Sha3Hashing
[ RUN      ] Sha3Hashing.CompileTimeEvalSha3_224
[       OK ] Sha3Hashing.CompileTimeEvalSha3_224 (0 ms)
[ RUN      ] Sha3Hashing.Sha3_224IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_224IncrementalAbsorption (1 ms)
[ RUN      ] Sha3Hashing.Sha3_224KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_224KnownAnswerTests (2 ms)
[ RUN      ] Sha3Hashing.CompileTimeEvalSha3_256
[       OK ] Sha3Hashing.CompileTimeEvalSha3_256 (0 ms)
[ RUN      ] Sha3Hashing.Sha3_256IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_256IncrementalAbsorption (1 ms)
[ RUN      ] Sha3Hashing.Sha3_256KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_256KnownAnswerTests (2 ms)
[ RUN      ] Sha3Hashing.CompileTimeEvalSha3_384
[       OK ] Sha3Hashing.CompileTimeEvalSha3_384 (0 ms)
[ RUN      ] Sha3Hashing.Sha3_384IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_384IncrementalAbsorption (1 ms)
[ RUN      ] Sha3Hashing.Sha3_384KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_384KnownAnswerTests (2 ms)
[ RUN      ] Sha3Hashing.CompileTimeEvalSha3_512
[       OK ] Sha3Hashing.CompileTimeEvalSha3_512 (0 ms)
[ RUN      ] Sha3Hashing.Sha3_512IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_512IncrementalAbsorption (2 ms)
[ RUN      ] Sha3Hashing.Sha3_512KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_512KnownAnswerTests (3 ms)
[----------] 12 tests from Sha3Hashing (18 ms total)

[----------] 6 tests from Sha3Xof
[ RUN      ] Sha3Xof.CompileTimeEvalShake128
[       OK ] Sha3Xof.CompileTimeEvalShake128 (0 ms)
[ RUN      ] Sha3Xof.Shake128IncrementalAbsorptionAndSqueezing
[       OK ] Sha3Xof.Shake128IncrementalAbsorptionAndSqueezing (994 ms)
[ RUN      ] Sha3Xof.Shake128KnownAnswerTests
[       OK ] Sha3Xof.Shake128KnownAnswerTests (2 ms)
[ RUN      ] Sha3Xof.CompileTimeEvalShake256
[       OK ] Sha3Xof.CompileTimeEvalShake256 (0 ms)
[ RUN      ] Sha3Xof.Shake256IncrementalAbsorptionAndSqueezing
[       OK ] Sha3Xof.Shake256IncrementalAbsorptionAndSqueezing (1065 ms)
[ RUN      ] Sha3Xof.Shake256KnownAnswerTests
[       OK ] Sha3Xof.Shake256KnownAnswerTests (3 ms)
[----------] 6 tests from Sha3Xof (2066 ms total)

[----------] Global test environment tear-down
[==========] 18 tests from 2 test suites ran. (2085 ms total)
[  PASSED  ] 18 tests.
```

## Benchmarking

For benchmarking SHA3 hash and extendable output functions, targeting CPU systems, using `google-benchmark`, issue following command.

> **Warning** You must disable CPU scaling during benchmarking, following [this](https://github.com/google/benchmark/blob/4931aefb51d1e5872b096a97f43e13fa0fc33c8c/docs/reducing_variance.md) guide.

> **Note** When benchmarking extendable output functions ( Xofs ), fixed length output of 32/ 64 -bytes are squeezed from sponge ( s.t. all bytes are read in a single call to `squeeze` function ), for each input message byte array of length N | 32 <= N <= 4096 and N = 2^i.

> **Note** Following performance figures were collected by issuing `make perf` - on machines running GNU/Linux kernel, with `google-benchmark` library compiled with *libPFM* support.

```bash
make perf      # You must issue this if you built your google-benchmark library with libPFM support.
make benchmark # Or you can simply use this.
```

### On 12th Gen Intel(R) Core(TM) i7-1260P ( compiled with GCC )

```bash
2023-08-06T12:42:35+04:00
Running ./benchmarks/perf.out
Run on (16 X 1126.69 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 0.72, 0.77, 0.66
***WARNING*** There are 65 benchmarks with threads and 2 performance counters were requested. Beware counters will reflect the combined usage across all threads.
-------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations     CYCLES CYCLES/ BYTE INSTRUCTIONS INSTRUCTIONS/ CYCLE bytes_per_second
-------------------------------------------------------------------------------------------------------------------------------------------------
bench_sha3::keccakf1600             215 ns          215 ns      3252501    958.826      4.79413       5.725k             5.97084       885.488M/s
bench_sha3::sha3_224/32             258 ns          258 ns      2714523   1.14981k      19.1635       6.088k             5.29479       222.178M/s
bench_sha3::sha3_224/64             257 ns          257 ns      2721160   1.13936k      12.3844       6.088k             5.34333       341.217M/s
bench_sha3::sha3_224/128            256 ns          256 ns      2721895    1.1275k      7.22757       6.098k             5.40842       580.521M/s
bench_sha3::sha3_224/256            470 ns          471 ns      1494477   2.07718k        7.314      11.832k             5.69619       575.586M/s
bench_sha3::sha3_224/512            951 ns          951 ns       732717   4.17011k      7.72243      23.264k             5.57875       541.539M/s
bench_sha3::sha3_224/1024          1911 ns         1912 ns       364988   8.34463k      7.93215      46.117k             5.52655       524.808M/s
bench_sha3::sha3_224/2048          3604 ns         3605 ns       193716   15.6681k      7.54723       86.13k             5.49717       549.239M/s
bench_sha3::sha3_224/4096          6979 ns         6980 ns       100880   30.3317k      7.35493     166.154k             5.47789       563.482M/s
bench_sha3::sha3_256/32             266 ns          266 ns      2633906   1.15671k      18.0737       6.083k             5.25886       229.526M/s
bench_sha3::sha3_256/64             264 ns          264 ns      2653244     1.144k      11.9167       6.083k             5.31731       346.923M/s
bench_sha3::sha3_256/128            263 ns          263 ns      2648918   1.13429k      7.08929       6.093k             5.37166       579.285M/s
bench_sha3::sha3_256/256            510 ns          511 ns      1375345    2.1968k      7.62776       12.59k             5.73107       537.984M/s
bench_sha3::sha3_256/512           1056 ns         1056 ns       663524   4.51611k      8.30168      25.548k             5.65708       491.438M/s
bench_sha3::sha3_256/1024          2154 ns         2154 ns       326352   9.15949k      8.67376      51.464k             5.61865       467.442M/s
bench_sha3::sha3_256/2048          4320 ns         4320 ns       162150   18.4495k      8.86996     103.287k             5.59836       459.151M/s
bench_sha3::sha3_256/4096          8378 ns         8379 ns        83662   35.7843k      8.66868      200.47k             5.60218       469.818M/s
bench_sha3::sha3_384/32             251 ns          251 ns      2780712    1080.99      13.5124        6.18k             5.71697       303.684M/s
bench_sha3::sha3_384/64             252 ns          252 ns      2776134    1083.49      9.67401        6.18k              5.7038       423.275M/s
bench_sha3::sha3_384/128            478 ns          478 ns      1465835   2.04213k       11.603      12.038k             5.89482       351.329M/s
bench_sha3::sha3_384/256            711 ns          711 ns       985212   3.02232k      9.94185     17.8171k             5.89516       407.977M/s
bench_sha3::sha3_384/512           1180 ns         1181 ns       589339   5.02766k      8.97796      29.563k             5.88008       452.368M/s
bench_sha3::sha3_384/1024          2339 ns         2339 ns       298201   9.98902k      9.31811      58.753k             5.88176       437.054M/s
bench_sha3::sha3_384/2048          4689 ns         4690 ns       149894   19.9591k      9.52248     117.133k             5.86864       426.214M/s
bench_sha3::sha3_384/4096          9365 ns         9366 ns        74481   39.8012k      9.60453     233.883k             5.87629       421.944M/s
bench_sha3::sha3_512/32             250 ns          250 ns      2801202    1063.64      11.0796       5.977k             5.61936       366.876M/s
bench_sha3::sha3_512/64             250 ns          250 ns      2815614    1057.31      8.26026       5.977k             5.65301       488.766M/s
bench_sha3::sha3_512/128            483 ns          483 ns      1448948   2.05695k      10.7133      12.104k             5.88444        378.98M/s
bench_sha3::sha3_512/256            973 ns          973 ns       720554   4.14606k      12.9564      24.332k              5.8687       313.546M/s
bench_sha3::sha3_512/512           1949 ns         1949 ns       358595   8.34309k      14.4845      48.789k             5.84783       281.814M/s
bench_sha3::sha3_512/1024          3791 ns         3791 ns       179316   16.2029k      14.8924      91.585k             5.65237       273.687M/s
bench_sha3::sha3_512/2048          7113 ns         7114 ns        98033   30.4241k      14.4054     177.182k             5.82374       283.128M/s
bench_sha3::sha3_512/4096         13969 ns        13971 ns        50154   59.6554k      14.3402     348.374k             5.83977       283.964M/s
bench_sha3::shake128/32/32          286 ns          286 ns      2448824   1.21742k      19.0221       6.473k               5.317        213.46M/s
bench_sha3::shake128/64/32          284 ns          285 ns      2463723   1.20557k       12.558       6.473k             5.36927       321.784M/s
bench_sha3::shake128/128/32         285 ns          285 ns      2468293   1.20591k      7.53695       6.483k             5.37602       536.013M/s
bench_sha3::shake128/256/32         518 ns          518 ns      1342117   2.19534k      7.62272      12.299k             5.60231       529.889M/s
bench_sha3::shake128/512/32        1012 ns         1012 ns       690130   4.28544k      7.87766       23.88k             5.57235       512.544M/s
bench_sha3::shake128/1024/32       1753 ns         1754 ns       399707    7.4415k      7.04687      41.263k             5.54499       574.254M/s
bench_sha3::shake128/2048/32       3205 ns         3205 ns       216136   13.7278k       6.5999     75.9721k             5.53419       618.934M/s
bench_sha3::shake128/4096/32       5956 ns         5957 ns       117343   26.3739k      6.38902     145.574k             5.51963       660.885M/s
bench_sha3::shake128/32/64          274 ns          274 ns      2543979   1.22178k      12.7269       6.473k               5.298       333.549M/s
bench_sha3::shake128/64/64          277 ns          277 ns      2569840   1.20788k      9.43658       6.473k             5.35897        441.37M/s
bench_sha3::shake128/128/64         285 ns          285 ns      2453965   1.20748k      6.28894       6.483k             5.36904       642.662M/s
bench_sha3::shake128/256/64         519 ns          519 ns      1347704   2.19589k      6.86215      12.299k             5.60093       587.764M/s
bench_sha3::shake128/512/64        1012 ns         1013 ns       684498   4.28868k      7.44563       23.88k             5.56814       542.467M/s
bench_sha3::shake128/1024/64       1774 ns         1774 ns       398184    7.4383k      6.83667      41.263k             5.54737       584.882M/s
bench_sha3::shake128/2048/64       3262 ns         3262 ns       214161   13.7421k       6.5067      76.034k             5.53291       617.423M/s
bench_sha3::shake128/4096/64       6250 ns         6251 ns       112566   26.3732k      6.33972     145.574k             5.51977       634.651M/s
bench_sha3::shake256/32/32          281 ns          281 ns      2491300   1.19174k      18.6209       6.191k             5.19494       217.215M/s
bench_sha3::shake256/64/32          278 ns          278 ns      2512517   1.18054k      12.2973       6.191k             5.24422         328.9M/s
bench_sha3::shake256/128/32         280 ns          280 ns      2508561   1.18298k      7.39365       6.201k             5.24183        545.77M/s
bench_sha3::shake256/256/32         502 ns          502 ns      1397585   2.12123k      7.36539      12.148k             5.72686       547.578M/s
bench_sha3::shake256/512/32        1008 ns         1008 ns       692518    4.2558k      7.82316          24k             5.63936       514.549M/s
bench_sha3::shake256/1024/32       2023 ns         2023 ns       345617   8.52332k      8.07132      47.704k             5.59688       497.832M/s
bench_sha3::shake256/2048/32       4055 ns         4055 ns       171971   17.0938k      8.21819      95.103k             5.56358       489.181M/s
bench_sha3::shake256/4096/32       7892 ns         7892 ns        89639   33.1722k      8.03589     183.991k             5.54655       498.813M/s
bench_sha3::shake256/32/64          280 ns          280 ns      2493822   1.19248k      12.4217       6.191k             5.19169       326.418M/s
bench_sha3::shake256/64/64          278 ns          278 ns      2510855   1.18261k       9.2391       6.191k             5.23505       438.551M/s
bench_sha3::shake256/128/64         279 ns          279 ns      2508278   1.18189k      6.15569       6.201k             5.24667       657.321M/s
bench_sha3::shake256/256/64         502 ns          502 ns      1394148    2.0532k      6.41626     11.7545k             5.72495       607.658M/s
bench_sha3::shake256/512/64         986 ns          986 ns       720022   4.15368k      7.21125     23.4026k             5.63419       556.862M/s
bench_sha3::shake256/1024/64       1974 ns         1974 ns       354117   8.53791k      7.84735      47.704k             5.58731        525.56M/s
bench_sha3::shake256/2048/64       3925 ns         3925 ns       177954   17.1122k      8.10235      95.103k             5.55763        513.12M/s
bench_sha3::shake256/4096/64       7629 ns         7630 ns        91841   33.1481k       7.9683     183.991k             5.55057       519.982M/s
```

## Usage

`sha3` - C++ header-only library is written such that it's fairly easy for one to start using it in their project. All one needs to do

- Include proper header files ( select which scheme you need by name ).
- Use proper struct(s)/ API(s)/ constant(s) ( see [usage examples](./example) or [test cases](./tests/) ).
- When compiling, let your compiler know where it can find respective header files, which is `./include` directory.

Scheme | Header | Namespace | Example
--- | --- | --- | --:
SHA3-224 | ./include/sha3_224.hpp | `sha3_224::` | [example/sha3_224.cpp](./example/sha3_224.cpp)
SHA3-256 | ./include/sha3_256.hpp | `sha3_256::` | [example/sha3_256.cpp](./example/sha3_256.cpp)
SHA3-384 | ./include/sha3_384.hpp | `sha3_384::` | [example/sha3_384.cpp](./example/sha3_384.cpp)
SHA3-512 | ./include/sha3_512.hpp | `sha3_512::` | [example/sha3_512.cpp](./example/sha3_512.cpp)
SHAKE128 | ./include/shake128.hpp | `shake128::` | [example/shake128.cpp](./example/shake128.cpp)
SHAKE256 | ./include/shake256.hpp | `shake256::` | [example/shake256.cpp](./example/shake256.cpp)

As this library implements all Sha3 hash functions and xofs as `constexpr` - one can evaluate say Sha3-256 digest of some statically defined input message during program compilation time. Let's see how to do that and for ensuring that it computes correct message digest, we'll use static assertions.

```cpp
#include "sha3_256.hpp"

// Eval SHA3-256 hash on statically defined input message during
// compilation-time.
constexpr std::array<uint8_t, sha3_256::DIGEST_LEN>
eval_sha3_256()
{
  // Statically defined input.
  std::array<uint8_t, sha3_256::DIGEST_LEN * 2> data{};
  std::iota(data.begin(), data.end(), 0);

  // To be computed output.
  std::array<uint8_t, sha3_256::DIGEST_LEN> md{};

  sha3_256::sha3_256_t hasher;
  hasher.absorb(data);
  hasher.finalize();
  hasher.digest(md);

  return md;
}

int
main()
{

  // Input  = 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f
  // Output = c8ad478f4e1dd9d47dfc3b985708d92db1f8db48fe9cddd459e63c321f490402
  constexpr auto md = eval_sha3_256();
  static_assert(md ==
                  std::array<uint8_t, sha3_256::DIGEST_LEN>{
                    200, 173, 71, 143, 78, 29,  217, 212, 125, 252, 59,
                    152, 87,  8,  217, 45, 177, 248, 219, 72,  254, 156,
                    221, 212, 89, 230, 60, 50,  31,  73,  4,   2 },
                "Must be able to compute Sha3-256 hash during compile-time !");

  return 0;
}
```

I maintain examples of using Sha3 hash function and xof API, inside [examples](./example/) directory.

```bash
$ g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_224.cpp && ./a.out
SHA3-224

Input  : 043f0fa310343b6ca42c3d2ab6f168574fd41774d49c9c1e5922c2cd60b43dbb
Output : 3bfbd5e41e850f29daf9c08dbcaca7c43ca939e7d6c0b6d8993c6af4

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_256.cpp && ./a.out
SHA3-256

Input  : 70a3bf382218c7f4ae25775ab1d21f9d48e2f03af70dcdec790a338e982e6fa8
Output : 57be0ef9634da2d94219c53032809f4ffc145df6782279a8059afe607715d675

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_384.cpp && ./a.out
SHA3-384

Input  : 314686636dc0499f2ebf0a201fe2d44e2e8888ac1109939998230f2cba5d0e94
Output : 554f4506a1b73724d0ce25cc4a0c0b4fc26478cde43013a59c7e25a22e3e73fbcfa731fe9f5050f77e82530fba7de512

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/sha3_512.cpp && ./a.out
SHA3-512

Input  : 2c3c0ae485204067f1ecbc69a8fefd19a94c9c1552158a8d57a6612b3202f373
Output : 578386bdd6eb816d6d0cbc984351c889f70675a2661ba605aa65ce204b88a6d6553946c52eeac937647929f99a2ddd7cc6645bc1e89df8ab795faba6dd7d0d45

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/shake128.cpp && ./a.out
SHAKE-128

Input  : 8814e9f091cd4ee6ac6795be43b25b4d741143f4d7f7e9858731447359eaa1e8
Output : d32991406e38740f9b9b2674e59259891bfd23f9d6ea71a816c3133466163dacb3b1cefc341cbda8

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/shake256.cpp && ./a.out
SHAKE-256

Input  : a6506638e34127e0a8415241479c968c20422f46497663eaf244f205a756f0b3
Output : ce679163b642380365c3c11dcbca7a36ddd01cefba35b8ec18ad937268f584999c6e8ae061c251dd
```
