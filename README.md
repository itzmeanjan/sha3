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

> [!WARNING]
> Above list may not be up-to-date !

Here I'm maintaining a zero-dependency, header-only C++ library, using modern C++ features ( such as C++{>=11} ), which is fairly easy-to-use in your project, implementing SHA3 [specification](https://dx.doi.org/10.6028/NIST.FIPS.202) i.e. NIST FIPS PUB 202. 

> [!NOTE]
> All Sha3 hash functions and xofs are implemented as `constexpr` functions - meaning for any statically defined input message these functions can be evaluated in compile-time in constant-expression context. See [tests](./tests).

Following algorithms ( with flexible interfaces ) are implemented in `sha3` library.

Algorithm | Input | Output | Behaviour | Namespace + Header
--- | :-: | :-: | :-: | --:
SHA3-224 | N ( >=0 ) -bytes message | 28 -bytes digest | Given N -bytes input message, this routine computes 28 -bytes sha3-224 digest, while *(incrementally)* consuming message into Keccak[448] sponge. | [`sha3_224::sha3_224_t`](./include/sha3_224.hpp)
SHA3-256 | N ( >=0 ) -bytes message | 32 -bytes digest | Given N -bytes input message, this routine computes 32 -bytes sha3-256 digest, while *(incrementally)* consuming message into Keccak[512] sponge. | [`sha3_256::sha3_256_t`](./include/sha3_256.hpp)
SHA3-384 | N ( >=0 ) -bytes message | 48 -bytes digest | Given N -bytes input message, this routine computes 48 -bytes sha3-384 digest, while *(incrementally)* consuming message into Keccak[768] sponge. | [`sha3_384::sha3_384_t`](./include/sha3_384.hpp)
SHA3-512 | N ( >=0 ) -bytes message | 64 -bytes digest | Given N -bytes input message, this routine computes 64 -bytes sha3-512 digest, while *(incrementally)* consuming message into Keccak[1024] sponge. | [`sha3_512::sha3_512_t`](./include/sha3_512.hpp)
SHAKE-128 | N ( >=0 ) -bytes message | M ( >=0 ) -bytes output | Given N -bytes input message, this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[256] sponge, which has already *(incrementally)* absorbed input bytes. | [`shake128::shake128_t`](./include/shake128.hpp)
SHAKE-256 | N ( >=0 ) -bytes message | M ( >=0 ) -bytes digest | Given N -bytes input message, this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[512] sponge, which has already *(incrementally)* absorbed input bytes. | [`shake256::shake256_t`](./include/shake256.hpp)

## Prerequisites

- A C++ compiler such as `g++`/ `clang++`, with support for C++20 standard library.

```bash
$ g++ --version
g++ (Ubuntu 13.2.0-4ubuntu3) 13.2.0

$ clang++ --version
Ubuntu clang version 17.0.2 (1~exp1ubuntu2.1)
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
make -j
```

```bash
Note: Randomizing tests' orders with a seed of 54033 .
[==========] Running 18 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 6 tests from Sha3Xof
[ RUN      ] Sha3Xof.Shake256KnownAnswerTests
[       OK ] Sha3Xof.Shake256KnownAnswerTests (2 ms)
[ RUN      ] Sha3Xof.CompileTimeEvalShake256
[       OK ] Sha3Xof.CompileTimeEvalShake256 (0 ms)
[ RUN      ] Sha3Xof.Shake128KnownAnswerTests
[       OK ] Sha3Xof.Shake128KnownAnswerTests (2 ms)
[ RUN      ] Sha3Xof.CompileTimeEvalShake128
[       OK ] Sha3Xof.CompileTimeEvalShake128 (0 ms)
[ RUN      ] Sha3Xof.Shake128IncrementalAbsorptionAndSqueezing
[       OK ] Sha3Xof.Shake128IncrementalAbsorptionAndSqueezing (915 ms)
[ RUN      ] Sha3Xof.Shake256IncrementalAbsorptionAndSqueezing
[       OK ] Sha3Xof.Shake256IncrementalAbsorptionAndSqueezing (994 ms)
[----------] 6 tests from Sha3Xof (1916 ms total)

[----------] 12 tests from Sha3Hashing
[ RUN      ] Sha3Hashing.Sha3_384KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_384KnownAnswerTests (2 ms)
[ RUN      ] Sha3Hashing.Sha3_224KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_224KnownAnswerTests (1 ms)
[ RUN      ] Sha3Hashing.Sha3_256KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_256KnownAnswerTests (2 ms)
[ RUN      ] Sha3Hashing.CompileTimeEvalSha3_384
[       OK ] Sha3Hashing.CompileTimeEvalSha3_384 (0 ms)
[ RUN      ] Sha3Hashing.CompileTimeEvalSha3_224
[       OK ] Sha3Hashing.CompileTimeEvalSha3_224 (0 ms)
[ RUN      ] Sha3Hashing.Sha3_224IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_224IncrementalAbsorption (1 ms)
[ RUN      ] Sha3Hashing.Sha3_384IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_384IncrementalAbsorption (1 ms)
[ RUN      ] Sha3Hashing.Sha3_512IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_512IncrementalAbsorption (2 ms)
[ RUN      ] Sha3Hashing.Sha3_256IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_256IncrementalAbsorption (1 ms)
[ RUN      ] Sha3Hashing.CompileTimeEvalSha3_512
[       OK ] Sha3Hashing.CompileTimeEvalSha3_512 (0 ms)
[ RUN      ] Sha3Hashing.CompileTimeEvalSha3_256
[       OK ] Sha3Hashing.CompileTimeEvalSha3_256 (0 ms)
[ RUN      ] Sha3Hashing.Sha3_512KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_512KnownAnswerTests (2 ms)
[----------] 12 tests from Sha3Hashing (16 ms total)

[----------] Global test environment tear-down
[==========] 18 tests from 2 test suites ran. (1932 ms total)
[  PASSED  ] 18 tests.
```

## Benchmarking

For benchmarking SHA3 hash and extendable output functions, targeting CPU systems, using `google-benchmark`, issue following command.

> [!CAUTION]
> You must disable CPU frequency scaling during benchmarking, following [this](https://github.com/google/benchmark/blob/4931aefb51d1e5872b096a97f43e13fa0fc33c8c/docs/reducing_variance.md) guide.

> [!NOTE]
> When benchmarking extendable output functions ( Xofs ), fixed length output of 32/ 64 -bytes are squeezed from sponge ( s.t. all output bytes are requested in a single call to the `squeeze` function ), for input message byte array of length N s.t. N = 2^i (i.e. power of 2).

> [!NOTE]
> Following performance figures were collected by issuing `make perf` - on machines running GNU/Linux kernel, with `google-benchmark` library compiled with *libPFM* support.

```bash
make perf -j      # You must issue this if you built your google-benchmark library with libPFM support.
make benchmark -j # Or you can simply use this.
```

### On 12th Gen Intel(R) Core(TM) i7-1260P ( compiled with Clang-17.0.2 )

```bash
2023-12-03T19:35:49+04:00
Running ./build/perfs/perf.out
Run on (16 X 1789.64 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 1.22, 0.71, 0.55
----------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations     CYCLES CYCLES/ BYTE INSTRUCTIONS INSTRUCTIONS/ CYCLE bytes_per_second
----------------------------------------------------------------------------------------------------------------------------------------------
sha3_512/64_mean                 225 ns          225 ns            8    956.203      7.47033       4.681k             4.89542      542.458Mi/s
sha3_512/64_median               228 ns          228 ns            8    955.935      7.46824       4.681k             4.89678          536Mi/s
sha3_512/64_stddev              6.19 ns         6.19 ns            8    1.48628    0.0116115            0            7.60773m      15.3188Mi/s
sha3_512/64_cv                  2.75 %          2.75 %             8      0.16%        0.16%        0.00%               0.16%            2.82%
shake128/256/64_mean             437 ns          437 ns            8   1.87638k       5.8637       9.119k             4.85992       698.93Mi/s
shake128/256/64_median           439 ns          439 ns            8   1.87722k      5.86632       9.119k             4.85771      696.027Mi/s
shake128/256/64_stddev          13.3 ns         13.3 ns            8    5.49839    0.0171825            0            0.014246      21.3468Mi/s
shake128/256/64_cv              3.05 %          3.05 %             8      0.29%        0.29%        0.00%               0.29%            3.05%
keccak-p[1600, 24]_mean          194 ns          194 ns            8    835.387      4.17694         4.4k             5.26704      985.928Mi/s
keccak-p[1600, 24]_median        191 ns          191 ns            8    834.974      4.17487         4.4k             5.26962      997.234Mi/s
keccak-p[1600, 24]_stddev       5.97 ns         5.97 ns            8    1.75449     8.77243m            0            0.011035      29.7066Mi/s
keccak-p[1600, 24]_cv           3.08 %          3.08 %             8      0.21%        0.21%        0.00%               0.21%            3.01%
sha3_384/256_mean                649 ns          649 ns            8   2.83459k      9.32431      13.516k             4.76825      447.462Mi/s
sha3_384/256_median              642 ns          642 ns            8   2.83374k      9.32151      13.516k             4.76967       451.85Mi/s
sha3_384/256_stddev             21.4 ns         21.4 ns            8    3.06804    0.0100922            0            5.15513m      14.4049Mi/s
sha3_384/256_cv                 3.29 %          3.30 %             8      0.11%        0.11%        0.00%               0.11%            3.22%
sha3_256/4096_mean              6299 ns         6298 ns            8    26.587k      6.44066     135.597k             5.09994      625.764Mi/s
sha3_256/4096_median            6285 ns         6284 ns            8   26.9201k      6.52135     137.365k             5.10067      626.428Mi/s
sha3_256/4096_stddev             227 ns          227 ns            8    953.897      0.23108     5.00138k            7.14201m      23.0688Mi/s
sha3_256/4096_cv                3.60 %          3.60 %             8      3.59%        3.59%        3.69%               0.14%            3.69%
shake256/64/64_mean              219 ns          219 ns            8    942.636      7.36434       4.641k             4.92346      558.082Mi/s
shake256/64/64_median            216 ns          216 ns            8    941.567        7.356       4.641k             4.92902      565.267Mi/s
shake256/64/64_stddev           6.47 ns         6.47 ns            8    2.59835    0.0202996            0            0.013537      16.3288Mi/s
shake256/64/64_cv               2.96 %          2.96 %             8      0.28%        0.28%        0.00%               0.27%            2.93%
sha3_512/256_mean                827 ns          827 ns            8   3.58407k      11.2002      17.959k             5.01078      369.106Mi/s
sha3_512/256_median              826 ns          826 ns            8    3.5843k      11.2009      17.959k             5.01047      369.685Mi/s
sha3_512/256_stddev             24.9 ns         24.9 ns            8    3.29758    0.0103049            0            4.61196m      11.0903Mi/s
sha3_512/256_cv                 3.01 %          3.00 %             8      0.09%        0.09%        0.00%               0.09%            3.00%
sha3_256/16384_mean            24094 ns        24093 ns            8   104.645k      6.37456     535.526k             5.11758      650.171Mi/s
sha3_256/16384_median          23956 ns        23956 ns            8   104.673k      6.37627     535.526k             5.11619      653.524Mi/s
sha3_256/16384_stddev            621 ns          621 ns            8     202.09    0.0123105     8.35191m            9.89437m      16.2458Mi/s
sha3_256/16384_cv               2.58 %          2.58 %             8      0.19%        0.19%        0.00%               0.19%            2.50%
sha3_512/1024_mean              3033 ns         3033 ns            8   13.0967k      12.0374      66.512k             5.07856      342.345Mi/s
sha3_512/1024_median            3016 ns         3016 ns            8   13.0866k      12.0281      66.512k             5.08246      344.056Mi/s
sha3_512/1024_stddev            88.6 ns         88.6 ns            8    27.8628    0.0256092            0           0.0107723      9.92862Mi/s
sha3_512/1024_cv                2.92 %          2.92 %             8      0.21%        0.21%        0.00%               0.21%            2.90%
sha3_256/1024_mean              1648 ns         1648 ns            8    7.0526k      6.67859      35.624k              5.0512      611.471Mi/s
sha3_256/1024_median            1634 ns         1634 ns            8   7.05457k      6.68046      35.624k             5.04978      616.287Mi/s
sha3_256/1024_stddev            32.9 ns         32.9 ns            8    10.4676     9.91253m            0            7.49475m      11.9992Mi/s
sha3_256/1024_cv                2.00 %          2.00 %             8      0.15%        0.15%        0.00%               0.15%            1.96%
sha3_224/4096_mean              5953 ns         5953 ns            8   25.4087k      6.16119     128.559k             5.06028      661.755Mi/s
sha3_224/4096_median            5897 ns         5897 ns            8   25.3055k      6.13616     128.559k             5.08027       666.99Mi/s
sha3_224/4096_stddev             261 ns          260 ns            8    309.729    0.0751041            0           0.0602283      28.3863Mi/s
sha3_224/4096_cv                4.38 %          4.38 %             8      1.22%        1.22%        0.00%               1.19%            4.29%
sha3_224/1024_mean              1673 ns         1673 ns            8   7.12096k      6.76897     35.6113k             5.00095       600.04Mi/s
sha3_224/1024_median            1685 ns         1685 ns            8   7.11965k      6.76773      35.612k             5.00193      595.708Mi/s
sha3_224/1024_stddev            50.3 ns         50.4 ns            8    19.7032    0.0187293      1.89846           0.0138326      18.2197Mi/s
sha3_224/1024_cv                3.01 %          3.01 %             8      0.28%        0.28%        0.01%               0.28%            3.04%
shake256/1024/64_mean           1634 ns         1634 ns            8   7.04569k      6.47582      35.662k             5.06154      635.088Mi/s
shake256/1024/64_median         1632 ns         1632 ns            8   7.04427k      6.47451      35.662k             5.06256      635.609Mi/s
shake256/1024/64_stddev         32.7 ns         32.7 ns            8         11    0.0101103     738.212u            7.89669m      12.5819Mi/s
shake256/1024/64_cv             2.00 %          2.00 %             8      0.16%        0.16%        0.00%               0.16%            1.98%
sha3_256/64_mean                 218 ns          218 ns            8     937.19       9.7624       4.603k             4.91153      420.727Mi/s
sha3_256/64_median               217 ns          217 ns            8     936.03      9.75031       4.603k             4.91758      421.036Mi/s
sha3_256/64_stddev              5.25 ns         5.25 ns            8    2.70568    0.0281842     92.2765u           0.0141571      10.0807Mi/s
sha3_256/64_cv                  2.41 %          2.41 %             8      0.29%        0.29%        0.00%               0.29%            2.40%
sha3_256/256_mean                435 ns          435 ns            8   1.85983k      6.45775       9.078k             4.88112      632.292Mi/s
sha3_256/256_median              433 ns          433 ns            8    1.8607k      6.46078       9.078k              4.8788      633.833Mi/s
sha3_256/256_stddev             9.83 ns         9.83 ns            8    5.28817    0.0183617            0            0.013924      14.2778Mi/s
sha3_256/256_cv                 2.26 %          2.26 %             8      0.28%        0.28%        0.00%               0.29%            2.26%
shake256/16384/64_mean         24413 ns        24410 ns            8     104.5k      6.35339     535.564k             5.12502      643.389Mi/s
shake256/16384/64_median       24228 ns        24227 ns            8    104.45k      6.35032     535.564k             5.12746      647.466Mi/s
shake256/16384/64_stddev         907 ns          908 ns            8    244.845     0.014886            0           0.0119983      23.9988Mi/s
shake256/16384/64_cv            3.72 %          3.72 %             8      0.23%        0.23%        0.00%               0.23%            3.73%
sha3_512/16384_mean            45831 ns        45829 ns            8   196.148k      11.9253      1.0067M             5.13234      342.649Mi/s
sha3_512/16384_median          45724 ns        45723 ns            8   196.113k      11.9232      1.0067M             5.13323      343.064Mi/s
sha3_512/16384_stddev           1620 ns         1621 ns            8    261.495    0.0158983            0            6.83691m      12.1084Mi/s
sha3_512/16384_cv               3.54 %          3.54 %             8      0.13%        0.13%        0.00%               0.13%            3.53%
shake256/256/64_mean             427 ns          427 ns            8   1.85961k      5.81127       9.116k             4.90212      715.414Mi/s
shake256/256/64_median           426 ns          426 ns            8   1.85989k      5.81217       9.116k             4.90136      715.987Mi/s
shake256/256/64_stddev          9.74 ns         9.74 ns            8    2.92041     9.12628m            0            7.70012m      16.0912Mi/s
shake256/256/64_cv              2.28 %          2.28 %             8      0.16%        0.16%        0.00%               0.16%            2.25%
shake128/16384/64_mean         19929 ns        19927 ns            8   84.8882k        5.161     433.668k             5.10874      787.405Mi/s
shake128/16384/64_median       19791 ns        19789 ns            8   84.9721k      5.16611     434.304k             5.10972      792.663Mi/s
shake128/16384/64_stddev         354 ns          353 ns            8    439.399    0.0267144     1.79843k           0.0146744      13.7539Mi/s
shake128/16384/64_cv            1.78 %          1.77 %             8      0.52%        0.52%        0.41%               0.29%            1.75%
shake256/4096/64_mean           6298 ns         6298 ns            8   26.8982k      6.46592      137.24k             5.10222      630.773Mi/s
shake256/4096/64_median         6296 ns         6294 ns            8   26.8963k      6.46545     137.403k             5.10863      630.325Mi/s
shake256/4096/64_stddev          240 ns          239 ns            8    35.8161     8.60964m      460.515           0.0221194       24.287Mi/s
shake256/4096/64_cv             3.80 %          3.80 %             8      0.13%        0.13%        0.34%               0.43%            3.85%
sha3_224/256_mean                445 ns          445 ns            8   1.93263k      6.80504       9.065k             4.69053      609.739Mi/s
sha3_224/256_median              442 ns          442 ns            8   1.93258k      6.80484       9.065k             4.69064      613.474Mi/s
sha3_224/256_stddev             13.8 ns         13.8 ns            8    5.56544    0.0195966            0            0.013505      18.8611Mi/s
sha3_224/256_cv                 3.10 %          3.11 %             8      0.29%        0.29%        0.00%               0.29%            3.09%
shake128/1024/64_mean           1442 ns         1442 ns            8   6.19979k      5.69833      31.254k             5.04115      720.049Mi/s
shake128/1024/64_median         1433 ns         1432 ns            8   6.20089k      5.69935      31.254k             5.04025      724.357Mi/s
shake128/1024/64_stddev         37.3 ns         37.4 ns            8    8.82176     8.10824m            0            7.17323m      18.3348Mi/s
shake128/1024/64_cv             2.59 %          2.59 %             8      0.14%        0.14%        0.00%               0.14%            2.55%
sha3_224/64_mean                 228 ns          228 ns            8   1.00124k      10.8831       4.589k             4.58334      385.953Mi/s
sha3_224/64_median               227 ns          227 ns            8   1.00201k      10.8914       4.589k              4.5798      386.909Mi/s
sha3_224/64_stddev              8.83 ns         8.84 ns            8      3.406    0.0370218            0           0.0155949      14.9507Mi/s
sha3_224/64_cv                  3.88 %          3.88 %             8      0.34%        0.34%        0.00%               0.34%            3.87%
sha3_384/4096_mean              8435 ns         8435 ns            8   36.0124k      8.69024     177.019k             4.91553      469.007Mi/s
sha3_384/4096_median            8573 ns         8573 ns            8   36.0309k      8.69471     177.019k             4.91298      460.984Mi/s
sha3_384/4096_stddev             278 ns          278 ns            8    78.1854    0.0188671     2.08798m           0.0106739      15.9567Mi/s
sha3_384/4096_cv                3.29 %          3.29 %             8      0.22%        0.22%        0.00%               0.22%            3.40%
shake128/4096/64_mean           5111 ns         5111 ns            8   21.7892k      5.23779     110.977k             5.09326       776.85Mi/s
shake128/4096/64_median         5102 ns         5102 ns            8    21.802k      5.24086     110.977k             5.09022      777.628Mi/s
shake128/4096/64_stddev          151 ns          151 ns            8    75.1841    0.0180731     1.47642m           0.0175946       23.336Mi/s
shake128/4096/64_cv             2.95 %          2.95 %             8      0.35%        0.35%        0.00%               0.35%            3.00%
sha3_512/4096_mean             11629 ns        11628 ns            8   49.2415k      11.8369     251.901k             5.11566      341.514Mi/s
sha3_512/4096_median           11692 ns        11691 ns            8     49.22k      11.8317     251.901k             5.11786      339.466Mi/s
sha3_512/4096_stddev             388 ns          388 ns            8    143.593    0.0345176            0           0.0149116      11.6632Mi/s
sha3_512/4096_cv                3.34 %          3.34 %             8      0.29%        0.29%        0.00%               0.29%            3.42%
sha3_384/64_mean                 230 ns          230 ns            8   1.01329k      9.04723       4.644k             4.58312      465.285Mi/s
sha3_384/64_median               228 ns          228 ns            8    1.0128k      9.04287       4.644k              4.5853      468.517Mi/s
sha3_384/64_stddev              6.54 ns         6.53 ns            8    2.67893     0.023919     92.2765u           0.0120798      13.0905Mi/s
sha3_384/64_cv                  2.85 %          2.84 %             8      0.26%        0.26%        0.00%               0.26%            2.81%
sha3_224/16384_mean            22953 ns        22952 ns            8   98.7267k      6.01552     504.779k              5.1129       682.17Mi/s
sha3_224/16384_median          23027 ns        23026 ns            8   98.7082k      6.01439     504.779k             5.11385      679.745Mi/s
sha3_224/16384_stddev            449 ns          448 ns            8    132.239     8.05743m     0.010229            6.84699m      13.3126Mi/s
sha3_224/16384_cv               1.96 %          1.95 %             8      0.13%        0.13%        0.00%               0.13%            1.95%
sha3_384/1024_mean              2105 ns         2105 ns            8   9.13115k      8.51786      44.459k             4.86894      486.167Mi/s
sha3_384/1024_median            2105 ns         2105 ns            8   9.13143k      8.51812      44.459k             4.86879      485.653Mi/s
sha3_384/1024_stddev            66.1 ns         66.1 ns            8    9.99151     9.32044m     521.995u            5.33215m      15.4265Mi/s
sha3_384/1024_cv                3.14 %          3.14 %             8      0.11%        0.11%        0.00%               0.11%            3.17%
sha3_384/16384_mean            32258 ns        32257 ns            8    141.36k      8.60275     698.461k             4.94101      486.057Mi/s
sha3_384/16384_median          32037 ns        32037 ns            8   141.401k      8.60524     698.461k             4.93957      489.146Mi/s
sha3_384/16384_stddev            780 ns          780 ns            8    244.219    0.0148624    0.0118114            8.53943m      11.5467Mi/s
sha3_384/16384_cv               2.42 %          2.42 %             8      0.17%        0.17%        0.00%               0.17%            2.38%
shake128/64/64_mean              219 ns          219 ns            8     950.26      7.42391        4.64k              4.8829      558.266Mi/s
shake128/64/64_median            219 ns          219 ns            8    950.679      7.42718        4.64k             4.88072      557.482Mi/s
shake128/64/64_stddev           8.90 ns         8.90 ns            8    2.11967    0.0165599     92.2765u           0.0109007      23.2093Mi/s
shake128/64/64_cv               4.07 %          4.07 %             8      0.22%        0.22%        0.00%               0.22%            4.16%
```

## Usage

`sha3` - C++ header-only library is written such that it's fairly easy for one to start using it in their project. All one needs to do

- Include proper header files ( select which scheme you need by name ).
- Use proper struct(s)/ API(s)/ constant(s) ( see [usage examples](./examples) or [test cases](./tests/) ).
- When compiling, let your compiler know where it can find respective header files, which is `./include` directory.

Scheme | Header | Namespace | Example
--- | --- | --- | --:
SHA3-224 | ./include/sha3_224.hpp | `sha3_224::` | [examples/sha3_224.cpp](./examples/sha3_224.cpp)
SHA3-256 | ./include/sha3_256.hpp | `sha3_256::` | [examples/sha3_256.cpp](./examples/sha3_256.cpp)
SHA3-384 | ./include/sha3_384.hpp | `sha3_384::` | [examples/sha3_384.cpp](./examples/sha3_384.cpp)
SHA3-512 | ./include/sha3_512.hpp | `sha3_512::` | [examples/sha3_512.cpp](./examples/sha3_512.cpp)
SHAKE128 | ./include/shake128.hpp | `shake128::` | [examples/shake128.cpp](./examples/shake128.cpp)
SHAKE256 | ./include/shake256.hpp | `shake256::` | [examples/shake256.cpp](./examples/shake256.cpp)

As this library implements all Sha3 hash functions and xofs as `constexpr` - one can evaluate, say Sha3-256 digest of some statically defined input message, during program compilation time. Let's see how to do that and for ensuring that it computes correct message digest, we'll use static assertions.

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

I maintain examples of using Sha3 hash function and xof API, inside [examples](./examples/) directory.

```bash
$ g++ -std=c++20 -Wall -O3 -march=native -I include examples/sha3_224.cpp && ./a.out
SHA3-224

Input  : 043f0fa310343b6ca42c3d2ab6f168574fd41774d49c9c1e5922c2cd60b43dbb
Output : 3bfbd5e41e850f29daf9c08dbcaca7c43ca939e7d6c0b6d8993c6af4

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include examples/sha3_256.cpp && ./a.out
SHA3-256

Input  : 70a3bf382218c7f4ae25775ab1d21f9d48e2f03af70dcdec790a338e982e6fa8
Output : 57be0ef9634da2d94219c53032809f4ffc145df6782279a8059afe607715d675

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include examples/sha3_384.cpp && ./a.out
SHA3-384

Input  : 314686636dc0499f2ebf0a201fe2d44e2e8888ac1109939998230f2cba5d0e94
Output : 554f4506a1b73724d0ce25cc4a0c0b4fc26478cde43013a59c7e25a22e3e73fbcfa731fe9f5050f77e82530fba7de512

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include examples/sha3_512.cpp && ./a.out
SHA3-512

Input  : 2c3c0ae485204067f1ecbc69a8fefd19a94c9c1552158a8d57a6612b3202f373
Output : 578386bdd6eb816d6d0cbc984351c889f70675a2661ba605aa65ce204b88a6d6553946c52eeac937647929f99a2ddd7cc6645bc1e89df8ab795faba6dd7d0d45

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include examples/shake128.cpp && ./a.out
SHAKE-128

Input  : 8814e9f091cd4ee6ac6795be43b25b4d741143f4d7f7e9858731447359eaa1e8
Output : d32991406e38740f9b9b2674e59259891bfd23f9d6ea71a816c3133466163dacb3b1cefc341cbda8

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include examples/shake256.cpp && ./a.out
SHAKE-256

Input  : a6506638e34127e0a8415241479c968c20422f46497663eaf244f205a756f0b3
Output : ce679163b642380365c3c11dcbca7a36ddd01cefba35b8ec18ad937268f584999c6e8ae061c251dd
```
