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
make -j
```

```bash
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
[       OK ] Sha3Hashing.Sha3_512KnownAnswerTests (2 ms)
[----------] 12 tests from Sha3Hashing (16 ms total)

[----------] 6 tests from Sha3Xof
[ RUN      ] Sha3Xof.CompileTimeEvalShake128
[       OK ] Sha3Xof.CompileTimeEvalShake128 (0 ms)
[ RUN      ] Sha3Xof.Shake128IncrementalAbsorptionAndSqueezing
[       OK ] Sha3Xof.Shake128IncrementalAbsorptionAndSqueezing (927 ms)
[ RUN      ] Sha3Xof.Shake128KnownAnswerTests
[       OK ] Sha3Xof.Shake128KnownAnswerTests (2 ms)
[ RUN      ] Sha3Xof.CompileTimeEvalShake256
[       OK ] Sha3Xof.CompileTimeEvalShake256 (0 ms)
[ RUN      ] Sha3Xof.Shake256IncrementalAbsorptionAndSqueezing
[       OK ] Sha3Xof.Shake256IncrementalAbsorptionAndSqueezing (996 ms)
[ RUN      ] Sha3Xof.Shake256KnownAnswerTests
[       OK ] Sha3Xof.Shake256KnownAnswerTests (2 ms)
[----------] 6 tests from Sha3Xof (1929 ms total)

[----------] Global test environment tear-down
[==========] 18 tests from 2 test suites ran. (1945 ms total)
[  PASSED  ] 18 tests.
```

## Benchmarking

For benchmarking SHA3 hash and extendable output functions, targeting CPU systems, using `google-benchmark`, issue following command.

> **Warning** You must disable CPU frequency scaling during benchmarking, following [this](https://github.com/google/benchmark/blob/4931aefb51d1e5872b096a97f43e13fa0fc33c8c/docs/reducing_variance.md) guide.

> **Note** When benchmarking extendable output functions ( Xofs ), fixed length output of 32/ 64 -bytes are squeezed from sponge ( s.t. all bytes are read in a single call to `squeeze` function ), for each input message byte array of length N | 32 <= N <= 4096 and N = 2^i.

> **Note** Following performance figures were collected by issuing `make perf` - on machines running GNU/Linux kernel, with `google-benchmark` library compiled with *libPFM* support.

```bash
make perf -j      # You must issue this if you built your google-benchmark library with libPFM support.
make benchmark -j # Or you can simply use this.
```

### On 12th Gen Intel(R) Core(TM) i7-1260P ( compiled with Clang-16.0.0 )

```bash
2023-09-28T00:26:51+04:00
Running ./build/perfs/perf.out
Run on (16 X 2500 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 0.99, 0.46, 0.49
----------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations     CYCLES CYCLES/ BYTE INSTRUCTIONS INSTRUCTIONS/ CYCLE bytes_per_second
----------------------------------------------------------------------------------------------------------------------------------------------
shake128/256/32_mean             404 ns          404 ns            8    1.8811k      6.53159       9.121k             4.84878      679.718Mi/s
shake128/256/32_median           403 ns          403 ns            8   1.88045k      6.52935       9.121k             4.85043      681.068Mi/s
shake128/256/32_stddev          2.50 ns         2.50 ns            8    3.73978    0.0129853            0            9.62625m      4.17064Mi/s
shake128/256/32_cv              0.62 %          0.62 %             8      0.20%        0.20%        0.00%               0.20%            0.61%
sha3_224/4096_mean              5442 ns         5442 ns            8   25.1696k      6.10321     128.559k             5.10772      722.846Mi/s
sha3_224/4096_median            5405 ns         5405 ns            8   25.1668k      6.10252     128.559k             5.10829      727.694Mi/s
sha3_224/4096_stddev            68.5 ns         68.6 ns            8    49.7488    0.0120632            0           0.0100904      9.02988Mi/s
sha3_224/4096_cv                1.26 %          1.26 %             8      0.20%        0.20%        0.00%               0.20%            1.25%
sha3_512/1024_mean              2800 ns         2800 ns            8   13.0365k      11.9821      66.515k             5.10226      370.608Mi/s
sha3_512/1024_median            2793 ns         2793 ns            8   13.0271k      11.9734      66.515k              5.1059       371.49Mi/s
sha3_512/1024_stddev            22.9 ns         23.0 ns            8    37.9431    0.0348742            0           0.0148212      2.99632Mi/s
sha3_512/1024_cv                0.82 %          0.82 %             8      0.29%        0.29%        0.00%               0.29%            0.81%
shake256/512/64_mean             773 ns          773 ns            8   3.60003k      6.25005      17.966k             4.99056      710.886Mi/s
shake256/512/64_median           772 ns          772 ns            8   3.59628k      6.24355      17.966k             4.99572      711.397Mi/s
shake256/512/64_stddev          4.43 ns         4.44 ns            8    12.4948    0.0216924            0           0.0172918      4.06997Mi/s
shake256/512/64_cv              0.57 %          0.57 %             8      0.35%        0.35%        0.00%               0.35%            0.57%
sha3_224/1024_mean              1513 ns         1513 ns            8   7.05273k      6.70412      35.612k             5.04953      662.988Mi/s
sha3_224/1024_median            1506 ns         1506 ns            8   7.03748k      6.68962      35.612k             5.06033      666.216Mi/s
sha3_224/1024_stddev            15.1 ns         15.2 ns            8    39.5373     0.037583            0           0.0280081      6.56381Mi/s
sha3_224/1024_cv                1.00 %          1.00 %             8      0.56%        0.56%        0.00%               0.55%            0.99%
shake256/256/32_mean             401 ns          401 ns            8   1.86655k      6.48108       9.115k             4.88354      684.936Mi/s
shake256/256/32_median           401 ns          401 ns            8   1.86302k      6.46881       9.115k             4.89263      685.062Mi/s
shake256/256/32_stddev          3.31 ns         3.32 ns            8    12.8624    0.0446611     130.499u           0.0333172      5.66607Mi/s
shake256/256/32_cv              0.83 %          0.83 %             8      0.69%        0.69%        0.00%               0.68%            0.83%
keccak-p[1600, 24]_mean          182 ns          182 ns            8    838.098      4.19049         4.4k              5.2501      1.02629Gi/s
keccak-p[1600, 24]_median        181 ns          181 ns            8    835.532      4.17766         4.4k             5.26611      1.03131Gi/s
keccak-p[1600, 24]_stddev       1.62 ns         1.62 ns            8    4.24311    0.0212155            0            0.026518      9.33705Mi/s
keccak-p[1600, 24]_cv           0.89 %          0.89 %             8      0.51%        0.51%        0.00%               0.51%            0.89%
sha3_256/2048_mean              2996 ns         2996 ns            8   13.9702k      6.71646      71.007k             5.08275      662.219Mi/s
sha3_256/2048_median            2990 ns         2990 ns            8   13.9681k      6.71544      71.007k             5.08351      663.318Mi/s
sha3_256/2048_stddev            19.8 ns         19.7 ns            8    27.9509    0.0134379            0            0.010161      4.30894Mi/s
sha3_256/2048_cv                0.66 %          0.66 %             8      0.20%        0.20%        0.00%               0.20%            0.65%
sha3_224/64_mean                 203 ns          203 ns            8    945.634      10.2786       4.593k             4.85717      432.388Mi/s
sha3_224/64_median               202 ns          202 ns            8     944.35      10.2647       4.593k             4.86366      433.544Mi/s
sha3_224/64_stddev              1.37 ns         1.38 ns            8    4.77567    0.0519095     65.2493u           0.0244875      2.91866Mi/s
sha3_224/64_cv                  0.68 %          0.68 %             8      0.51%        0.51%        0.00%               0.50%            0.68%
sha3_512/32_mean                 201 ns          201 ns            8     939.76      9.78916       4.685k             4.98536       455.13Mi/s
sha3_512/32_median               201 ns          201 ns            8    939.076      9.78204       4.685k             4.98895      455.168Mi/s
sha3_512/32_stddev             0.647 ns        0.647 ns            8    3.07294    0.0320098            0           0.0162857      1.45904Mi/s
sha3_512/32_cv                  0.32 %          0.32 %             8      0.33%        0.33%        0.00%               0.33%            0.32%
shake256/128/64_mean             202 ns          202 ns            8    943.386      4.91347       4.655k             4.93445        906.1Mi/s
shake256/128/64_median           202 ns          202 ns            8    943.224      4.91262       4.655k              4.9352      905.985Mi/s
shake256/128/64_stddev          1.42 ns         1.41 ns            8    4.36384    0.0227283            0           0.0228207      6.31483Mi/s
shake256/128/64_cv              0.70 %          0.70 %             8      0.46%        0.46%        0.00%               0.46%            0.70%
sha3_384/128_mean                394 ns          394 ns            8   1.83584k      10.4309       9.096k             4.95488      425.549Mi/s
sha3_384/128_median              395 ns          395 ns            8   1.82977k      10.3964       9.096k             4.97113      425.311Mi/s
sha3_384/128_stddev             2.95 ns         2.94 ns            8    12.1804    0.0692067     184.553u           0.0327429      3.17047Mi/s
sha3_384/128_cv                 0.75 %          0.75 %             8      0.66%        0.66%        0.00%               0.66%            0.75%
shake128/128/32_mean             205 ns          205 ns            8    957.313      5.98321       4.652k             4.85947      743.436Mi/s
shake128/128/32_median           205 ns          205 ns            8    957.071      5.98169       4.652k             4.86067      744.739Mi/s
shake128/128/32_stddev          1.57 ns         1.57 ns            8    2.86577     0.017911     65.2493u            0.014513      5.66034Mi/s
shake128/128/32_cv              0.77 %          0.77 %             8      0.30%        0.30%        0.00%               0.30%            0.76%
sha3_256/512_mean                771 ns          771 ns            8   3.58892k      6.59729      17.928k             4.99538      672.979Mi/s
sha3_256/512_median              769 ns          769 ns            8   3.58998k      6.59923      17.928k              4.9939      675.009Mi/s
sha3_256/512_stddev             6.24 ns         6.23 ns            8    4.82632     8.87191m            0            6.72017m      5.38628Mi/s
sha3_256/512_cv                 0.81 %          0.81 %             8      0.13%        0.13%        0.00%               0.13%            0.80%
shake256/4096/32_mean           5794 ns         5794 ns            8    26.935k      6.52496     137.403k             5.10129      679.487Mi/s
shake256/4096/32_median         5782 ns         5781 ns            8   26.9186k      6.52097     137.403k             5.10439      680.931Mi/s
shake256/4096/32_stddev         43.6 ns         43.6 ns            8    55.3877    0.0134176            0           0.0104728      5.07969Mi/s
shake256/4096/32_cv             0.75 %          0.75 %             8      0.21%        0.21%        0.00%               0.21%            0.75%
sha3_512/4096_mean             10608 ns        10608 ns            8   49.1727k      11.8204     251.904k             5.12298       374.04Mi/s
sha3_512/4096_median           10575 ns        10575 ns            8   49.0487k      11.7906     251.904k             5.13579      375.155Mi/s
sha3_512/4096_stddev             119 ns          119 ns            8    272.973    0.0656186     5.11448m           0.0283157      4.16745Mi/s
sha3_512/4096_cv                1.12 %          1.12 %             8      0.56%        0.56%        0.00%               0.55%            1.11%
shake128/32/64_mean              206 ns          206 ns            8     962.01      10.0209       4.642k             4.82538       443.86Mi/s
shake128/32/64_median            206 ns          206 ns            8    962.096      10.0218       4.642k             4.82488      444.605Mi/s
shake128/32/64_stddev           1.13 ns         1.13 ns            8    3.67573    0.0382888            0           0.0184227      2.43211Mi/s
shake128/32/64_cv               0.55 %          0.55 %             8      0.38%        0.38%        0.00%               0.38%            0.55%
shake128/64/64_mean              207 ns          207 ns            8    962.707      7.52115       4.642k             4.82183      591.081Mi/s
shake128/64/64_median            206 ns          206 ns            8    962.457       7.5192       4.642k             4.82307      591.155Mi/s
shake128/64/64_stddev          0.616 ns        0.615 ns            8    1.37175    0.0107168            0            6.86937m      1.76213Mi/s
shake128/64/64_cv               0.30 %          0.30 %             8      0.14%        0.14%        0.00%               0.14%            0.30%
sha3_224/32_mean                 203 ns          203 ns            8    945.045      15.7508       4.593k             4.86014      281.692Mi/s
sha3_224/32_median               203 ns          203 ns            8    944.221       15.737       4.593k             4.86433      281.521Mi/s
sha3_224/32_stddev             0.967 ns        0.969 ns            8    3.40145    0.0566908            0           0.0174181      1.34482Mi/s
sha3_224/32_cv                  0.48 %          0.48 %             8      0.36%        0.36%        0.00%               0.36%            0.48%
shake256/32/32_mean              204 ns          204 ns            8    952.237      14.8787       4.645k             4.87803      299.079Mi/s
shake256/32/32_median            204 ns          204 ns            8    951.435      14.8662       4.645k              4.8821      298.842Mi/s
shake256/32/32_stddev          0.987 ns        0.988 ns            8    3.01514    0.0471116            0            0.015408      1.44597Mi/s
shake256/32/32_cv               0.48 %          0.48 %             8      0.32%        0.32%        0.00%               0.32%            0.48%
sha3_256/1024_mean              1524 ns         1524 ns            8    7.0666k      6.69186      35.624k             5.04119      661.068Mi/s
sha3_256/1024_median            1514 ns         1514 ns            8   7.06755k      6.69276      35.624k              5.0405      665.299Mi/s
sha3_256/1024_stddev            16.6 ns         16.6 ns            8    9.93354     9.40677m            0            7.08501m      7.15499Mi/s
sha3_256/1024_cv                1.09 %          1.09 %             8      0.14%        0.14%        0.00%               0.14%            1.08%
sha3_224/512_mean                772 ns          772 ns            8   3.57828k      6.62644      17.919k             5.00775       667.31Mi/s
sha3_224/512_median              767 ns          767 ns            8   3.57426k        6.619      17.919k             5.01334      671.227Mi/s
sha3_224/512_stddev             9.68 ns         9.67 ns            8    9.12992    0.0169073            0           0.0127429      8.29523Mi/s
sha3_224/512_cv                 1.25 %          1.25 %             8      0.26%        0.26%        0.00%               0.25%            1.24%
sha3_384/1024_mean              1897 ns         1897 ns            8   8.84416k      8.25015      44.462k             5.02805       538.93Mi/s
sha3_384/1024_median            1892 ns         1892 ns            8   8.80805k      8.21647      44.462k             5.04788      540.336Mi/s
sha3_384/1024_stddev            25.6 ns         25.6 ns            8    117.787     0.109876            0             0.06622      7.22753Mi/s
sha3_384/1024_cv                1.35 %          1.35 %             8      1.33%        1.33%        0.00%               1.32%            1.34%
sha3_512/64_mean                 201 ns          201 ns            8    928.485      7.25379       4.685k             5.04593      608.268Mi/s
sha3_512/64_median               201 ns          201 ns            8    927.089      7.24288       4.685k             5.05346      608.446Mi/s
sha3_512/64_stddev              2.17 ns         2.14 ns            8    3.76078    0.0293811            0           0.0203244      6.46642Mi/s
sha3_512/64_cv                  1.08 %          1.07 %             8      0.41%        0.41%        0.00%               0.40%            1.06%
shake256/256/64_mean             400 ns          400 ns            8   1.86634k      5.83231       9.115k              4.8841      762.233Mi/s
shake256/256/64_median           399 ns          399 ns            8   1.86093k      5.81541       9.115k             4.89809      764.124Mi/s
shake256/256/64_stddev          2.96 ns         2.94 ns            8    13.1456    0.0410799     130.499u           0.0341533      5.56863Mi/s
shake256/256/64_cv              0.74 %          0.73 %             8      0.70%        0.70%        0.00%               0.70%            0.73%
sha3_256/256_mean                399 ns          399 ns            8   1.85675k      6.44703       9.077k             4.88869      689.202Mi/s
sha3_256/256_median              398 ns          398 ns            8   1.85628k      6.44543       9.077k             4.88988       689.82Mi/s
sha3_256/256_stddev             2.29 ns         2.30 ns            8    4.78397     0.016611     130.499u             0.01258       3.9746Mi/s
sha3_256/256_cv                 0.57 %          0.58 %             8      0.26%        0.26%        0.00%               0.26%            0.58%
shake128/64/32_mean              208 ns          208 ns            8    964.543      10.0473       4.642k             4.81289       441.12Mi/s
shake128/64/32_median            207 ns          207 ns            8    961.749      10.0182       4.642k             4.82664      441.543Mi/s
shake128/64/32_stddev           1.57 ns         1.57 ns            8    7.42702    0.0773647     65.2493u           0.0366883      3.33845Mi/s
shake128/64/32_cv               0.76 %          0.76 %             8      0.77%        0.77%        0.00%               0.76%            0.76%
shake256/512/32_mean             774 ns          774 ns            8   3.61355k      6.64255      17.966k             4.97211      670.336Mi/s
shake256/512/32_median           772 ns          772 ns            8   3.59811k      6.61418      17.966k             4.99318      672.024Mi/s
shake256/512/32_stddev          5.86 ns         5.86 ns            8    28.5262    0.0524378            0            0.039006      5.05787Mi/s
shake256/512/32_cv              0.76 %          0.76 %             8      0.79%        0.79%        0.00%               0.78%            0.75%
shake128/1024/64_mean           1335 ns         1335 ns            8    6.2397k      5.73502      31.256k             5.00927      777.387Mi/s
shake128/1024/64_median         1334 ns         1334 ns            8   6.23592k      5.73154      31.256k             5.01225      778.091Mi/s
shake128/1024/64_stddev         5.22 ns         5.21 ns            8    21.8265    0.0200611            0           0.0174738      3.02758Mi/s
shake128/1024/64_cv             0.39 %          0.39 %             8      0.35%        0.35%        0.00%               0.35%            0.39%
sha3_224/256_mean                399 ns          399 ns            8   1.85846k      6.54388       9.064k             4.87731       678.84Mi/s
sha3_224/256_median              400 ns          400 ns            8   1.85423k      6.52899       9.064k             4.88828      677.721Mi/s
sha3_224/256_stddev             2.84 ns         2.82 ns            8    11.5114    0.0405329     130.499u           0.0300538      4.80057Mi/s
sha3_224/256_cv                 0.71 %          0.71 %             8      0.62%        0.62%        0.00%               0.62%            0.71%
sha3_256/4096_mean              5786 ns         5786 ns            8    26.956k      6.53003     137.365k             5.09591      680.427Mi/s
sha3_256/4096_median            5778 ns         5778 ns            8   26.9489k      6.52831     137.365k             5.09725      681.329Mi/s
sha3_256/4096_stddev            31.7 ns         31.6 ns            8     44.925     0.010883            0             8.4903m      3.67827Mi/s
sha3_256/4096_cv                0.55 %          0.55 %             8      0.17%        0.17%        0.00%               0.17%            0.54%
shake256/128/32_mean             204 ns          204 ns            8    950.102      5.93814       4.655k             4.89966      746.432Mi/s
shake256/128/32_median           204 ns          204 ns            8    948.572      5.92858       4.655k             4.90738      748.145Mi/s
shake256/128/32_stddev          2.63 ns         2.63 ns            8    6.21352    0.0388345            0           0.0319121      9.53061Mi/s
shake256/128/32_cv              1.29 %          1.29 %             8      0.65%        0.65%        0.00%               0.65%            1.28%
sha3_224/2048_mean              2815 ns         2815 ns            8   13.0983k      6.30941      66.595k             5.08424      703.339Mi/s
sha3_224/2048_median            2807 ns         2807 ns            8   13.0962k      6.30837      66.595k             5.08507      705.217Mi/s
sha3_224/2048_stddev            20.4 ns         20.4 ns            8    14.3524     6.91349m            0            5.56137m      5.04063Mi/s
sha3_224/2048_cv                0.73 %          0.73 %             8      0.11%        0.11%        0.00%               0.11%            0.72%
shake128/2048/32_mean           2473 ns         2473 ns            8   11.4745k      5.51658      57.831k                5.04      802.304Mi/s
shake128/2048/32_median         2464 ns         2464 ns            8   11.4807k      5.51954      57.831k             5.03726      805.015Mi/s
shake128/2048/32_stddev         25.3 ns         25.2 ns            8    31.1963    0.0149982            0           0.0136999      8.11501Mi/s
shake128/2048/32_cv             1.02 %          1.02 %             8      0.27%        0.27%        0.00%               0.27%            1.01%
sha3_256/128_mean                201 ns          201 ns            8    937.095      5.85684       4.617k             4.92697      757.696Mi/s
sha3_256/128_median              201 ns          201 ns            8    936.939      5.85587       4.617k             4.92775      759.096Mi/s
sha3_256/128_stddev             1.62 ns         1.60 ns            8    2.74604    0.0171628     65.2493u           0.0144352      5.97373Mi/s
sha3_256/128_cv                 0.81 %          0.79 %             8      0.29%        0.29%        0.00%               0.29%            0.79%
shake256/2048/32_mean           3013 ns         3013 ns            8   14.0337k      6.74695      71.045k             5.06254       658.38Mi/s
shake256/2048/32_median         3005 ns         3005 ns            8   14.0172k      6.73904      71.045k             5.06842      660.149Mi/s
shake256/2048/32_stddev         23.6 ns         23.7 ns            8    54.7476    0.0263209     1.47642m           0.0197002      5.11296Mi/s
shake256/2048/32_cv             0.78 %          0.79 %             8      0.39%        0.39%        0.00%               0.39%            0.78%
shake128/512/32_mean             779 ns          779 ns            8   3.62093k      6.65612      17.971k             4.96312      666.211Mi/s
shake128/512/32_median           777 ns          777 ns            8   3.62315k       6.6602      17.971k             4.96005      667.505Mi/s
shake128/512/32_stddev          6.32 ns         6.31 ns            8    8.91172    0.0163818            0           0.0122449       5.3664Mi/s
shake128/512/32_cv              0.81 %          0.81 %             8      0.25%        0.25%        0.00%               0.25%            0.81%
shake128/512/64_mean             776 ns          776 ns            8   3.61477k      6.27564      17.971k             4.97157      707.739Mi/s
shake128/512/64_median           774 ns          774 ns            8   3.61117k      6.26939      17.971k             4.97651      709.505Mi/s
shake128/512/64_stddev          6.98 ns         6.97 ns            8    8.99026    0.0156081     260.997u           0.0123496       6.2756Mi/s
shake128/512/64_cv              0.90 %          0.90 %             8      0.25%        0.25%        0.00%               0.25%            0.89%
shake256/64/32_mean              204 ns          204 ns            8     954.54      9.94312       4.645k             4.86629      448.021Mi/s
shake256/64/32_median            204 ns          204 ns            8    954.939      9.94728       4.645k             4.86421      448.145Mi/s
shake256/64/32_stddev          0.967 ns        0.952 ns            8    3.91661    0.0407981     65.2493u           0.0199623      2.08652Mi/s
shake256/64/32_cv               0.47 %          0.47 %             8      0.41%        0.41%        0.00%               0.41%            0.47%
shake256/4096/64_mean           5825 ns         5825 ns            8   26.9276k      6.47299     137.403k              5.1027      681.187Mi/s
shake256/4096/64_median         5808 ns         5808 ns            8   26.9402k      6.47601     137.403k             5.10029      683.087Mi/s
shake256/4096/64_stddev         77.9 ns         78.1 ns            8    52.7763    0.0126866            0           0.0100034        9.092Mi/s
shake256/4096/64_cv             1.34 %          1.34 %             8      0.20%        0.20%        0.00%               0.20%            1.33%
sha3_384/32_mean                 203 ns          203 ns            8     946.33      11.8291       4.648k             4.91191      375.651Mi/s
sha3_384/32_median               202 ns          202 ns            8    944.004         11.8       4.648k             4.92372      376.875Mi/s
sha3_384/32_stddev              1.74 ns         1.74 ns            8    7.91094    0.0988867            0           0.0408276      3.19635Mi/s
sha3_384/32_cv                  0.86 %          0.86 %             8      0.84%        0.84%        0.00%               0.83%            0.85%
sha3_384/512_mean                956 ns          956 ns            8   4.44111k      7.93055      22.367k             5.03679       558.53Mi/s
sha3_384/512_median              954 ns          954 ns            8   4.44064k      7.92971      22.367k             5.03689      559.782Mi/s
sha3_384/512_stddev             9.78 ns         9.79 ns            8    43.8978    0.0783889            0           0.0496609      5.72113Mi/s
sha3_384/512_cv                 1.02 %          1.02 %             8      0.99%        0.99%        0.00%               0.99%            1.02%
shake256/32/64_mean              204 ns          204 ns            8    953.726      9.93464       4.645k             4.87052      448.483Mi/s
shake256/32/64_median            204 ns          204 ns            8    952.316      9.91995       4.645k             4.87759      448.816Mi/s
shake256/32/64_stddev           1.23 ns         1.22 ns            8      5.562    0.0579375            0            0.028235      2.67363Mi/s
shake256/32/64_cv               0.60 %          0.60 %             8      0.58%        0.58%        0.00%               0.58%            0.60%
shake256/2048/64_mean           3013 ns         3013 ns            8    14.024k      6.64017      71.045k             5.06597      668.515Mi/s
shake256/2048/64_median         3004 ns         3004 ns            8   14.0141k      6.63546      71.045k             5.06954      670.413Mi/s
shake256/2048/64_stddev         21.8 ns         21.8 ns            8    32.7014    0.0154836            0           0.0117985       4.7817Mi/s
shake256/2048/64_cv             0.72 %          0.72 %             8      0.23%        0.23%        0.00%               0.23%            0.72%
shake128/128/64_mean             206 ns          206 ns            8    959.929      4.99963       4.652k             4.84621      889.835Mi/s
shake128/128/64_median           206 ns          206 ns            8    960.638      5.00332       4.652k             4.84262      890.331Mi/s
shake128/128/64_stddev         0.687 ns        0.689 ns            8    1.92038     0.010002            0            9.69947m      2.97036Mi/s
shake128/128/64_cv              0.33 %          0.33 %             8      0.20%        0.20%        0.00%               0.20%            0.33%
sha3_512/512_mean               1500 ns         1500 ns            8    7.0069k      12.1648      35.619k             5.08351      366.331Mi/s
sha3_512/512_median             1496 ns         1495 ns            8   6.99083k      12.1369      35.619k              5.0951      367.318Mi/s
sha3_512/512_stddev             7.23 ns         7.24 ns            8    32.2063    0.0559137            0           0.0232405      1.75911Mi/s
sha3_512/512_cv                 0.48 %          0.48 %             8      0.46%        0.46%        0.00%               0.46%            0.48%
sha3_512/256_mean                765 ns          765 ns            8   3.55854k      11.1204      17.962k             5.04763       398.72Mi/s
sha3_512/256_median              763 ns          763 ns            8   3.55524k      11.1101      17.962k             5.05226      400.118Mi/s
sha3_512/256_stddev             7.03 ns         7.02 ns            8    11.6921    0.0365379            0           0.0165353      3.63876Mi/s
sha3_512/256_cv                 0.92 %          0.92 %             8      0.33%        0.33%        0.00%               0.33%            0.91%
shake128/1024/32_mean           1340 ns         1340 ns            8   6.24471k      5.91355      31.256k             5.00522      751.532Mi/s
shake128/1024/32_median         1337 ns         1337 ns            8   6.24332k      5.91224      31.256k             5.00631      753.512Mi/s
shake128/1024/32_stddev         10.9 ns         10.9 ns            8    15.0203    0.0142238            0           0.0120483      6.02074Mi/s
shake128/1024/32_cv             0.81 %          0.81 %             8      0.24%        0.24%        0.00%               0.24%            0.80%
shake256/64/64_mean              205 ns          205 ns            8    956.272      7.47088       4.645k              4.8575       595.88Mi/s
shake256/64/64_median            205 ns          205 ns            8    955.917       7.4681       4.645k             4.85921      595.701Mi/s
shake256/64/64_stddev          0.919 ns        0.914 ns            8    4.65295    0.0363512            0           0.0235847      2.65936Mi/s
shake256/64/64_cv               0.45 %          0.45 %             8      0.49%        0.49%        0.00%               0.49%            0.45%
sha3_384/2048_mean              3758 ns         3758 ns            8   17.4866k      8.34283      88.652k             5.07009      532.031Mi/s
sha3_384/2048_median            3747 ns         3747 ns            8   17.4562k      8.32836      88.652k             5.07853      533.409Mi/s
sha3_384/2048_stddev            43.0 ns         43.1 ns            8    160.048    0.0763588            0           0.0458809      6.04994Mi/s
sha3_384/2048_cv                1.14 %          1.15 %             8      0.92%        0.92%        0.00%               0.90%            1.14%
sha3_384/256_mean                582 ns          582 ns            8   2.70432k      8.89579      13.519k              4.9995      498.547Mi/s
sha3_384/256_median              582 ns          582 ns            8   2.69413k      8.86227      13.519k             5.01802      498.469Mi/s
sha3_384/256_stddev             6.00 ns         6.01 ns            8    27.8996    0.0917749     184.553u           0.0510514      5.14854Mi/s
sha3_384/256_cv                 1.03 %          1.03 %             8      1.03%        1.03%        0.00%               1.02%            1.03%
sha3_384/4096_mean              7496 ns         7496 ns            8   34.9206k       8.4268     177.022k             5.07007      527.308Mi/s
sha3_384/4096_median            7514 ns         7514 ns            8   34.8396k      8.40725     177.022k             5.08115      525.953Mi/s
sha3_384/4096_stddev            92.4 ns         92.5 ns            8    472.158     0.113938     4.17596m            0.068327      6.51062Mi/s
sha3_384/4096_cv                1.23 %          1.23 %             8      1.35%        1.35%        0.00%               1.35%            1.23%
sha3_224/128_mean                201 ns          201 ns            8    938.793       6.0179       4.603k             4.90315      738.686Mi/s
sha3_224/128_median              201 ns          201 ns            8    938.121       6.0136       4.603k             4.90662      739.086Mi/s
sha3_224/128_stddev            0.739 ns        0.741 ns            8    3.13021    0.0200655            0           0.0162713      2.71531Mi/s
sha3_224/128_cv                 0.37 %          0.37 %             8      0.33%        0.33%        0.00%               0.33%            0.37%
shake128/4096/32_mean           4746 ns         4745 ns            8   21.9178k      5.30955     110.979k             5.06344      829.638Mi/s
shake128/4096/32_median         4749 ns         4749 ns            8   21.9087k      5.30734     110.979k             5.06552      828.988Mi/s
shake128/4096/32_stddev         42.6 ns         42.4 ns            8    49.7527    0.0120525            0           0.0114814      7.42172Mi/s
shake128/4096/32_cv             0.90 %          0.89 %             8      0.23%        0.23%        0.00%               0.23%            0.89%
sha3_512/128_mean                396 ns          396 ns            8   1.83671k      9.56618       9.131k             4.97147      462.852Mi/s
sha3_512/128_median              394 ns          394 ns            8   1.83354k      9.54969       9.131k             4.97998       464.37Mi/s
sha3_512/128_stddev             3.72 ns         3.72 ns            8    7.65064    0.0398471     130.499u           0.0206394      4.32605Mi/s
sha3_512/128_cv                 0.94 %          0.94 %             8      0.42%        0.42%        0.00%               0.42%            0.93%
shake128/4096/64_mean           4696 ns         4696 ns            8   21.9284k      5.27125     110.979k               5.061      844.791Mi/s
shake128/4096/64_median         4696 ns         4696 ns            8   21.9212k      5.26952     110.979k             5.06264      844.746Mi/s
shake128/4096/64_stddev         13.1 ns         13.0 ns            8    57.2348    0.0137584            0           0.0131983      2.33139Mi/s
shake128/4096/64_cv             0.28 %          0.28 %             8      0.26%        0.26%        0.00%               0.26%            0.28%
shake128/2048/64_mean           2473 ns         2473 ns            8   11.4746k      5.43305      57.831k             5.03996      814.633Mi/s
shake128/2048/64_median         2460 ns         2460 ns            8   11.4644k      5.42821      57.831k             5.04441      818.877Mi/s
shake128/2048/64_stddev         31.3 ns         31.1 ns            8    37.0918    0.0175624     1.04399m           0.0162018      10.0718Mi/s
shake128/2048/64_cv             1.26 %          1.26 %             8      0.32%        0.32%        0.00%               0.32%            1.24%
shake256/1024/32_mean           1525 ns         1525 ns            8    7.0967k      6.72036      35.662k             5.02538      660.417Mi/s
shake256/1024/32_median         1521 ns         1521 ns            8   7.07815k      6.70279      35.662k             5.03833      662.297Mi/s
shake256/1024/32_stddev         15.5 ns         15.3 ns            8     51.565    0.0488305            0           0.0362547      6.56355Mi/s
shake256/1024/32_cv             1.01 %          1.00 %             8      0.73%        0.73%        0.00%               0.72%            0.99%
sha3_384/64_mean                 205 ns          204 ns            8    949.369      8.47651       4.648k             4.89622       522.35Mi/s
sha3_384/64_median               205 ns          205 ns            8    949.919      8.48142       4.648k             4.89305      521.644Mi/s
sha3_384/64_stddev              1.96 ns         1.96 ns            8    8.47591    0.0756778            0           0.0433535      5.01852Mi/s
sha3_384/64_cv                  0.96 %          0.96 %             8      0.89%        0.89%        0.00%               0.89%            0.96%
sha3_256/64_mean                 203 ns          203 ns            8    947.244      9.86712       4.607k             4.86361      450.325Mi/s
sha3_256/64_median               203 ns          203 ns            8    947.879      9.87374       4.607k             4.86032      451.135Mi/s
sha3_256/64_stddev              1.10 ns         1.10 ns            8    2.38802    0.0248752     92.2765u           0.0122721      2.42578Mi/s
sha3_256/64_cv                  0.54 %          0.54 %             8      0.25%        0.25%        0.00%               0.25%            0.54%
sha3_512/2048_mean              5405 ns         5405 ns            8   25.1008k      11.8848     128.312k             5.11191      372.684Mi/s
sha3_512/2048_median            5386 ns         5386 ns            8   25.0979k      11.8835     128.312k             5.11247      373.968Mi/s
sha3_512/2048_stddev            46.5 ns         46.6 ns            8    73.9592    0.0350186            0           0.0150641      3.18755Mi/s
sha3_512/2048_cv                0.86 %          0.86 %             8      0.29%        0.29%        0.00%               0.29%            0.86%
shake128/32/32_mean              206 ns          206 ns            8    956.929       14.952       4.642k             4.85108      296.932Mi/s
shake128/32/32_median            206 ns          206 ns            8    955.683      14.9325       4.642k             4.85726      296.923Mi/s
shake128/32/32_stddev           1.37 ns         1.37 ns            8    5.68301    0.0887971     65.2493u           0.0286309      1.97553Mi/s
shake128/32/32_cv               0.67 %          0.66 %             8      0.59%        0.59%        0.00%               0.59%            0.67%
sha3_256/32_mean                 203 ns          203 ns            8    946.606      14.7907       4.607k             4.86687       301.04Mi/s
sha3_256/32_median               203 ns          203 ns            8    946.483      14.7888       4.607k             4.86749      301.404Mi/s
sha3_256/32_stddev              1.12 ns         1.12 ns            8    1.52373    0.0238083            0            7.83437m       1.6544Mi/s
sha3_256/32_cv                  0.55 %          0.55 %             8      0.16%        0.16%        0.00%               0.16%            0.55%
shake128/256/64_mean             405 ns          405 ns            8   1.88308k      5.88462       9.121k             4.84389      753.481Mi/s
shake128/256/64_median           405 ns          405 ns            8   1.88021k      5.87565       9.121k             4.85106      753.396Mi/s
shake128/256/64_stddev          2.25 ns         2.24 ns            8    13.7584     0.042995     130.499u           0.0352161      4.15166Mi/s
shake128/256/64_cv              0.55 %          0.55 %             8      0.73%        0.73%        0.00%               0.73%            0.55%
shake256/1024/64_mean           1521 ns         1521 ns            8   7.09027k      6.51679      35.662k             5.02981      682.089Mi/s
shake256/1024/64_median         1520 ns         1520 ns            8     7.102k      6.52757      35.662k              5.0214      682.795Mi/s
shake256/1024/64_stddev         13.8 ns         13.8 ns            8    34.0593    0.0313045            0           0.0241974      6.11384Mi/s
shake256/1024/64_cv             0.90 %          0.91 %             8      0.48%        0.48%        0.00%               0.48%            0.90%
```

### On 12th Gen Intel(R) Core(TM) i7-1260P ( compiled with GCC-13.1.0 )

```bash
2023-09-28T00:29:54+04:00
Running ./build/perfs/perf.out
Run on (16 X 3963.16 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 0.58, 0.57, 0.54
----------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations     CYCLES CYCLES/ BYTE INSTRUCTIONS INSTRUCTIONS/ CYCLE bytes_per_second
----------------------------------------------------------------------------------------------------------------------------------------------
shake256/128/64_mean             245 ns          245 ns            8   1.14547k        5.966       5.942k             5.18738      747.496Mi/s
shake256/128/64_median           245 ns          245 ns            8    1.1456k      5.96665       5.942k             5.18682      748.419Mi/s
shake256/128/64_stddev         0.718 ns        0.717 ns            8   0.776614     4.04487m            0            3.51836m      2.18134Mi/s
shake256/128/64_cv              0.29 %          0.29 %             8      0.07%        0.07%        0.00%               0.07%            0.29%
sha3_512/256_mean                843 ns          843 ns            8   3.93853k      12.3079      22.852k             5.80221      362.096Mi/s
sha3_512/256_median              843 ns          843 ns            8   3.93466k      12.2958      22.852k             5.80787      362.055Mi/s
sha3_512/256_stddev             2.81 ns         2.79 ns            8    10.7101     0.033469     260.997u           0.0157619        1.199Mi/s
sha3_512/256_cv                 0.33 %          0.33 %             8      0.27%        0.27%        0.00%               0.27%            0.33%
sha3_384/512_mean               1060 ns         1060 ns            8   4.95146k      8.84189      28.811k             5.81874      503.741Mi/s
sha3_384/512_median             1060 ns         1060 ns            8   4.95196k      8.84278      28.811k              5.8181      503.927Mi/s
sha3_384/512_stddev             4.98 ns         4.99 ns            8    14.4451    0.0257948     369.106u           0.0169559      2.36246Mi/s
sha3_384/512_cv                 0.47 %          0.47 %             8      0.29%        0.29%        0.00%               0.29%            0.47%
shake256/512/64_mean             877 ns          877 ns            8   4.09139k      7.10312      23.095k              5.6448      626.642Mi/s
shake256/512/64_median           877 ns          877 ns            8   4.09511k      7.10957      23.095k             5.63965      626.121Mi/s
shake256/512/64_stddev          2.93 ns         2.91 ns            8    8.26898    0.0143559     260.997u           0.0114132      2.08255Mi/s
shake256/512/64_cv              0.33 %          0.33 %             8      0.20%        0.20%        0.00%               0.20%            0.33%
sha3_224/32_mean                 247 ns          247 ns            8   1.15488k      19.2481       5.875k              5.0871       231.75Mi/s
sha3_224/32_median               247 ns          247 ns            8   1.15488k       19.248       5.875k             5.08712      231.763Mi/s
sha3_224/32_stddev             0.233 ns        0.234 ns            8    1.62176    0.0270294            0            7.14557m      225.364Ki/s
sha3_224/32_cv                  0.09 %          0.09 %             8      0.14%        0.14%        0.00%               0.14%            0.09%
shake256/256/64_mean             440 ns          439 ns            8    2.0519k       6.4122      11.673k              5.6889      694.388Mi/s
shake256/256/64_median           439 ns          439 ns            8   2.05055k      6.40795      11.673k             5.69263      694.615Mi/s
shake256/256/64_stddev          1.75 ns         1.74 ns            8    5.60622    0.0175194            0           0.0155027      2.73065Mi/s
shake256/256/64_cv              0.40 %          0.40 %             8      0.27%        0.27%        0.00%               0.27%            0.39%
shake128/4096/64_mean           5538 ns         5538 ns            8   25.5698k      6.14658     142.914k              5.5892      716.744Mi/s
shake128/4096/64_median         5472 ns         5472 ns            8   25.5688k      6.14634     142.914k              5.5894      725.056Mi/s
shake128/4096/64_stddev          127 ns          127 ns            8    62.3565    0.0149895            0           0.0136312       16.044Mi/s
shake128/4096/64_cv             2.29 %          2.29 %             8      0.24%        0.24%        0.00%               0.24%            2.24%
shake128/256/32_mean             441 ns          441 ns            8   2.06289k      7.16281      11.686k             5.66492      622.319Mi/s
shake128/256/32_median           441 ns          441 ns            8   2.06168k      7.15862      11.686k             5.66819      622.481Mi/s
shake128/256/32_stddev          1.81 ns         1.81 ns            8    6.53558     0.022693            0           0.0179327      2.54478Mi/s
shake128/256/32_cv              0.41 %          0.41 %             8      0.32%        0.32%        0.00%               0.32%            0.41%
shake128/128/64_mean             247 ns          247 ns            8   1.15382k      6.00948       5.961k             5.16637       742.82Mi/s
shake128/128/64_median           246 ns          246 ns            8   1.15284k      6.00437       5.961k             5.17071      743.342Mi/s
shake128/128/64_stddev         0.933 ns        0.926 ns            8    3.78453    0.0197111            0           0.0169208      2.78829Mi/s
shake128/128/64_cv              0.38 %          0.38 %             8      0.33%        0.33%        0.00%               0.33%            0.38%
sha3_512/32_mean                 226 ns          226 ns            8   1.05713k      11.0118       5.758k             5.44688      405.012Mi/s
sha3_512/32_median               226 ns          226 ns            8   1.05658k       11.006       5.758k             5.44967       405.73Mi/s
sha3_512/32_stddev              1.10 ns         1.11 ns            8    3.69825    0.0385235            0           0.0190198      1.97793Mi/s
sha3_512/32_cv                  0.49 %          0.49 %             8      0.35%        0.35%        0.00%               0.35%            0.49%
sha3_512/64_mean                 220 ns          220 ns            8   1.02527k      8.00992       5.758k             5.61609      555.429Mi/s
sha3_512/64_median               220 ns          220 ns            8   1.02593k      8.01511       5.758k             5.61245      555.741Mi/s
sha3_512/64_stddev              1.51 ns         1.51 ns            8    1.62547     0.012699            0             8.9139m      3.78522Mi/s
sha3_512/64_cv                  0.68 %          0.69 %             8      0.16%        0.16%        0.00%               0.16%            0.68%
sha3_224/512_mean                875 ns          875 ns            8   4.09709k      7.58721      22.978k             5.60837       588.25Mi/s
sha3_224/512_median              875 ns          875 ns            8   4.09793k      7.58877      22.978k             5.60722      588.267Mi/s
sha3_224/512_stddev             1.00 ns        0.990 ns            8    4.45903     8.25747m            0            6.10485m      680.782Ki/s
sha3_224/512_cv                 0.11 %          0.11 %             8      0.11%        0.11%        0.00%               0.11%            0.11%
shake128/64/32_mean              249 ns          249 ns            8   1.15623k       12.044       5.951k             5.14704       367.66Mi/s
shake128/64/32_median            248 ns          248 ns            8   1.15397k      12.0206       5.951k             5.15696      369.235Mi/s
shake128/64/32_stddev           3.95 ns         3.97 ns            8    6.24157    0.0650164            0           0.0276247       5.7059Mi/s
shake128/64/32_cv               1.59 %          1.60 %             8      0.54%        0.54%        0.00%               0.54%            1.55%
shake256/128/32_mean             245 ns          245 ns            8   1.14471k      7.15445       5.942k             5.19083      622.669Mi/s
shake256/128/32_median           245 ns          245 ns            8   1.14497k      7.15605       5.942k             5.18967       622.44Mi/s
shake256/128/32_stddev         0.688 ns        0.679 ns            8   0.958611     5.99132m            0            4.34907m      1.72632Mi/s
shake256/128/32_cv              0.28 %          0.28 %             8      0.08%        0.08%        0.00%               0.08%            0.28%
sha3_224/2048_mean              3269 ns         3269 ns            8   15.2921k      7.36614      85.569k             5.59568       605.65Mi/s
sha3_224/2048_median            3272 ns         3272 ns            8    15.291k       7.3656      85.569k             5.59604      605.056Mi/s
sha3_224/2048_stddev            9.70 ns         9.76 ns            8    45.7974    0.0220604            0           0.0167591      1.81058Mi/s
sha3_224/2048_cv                0.30 %          0.30 %             8      0.30%        0.30%        0.00%               0.30%            0.30%
sha3_384/1024_mean              2107 ns         2107 ns            8   9.85132k      9.18967      57.511k             5.83792      485.281Mi/s
sha3_384/1024_median            2106 ns         2106 ns            8   9.85033k      9.18874      57.511k             5.83849      485.549Mi/s
sha3_384/1024_stddev            8.09 ns         8.10 ns            8    20.3939    0.0190241            0           0.0120926      1.85935Mi/s
sha3_384/1024_cv                0.38 %          0.38 %             8      0.21%        0.21%        0.00%               0.21%            0.38%
sha3_512/4096_mean             11932 ns        11931 ns            8   55.6148k      13.3689     323.549k             5.81771       332.53Mi/s
sha3_512/4096_median           11922 ns        11921 ns            8    55.732k      13.3971      324.21k             5.81614      332.797Mi/s
sha3_512/4096_stddev            76.3 ns         75.9 ns            8     318.26    0.0765047     1.86993k           0.0207988      2.09754Mi/s
sha3_512/4096_cv                0.64 %          0.64 %             8      0.57%        0.57%        0.58%               0.36%            0.63%
sha3_256/64_mean                 245 ns          245 ns            8   1.13991k      11.8741       5.865k             5.14517      374.152Mi/s
sha3_256/64_median               244 ns          244 ns            8   1.13976k      11.8725       5.865k             5.14582      375.617Mi/s
sha3_256/64_stddev              3.19 ns         3.19 ns            8     2.7943    0.0291073     92.2765u           0.0126225      4.75222Mi/s
sha3_256/64_cv                  1.30 %          1.30 %             8      0.25%        0.25%        0.00%               0.25%            1.27%
shake256/32/64_mean              245 ns          245 ns            8   1.14802k      11.9585       5.932k              5.1678      373.065Mi/s
shake256/32/64_median            247 ns          247 ns            8   1.15422k      12.0231       5.932k             5.13941      371.398Mi/s
shake256/32/64_stddev           2.91 ns         2.91 ns            8    13.3367     0.138924     92.2765u           0.0612755      4.50492Mi/s
shake256/32/64_cv               1.18 %          1.18 %             8      1.16%        1.16%        0.00%               1.19%            1.21%
shake256/2048/32_mean           3483 ns         3483 ns            8   16.2668k      7.82058      91.618k             5.63226       569.58Mi/s
shake256/2048/32_median         3489 ns         3488 ns            8   16.2848k      7.82925      91.618k             5.62597      568.691Mi/s
shake256/2048/32_stddev         16.0 ns         15.9 ns            8    54.7669    0.0263303     1.04399m           0.0190155      2.61441Mi/s
shake256/2048/32_cv             0.46 %          0.46 %             8      0.34%        0.34%        0.00%               0.34%            0.46%
shake256/1024/64_mean           1746 ns         1746 ns            8   8.15656k      7.49684      45.939k             5.63216      594.254Mi/s
shake256/1024/64_median         1743 ns         1743 ns            8   8.15844k      7.49857      45.939k             5.63085      595.327Mi/s
shake256/1024/64_stddev         6.84 ns         6.84 ns            8     7.8849     7.24716m            0            5.44651m       2.3167Mi/s
shake256/1024/64_cv             0.39 %          0.39 %             8      0.10%        0.10%        0.00%               0.10%            0.39%
shake256/64/64_mean              246 ns          246 ns            8   1.14736k      8.96377       5.932k             5.17025      497.031Mi/s
shake256/64/64_median            245 ns          245 ns            8   1.14708k      8.96157       5.932k             5.17139      497.932Mi/s
shake256/64/64_stddev           1.38 ns         1.38 ns            8    6.06884    0.0474128            0           0.0274622      2.79117Mi/s
shake256/64/64_cv               0.56 %          0.56 %             8      0.53%        0.53%        0.00%               0.53%            0.56%
shake128/32/32_mean              251 ns          251 ns            8   1.16195k      18.1554       5.951k             5.12163      243.099Mi/s
shake128/32/32_median            249 ns          249 ns            8   1.16255k      18.1648       5.951k             5.11892       244.86Mi/s
shake128/32/32_stddev           4.46 ns         4.46 ns            8    4.06396    0.0634993     92.2765u           0.0179259      4.24053Mi/s
shake128/32/32_cv               1.78 %          1.77 %             8      0.35%        0.35%        0.00%               0.35%            1.74%
sha3_224/4096_mean              6356 ns         6355 ns            8   29.5396k      7.16286     165.243k             5.59397      619.038Mi/s
sha3_224/4096_median            6320 ns         6319 ns            8   29.5232k      7.15887     165.243k             5.59706      622.425Mi/s
sha3_224/4096_stddev             118 ns          118 ns            8    65.8206    0.0159604            0           0.0124454      11.1176Mi/s
sha3_224/4096_cv                1.86 %          1.86 %             8      0.22%        0.22%        0.00%               0.22%            1.80%
shake256/2048/64_mean           3481 ns         3481 ns            8   16.2835k         7.71      91.618k             5.62648      578.686Mi/s
shake256/2048/64_median         3474 ns         3474 ns            8   16.2645k      7.70101      91.618k             5.63299      579.711Mi/s
shake256/2048/64_stddev         16.7 ns         16.7 ns            8    54.1495     0.025639            0           0.0186366      2.75786Mi/s
shake256/2048/64_cv             0.48 %          0.48 %             8      0.33%        0.33%        0.00%               0.33%            0.48%
shake128/512/64_mean             887 ns          887 ns            8   4.11732k      7.14812      23.089k             5.60781       619.56Mi/s
shake128/512/64_median           880 ns          880 ns            8   4.11535k      7.14471      23.089k             5.61046      623.901Mi/s
shake128/512/64_stddev          15.8 ns         15.8 ns            8    9.50727    0.0165057            0           0.0129406      10.7012Mi/s
shake128/512/64_cv              1.78 %          1.78 %             8      0.23%        0.23%        0.00%               0.23%            1.73%
shake128/1024/64_mean           1545 ns         1545 ns            8   7.18238k      6.60145      40.205k             5.59776      671.889Mi/s
shake128/1024/64_median         1535 ns         1535 ns            8   7.18073k      6.59994      40.205k             5.59901      675.917Mi/s
shake128/1024/64_stddev         26.6 ns         26.6 ns            8    19.1698    0.0176193     738.212u           0.0149057      11.1876Mi/s
shake128/1024/64_cv             1.72 %          1.72 %             8      0.27%        0.27%        0.00%               0.27%            1.67%
shake128/2048/64_mean           2845 ns         2844 ns            8   13.2915k      6.29332      74.442k             5.60074        708.1Mi/s
shake128/2048/64_median         2843 ns         2843 ns            8   13.2961k      6.29551      74.442k             5.59877      708.568Mi/s
shake128/2048/64_stddev         9.78 ns         9.65 ns            8    23.5262    0.0111393            0            9.92158m      2.39685Mi/s
shake128/2048/64_cv             0.34 %          0.34 %             8      0.18%        0.18%        0.00%               0.18%            0.34%
sha3_512/2048_mean              6092 ns         6091 ns            8   28.4927k      13.4909     165.002k             5.79115      330.674Mi/s
sha3_512/2048_median            6083 ns         6083 ns            8   28.4712k      13.4807     165.002k              5.7954      331.111Mi/s
sha3_512/2048_stddev            33.1 ns         33.2 ns            8     136.56    0.0646589     2.08798m           0.0277255      1.80007Mi/s
sha3_512/2048_cv                0.54 %          0.55 %             8      0.48%        0.48%        0.00%               0.48%            0.54%
sha3_256/32_mean                 245 ns          245 ns            8   1.14138k      17.8341       5.865k             5.13852      249.327Mi/s
sha3_256/32_median               244 ns          244 ns            8   1.14189k      17.8421       5.865k             5.13621        250.2Mi/s
sha3_256/32_stddev              2.52 ns         2.52 ns            8    1.57045    0.0245382     92.2765u            7.08487m       2.5171Mi/s
sha3_256/32_cv                  1.03 %          1.03 %             8      0.14%        0.14%        0.00%               0.14%            1.01%
sha3_256/2048_mean              3566 ns         3565 ns            8   15.8549k      7.62255     89.2494k             5.62865       558.08Mi/s
sha3_256/2048_median            3490 ns         3490 ns            8   16.2534k      7.81416      91.569k              5.6323      568.441Mi/s
sha3_256/2048_stddev             226 ns          226 ns            8     1.147k     0.551441     6.56073k           0.0127125      31.2336Mi/s
sha3_256/2048_cv                6.34 %          6.34 %             8      7.23%        7.23%        7.35%               0.23%            5.60%
sha3_256/4096_mean              6738 ns         6738 ns            8   31.4957k      7.62977     177.247k             5.62771      584.305Mi/s
sha3_256/4096_median            6731 ns         6731 ns            8   31.5238k      7.63657     177.247k             5.62265      584.891Mi/s
sha3_256/4096_stddev            33.0 ns         33.1 ns            8    102.078    0.0247283            0           0.0183182      2.85896Mi/s
sha3_256/4096_cv                0.49 %          0.49 %             8      0.32%        0.32%        0.00%               0.33%            0.49%
sha3_224/128_mean                244 ns          244 ns            8   1.14404k      7.33361       5.885k             5.14404      608.584Mi/s
sha3_224/128_median              244 ns          244 ns            8   1.14392k      7.33285       5.885k             5.14457      608.898Mi/s
sha3_224/128_stddev            0.488 ns        0.496 ns            8    0.96802     6.20526m     130.499u            4.35384m      1.23405Mi/s
sha3_224/128_cv                 0.20 %          0.20 %             8      0.08%        0.08%        0.00%               0.08%            0.20%
sha3_224/1024_mean              1747 ns         1747 ns            8   8.17144k      7.76753      45.731k             5.59645      574.259Mi/s
sha3_224/1024_median            1748 ns         1748 ns            8   8.17102k      7.76713      45.731k             5.59673      573.992Mi/s
sha3_224/1024_stddev            3.72 ns         3.75 ns            8    12.4806    0.0118637     738.212u             8.5396m      1.23243Mi/s
sha3_224/1024_cv                0.21 %          0.21 %             8      0.15%        0.15%        0.00%               0.15%            0.21%
sha3_224/256_mean                440 ns          440 ns            8   2.05684k       7.2424      11.596k              5.6378      615.504Mi/s
sha3_224/256_median              439 ns          439 ns            8   2.05673k      7.24199      11.596k             5.63809      616.902Mi/s
sha3_224/256_stddev             2.07 ns         2.06 ns            8    4.63191    0.0163095     184.553u           0.0127023      2.86889Mi/s
sha3_224/256_cv                 0.47 %          0.47 %             8      0.23%        0.23%        0.00%               0.23%            0.47%
sha3_256/512_mean                872 ns          872 ns            8   4.07556k      7.49185      23.034k             5.65176      595.253Mi/s
sha3_256/512_median              871 ns          871 ns            8   4.07202k      7.48532      23.034k             5.65666      595.613Mi/s
sha3_256/512_stddev             2.27 ns         2.27 ns            8    8.80124    0.0161787     260.997u           0.0121697       1.5519Mi/s
sha3_256/512_cv                 0.26 %          0.26 %             8      0.22%        0.22%        0.00%               0.22%            0.26%
sha3_256/256_mean                438 ns          438 ns            8   2.04315k      7.09428       11.61k             5.68245      627.487Mi/s
sha3_256/256_median              438 ns          438 ns            8   2.04346k      7.09533       11.61k             5.68156      627.774Mi/s
sha3_256/256_stddev             1.59 ns         1.58 ns            8    6.94241    0.0241056     184.553u           0.0192416      2.26559Mi/s
sha3_256/256_cv                 0.36 %          0.36 %             8      0.34%        0.34%        0.00%               0.34%            0.36%
sha3_384/4096_mean              8391 ns         8390 ns            8   39.2367k      9.46832     229.701k             5.85426      471.036Mi/s
sha3_384/4096_median            8391 ns         8390 ns            8   39.2153k      9.46316     229.701k             5.85743      471.022Mi/s
sha3_384/4096_stddev            16.6 ns         16.8 ns            8    70.1504    0.0169282            0           0.0104541      964.569Ki/s
sha3_384/4096_cv                0.20 %          0.20 %             8      0.18%        0.18%        0.00%               0.18%            0.20%
sha3_512/1024_mean              3183 ns         3183 ns            8   14.7223k      13.5316      85.397k             5.80062      326.161Mi/s
sha3_512/1024_median            3159 ns         3159 ns            8    14.722k      13.5312      85.397k             5.80066      328.475Mi/s
sha3_512/1024_stddev            70.0 ns         70.1 ns            8     68.005    0.0625046     1.04399m           0.0267782      6.87768Mi/s
sha3_512/1024_cv                2.20 %          2.20 %             8      0.46%        0.46%        0.00%               0.46%            2.11%
shake256/4096/32_mean           6752 ns         6752 ns            8   31.5834k      7.65101     177.281k             5.61318       583.05Mi/s
shake256/4096/32_median         6755 ns         6755 ns            8   31.5692k      7.64757     177.281k             5.61564      582.755Mi/s
shake256/4096/32_stddev         27.0 ns         26.9 ns            8    121.927    0.0295367            0           0.0216807      2.32869Mi/s
shake256/4096/32_cv             0.40 %          0.40 %             8      0.39%        0.39%        0.00%               0.39%            0.40%
shake128/32/64_mean              249 ns          249 ns            8   1.16179k      12.1019       5.951k             5.12233      368.113Mi/s
shake128/32/64_median            248 ns          248 ns            8   1.16228k      12.1071       5.951k              5.1201       368.52Mi/s
shake128/32/64_stddev           1.15 ns         1.15 ns            8    3.63333    0.0378472            0           0.0160065      1.68779Mi/s
shake128/32/64_cv               0.46 %          0.46 %             8      0.31%        0.31%        0.00%               0.31%            0.46%
shake256/64/32_mean              244 ns          244 ns            8   1.13863k      11.8607       5.932k              5.2105      376.012Mi/s
shake256/64/32_median            245 ns          245 ns            8   1.14479k      11.9249       5.932k             5.18174      374.238Mi/s
shake256/64/32_stddev           3.03 ns         3.03 ns            8    14.3384     0.149358     92.2765u           0.0671516      4.77093Mi/s
shake256/64/32_cv               1.24 %          1.24 %             8      1.26%        1.26%        0.00%               1.29%            1.27%
shake128/1024/32_mean           1537 ns         1537 ns            8   7.17897k      6.79827      40.205k             5.60042      655.348Mi/s
shake128/1024/32_median         1537 ns         1537 ns            8   7.18043k      6.79965      40.205k             5.59925      655.288Mi/s
shake128/1024/32_stddev         3.92 ns         3.93 ns            8    17.9484    0.0169966            0           0.0140213      1.67656Mi/s
shake128/1024/32_cv             0.25 %          0.26 %             8      0.25%        0.25%        0.00%               0.25%            0.26%
shake256/1024/32_mean           1747 ns         1747 ns            8    8.1659k      7.73286      45.939k             5.62576      576.577Mi/s
shake256/1024/32_median         1745 ns         1745 ns            8   8.15735k      7.72476      45.939k             5.63161      577.267Mi/s
shake256/1024/32_stddev         5.79 ns         5.80 ns            8    25.7176    0.0243538            0           0.0176531      1.90665Mi/s
shake256/1024/32_cv             0.33 %          0.33 %             8      0.31%        0.31%        0.00%               0.31%            0.33%
sha3_384/32_mean                 219 ns          219 ns            8   1.01266k      12.6583        5.81k             5.73736      348.147Mi/s
sha3_384/32_median               218 ns          218 ns            8   1.01179k      12.6474        5.81k             5.74228      350.502Mi/s
sha3_384/32_stddev              5.59 ns         5.60 ns            8    1.79959    0.0224949            0           0.0101833      8.45645Mi/s
sha3_384/32_cv                  2.55 %          2.55 %             8      0.18%        0.18%        0.00%               0.18%            2.43%
shake128/2048/32_mean           2844 ns         2844 ns            8   13.2812k       6.3852      74.442k             5.60507      697.511Mi/s
shake128/2048/32_median         2843 ns         2843 ns            8   13.2781k      6.38372      74.442k             5.60636      697.654Mi/s
shake128/2048/32_stddev         8.30 ns         8.23 ns            8    20.8646    0.0100311            0            8.80209m      2.01851Mi/s
shake128/2048/32_cv             0.29 %          0.29 %             8      0.16%        0.16%        0.00%               0.16%            0.29%
sha3_384/256_mean                647 ns          647 ns            8   2.99292k      9.84514      17.321k             5.78733      448.585Mi/s
sha3_384/256_median              640 ns          640 ns            8   2.99156k      9.84064      17.321k             5.78997      452.707Mi/s
sha3_384/256_stddev             16.8 ns         16.7 ns            8    3.20256    0.0105347     260.997u            6.18952m      11.0351Mi/s
sha3_384/256_cv                 2.59 %          2.59 %             8      0.11%        0.11%        0.00%               0.11%            2.46%
sha3_384/128_mean                430 ns          430 ns            8   2.00412k       11.387       11.58k             5.77813      390.027Mi/s
sha3_384/128_median              431 ns          431 ns            8   2.00512k      11.3927       11.58k             5.77523      389.756Mi/s
sha3_384/128_stddev             2.41 ns         2.42 ns            8    5.06741    0.0287921     291.804u           0.0146303      2.19094Mi/s
sha3_384/128_cv                 0.56 %          0.56 %             8      0.25%        0.25%        0.00%               0.25%            0.56%
shake128/256/64_mean             442 ns          442 ns            8   2.06539k      6.45435      11.686k             5.65802      690.874Mi/s
shake128/256/64_median           441 ns          441 ns            8    2.0642k      6.45062      11.686k             5.66128      691.569Mi/s
shake128/256/64_stddev          1.63 ns         1.62 ns            8    3.41798    0.0106812            0            9.35529m      2.52573Mi/s
shake128/256/64_cv              0.37 %          0.37 %             8      0.17%        0.17%        0.00%               0.17%            0.37%
sha3_256/1024_mean              1740 ns         1740 ns            8   8.13513k      7.70372      45.882k             5.64003      578.759Mi/s
sha3_256/1024_median            1741 ns         1741 ns            8   8.13241k      7.70115      45.882k             5.64188      578.482Mi/s
sha3_256/1024_stddev            5.31 ns         5.26 ns            8    25.5402    0.0241858            0           0.0177089      1.75084Mi/s
sha3_256/1024_cv                0.31 %          0.30 %             8      0.31%        0.31%        0.00%               0.31%            0.30%
shake256/256/32_mean             445 ns          445 ns            8   2.05094k      7.12134      11.673k             5.69154      617.587Mi/s
shake256/256/32_median           439 ns          439 ns            8   2.05034k      7.11925      11.673k             5.69319      626.315Mi/s
shake256/256/32_stddev          12.1 ns         12.1 ns            8    3.70757    0.0128735            0           0.0102656       16.319Mi/s
shake256/256/32_cv              2.72 %          2.72 %             8      0.18%        0.18%        0.00%               0.18%            2.64%
shake256/512/32_mean             880 ns          880 ns            8   4.08658k      7.51209      23.095k             5.65144      589.426Mi/s
shake256/512/32_median           875 ns          875 ns            8   4.08692k      7.51272      23.095k             5.65096       592.81Mi/s
shake256/512/32_stddev          17.4 ns         17.4 ns            8    5.65815     0.010401     369.106u            7.82894m      11.1944Mi/s
shake256/512/32_cv              1.98 %          1.98 %             8      0.14%        0.14%        0.00%               0.14%            1.90%
shake128/4096/32_mean           5464 ns         5463 ns            8   25.5325k      6.18521     142.914k             5.59737      720.604Mi/s
shake128/4096/32_median         5464 ns         5464 ns            8   25.5609k      6.19208     142.914k             5.59112      720.493Mi/s
shake128/4096/32_stddev         11.0 ns         11.2 ns            8    77.0315    0.0186607            0           0.0169009      1.47618Mi/s
shake128/4096/32_cv             0.20 %          0.20 %             8      0.30%        0.30%        0.00%               0.30%            0.20%
shake256/4096/64_mean           6759 ns         6759 ns            8   31.6092k      7.59836     177.281k             5.60856       586.98Mi/s
shake256/4096/64_median         6752 ns         6752 ns            8   31.5991k      7.59594     177.281k             5.61032      587.612Mi/s
shake256/4096/64_stddev         20.4 ns         20.0 ns            8     72.255     0.017369     2.95285m           0.0128182      1.73589Mi/s
shake256/4096/64_cv             0.30 %          0.30 %             8      0.23%        0.23%        0.00%               0.23%            0.30%
sha3_512/128_mean                426 ns          426 ns            8   1.97207k      10.2712       11.48k             5.82134      430.481Mi/s
sha3_512/128_median              422 ns          422 ns            8   1.97078k      10.2645       11.48k              5.8251      434.262Mi/s
sha3_512/128_stddev             11.5 ns         11.5 ns            8    5.80241    0.0302209     130.499u           0.0171032      11.0438Mi/s
sha3_512/128_cv                 2.71 %          2.71 %             8      0.29%        0.29%        0.00%               0.29%            2.57%
shake128/64/64_mean              246 ns          246 ns            8   1.15163k      8.99713       5.951k             5.16745      495.592Mi/s
shake128/64/64_median            246 ns          246 ns            8   1.15148k       8.9959       5.951k             5.16815      495.691Mi/s
shake128/64/64_stddev          0.521 ns        0.518 ns            8    1.10259     8.61395m            0            4.94573m      1.04106Mi/s
shake128/64/64_cv               0.21 %          0.21 %             8      0.10%        0.10%        0.00%               0.10%            0.21%
sha3_256/128_mean                244 ns          244 ns            8   1.14006k      7.12538       5.875k             5.15324      625.898Mi/s
sha3_256/128_median              244 ns          244 ns            8   1.14022k       7.1264       5.875k             5.15249      625.903Mi/s
sha3_256/128_stddev            0.457 ns        0.441 ns            8   0.548307     3.42692m            0            2.47936m      1.13095Mi/s
sha3_256/128_cv                 0.19 %          0.18 %             8      0.05%        0.05%        0.00%               0.05%            0.18%
shake256/32/32_mean              247 ns          247 ns            8   1.15303k      18.0162       5.932k             5.14472      247.503Mi/s
shake256/32/32_median            247 ns          247 ns            8    1.1539k      18.0297       5.932k             5.14083      247.286Mi/s
shake256/32/32_stddev          0.840 ns        0.839 ns            8    3.00557     0.046962            0           0.0134412      864.472Ki/s
shake256/32/32_cv               0.34 %          0.34 %             8      0.26%        0.26%        0.00%               0.26%            0.34%
keccak-p[1600, 24]_mean          204 ns          204 ns            8      952.3       4.7615     5.67837k             5.96281       933.32Mi/s
keccak-p[1600, 24]_median        204 ns          204 ns            8    954.373      4.77187        5.69k             5.96203      934.477Mi/s
keccak-p[1600, 24]_stddev      0.783 ns        0.773 ns            8    5.88951    0.0294475       32.896            5.09037m      3.50326Mi/s
keccak-p[1600, 24]_cv           0.38 %          0.38 %             8      0.62%        0.62%        0.58%               0.09%            0.38%
sha3_224/64_mean                 247 ns          247 ns            8   1.14788k       12.477       5.875k             5.11813      355.328Mi/s
sha3_224/64_median               246 ns          246 ns            8   1.14767k      12.4746       5.875k             5.11908      357.321Mi/s
sha3_224/64_stddev              3.77 ns         3.78 ns            8     1.1059    0.0120206     92.2765u            4.93018m      5.27133Mi/s
sha3_224/64_cv                  1.53 %          1.53 %             8      0.10%        0.10%        0.00%               0.10%            1.48%
sha3_384/64_mean                 216 ns          216 ns            8   1.01234k      9.03873        5.81k              5.7392      493.442Mi/s
sha3_384/64_median               216 ns          216 ns            8   1.01214k      9.03698        5.81k             5.74031      493.896Mi/s
sha3_384/64_stddev             0.394 ns        0.396 ns            8    1.35902    0.0121341     92.2765u            7.70171m      922.597Ki/s
sha3_384/64_cv                  0.18 %          0.18 %             8      0.13%        0.13%        0.00%               0.13%            0.18%
shake128/512/32_mean             880 ns          880 ns            8   4.11171k      7.55829      23.089k             5.61545      589.858Mi/s
shake128/512/32_median           879 ns          879 ns            8   4.11033k      7.55575      23.089k             5.61732      590.224Mi/s
shake128/512/32_stddev          2.45 ns         2.45 ns            8     8.4953    0.0156164     260.997u           0.0115717       1.6421Mi/s
shake128/512/32_cv              0.28 %          0.28 %             8      0.21%        0.21%        0.00%               0.21%            0.28%
sha3_512/512_mean               1692 ns         1692 ns            8   7.84619k      13.6219      45.597k             5.81139       324.86Mi/s
sha3_512/512_median             1680 ns         1680 ns            8   7.84647k      13.6223      45.597k             5.81116      326.963Mi/s
sha3_512/512_stddev             37.3 ns         37.3 ns            8    20.6627    0.0358727     738.212u           0.0153039      6.87317Mi/s
sha3_512/512_cv                 2.21 %          2.21 %             8      0.26%        0.26%        0.00%               0.26%            2.12%
shake128/128/32_mean             247 ns          247 ns            8   1.15551k      7.22196       5.961k             5.15876      617.445Mi/s
shake128/128/32_median           247 ns          247 ns            8    1.1556k      7.22248       5.961k             5.15837      617.808Mi/s
shake128/128/32_stddev         0.537 ns        0.538 ns            8    1.86499    0.0116562            0            8.32386m      1.34127Mi/s
shake128/128/32_cv              0.22 %          0.22 %             8      0.16%        0.16%        0.00%               0.16%            0.22%
sha3_384/2048_mean              4208 ns         4208 ns            8   19.6412k      9.37082     114.911k             5.85055      475.065Mi/s
sha3_384/2048_median            4208 ns         4208 ns            8   19.6419k      9.37111     114.911k             5.85031      475.025Mi/s
sha3_384/2048_stddev            19.2 ns         19.2 ns            8    61.9245    0.0295441            0           0.0184241      2.15794Mi/s
sha3_384/2048_cv                0.46 %          0.46 %             8      0.32%        0.32%        0.00%               0.31%            0.45%
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
