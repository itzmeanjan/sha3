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
2023-12-22T20:37:13+04:00
Running ./build/perfs/perf.out
Run on (16 X 2858.27 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 0.19, 0.42, 0.52
-------------------------------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations     CYCLES CYCLES/ BYTE bytes_per_second
-------------------------------------------------------------------------------------------------------------
keccak-p[1600, 24]_mean          181 ns          181 ns           10    837.237      4.18618      1.02809Gi/s
keccak-p[1600, 24]_median        181 ns          181 ns           10    836.973      4.18486      1.03168Gi/s
keccak-p[1600, 24]_stddev       2.23 ns         2.25 ns           10     1.8613     9.30649m       13.014Mi/s
keccak-p[1600, 24]_cv           1.23 %          1.24 %            10      0.22%        0.22%            1.24%
keccak-p[1600, 24]_min           179 ns          179 ns           10    834.762      4.17381      1.00823Gi/s
keccak-p[1600, 24]_max           185 ns          185 ns           10    840.275      4.20138      1.04163Gi/s
shake256/256/64_mean             405 ns          405 ns           10   1.83538k      5.73557      755.048Mi/s
shake256/256/64_median           399 ns          399 ns           10   1.86572k      5.83036      764.974Mi/s
shake256/256/64_stddev          12.4 ns         12.4 ns           10    99.3426     0.310446       21.748Mi/s
shake256/256/64_cv              3.07 %          3.06 %            10      5.41%        5.41%            2.88%
shake256/256/64_min              398 ns          398 ns           10   1.55283k       4.8526       697.32Mi/s
shake256/256/64_max              438 ns          438 ns           10   1.87209k      5.85027      767.629Mi/s
sha3_256/1024_mean              1537 ns         1537 ns           10   7.07262k      6.69756      655.479Mi/s
sha3_256/1024_median            1529 ns         1529 ns           10   7.07352k      6.69841      658.739Mi/s
sha3_256/1024_stddev            24.4 ns         24.4 ns           10    7.30414      6.9168m      10.3573Mi/s
sha3_256/1024_cv                1.59 %          1.59 %            10      0.10%        0.10%            1.58%
sha3_256/1024_min               1511 ns         1511 ns           10   7.05944k      6.68508      640.517Mi/s
sha3_256/1024_max               1572 ns         1572 ns           10   7.08317k      6.70755       666.49Mi/s
shake128/256/64_mean             404 ns          404 ns           10   1.87853k      5.87041      755.517Mi/s
shake128/256/64_median           403 ns          403 ns           10   1.87905k      5.87203      756.392Mi/s
shake128/256/64_stddev          4.55 ns         4.55 ns           10    6.58881      0.02059      8.40068Mi/s
shake128/256/64_cv              1.13 %          1.13 %            10      0.35%        0.35%            1.11%
shake128/256/64_min              399 ns          399 ns           10   1.86691k       5.8341      736.181Mi/s
shake128/256/64_max              415 ns          415 ns           10   1.88664k      5.89574      765.307Mi/s
sha3_224/256_mean                404 ns          404 ns           10   1.87892k      6.61593      670.059Mi/s
sha3_224/256_median              404 ns          404 ns           10   1.87753k      6.61101       671.18Mi/s
sha3_224/256_stddev             3.60 ns         3.60 ns           10    5.79879    0.0204183      5.91264Mi/s
sha3_224/256_cv                 0.89 %          0.89 %            10      0.31%        0.31%            0.88%
sha3_224/256_min                 400 ns          400 ns           10   1.87165k      6.59031      657.545Mi/s
sha3_224/256_max                 412 ns          412 ns           10   1.89078k      6.65769       676.45Mi/s
sha3_224/16384_mean            21890 ns        21889 ns           10   98.2326k      5.98541      715.351Mi/s
sha3_224/16384_median          21813 ns        21812 ns           10   99.5154k      6.06357      717.561Mi/s
sha3_224/16384_stddev            480 ns          479 ns           10   4.49737k     0.274029      15.2321Mi/s
sha3_224/16384_cv               2.19 %          2.19 %            10      4.58%        4.58%            2.13%
sha3_224/16384_min             21232 ns        21231 ns           10   85.4646k      5.20744      679.143Mi/s
sha3_224/16384_max             23051 ns        23046 ns           10   100.452k      6.12062      737.194Mi/s
sha3_512/16384_mean            43882 ns        43881 ns           10   202.507k      12.3119      357.523Mi/s
sha3_512/16384_median          43953 ns        43950 ns           10   202.444k      12.3081      356.911Mi/s
sha3_512/16384_stddev            581 ns          581 ns           10    341.924    0.0207882       4.7201Mi/s
sha3_512/16384_cv               1.32 %          1.32 %            10      0.17%        0.17%            1.32%
sha3_512/16384_min             43228 ns        43227 ns           10   201.982k      12.2801      350.364Mi/s
sha3_512/16384_max             44771 ns        44771 ns           10   203.122k      12.3493      362.873Mi/s
sha3_384/16384_mean            29721 ns        29720 ns           10   136.298k      8.29467      527.464Mi/s
sha3_384/16384_median          29896 ns        29895 ns           10   136.401k      8.30095      524.211Mi/s
sha3_384/16384_stddev            588 ns          588 ns           10    455.296    0.0277079      10.4574Mi/s
sha3_384/16384_cv               1.98 %          1.98 %            10      0.33%        0.33%            1.98%
sha3_384/16384_min             28915 ns        28915 ns           10   135.525k      8.24765      513.009Mi/s
sha3_384/16384_max             30547 ns        30547 ns           10   136.905k      8.33161      541.967Mi/s
shake128/4096/64_mean           4720 ns         4720 ns           10   21.7808k      5.23577      840.751Mi/s
shake128/4096/64_median         4693 ns         4693 ns           10   21.7731k      5.23392      845.389Mi/s
shake128/4096/64_stddev         83.6 ns         83.6 ns           10    84.5016    0.0203129      14.5613Mi/s
shake128/4096/64_cv             1.77 %          1.77 %            10      0.39%        0.39%            1.73%
shake128/4096/64_min            4639 ns         4639 ns           10   21.6862k      5.21303      806.832Mi/s
shake128/4096/64_max            4917 ns         4917 ns           10   21.8929k      5.26272      855.194Mi/s
sha3_384/4096_mean              7474 ns         7474 ns           10    34.608k      8.35135      528.941Mi/s
sha3_384/4096_median            7413 ns         7413 ns           10    34.606k      8.35088      533.147Mi/s
sha3_384/4096_stddev             146 ns          146 ns           10    77.1853    0.0186258      10.0874Mi/s
sha3_384/4096_cv                1.96 %          1.96 %            10      0.22%        0.22%            1.91%
sha3_384/4096_min               7371 ns         7371 ns           10   34.4843k      8.32149      507.015Mi/s
sha3_384/4096_max               7795 ns         7795 ns           10    34.771k      8.39068      536.187Mi/s
sha3_224/4096_mean              5532 ns         5532 ns           10   25.4595k      6.17349      711.141Mi/s
sha3_224/4096_median            5545 ns         5545 ns           10   25.4525k      6.17181      709.282Mi/s
sha3_224/4096_stddev            94.2 ns         94.2 ns           10    67.6755    0.0164102      11.9676Mi/s
sha3_224/4096_cv                1.70 %          1.70 %            10      0.27%        0.27%            1.68%
sha3_224/4096_min               5432 ns         5432 ns           10   25.3394k      6.14437      686.076Mi/s
sha3_224/4096_max               5733 ns         5733 ns           10   25.6042k      6.20858      724.059Mi/s
shake256/1024/64_mean           1535 ns         1534 ns           10   7.05342k      6.48292       676.32Mi/s
shake256/1024/64_median         1540 ns         1540 ns           10   7.06512k      6.49368      673.576Mi/s
shake256/1024/64_stddev         22.5 ns         22.7 ns           10    42.2604    0.0388423      10.0294Mi/s
shake256/1024/64_cv             1.47 %          1.48 %            10      0.60%        0.60%            1.48%
shake256/1024/64_min            1508 ns         1508 ns           10   6.93534k      6.37439      662.005Mi/s
shake256/1024/64_max            1567 ns         1567 ns           10   7.07912k      6.50655      688.122Mi/s
sha3_512/64_mean                 215 ns          215 ns           10   1.00542k      7.85481      566.518Mi/s
sha3_512/64_median               215 ns          215 ns           10   1.00529k      7.85385      568.437Mi/s
sha3_512/64_stddev              1.52 ns         1.52 ns           10    2.25005    0.0175785      3.96599Mi/s
sha3_512/64_cv                  0.71 %          0.70 %            10      0.22%        0.22%            0.70%
sha3_512/64_min                  214 ns          214 ns           10   1.00278k      7.83418      559.677Mi/s
sha3_512/64_max                  218 ns          218 ns           10    1.0107k      7.89611      570.428Mi/s
shake128/64/64_mean              206 ns          206 ns           10    951.967      7.43724      591.569Mi/s
shake128/64/64_median            206 ns          206 ns           10     951.08      7.43031      593.714Mi/s
shake128/64/64_stddev           3.13 ns         3.13 ns           10    4.31938    0.0337451      8.87081Mi/s
shake128/64/64_cv               1.52 %          1.52 %            10      0.45%        0.45%            1.50%
shake128/64/64_min               202 ns          202 ns           10     946.77      7.39664      572.961Mi/s
shake128/64/64_max               213 ns          213 ns           10    961.409      7.51101      602.924Mi/s
sha3_256/16384_mean            22972 ns        22971 ns           10   104.863k      6.38786      681.748Mi/s
sha3_256/16384_median          23020 ns        23020 ns           10   104.825k      6.38551      680.089Mi/s
sha3_256/16384_stddev            426 ns          426 ns           10    259.584    0.0158129      12.6967Mi/s
sha3_256/16384_cv               1.85 %          1.86 %            10      0.25%        0.25%            1.86%
sha3_256/16384_min             22360 ns        22359 ns           10    104.49k      6.36511      664.996Mi/s
sha3_256/16384_max             23542 ns        23542 ns           10    105.24k      6.41079      700.201Mi/s
sha3_512/1024_mean              2903 ns         2903 ns           10    13.506k      12.4136      357.412Mi/s
sha3_512/1024_median            2897 ns         2897 ns           10   13.5026k      12.4105      358.117Mi/s
sha3_512/1024_stddev            28.1 ns         28.1 ns           10    15.7898    0.0145126      3.40526Mi/s
sha3_512/1024_cv                0.97 %          0.97 %            10      0.12%        0.12%            0.95%
sha3_512/1024_min               2879 ns         2879 ns           10   13.4818k      12.3913      348.632Mi/s
sha3_512/1024_max               2976 ns         2976 ns           10   13.5312k      12.4367      360.364Mi/s
sha3_384/1024_mean              1909 ns         1909 ns           10   8.76234k      8.17383      535.654Mi/s
sha3_384/1024_median            1901 ns         1901 ns           10   8.76342k      8.17483      537.835Mi/s
sha3_384/1024_stddev            37.4 ns         37.4 ns           10     17.098    0.0159496      10.4208Mi/s
sha3_384/1024_cv                1.96 %          1.96 %            10      0.20%        0.20%            1.95%
sha3_384/1024_min               1867 ns         1867 ns           10   8.73761k      8.15075      519.858Mi/s
sha3_384/1024_max               1967 ns         1967 ns           10   8.79026k      8.19987      547.652Mi/s
sha3_512/256_mean                795 ns          795 ns           10   3.69684k      11.5526        384.1Mi/s
sha3_512/256_median              791 ns          790 ns           10   3.69629k      11.5509      386.158Mi/s
sha3_512/256_stddev             8.66 ns         8.71 ns           10    4.86183    0.0151932      4.15888Mi/s
sha3_512/256_cv                 1.09 %          1.10 %            10      0.13%        0.13%            1.08%
sha3_512/256_min                 788 ns          788 ns           10   3.68917k      11.5287      376.657Mi/s
sha3_512/256_max                 810 ns          810 ns           10   3.70461k      11.5769      387.253Mi/s
sha3_512/4096_mean             11001 ns        11000 ns           10   50.7833k      12.2075      360.737Mi/s
sha3_512/4096_median           10991 ns        10991 ns           10   50.7718k      12.2048      360.975Mi/s
sha3_512/4096_stddev             148 ns          150 ns           10    86.6208    0.0208223       4.8742Mi/s
sha3_512/4096_cv                1.35 %          1.36 %            10      0.17%        0.17%            1.35%
sha3_512/4096_min              10845 ns        10845 ns           10     50.66k      12.1779      351.713Mi/s
sha3_512/4096_max              11282 ns        11280 ns           10    50.941k      12.2454      365.819Mi/s
sha3_224/64_mean                 206 ns          206 ns           10    962.044       10.457      425.523Mi/s
sha3_224/64_median               206 ns          206 ns           10    961.039      10.4461      426.623Mi/s
sha3_224/64_stddev              2.55 ns         2.56 ns           10    4.39172     0.047736      5.14888Mi/s
sha3_224/64_cv                  1.24 %          1.24 %            10      0.46%        0.46%            1.21%
sha3_224/64_min                  204 ns          204 ns           10    957.616      10.4089      411.863Mi/s
sha3_224/64_max                  213 ns          213 ns           10    971.505      10.5598      429.501Mi/s
sha3_256/64_mean                 204 ns          204 ns           10    945.093      9.84472       448.01Mi/s
sha3_256/64_median               203 ns          203 ns           10    946.431      9.85865      450.786Mi/s
sha3_256/64_stddev              3.35 ns         3.35 ns           10    4.88271    0.0508616      7.25403Mi/s
sha3_256/64_cv                  1.64 %          1.64 %            10      0.52%        0.52%            1.62%
sha3_256/64_min                  200 ns          200 ns           10    938.206      9.77298      434.522Mi/s
sha3_256/64_max                  211 ns          211 ns           10    952.231      9.91908       456.64Mi/s
shake128/16384/64_mean         18721 ns        18718 ns           10   85.4555k      5.19549      838.278Mi/s
shake128/16384/64_median       18721 ns        18721 ns           10   85.5107k      5.19885      837.893Mi/s
shake128/16384/64_stddev         337 ns          341 ns           10    473.477    0.0287863      15.2678Mi/s
shake128/16384/64_cv            1.80 %          1.82 %            10      0.55%        0.55%            1.82%
shake128/16384/64_min          18248 ns        18223 ns           10   84.8427k      5.15824       814.36Mi/s
shake128/16384/64_max          19262 ns        19262 ns           10   86.2438k      5.24342      860.788Mi/s
sha3_224/1024_mean              1559 ns         1559 ns           10   7.13697k      6.78419      643.852Mi/s
sha3_224/1024_median            1557 ns         1557 ns           10   7.13532k      6.78262      644.457Mi/s
sha3_224/1024_stddev            32.5 ns         32.5 ns           10    19.8615    0.0188797      13.3742Mi/s
sha3_224/1024_cv                2.08 %          2.08 %            10      0.28%        0.28%            2.08%
sha3_224/1024_min               1519 ns         1519 ns           10   7.10831k      6.75694      624.346Mi/s
sha3_224/1024_max               1607 ns         1607 ns           10   7.16852k      6.81418      660.307Mi/s
sha3_384/256_mean                585 ns          585 ns           10   2.69999k      8.88155      495.407Mi/s
sha3_384/256_median              580 ns          580 ns           10   2.69742k      8.87311       500.05Mi/s
sha3_384/256_stddev             11.1 ns         11.0 ns           10    10.1907    0.0335222       9.2635Mi/s
sha3_384/256_cv                 1.89 %          1.89 %            10      0.38%        0.38%            1.87%
sha3_384/256_min                 575 ns          574 ns           10    2.6921k       8.8556      481.779Mi/s
sha3_384/256_max                 602 ns          602 ns           10   2.72732k      8.97144      504.645Mi/s
shake256/64/64_mean              206 ns          206 ns           10    950.677      7.42717      592.476Mi/s
shake256/64/64_median            205 ns          205 ns           10    949.802      7.42033      594.077Mi/s
shake256/64/64_stddev           3.04 ns         3.04 ns           10    3.81902    0.0298361      8.68042Mi/s
shake256/64/64_cv               1.47 %          1.47 %            10      0.40%        0.40%            1.47%
shake256/64/64_min               202 ns          202 ns           10    944.628      7.37991      577.594Mi/s
shake256/64/64_max               211 ns          211 ns           10    957.561      7.48094      602.936Mi/s
sha3_384/64_mean                 205 ns          205 ns           10    943.255      8.42192      522.395Mi/s
sha3_384/64_median               202 ns          202 ns           10    943.619      8.42517      527.796Mi/s
sha3_384/64_stddev              3.85 ns         3.85 ns           10    2.36019    0.0210731      9.73153Mi/s
sha3_384/64_cv                  1.88 %          1.88 %            10      0.25%        0.25%            1.86%
sha3_384/64_min                  201 ns          201 ns           10    939.775      8.39085      506.249Mi/s
sha3_384/64_max                  211 ns          211 ns           10    947.313      8.45815      531.712Mi/s
shake256/16384/64_mean         22849 ns        22848 ns           10       105k      6.38374      686.812Mi/s
shake256/16384/64_median       22653 ns        22652 ns           10   104.915k      6.37861      692.507Mi/s
shake256/16384/64_stddev         494 ns          494 ns           10    317.577     0.019308      14.6152Mi/s
shake256/16384/64_cv            2.16 %          2.16 %            10      0.30%        0.30%            2.13%
shake256/16384/64_min          22345 ns        22345 ns           10   104.705k      6.36585      658.002Mi/s
shake256/16384/64_max          23839 ns        23839 ns           10   105.582k      6.41916      701.993Mi/s
shake128/1024/64_mean           1372 ns         1372 ns           10   6.23241k      5.72832      756.408Mi/s
shake128/1024/64_median         1369 ns         1369 ns           10   6.23393k      5.72972      757.977Mi/s
shake128/1024/64_stddev         29.2 ns         29.2 ns           10    25.4898    0.0234281       16.116Mi/s
shake128/1024/64_cv             2.13 %          2.13 %            10      0.41%        0.41%            2.13%
shake128/1024/64_min            1329 ns         1329 ns           10   6.18661k      5.68622      734.011Mi/s
shake128/1024/64_max            1414 ns         1414 ns           10   6.28484k      5.77651      780.865Mi/s
sha3_256/4096_mean              5945 ns         5945 ns           10   27.0144k      6.54418      662.389Mi/s
sha3_256/4096_median            5938 ns         5938 ns           10    26.994k      6.53924       662.99Mi/s
sha3_256/4096_stddev            97.4 ns         97.4 ns           10    80.1626    0.0194192       10.923Mi/s
sha3_256/4096_cv                1.64 %          1.64 %            10      0.30%        0.30%            1.65%
sha3_256/4096_min               5755 ns         5755 ns           10    26.905k      6.51768       647.36Mi/s
sha3_256/4096_max               6082 ns         6081 ns           10   27.1688k      6.58159      684.015Mi/s
sha3_256/256_mean                402 ns          402 ns           10   1.86848k      6.48779      683.156Mi/s
sha3_256/256_median              400 ns          400 ns           10   1.86764k      6.48487      686.565Mi/s
sha3_256/256_stddev             4.91 ns         4.91 ns           10    4.08667    0.0141898      8.19903Mi/s
sha3_256/256_cv                 1.22 %          1.22 %            10      0.22%        0.22%            1.20%
sha3_256/256_min                 398 ns          398 ns           10   1.86333k       6.4699      662.983Mi/s
sha3_256/256_max                 414 ns          414 ns           10   1.87568k      6.51278      690.525Mi/s
shake256/4096/64_mean           5906 ns         5905 ns           10   26.9611k      6.48104      672.022Mi/s
shake256/4096/64_median         5899 ns         5899 ns           10   26.9527k        6.479      672.564Mi/s
shake256/4096/64_stddev          107 ns          107 ns           10    52.6918    0.0126663      12.1185Mi/s
shake256/4096/64_cv             1.81 %          1.81 %            10      0.20%        0.20%            1.80%
shake256/4096/64_min            5751 ns         5751 ns           10     26.88k      6.46155      653.138Mi/s
shake256/4096/64_max            6075 ns         6074 ns           10   27.0706k      6.50736       689.89Mi/s
```

### On ARM Cortex-A72 i.e. Raspberry Pi 4B ( compiled with Clang-16.0.6 )

```bash
2023-12-22T22:28:29+05:30
Running ./build/perfs/perf.out
Run on (4 X 1800 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 48 KiB (x4)
  L2 Unified 1024 KiB (x1)
Load Average: 2.66, 3.10, 2.04
-------------------------------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations     CYCLES CYCLES/ BYTE bytes_per_second
-------------------------------------------------------------------------------------------------------------
sha3_384/256_mean               4943 ns         4941 ns           10   8.88107k       29.214      58.6747Mi/s
sha3_384/256_median             4943 ns         4941 ns           10   8.88101k      29.2138      58.6736Mi/s
sha3_384/256_stddev             1.08 ns        0.599 ns           10    0.37254     1.22546m      7.28721Ki/s
sha3_384/256_cv                 0.02 %          0.01 %            10      0.00%        0.00%            0.01%
sha3_384/256_min                4941 ns         4940 ns           10   8.88067k      29.2127       58.666Mi/s
sha3_384/256_max                4944 ns         4942 ns           10   8.88169k      29.2161       58.686Mi/s
sha3_224/1024_mean             13233 ns        13229 ns           10   23.7753k      22.6001        75.84Mi/s
sha3_224/1024_median           13233 ns        13229 ns           10    23.774k      22.5989      75.8375Mi/s
sha3_224/1024_stddev            5.09 ns         4.63 ns           10    6.07897     5.77849m      27.1864Ki/s
sha3_224/1024_cv                0.04 %          0.04 %            10      0.03%        0.03%            0.04%
sha3_224/1024_min              13224 ns        13221 ns           10   23.7677k      22.5929      75.7993Mi/s
sha3_224/1024_max              13241 ns        13236 ns           10   23.7864k      22.6106      75.8855Mi/s
shake128/256/64_mean            3553 ns         3552 ns           10   6.38394k      19.9498      85.9233Mi/s
shake128/256/64_median          3553 ns         3552 ns           10   6.38396k      19.9499      85.9209Mi/s
shake128/256/64_stddev         0.927 ns        0.618 ns           10   0.327318     1.02287m      15.3177Ki/s
shake128/256/64_cv              0.03 %          0.02 %            10      0.01%        0.01%            0.02%
shake128/256/64_min             3552 ns         3551 ns           10   6.38342k      19.9482      85.8936Mi/s
shake128/256/64_max             3555 ns         3553 ns           10   6.38454k      19.9517       85.942Mi/s
sha3_256/256_mean               3496 ns         3495 ns           10   6.28129k        21.81      78.5915Mi/s
sha3_256/256_median             3496 ns         3495 ns           10   6.28122k      21.8098      78.5964Mi/s
sha3_256/256_stddev             1.65 ns         1.07 ns           10   0.237549     824.824u      24.5521Ki/s
sha3_256/256_cv                 0.05 %          0.03 %            10      0.00%        0.00%            0.03%
sha3_256/256_min                3495 ns         3494 ns           10   6.28105k      21.8092      78.5259Mi/s
sha3_256/256_max                3500 ns         3498 ns           10   6.28184k      21.8119      78.6115Mi/s
sha3_512/16384_mean           367366 ns       367240 ns           10   659.951k      40.1235      42.7134Mi/s
sha3_512/16384_median         367342 ns       367217 ns           10   659.873k      40.1187       42.716Mi/s
sha3_512/16384_stddev            300 ns          279 ns           10    471.478    0.0286648       33.279Ki/s
sha3_512/16384_cv               0.08 %          0.08 %            10      0.07%        0.07%            0.08%
sha3_512/16384_min            366945 ns       366874 ns           10   659.364k      40.0878       42.674Mi/s
sha3_512/16384_max            367765 ns       367578 ns           10   660.686k      40.1682      42.7559Mi/s
shake256/256/64_mean            3501 ns         3500 ns           10   6.29015k      19.6567      87.2001Mi/s
shake256/256/64_median          3501 ns         3500 ns           10   6.29016k      19.6567      87.1962Mi/s
shake256/256/64_stddev         0.743 ns        0.490 ns           10   0.228355     713.608u      12.5052Ki/s
shake256/256/64_cv              0.02 %          0.01 %            10      0.00%        0.00%            0.01%
shake256/256/64_min             3500 ns         3499 ns           10   6.28986k      19.6558      87.1844Mi/s
shake256/256/64_max             3502 ns         3500 ns           10   6.29056k       19.658      87.2211Mi/s
sha3_512/256_mean               6548 ns         6546 ns           10   11.7647k      36.7648      46.6206Mi/s
sha3_512/256_median             6548 ns         6547 ns           10   11.7674k      36.7731      46.6163Mi/s
sha3_512/256_stddev             3.88 ns         3.93 ns           10    6.45845    0.0201827      28.6518Ki/s
sha3_512/256_cv                 0.06 %          0.06 %            10      0.05%        0.05%            0.06%
sha3_512/256_min                6543 ns         6541 ns           10   11.7555k       36.736      46.5696Mi/s
sha3_512/256_max                6556 ns         6553 ns           10   11.7721k      36.7877      46.6592Mi/s
sha3_384/64_mean                1713 ns         1713 ns           10   3.07815k      27.4835      62.3696Mi/s
sha3_384/64_median              1713 ns         1713 ns           10    3.0782k      27.4839      62.3714Mi/s
sha3_384/64_stddev             0.979 ns        0.880 ns           10    1.45719    0.0130106      32.8174Ki/s
sha3_384/64_cv                  0.06 %          0.05 %            10      0.05%        0.05%            0.05%
sha3_384/64_min                 1712 ns         1711 ns           10   3.07641k       27.468       62.324Mi/s
sha3_384/64_max                 1714 ns         1714 ns           10    3.0801k      27.5009      62.4126Mi/s
sha3_384/16384_mean           255253 ns       255187 ns           10   458.662k      27.9127       61.409Mi/s
sha3_384/16384_median         255323 ns       255260 ns           10   458.788k      27.9204      61.3915Mi/s
sha3_384/16384_stddev            178 ns          177 ns           10    329.243    0.0200367      43.5852Ki/s
sha3_384/16384_cv               0.07 %          0.07 %            10      0.07%        0.07%            0.07%
sha3_384/16384_min            254996 ns       254914 ns           10   458.152k      27.8817       61.364Mi/s
sha3_384/16384_max            255452 ns       255374 ns           10   458.957k      27.9307      61.4746Mi/s
sha3_256/64_mean                1872 ns         1871 ns           10   3.36344k      35.0358      48.9256Mi/s
sha3_256/64_median              1872 ns         1871 ns           10   3.36339k      35.0354      48.9247Mi/s
sha3_256/64_stddev             0.255 ns        0.155 ns           10   0.157224     1.63775m      4.15276Ki/s
sha3_256/64_cv                  0.01 %          0.01 %            10      0.00%        0.00%            0.01%
sha3_256/64_min                 1871 ns         1871 ns           10   3.36325k      35.0339      48.9202Mi/s
sha3_256/64_max                 1872 ns         1871 ns           10   3.36375k       35.039      48.9341Mi/s
shake256/1024/64_mean          13212 ns        13209 ns           10   23.7416k      21.8213      78.5551Mi/s
shake256/1024/64_median        13212 ns        13208 ns           10   23.7398k      21.8197      78.5571Mi/s
shake256/1024/64_stddev         2.97 ns         2.57 ns           10    3.89029     3.57563m       15.654Ki/s
shake256/1024/64_cv             0.02 %          0.02 %            10      0.02%        0.02%            0.02%
shake256/1024/64_min           13208 ns        13205 ns           10   23.7381k      21.8181      78.5244Mi/s
shake256/1024/64_max           13217 ns        13214 ns           10   23.7489k       21.828      78.5745Mi/s
sha3_512/64_mean                1712 ns         1711 ns           10   3.07627k      24.0333      71.3246Mi/s
sha3_512/64_median              1712 ns         1711 ns           10   3.07618k      24.0327      71.3253Mi/s
sha3_512/64_stddev             0.572 ns        0.380 ns           10   0.367503     2.87112m      16.2209Ki/s
sha3_512/64_cv                  0.03 %          0.02 %            10      0.01%        0.01%            0.02%
sha3_512/64_min                 1711 ns         1711 ns           10    3.0756k      24.0281      71.3013Mi/s
sha3_512/64_max                 1713 ns         1712 ns           10   3.07693k      24.0385      71.3504Mi/s
shake256/64/64_mean             1878 ns         1877 ns           10   3.37372k      26.3572      65.0314Mi/s
shake256/64/64_median           1878 ns         1877 ns           10   3.37375k      26.3574      65.0351Mi/s
shake256/64/64_stddev          0.837 ns        0.588 ns           10   0.239178     1.86858m      20.8361Ki/s
shake256/64/64_cv               0.04 %          0.03 %            10      0.01%        0.01%            0.03%
shake256/64/64_min              1877 ns         1877 ns           10    3.3734k      26.3547      64.9787Mi/s
shake256/64/64_max              1880 ns         1879 ns           10   3.37421k       26.361      65.0501Mi/s
shake128/64/64_mean             1926 ns         1925 ns           10   3.46032k      27.0337      63.4077Mi/s
shake128/64/64_median           1926 ns         1925 ns           10   3.46036k      27.0341      63.4059Mi/s
shake128/64/64_stddev          0.745 ns        0.558 ns           10    0.61368     4.79438m      18.8164Ki/s
shake128/64/64_cv               0.04 %          0.03 %            10      0.02%        0.02%            0.03%
shake128/64/64_min              1925 ns         1924 ns           10   3.45934k      27.0261      63.3744Mi/s
shake128/64/64_max              1927 ns         1926 ns           10   3.46119k      27.0406      63.4346Mi/s
sha3_224/256_mean               3509 ns         3508 ns           10   6.30456k      22.1991       77.216Mi/s
sha3_224/256_median             3508 ns         3507 ns           10   6.30454k      22.1991      77.2192Mi/s
sha3_224/256_stddev            0.834 ns        0.583 ns           10   0.182439      642.39u      13.1501Ki/s
sha3_224/256_cv                 0.02 %          0.02 %            10      0.00%        0.00%            0.02%
sha3_224/256_min                3508 ns         3507 ns           10    6.3043k      22.1982      77.1953Mi/s
sha3_224/256_max                3510 ns         3509 ns           10   6.30483k      22.2001      77.2309Mi/s
sha3_256/1024_mean             13216 ns        13212 ns           10   23.7456k      22.4864      76.2252Mi/s
sha3_256/1024_median           13215 ns        13211 ns           10   23.7457k      22.4864      76.2324Mi/s
sha3_256/1024_stddev            7.18 ns         7.23 ns           10    13.2008    0.0125007      42.7151Ki/s
sha3_256/1024_cv                0.05 %          0.05 %            10      0.06%        0.06%            0.05%
sha3_256/1024_min              13206 ns        13202 ns           10    23.731k      22.4725      76.1697Mi/s
sha3_256/1024_max              13226 ns        13222 ns           10     23.76k         22.5      76.2834Mi/s
sha3_224/64_mean                1885 ns         1884 ns           10   3.38691k      36.8142      46.5611Mi/s
sha3_224/64_median              1885 ns         1884 ns           10   3.38676k      36.8126       46.561Mi/s
sha3_224/64_stddev             0.402 ns        0.312 ns           10   0.431011     4.68491m      7.88678Ki/s
sha3_224/64_cv                  0.02 %          0.02 %            10      0.01%        0.01%            0.02%
sha3_224/64_min                 1884 ns         1884 ns           10   3.38652k        36.81      46.5443Mi/s
sha3_224/64_max                 1886 ns         1885 ns           10   3.38806k      36.8268      46.5747Mi/s
shake256/16384/64_mean        196275 ns       196221 ns           10    352.67k      21.4415      79.9407Mi/s
shake256/16384/64_median      196279 ns       196219 ns           10   352.626k      21.4388      79.9415Mi/s
shake256/16384/64_stddev         151 ns          149 ns           10    274.278    0.0166754      62.2572Ki/s
shake256/16384/64_cv            0.08 %          0.08 %            10      0.08%        0.08%            0.08%
shake256/16384/64_min         196056 ns       196018 ns           10   352.321k      21.4203       79.844Mi/s
shake256/16384/64_max         196519 ns       196458 ns           10   353.092k      21.4672      80.0234Mi/s
shake128/16384/64_mean        159491 ns       159442 ns           10   286.555k      17.4219      98.3813Mi/s
shake128/16384/64_median      159429 ns       159382 ns           10    286.44k      17.4149      98.4179Mi/s
shake128/16384/64_stddev         252 ns          243 ns           10    411.169    0.0249981      153.209Ki/s
shake128/16384/64_cv            0.16 %          0.15 %            10      0.14%        0.14%            0.15%
shake128/16384/64_min         159179 ns       159148 ns           10   286.062k      17.3919       98.064Mi/s
shake128/16384/64_max         160029 ns       159957 ns           10   287.434k      17.4753      98.5625Mi/s
sha3_512/4096_mean             91818 ns        91794 ns           10   164.996k      39.6624      43.2193Mi/s
sha3_512/4096_median           91824 ns        91803 ns           10       165k      39.6635      43.2153Mi/s
sha3_512/4096_stddev            23.7 ns         19.0 ns           10    26.2386     6.30735m      9.15854Ki/s
sha3_512/4096_cv                0.03 %          0.02 %            10      0.02%        0.02%            0.02%
sha3_512/4096_min              91779 ns        91764 ns           10   164.962k      39.6544      43.2106Mi/s
sha3_512/4096_max              91842 ns        91813 ns           10   165.027k        39.67      43.2335Mi/s
sha3_384/4096_mean             64650 ns        64633 ns           10   116.172k      28.0338      61.1454Mi/s
sha3_384/4096_median           64648 ns        64634 ns           10   116.179k      28.0354      61.1446Mi/s
sha3_384/4096_stddev            18.7 ns         16.2 ns           10    22.5271     5.43608m       15.713Ki/s
sha3_384/4096_cv                0.03 %          0.03 %            10      0.02%        0.02%            0.03%
sha3_384/4096_min              64620 ns        64609 ns           10   116.135k      28.0248      61.1187Mi/s
sha3_384/4096_max              64679 ns        64662 ns           10   116.212k      28.0434      61.1682Mi/s
shake128/1024/64_mean          11665 ns        11661 ns           10   20.9578k      19.2627      88.9798Mi/s
shake128/1024/64_median        11664 ns        11661 ns           10    20.957k      19.2619      88.9816Mi/s
shake128/1024/64_stddev         3.34 ns         2.47 ns           10    2.30432     2.11794m      19.2591Ki/s
shake128/1024/64_cv             0.03 %          0.02 %            10      0.01%        0.01%            0.02%
shake128/1024/64_min           11659 ns        11657 ns           10   20.9553k      19.2604      88.9427Mi/s
shake128/1024/64_max           11672 ns        11666 ns           10    20.961k      19.2656      89.0097Mi/s
sha3_256/16384_mean           196371 ns       196308 ns           10   352.791k      21.4907      79.7498Mi/s
sha3_256/16384_median         196381 ns       196293 ns           10    352.73k      21.4869       79.756Mi/s
sha3_256/16384_stddev            166 ns          181 ns           10    384.334    0.0234122      75.1445Ki/s
sha3_256/16384_cv               0.08 %          0.09 %            10      0.11%        0.11%            0.09%
sha3_256/16384_min            196068 ns       196002 ns           10   352.231k      21.4566      79.6597Mi/s
sha3_256/16384_max            196561 ns       196530 ns           10   353.287k      21.5209      79.8744Mi/s
sha3_256/4096_mean             50418 ns        50402 ns           10   90.5849k       21.944      78.1071Mi/s
sha3_256/4096_median           50412 ns        50397 ns           10   90.5776k      21.9423      78.1157Mi/s
sha3_256/4096_stddev            25.0 ns         17.2 ns           10    20.5874     4.98725m      27.2261Ki/s
sha3_256/4096_cv                0.05 %          0.03 %            10      0.02%        0.02%            0.03%
sha3_256/4096_min              50386 ns        50377 ns           10   90.5655k      21.9393      78.0548Mi/s
sha3_256/4096_max              50474 ns        50436 ns           10   90.6293k      21.9548      78.1461Mi/s
sha3_224/4096_mean             47250 ns        47238 ns           10   84.9067k      20.5884      83.2579Mi/s
sha3_224/4096_median           47246 ns        47232 ns           10   84.8967k       20.586      83.2685Mi/s
sha3_224/4096_stddev            18.7 ns         15.6 ns           10    19.1114     4.63419m      28.1343Ki/s
sha3_224/4096_cv                0.04 %          0.03 %            10      0.02%        0.02%            0.03%
sha3_224/4096_min              47230 ns        47222 ns           10   84.8926k       20.585      83.2037Mi/s
sha3_224/4096_max              47288 ns        47269 ns           10   84.9454k      20.5978      83.2869Mi/s
keccak-p[1600, 24]_mean         1596 ns         1596 ns           10   2.86863k      14.3432      119.512Mi/s
keccak-p[1600, 24]_median       1596 ns         1596 ns           10    2.8686k       14.343      119.513Mi/s
keccak-p[1600, 24]_stddev      0.372 ns        0.257 ns           10   0.119287     596.436u      19.6693Ki/s
keccak-p[1600, 24]_cv           0.02 %          0.02 %            10      0.00%        0.00%            0.02%
keccak-p[1600, 24]_min          1596 ns         1596 ns           10   2.86855k      14.3427      119.478Mi/s
keccak-p[1600, 24]_max          1597 ns         1596 ns           10   2.86896k      14.3448      119.541Mi/s
shake128/4096/64_mean          40857 ns        40845 ns           10    73.408k      17.6462      97.1305Mi/s
shake128/4096/64_median        40837 ns        40827 ns           10   73.3832k      17.6402      97.1728Mi/s
shake128/4096/64_stddev         37.4 ns         35.4 ns           10    45.2564    0.0108789      86.1828Ki/s
shake128/4096/64_cv             0.09 %          0.09 %            10      0.06%        0.06%            0.09%
shake128/4096/64_min           40827 ns        40818 ns           10   73.3707k      17.6372      96.9358Mi/s
shake128/4096/64_max           40941 ns        40927 ns           10   73.5104k      17.6708      97.1944Mi/s
sha3_384/1024_mean             16247 ns        16241 ns           10   29.1909k      27.2303      62.9467Mi/s
sha3_384/1024_median           16247 ns        16241 ns           10   29.1903k      27.2298       62.948Mi/s
sha3_384/1024_stddev            4.54 ns         2.93 ns           10    1.83977      1.7162m      11.6162Ki/s
sha3_384/1024_cv                0.03 %          0.02 %            10      0.01%        0.01%            0.02%
sha3_384/1024_min              16241 ns        16237 ns           10   29.1888k      27.2283      62.9262Mi/s
sha3_384/1024_max              16254 ns        16247 ns           10   29.1941k      27.2333      62.9616Mi/s
sha3_224/16384_mean           185317 ns       185264 ns           10    332.97k      20.2882      84.4834Mi/s
sha3_224/16384_median         185397 ns       185354 ns           10   333.143k      20.2987      84.4422Mi/s
sha3_224/16384_stddev            219 ns          224 ns           10    419.686    0.0255719      104.672Ki/s
sha3_224/16384_cv               0.12 %          0.12 %            10      0.13%        0.13%            0.12%
sha3_224/16384_min            184990 ns       184929 ns           10   332.357k      20.2509      84.3771Mi/s
sha3_224/16384_max            185566 ns       185497 ns           10   333.366k      20.3123      84.6365Mi/s
sha3_512/1024_mean             24255 ns        24247 ns           10   43.5775k      40.0528      42.7932Mi/s
sha3_512/1024_median           24255 ns        24248 ns           10   43.5768k      40.0522      42.7908Mi/s
sha3_512/1024_stddev            6.55 ns         5.50 ns           10    6.53887        6.01m      9.94629Ki/s
sha3_512/1024_cv                0.03 %          0.02 %            10      0.02%        0.02%            0.02%
sha3_512/1024_min              24246 ns        24239 ns           10   43.5684k      40.0445      42.7794Mi/s
sha3_512/1024_max              24263 ns        24255 ns           10    43.588k      40.0625      42.8066Mi/s
shake256/4096/64_mean          50411 ns        50398 ns           10   90.5836k      21.7749      78.7198Mi/s
shake256/4096/64_median        50411 ns        50395 ns           10   90.5739k      21.7726      78.7242Mi/s
shake256/4096/64_stddev         17.2 ns         15.7 ns           10    23.0543     5.54191m      25.1537Ki/s
shake256/4096/64_cv             0.03 %          0.03 %            10      0.03%        0.03%            0.03%
shake256/4096/64_min           50385 ns        50376 ns           10   90.5585k      21.7689      78.6656Mi/s
shake256/4096/64_max           50447 ns        50432 ns           10   90.6309k      21.7863      78.7533Mi/s
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
