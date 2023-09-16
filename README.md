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
2023-09-16T13:19:21+04:00
Running ./build/perfs/perf.out
Run on (16 X 1671.82 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 0.66, 0.39, 0.47
----------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations     CYCLES CYCLES/ BYTE INSTRUCTIONS INSTRUCTIONS/ CYCLE bytes_per_second
----------------------------------------------------------------------------------------------------------------------------------------------------
bench_sha3_384/2048                   3729 ns         3728 ns        37836   17.4502k      8.32548      88.652k             5.08029      536.157Mi/s
bench_sha3_384/2048                   3852 ns         3851 ns        37836   17.3613k      8.28308      88.652k             5.10629      519.011Mi/s
bench_sha3_384/2048                   3997 ns         3997 ns        37836   17.3627k      8.28372      88.652k             5.10589      500.091Mi/s
bench_sha3_384/2048                   3999 ns         3999 ns        37836   17.3348k      8.27044      88.652k             5.11409      499.905Mi/s
bench_sha3_384/2048                   3980 ns         3979 ns        37836   17.3087k      8.25797      88.652k             5.12182      502.337Mi/s
bench_sha3_384/2048                   4014 ns         4013 ns        37836   17.3587k      8.28185      88.652k             5.10705      498.085Mi/s
bench_sha3_384/2048                   4009 ns         4009 ns        37836   17.3448k      8.27521      88.652k             5.11115      498.654Mi/s
bench_sha3_384/2048                   4035 ns         4034 ns        37836   17.3819k      8.29287      88.652k             5.10026      495.475Mi/s
bench_sha3_384/2048_mean              3952 ns         3951 ns            8   17.3629k      8.28383      88.652k             5.10585      506.214Mi/s
bench_sha3_384/2048_median            3998 ns         3998 ns            8     17.36k      8.28246      88.652k             5.10667      499.998Mi/s
bench_sha3_384/2048_stddev             106 ns          106 ns            8    41.4929    0.0197962            0           0.0121737      14.0979Mi/s
bench_sha3_384/2048_cv                2.69 %          2.69 %             8      0.24%        0.24%        0.00%               0.24%            2.78%
bench_shake128/512/64                  829 ns          829 ns       168496   3.62426k      6.29213      17.971k             4.95852      662.516Mi/s
bench_shake128/512/64                  830 ns          829 ns       168496    3.6303k       6.3026      17.971k             4.95028      662.251Mi/s
bench_shake128/512/64                  830 ns          830 ns       168496   3.61578k       6.2774      17.971k             4.97015      661.881Mi/s
bench_shake128/512/64                  827 ns          827 ns       168496    3.6231k      6.29011      17.971k             4.96011      664.279Mi/s
bench_shake128/512/64                  830 ns          830 ns       168496   3.61314k      6.27282      17.971k             4.97379      662.172Mi/s
bench_shake128/512/64                  835 ns          835 ns       168496   3.62525k      6.29384      17.971k             4.95717      658.118Mi/s
bench_shake128/512/64                  844 ns          844 ns       168496   3.62003k      6.28477      17.971k             4.96433       650.53Mi/s
bench_shake128/512/64                  840 ns          840 ns       168496    3.6192k      6.28334      17.971k             4.96546      653.995Mi/s
bench_shake128/512/64_mean             833 ns          833 ns            8   3.62138k      6.28713      17.971k             4.96248      659.468Mi/s
bench_shake128/512/64_median           830 ns          830 ns            8   3.62157k      6.28744      17.971k             4.96222      662.026Mi/s
bench_shake128/512/64_stddev          6.17 ns         6.17 ns            8     5.4979     9.54496m            0            7.53348m       4.8548Mi/s
bench_shake128/512/64_cv              0.74 %          0.74 %             8      0.15%        0.15%        0.00%               0.15%            0.74%
bench_shake128/256/64                  401 ns          401 ns       342656   1.86997k      5.84365       9.121k             4.87763      761.523Mi/s
bench_shake128/256/64                  403 ns          403 ns       342656    1.8835k      5.88594       9.121k             4.84258      757.324Mi/s
bench_shake128/256/64                  406 ns          406 ns       342656   1.89358k      5.91744       9.121k              4.8168      751.832Mi/s
bench_shake128/256/64                  423 ns          423 ns       342656   1.87109k      5.84716       9.121k              4.8747      721.469Mi/s
bench_shake128/256/64                  429 ns          429 ns       342656   1.88269k      5.88341       9.121k             4.84466      711.093Mi/s
bench_shake128/256/64                  430 ns          430 ns       342656   1.87382k      5.85569       9.121k              4.8676      709.394Mi/s
bench_shake128/256/64                  432 ns          432 ns       342656   1.86839k      5.83873       9.121k             4.88173      706.286Mi/s
bench_shake128/256/64                  435 ns          435 ns       342656   1.87307k      5.85333       9.121k             4.86955      702.035Mi/s
bench_shake128/256/64_mean             420 ns          420 ns            8   1.87701k      5.86567       9.121k             4.85941      727.619Mi/s
bench_shake128/256/64_median           426 ns          426 ns            8   1.87344k      5.85451       9.121k             4.86858      716.281Mi/s
bench_shake128/256/64_stddev          14.2 ns         14.2 ns            8    8.73074    0.0272836     130.499u           0.0225201      24.9881Mi/s
bench_shake128/256/64_cv              3.39 %          3.39 %             8      0.47%        0.47%        0.00%               0.46%            3.43%
bench_shake256/1024/64                1524 ns         1524 ns        93360   7.07253k      6.50049      35.662k             5.04232      681.023Mi/s
bench_shake256/1024/64                1524 ns         1524 ns        93360   7.07251k      6.50046      35.662k             5.04234      680.835Mi/s
bench_shake256/1024/64                1560 ns         1560 ns        93360   7.07421k      6.50203      35.662k             5.04113      665.214Mi/s
bench_shake256/1024/64                1592 ns         1592 ns        93360   7.07682k      6.50443      35.662k             5.03927      651.941Mi/s
bench_shake256/1024/64                1622 ns         1622 ns        93360    7.0683k       6.4966      35.662k             5.04534      639.682Mi/s
bench_shake256/1024/64                1660 ns         1660 ns        93360   7.08664k      6.51346      35.662k             5.03228      625.145Mi/s
bench_shake256/1024/64                1648 ns         1648 ns        93360   7.06576k      6.49426      35.662k             5.04716      629.508Mi/s
bench_shake256/1024/64                1644 ns         1644 ns        93360   7.06921k      6.49743      35.662k              5.0447      631.252Mi/s
bench_shake256/1024/64_mean           1597 ns         1597 ns            8   7.07325k      6.50115      35.662k             5.04182      650.575Mi/s
bench_shake256/1024/64_median         1607 ns         1607 ns            8   7.07252k      6.50048      35.662k             5.04233      645.811Mi/s
bench_shake256/1024/64_stddev         55.5 ns         55.4 ns            8    6.44785     5.92633m     738.212u            4.59193m      22.8082Mi/s
bench_shake256/1024/64_cv             3.47 %          3.47 %             8      0.09%        0.09%        0.00%               0.09%            3.51%
bench_shake128/32/64                   205 ns          205 ns       669990    960.871      10.0091       4.642k             4.83103      446.162Mi/s
bench_shake128/32/64                   205 ns          205 ns       669990    958.649      9.98593       4.642k             4.84223      445.909Mi/s
bench_shake128/32/64                   208 ns          208 ns       669990     960.45      10.0047       4.642k             4.83315      439.171Mi/s
bench_shake128/32/64                   218 ns          217 ns       669990    961.141      10.0119       4.642k             4.82968      420.944Mi/s
bench_shake128/32/64                   218 ns          218 ns       669990    964.391      10.0457       4.642k              4.8134      419.925Mi/s
bench_shake128/32/64                   218 ns          218 ns       669990    966.989      10.0728       4.642k             4.80047      419.361Mi/s
bench_shake128/32/64                   221 ns          221 ns       669990    958.005      9.97922       4.642k             4.84549      413.921Mi/s
bench_shake128/32/64                   222 ns          222 ns       669990    959.948      9.99946       4.642k             4.83568      412.133Mi/s
bench_shake128/32/64_mean              215 ns          215 ns            8    961.305      10.0136       4.642k             4.82889      427.191Mi/s
bench_shake128/32/64_median            218 ns          218 ns            8     960.66      10.0069       4.642k             4.83209      420.434Mi/s
bench_shake128/32/64_stddev           7.03 ns         7.03 ns            8    2.98955    0.0311412     65.2493u           0.0149789      14.1911Mi/s
bench_shake128/32/64_cv               3.28 %          3.28 %             8      0.31%        0.31%        0.00%               0.31%            3.32%
bench_shake128/4096/32                4691 ns         4690 ns        29922   21.9149k      5.30885     110.979k             5.06408      839.338Mi/s
bench_shake128/4096/32                4875 ns         4874 ns        29922   21.8953k       5.3041     110.979k             5.06861       807.65Mi/s
bench_shake128/4096/32                4884 ns         4883 ns        29922   21.9007k      5.30539     110.979k             5.06738       806.16Mi/s
bench_shake128/4096/32                4986 ns         4986 ns        29922   21.9792k      5.32441     110.979k             5.04929      789.618Mi/s
bench_shake128/4096/32                5122 ns         5121 ns        29922   22.2112k      5.38062     110.979k             4.99654      768.682Mi/s
bench_shake128/4096/32                5071 ns         5070 ns        29922   21.9845k       5.3257     110.979k             5.04806      776.435Mi/s
bench_shake128/4096/32                5070 ns         5070 ns        29922   22.0526k      5.34219     110.979k             5.03248       776.47Mi/s
bench_shake128/4096/32                5092 ns         5092 ns        29922   21.8887k       5.3025     110.979k             5.07015      773.194Mi/s
bench_shake128/4096/32_mean           4974 ns         4973 ns            8   21.9784k      5.32422     110.979k             5.04957      792.193Mi/s
bench_shake128/4096/32_median         5028 ns         5028 ns            8    21.947k      5.31663     110.979k             5.05668      783.044Mi/s
bench_shake128/4096/32_stddev          148 ns          148 ns            8    110.027    0.0266539            0           0.0251312      24.1203Mi/s
bench_shake128/4096/32_cv             2.97 %          2.97 %             8      0.50%        0.50%        0.00%               0.50%            3.04%
bench_sha3_512/2048                   5407 ns         5406 ns        25730   25.0069k      11.8404     128.312k             5.13107      372.548Mi/s
bench_sha3_512/2048                   5690 ns         5689 ns        25730   25.0531k      11.8622     128.312k             5.12161      354.025Mi/s
bench_sha3_512/2048                   5747 ns         5746 ns        25730   25.0135k      11.8435     128.312k             5.12971      350.518Mi/s
bench_sha3_512/2048                   5782 ns         5781 ns        25730   25.0053k      11.8397     128.312k             5.13138      348.437Mi/s
bench_sha3_512/2048                   5773 ns         5772 ns        25730   24.9907k      11.8327     128.312k             5.13438      348.932Mi/s
bench_sha3_512/2048                   5791 ns         5790 ns        25730   25.0605k      11.8658     128.312k             5.12009      347.846Mi/s
bench_sha3_512/2048                   5822 ns         5822 ns        25730   25.0037k      11.8389     128.312k             5.13172      345.973Mi/s
bench_sha3_512/2048                   5805 ns         5804 ns        25730   24.9687k      11.8223     128.312k             5.13892      347.023Mi/s
bench_sha3_512/2048_mean              5727 ns         5726 ns            8   25.0128k      11.8432     128.312k             5.12986      351.913Mi/s
bench_sha3_512/2048_median            5777 ns         5776 ns            8   25.0061k        11.84     128.312k             5.13123      348.685Mi/s
bench_sha3_512/2048_stddev             135 ns          135 ns            8    30.4806    0.0144321     1.47642m            6.24835m      8.69235Mi/s
bench_sha3_512/2048_cv                2.36 %          2.37 %             8      0.12%        0.12%        0.00%               0.12%            2.47%
bench_sha3_256/128                     210 ns          210 ns       673566    939.393      5.87121       4.617k             4.91488      727.519Mi/s
bench_sha3_256/128                     212 ns          212 ns       673566    940.894      5.88059       4.617k             4.90704      720.304Mi/s
bench_sha3_256/128                     216 ns          216 ns       673566    938.558      5.86599       4.617k             4.91925      705.323Mi/s
bench_sha3_256/128                     214 ns          214 ns       673566    933.135      5.83209       4.617k             4.94784      713.244Mi/s
bench_sha3_256/128                     218 ns          218 ns       673566    939.722      5.87326       4.617k             4.91316      698.704Mi/s
bench_sha3_256/128                     217 ns          217 ns       673566     936.36      5.85225       4.617k              4.9308      703.861Mi/s
bench_sha3_256/128                     215 ns          215 ns       673566    934.094      5.83809       4.617k             4.94276      710.778Mi/s
bench_sha3_256/128                     216 ns          216 ns       673566    934.881      5.84301       4.617k              4.9386       706.76Mi/s
bench_sha3_256/128_mean                215 ns          215 ns            8     937.13      5.85706       4.617k             4.92679      710.812Mi/s
bench_sha3_256/128_median              215 ns          215 ns            8    937.459      5.85912       4.617k             4.92502      708.769Mi/s
bench_sha3_256/128_stddev             2.82 ns         2.82 ns            8    2.90007    0.0181254            0           0.0152517      9.38567Mi/s
bench_sha3_256/128_cv                 1.31 %          1.31 %             8      0.31%        0.31%        0.00%               0.31%            1.32%
bench_sha3_256/4096                   5760 ns         5760 ns        24448    26.828k      6.49903     137.365k             5.12021      683.488Mi/s
bench_sha3_256/4096                   6078 ns         6077 ns        24448    26.843k      6.50266     137.365k             5.11735      647.786Mi/s
bench_sha3_256/4096                   6110 ns         6109 ns        24448    26.967k       6.5327     137.365k             5.09382      644.396Mi/s
bench_sha3_256/4096                   6218 ns         6218 ns        24448   26.9997k      6.54062     137.365k             5.08765       633.15Mi/s
bench_sha3_256/4096                   6251 ns         6249 ns        24448   26.9009k      6.51669     137.365k             5.10633      629.966Mi/s
bench_sha3_256/4096                   6276 ns         6275 ns        24448   27.0429k      6.55109     137.365k             5.07952       627.33Mi/s
bench_sha3_256/4096                   6277 ns         6276 ns        24448   26.9523k      6.52914     137.365k              5.0966      627.234Mi/s
bench_sha3_256/4096                   6249 ns         6248 ns        24448   26.9944k      6.53933     137.365k             5.08866      630.092Mi/s
bench_sha3_256/4096_mean              6152 ns         6152 ns            8    26.941k      6.52641     137.365k             5.09877       640.43Mi/s
bench_sha3_256/4096_median            6233 ns         6233 ns            8   26.9596k      6.53092     137.365k             5.09521      631.621Mi/s
bench_sha3_256/4096_stddev             175 ns          175 ns            8    76.9602    0.0186435            0           0.0145786      19.0448Mi/s
bench_sha3_256/4096_cv                2.85 %          2.85 %             8      0.29%        0.29%        0.00%               0.29%            2.97%
bench_sha3_512/512                    1498 ns         1498 ns        93793   7.00174k      12.1558      35.619k             5.08717      366.798Mi/s
bench_sha3_512/512                    1562 ns         1562 ns        93793   6.99432k      12.1429      35.619k             5.09256       351.65Mi/s
bench_sha3_512/512                    1595 ns         1595 ns        93793   6.99513k      12.1443      35.619k             5.09197      344.387Mi/s
bench_sha3_512/512                    1615 ns         1615 ns        93793    7.0077k      12.1661      35.619k             5.08284      340.064Mi/s
bench_sha3_512/512                    1614 ns         1614 ns        93793   6.99579k      12.1455      35.619k             5.09149      340.296Mi/s
bench_sha3_512/512                    1623 ns         1623 ns        93793   6.99441k      12.1431      35.619k              5.0925       338.51Mi/s
bench_sha3_512/512                    1623 ns         1623 ns        93793   6.98776k      12.1315      35.619k             5.09734       338.47Mi/s
bench_sha3_512/512                    1621 ns         1621 ns        93793    6.9846k       12.126      35.619k             5.09965      338.914Mi/s
bench_sha3_512/512_mean               1594 ns         1594 ns            8   6.99518k      12.1444      35.619k             5.09194      344.886Mi/s
bench_sha3_512/512_median             1615 ns         1615 ns            8   6.99477k      12.1437      35.619k             5.09224       340.18Mi/s
bench_sha3_512/512_stddev             44.0 ns         44.0 ns            8    7.25491    0.0125953            0            5.27969m      9.91375Mi/s
bench_sha3_512/512_cv                 2.76 %          2.76 %             8      0.10%        0.10%        0.00%               0.10%            2.87%
bench_sha3_512/1024                   2774 ns         2774 ns        50748   13.0017k      11.9501      66.515k             5.11587      374.013Mi/s
bench_sha3_512/1024                   2888 ns         2888 ns        50748   12.9689k        11.92      66.515k              5.1288      359.251Mi/s
bench_sha3_512/1024                   2965 ns         2965 ns        50748   13.0079k      11.9558      66.515k             5.11342      349.908Mi/s
bench_sha3_512/1024                   2993 ns         2992 ns        50748   13.0125k        11.96      66.515k             5.11163      346.774Mi/s
bench_sha3_512/1024                   3007 ns         3007 ns        50748   13.0072k      11.9551      66.515k             5.11371       345.07Mi/s
bench_sha3_512/1024                   3011 ns         3011 ns        50748   13.0111k      11.9588      66.515k             5.11216      344.616Mi/s
bench_sha3_512/1024                   3003 ns         3003 ns        50748   12.9663k      11.9176      66.515k             5.12983      345.503Mi/s
bench_sha3_512/1024                   3038 ns         3038 ns        50748    13.017k      11.9641      66.515k             5.10987      341.561Mi/s
bench_sha3_512/1024_mean              2960 ns         2960 ns            8   12.9991k      11.9477      66.515k             5.11691      350.837Mi/s
bench_sha3_512/1024_median            2998 ns         2998 ns            8   13.0076k      11.9555      66.515k             5.11356      346.139Mi/s
bench_sha3_512/1024_stddev            87.4 ns         87.4 ns            8    19.9264    0.0183147     1.04399m            7.85496m      10.7688Mi/s
bench_sha3_512/1024_cv                2.95 %          2.95 %             8      0.15%        0.15%        0.00%               0.15%            3.07%
bench_shake128/1024/32                1368 ns         1368 ns       101983   6.25126k      5.91976      31.256k             4.99995      736.088Mi/s
bench_shake128/1024/32                1420 ns         1420 ns       101983    6.2497k      5.91828      31.256k              5.0012      709.405Mi/s
bench_shake128/1024/32                1436 ns         1436 ns       101983   6.24111k      5.91014      31.256k             5.00809      701.217Mi/s
bench_shake128/1024/32                1443 ns         1443 ns       101983   6.26553k      5.93327      31.256k             4.98856      698.081Mi/s
bench_shake128/1024/32                1445 ns         1445 ns       101983   6.25236k      5.92079      31.256k             4.99907      696.826Mi/s
bench_shake128/1024/32                1437 ns         1437 ns       101983   6.24308k      5.91201      31.256k              5.0065      700.642Mi/s
bench_shake128/1024/32                1459 ns         1459 ns       101983   6.25845k      5.92657      31.256k              4.9942      690.252Mi/s
bench_shake128/1024/32                1449 ns         1449 ns       101983   6.23011k      5.89972      31.256k             5.01693      694.962Mi/s
bench_shake128/1024/32_mean           1432 ns         1432 ns            8   6.24895k      5.91757      31.256k             5.00181      703.434Mi/s
bench_shake128/1024/32_median         1440 ns         1440 ns            8   6.25048k      5.91902      31.256k             5.00057      699.362Mi/s
bench_shake128/1024/32_stddev         28.2 ns         28.3 ns            8     10.915    0.0103362     521.995u            8.73987m      14.3052Mi/s
bench_shake128/1024/32_cv             1.97 %          1.97 %             8      0.17%        0.17%        0.00%               0.17%            2.03%
bench_sha3_224/2048                   2802 ns         2801 ns        50184   13.0749k      6.29812      66.595k             5.09335      706.703Mi/s
bench_sha3_224/2048                   2872 ns         2872 ns        50184     13.11k      6.31503      66.595k             5.07971      689.369Mi/s
bench_sha3_224/2048                   2904 ns         2904 ns        50184   13.1514k      6.33499      66.595k              5.0637      681.854Mi/s
bench_sha3_224/2048                   3004 ns         3004 ns        50184    13.131k      6.32514      66.595k             5.07159      659.086Mi/s
bench_sha3_224/2048                   3022 ns         3022 ns        50184   13.1197k      6.31969      66.595k             5.07597      655.134Mi/s
bench_sha3_224/2048                   3035 ns         3035 ns        50184   13.1252k      6.32233      66.595k             5.07385      652.341Mi/s
bench_sha3_224/2048                   3051 ns         3050 ns        50184   13.0821k       6.3016      66.595k             5.09054      649.073Mi/s
bench_sha3_224/2048                   3041 ns         3041 ns        50184   13.0965k      6.30853      66.595k             5.08494      651.138Mi/s
bench_sha3_224/2048_mean              2966 ns         2966 ns            8   13.1113k      6.31568      66.595k              5.0792      668.087Mi/s
bench_sha3_224/2048_median            3013 ns         3013 ns            8   13.1148k      6.31736      66.595k             5.07784       657.11Mi/s
bench_sha3_224/2048_stddev            94.0 ns         94.0 ns            8    25.7957    0.0124257     1.04399m            9.99328m      21.6438Mi/s
bench_sha3_224/2048_cv                3.17 %          3.17 %             8      0.20%        0.20%        0.00%               0.20%            3.24%
bench_sha3_512/128                     401 ns          401 ns       349517   1.83157k      9.53941       9.131k             4.98535      456.682Mi/s
bench_sha3_512/128                     409 ns          409 ns       349517   1.83126k      9.53783       9.131k             4.98617      448.011Mi/s
bench_sha3_512/128                     414 ns          414 ns       349517   1.83391k      9.55162       9.131k             4.97898      441.994Mi/s
bench_sha3_512/128                     417 ns          417 ns       349517   1.83394k      9.55175       9.131k             4.97891      438.939Mi/s
bench_sha3_512/128                     420 ns          420 ns       349517   1.83418k      9.55301       9.131k             4.97825      435.618Mi/s
bench_sha3_512/128                     420 ns          420 ns       349517    1.8309k      9.53592       9.131k             4.98717      435.653Mi/s
bench_sha3_512/128                     422 ns          422 ns       349517   1.83331k       9.5485       9.131k              4.9806       434.15Mi/s
bench_sha3_512/128                     425 ns          425 ns       349517   1.83342k      9.54908       9.131k              4.9803      430.809Mi/s
bench_sha3_512/128_mean                416 ns          416 ns            8   1.83281k      9.54589       9.131k             4.98197      440.232Mi/s
bench_sha3_512/128_median              419 ns          419 ns            8   1.83337k      9.54879       9.131k             4.98045      437.296Mi/s
bench_sha3_512/128_stddev             7.90 ns         7.89 ns            8    1.34057     6.98213m            0            3.64504m      8.48465Mi/s
bench_sha3_512/128_cv                 1.90 %          1.90 %             8      0.07%        0.07%        0.00%               0.07%            1.93%
bench_sha3_384/64                      203 ns          203 ns       688512    943.968      8.42829       4.648k              4.9239      526.583Mi/s
bench_sha3_384/64                      212 ns          212 ns       688512    941.808        8.409       4.648k             4.93519      503.978Mi/s
bench_sha3_384/64                      210 ns          210 ns       688512    937.995      8.37495       4.648k             4.95525      507.492Mi/s
bench_sha3_384/64                      211 ns          211 ns       688512     940.87      8.40063       4.648k             4.94011      505.579Mi/s
bench_sha3_384/64                      216 ns          216 ns       688512    936.859      8.36481       4.648k             4.96126      493.813Mi/s
bench_sha3_384/64                      217 ns          217 ns       688512    937.664        8.372       4.648k               4.957      491.714Mi/s
bench_sha3_384/64                      217 ns          217 ns       688512    936.801       8.3643       4.648k             4.96157      491.302Mi/s
bench_sha3_384/64                      217 ns          217 ns       688512    934.875       8.3471       4.648k             4.97179      492.086Mi/s
bench_sha3_384/64_mean                 213 ns          213 ns            8    938.855      8.38263       4.648k             4.95076      501.568Mi/s
bench_sha3_384/64_median               214 ns          214 ns            8    937.829      8.37348       4.648k             4.95613      498.896Mi/s
bench_sha3_384/64_stddev              5.05 ns         5.05 ns            8     3.0501     0.027233     65.2493u           0.0160614      12.1613Mi/s
bench_sha3_384/64_cv                  2.37 %          2.37 %             8      0.32%        0.32%        0.00%               0.32%            2.42%
bench_shake128/128/64                  205 ns          205 ns       676129    960.735      5.00383       4.652k             4.84213      892.587Mi/s
bench_shake128/128/64                  206 ns          206 ns       676129    965.835      5.03039       4.652k             4.81656      887.645Mi/s
bench_shake128/128/64                  215 ns          215 ns       676129    963.574      5.01861       4.652k             4.82786      852.359Mi/s
bench_shake128/128/64                  216 ns          216 ns       676129    961.488      5.00775       4.652k             4.83834      847.192Mi/s
bench_shake128/128/64                  217 ns          217 ns       676129    962.308      5.01202       4.652k             4.83421      842.042Mi/s
bench_shake128/128/64                  221 ns          221 ns       676129    964.661      5.02428       4.652k             4.82242      827.558Mi/s
bench_shake128/128/64                  221 ns          221 ns       676129    959.368      4.99671       4.652k             4.84903      827.196Mi/s
bench_shake128/128/64                  220 ns          220 ns       676129        958      4.98958       4.652k             4.85595      832.947Mi/s
bench_shake128/128/64_mean             215 ns          215 ns            8    961.996       5.0104       4.652k             4.83581      851.191Mi/s
bench_shake128/128/64_median           217 ns          217 ns            8    961.898      5.00989       4.652k             4.83627      844.617Mi/s
bench_shake128/128/64_stddev          6.35 ns         6.36 ns            8    2.64975    0.0138008            0           0.0133213      25.6646Mi/s
bench_shake128/128/64_cv              2.95 %          2.95 %             8      0.28%        0.28%        0.00%               0.28%            3.02%
bench_keccak_permutation               182 ns          182 ns       784335    835.846      4.17923         4.4k             5.26413      1.02174Gi/s
bench_keccak_permutation               179 ns          179 ns       784335    835.806      4.17903         4.4k             5.26438      1.03879Gi/s
bench_keccak_permutation               181 ns          181 ns       784335    834.667      4.17333         4.4k             5.27157      1.02646Gi/s
bench_keccak_permutation               185 ns          185 ns       784335    834.655      4.17327         4.4k             5.27164      1.00858Gi/s
bench_keccak_permutation               189 ns          189 ns       784335    836.175      4.18088         4.4k             5.26206      1011.08Mi/s
bench_keccak_permutation               191 ns          191 ns       784335    835.892      4.17946         4.4k             5.26384      998.868Mi/s
bench_keccak_permutation               193 ns          193 ns       784335     835.81      4.17905         4.4k             5.26435      990.812Mi/s
bench_keccak_permutation               194 ns          194 ns       784335    835.684      4.17842         4.4k             5.26515      981.314Mi/s
bench_keccak_permutation_mean          187 ns          187 ns            8    835.567      4.17783         4.4k             5.26589      1021.99Mi/s
bench_keccak_permutation_median        187 ns          187 ns            8    835.808      4.17904         4.4k             5.26437      1021.94Mi/s
bench_keccak_permutation_stddev       5.60 ns         5.60 ns            8   0.576506     2.88253m            0            3.63544m      30.6465Mi/s
bench_keccak_permutation_cv           3.00 %          3.00 %             8      0.07%        0.07%        0.00%               0.07%            3.00%
bench_shake256/1024/32                1592 ns         1592 ns        87947   7.07504k      6.69985      35.662k             5.04054      632.527Mi/s
bench_shake256/1024/32                1619 ns         1618 ns        87947   7.06862k      6.69377      35.662k             5.04512       622.28Mi/s
bench_shake256/1024/32                1631 ns         1631 ns        87947   7.09097k      6.71493      35.662k             5.02921      617.639Mi/s
bench_shake256/1024/32                1648 ns         1648 ns        87947   7.10465k      6.72789      35.662k             5.01953      610.961Mi/s
bench_shake256/1024/32                1640 ns         1640 ns        87947   7.05596k      6.68178      35.662k             5.05417      613.915Mi/s
bench_shake256/1024/32                1649 ns         1649 ns        87947   7.08584k      6.71008      35.662k             5.03285      610.709Mi/s
bench_shake256/1024/32                1633 ns         1633 ns        87947   7.08024k      6.70477      35.662k             5.03684      616.888Mi/s
bench_shake256/1024/32                1634 ns         1634 ns        87947   7.06204k      6.68754      35.662k             5.04982      616.244Mi/s
bench_shake256/1024/32_mean           1631 ns         1631 ns            8   7.07792k      6.70258      35.662k             5.03851      617.645Mi/s
bench_shake256/1024/32_median         1634 ns         1633 ns            8   7.07764k      6.70231      35.662k             5.03869      616.566Mi/s
bench_shake256/1024/32_stddev         18.5 ns         18.5 ns            8    15.9565    0.0151103     521.995u           0.0113531      7.09326Mi/s
bench_shake256/1024/32_cv             1.13 %          1.13 %             8      0.23%        0.23%        0.00%               0.23%            1.15%
bench_sha3_256/2048                   2986 ns         2986 ns        47129   13.9601k      6.71157      71.007k             5.08644      664.296Mi/s
bench_sha3_256/2048                   3239 ns         3239 ns        47129   14.0037k      6.73254      71.007k              5.0706      612.411Mi/s
bench_sha3_256/2048                   3225 ns         3225 ns        47129   13.9799k      6.72112      71.007k             5.07921      615.114Mi/s
bench_sha3_256/2048                   3238 ns         3237 ns        47129   13.9845k      6.72331      71.007k             5.07755      612.721Mi/s
bench_sha3_256/2048                   3235 ns         3235 ns        47129   13.9608k      6.71191      71.007k             5.08618      613.188Mi/s
bench_sha3_256/2048                   3251 ns         3251 ns        47129   13.9934k      6.72757      71.007k             5.07434      610.145Mi/s
bench_sha3_256/2048                   3254 ns         3254 ns        47129   13.9748k      6.71864      71.007k             5.08109      609.537Mi/s
bench_sha3_256/2048                   3262 ns         3262 ns        47129   13.9963k      6.72899      71.007k             5.07327      608.047Mi/s
bench_sha3_256/2048_mean              3212 ns         3211 ns            8   13.9817k      6.72196      71.007k             5.07859      618.182Mi/s
bench_sha3_256/2048_median            3238 ns         3238 ns            8   13.9822k      6.72222      71.007k             5.07838      612.566Mi/s
bench_sha3_256/2048_stddev            91.8 ns         91.8 ns            8    16.0226     7.70319m            0            5.82088m      18.7693Mi/s
bench_sha3_256/2048_cv                2.86 %          2.86 %             8      0.11%        0.11%        0.00%               0.11%            3.04%
bench_sha3_224/1024                   1565 ns         1565 ns        90079    7.0687k       6.7193      35.612k             5.03799      640.948Mi/s
bench_sha3_224/1024                   1558 ns         1558 ns        90079   7.04229k      6.69419      35.612k             5.05688      643.876Mi/s
bench_sha3_224/1024                   1604 ns         1604 ns        90079   7.06251k      6.71341      35.612k              5.0424      625.476Mi/s
bench_sha3_224/1024                   1602 ns         1601 ns        90079   7.07053k      6.72104      35.612k             5.03668      626.465Mi/s
bench_sha3_224/1024                   1625 ns         1625 ns        90079   7.06865k      6.71924      35.612k             5.03802      617.449Mi/s
bench_sha3_224/1024                   1632 ns         1632 ns        90079   7.05329k      6.70465      35.612k             5.04899      614.696Mi/s
bench_sha3_224/1024                   1635 ns         1635 ns        90079    7.0524k       6.7038      35.612k             5.04963      613.633Mi/s
bench_sha3_224/1024                   1639 ns         1638 ns        90079   7.04595k      6.69767      35.612k             5.05425       612.36Mi/s
bench_sha3_224/1024_mean              1608 ns         1607 ns            8   7.05804k      6.70916      35.612k             5.04561      624.363Mi/s
bench_sha3_224/1024_median            1615 ns         1614 ns            8    7.0579k      6.70903      35.612k              5.0457      621.463Mi/s
bench_sha3_224/1024_stddev            31.4 ns         31.3 ns            8    11.0249    0.0104799     521.995u            7.88339m      12.3169Mi/s
bench_sha3_224/1024_cv                1.95 %          1.95 %             8      0.16%        0.16%        0.00%               0.16%            1.97%
bench_shake128/1024/64                1419 ns         1418 ns        98787   6.24949k      5.74402      31.256k             5.00137      731.507Mi/s
bench_shake128/1024/64                1431 ns         1431 ns        98787   6.24264k      5.73772      31.256k             5.00685      725.316Mi/s
bench_shake128/1024/64                1443 ns         1443 ns        98787   6.25066k      5.74509      31.256k             5.00044      719.121Mi/s
bench_shake128/1024/64                1443 ns         1443 ns        98787   6.25961k      5.75332      31.256k             4.99328       718.93Mi/s
bench_shake128/1024/64                1457 ns         1457 ns        98787   6.26101k      5.75461      31.256k             4.99216      712.112Mi/s
bench_shake128/1024/64                1462 ns         1461 ns        98787    6.2739k      5.76646      31.256k             4.98191      709.979Mi/s
bench_shake128/1024/64                1444 ns         1444 ns        98787   6.22493k      5.72144      31.256k              5.0211      718.437Mi/s
bench_shake128/1024/64                1456 ns         1455 ns        98787   6.23986k      5.73516      31.256k             5.00909      712.921Mi/s
bench_shake128/1024/64_mean           1444 ns         1444 ns            8   6.25026k      5.74473      31.256k             5.00078       718.54Mi/s
bench_shake128/1024/64_median         1444 ns         1444 ns            8   6.25007k      5.74455      31.256k              5.0009      718.683Mi/s
bench_shake128/1024/64_stddev         14.4 ns         14.4 ns            8     14.979    0.0137675     369.106u           0.0119878      7.17899Mi/s
bench_shake128/1024/64_cv             1.00 %          0.99 %             8      0.24%        0.24%        0.00%               0.24%            1.00%
bench_sha3_384/1024                   1882 ns         1882 ns        75567       8.8k      8.20895      44.462k              5.0525      543.128Mi/s
bench_sha3_384/1024                   1951 ns         1951 ns        75567   8.70959k      8.12462      44.462k             5.10495      523.922Mi/s
bench_sha3_384/1024                   2011 ns         2011 ns        75567   8.74824k      8.16068      44.462k             5.08239      508.443Mi/s
bench_sha3_384/1024                   2005 ns         2005 ns        75567   8.73809k       8.1512      44.462k              5.0883      509.845Mi/s
bench_sha3_384/1024                   2044 ns         2043 ns        75567    8.7551k      8.16707      44.462k             5.07841      500.324Mi/s
bench_sha3_384/1024                   2034 ns         2034 ns        75567   8.75529k      8.16725      44.462k              5.0783      502.697Mi/s
bench_sha3_384/1024                   2018 ns         2018 ns        75567   8.71911k       8.1335      44.462k             5.09937      506.656Mi/s
bench_sha3_384/1024                   2029 ns         2028 ns        75567   8.73161k      8.14516      44.462k             5.09207       504.01Mi/s
bench_sha3_384/1024_mean              1997 ns         1997 ns            8   8.74463k       8.1573      44.462k             5.08454      512.378Mi/s
bench_sha3_384/1024_median            2014 ns         2014 ns            8   8.74317k      8.15594      44.462k             5.08534      507.549Mi/s
bench_sha3_384/1024_stddev            54.1 ns         54.0 ns            8    27.7573     0.025893     521.995u           0.0161021      14.3553Mi/s
bench_sha3_384/1024_cv                2.71 %          2.70 %             8      0.32%        0.32%        0.00%               0.32%            2.80%
bench_sha3_512/256                     798 ns          798 ns       174954   3.55067k      11.0958      17.962k             5.05877      382.625Mi/s
bench_sha3_512/256                     814 ns          814 ns       174954    3.5532k      11.1038      17.962k             5.05516      374.767Mi/s
bench_sha3_512/256                     815 ns          815 ns       174954   3.55743k       11.117      17.962k             5.04915      374.663Mi/s
bench_sha3_512/256                     818 ns          818 ns       174954   3.55417k      11.1068      17.962k             5.05378      373.244Mi/s
bench_sha3_512/256                     818 ns          818 ns       174954   3.55305k      11.1033      17.962k             5.05538      373.241Mi/s
bench_sha3_512/256                     817 ns          817 ns       174954   3.55331k      11.1041      17.962k               5.055      373.589Mi/s
bench_sha3_512/256                     825 ns          825 ns       174954   3.55062k      11.0957      17.962k             5.05884      369.855Mi/s
bench_sha3_512/256                     827 ns          827 ns       174954   3.55481k      11.1088      17.962k             5.05288      368.879Mi/s
bench_sha3_512/256_mean                816 ns          816 ns            8   3.55341k      11.1044      17.962k             5.05487      373.858Mi/s
bench_sha3_512/256_median              817 ns          817 ns            8   3.55326k      11.1039      17.962k             5.05508      373.416Mi/s
bench_sha3_512/256_stddev             8.96 ns         8.95 ns            8    2.21053      6.9079m     260.997u             3.1439m      4.14005Mi/s
bench_sha3_512/256_cv                 1.10 %          1.10 %             8      0.06%        0.06%        0.00%               0.06%            1.11%
bench_sha3_224/4096                   5411 ns         5411 ns        26078   25.3015k      6.13519     128.559k             5.08108      726.854Mi/s
bench_sha3_224/4096                   5374 ns         5374 ns        26078   25.0997k      6.08626     128.559k             5.12193      731.909Mi/s
bench_sha3_224/4096                   5407 ns         5407 ns        26078   25.1083k      6.08835     128.559k             5.12017      727.412Mi/s
bench_sha3_224/4096                   5816 ns         5815 ns        26078   25.2121k      6.11351     128.559k              5.0991      676.292Mi/s
bench_sha3_224/4096                   5801 ns         5800 ns        26078   25.2763k      6.12907     128.559k             5.08615      678.065Mi/s
bench_sha3_224/4096                   5845 ns         5845 ns        26078    25.246k      6.12173     128.559k             5.09225      672.866Mi/s
bench_sha3_224/4096                   5825 ns         5824 ns        26078   25.2058k      6.11199     128.559k             5.10037      675.255Mi/s
bench_sha3_224/4096                   5868 ns         5867 ns        26078   25.2161k      6.11447     128.559k              5.0983       670.33Mi/s
bench_sha3_224/4096_mean              5668 ns         5668 ns            8   25.2082k      6.11257     128.559k             5.09992      694.873Mi/s
bench_sha3_224/4096_median            5809 ns         5808 ns            8   25.2141k      6.11399     128.559k              5.0987      677.179Mi/s
bench_sha3_224/4096_stddev             226 ns          225 ns            8    72.3092    0.0175338     2.08798m           0.0146465      28.1641Mi/s
bench_sha3_224/4096_cv                3.98 %          3.98 %             8      0.29%        0.29%        0.00%               0.29%            4.05%
bench_sha3_512/32                      209 ns          209 ns       669249    938.851       9.7797       4.685k             4.99014      437.178Mi/s
bench_sha3_512/32                      213 ns          213 ns       669249    937.586      9.76652       4.685k             4.99687       429.68Mi/s
bench_sha3_512/32                      214 ns          214 ns       669249    938.635      9.77745       4.685k             4.99129      427.875Mi/s
bench_sha3_512/32                      217 ns          217 ns       669249    938.877      9.77997       4.685k                4.99      421.361Mi/s
bench_sha3_512/32                      218 ns          218 ns       669249    936.124      9.75129       4.685k             5.00468      419.846Mi/s
bench_sha3_512/32                      217 ns          217 ns       669249    937.432      9.76492       4.685k              4.9977      422.776Mi/s
bench_sha3_512/32                      216 ns          216 ns       669249    937.663      9.76733       4.685k             4.99646       423.39Mi/s
bench_sha3_512/32                      218 ns          218 ns       669249    935.878      9.74873       4.685k               5.006      420.804Mi/s
bench_sha3_512/32_mean                 215 ns          215 ns            8    937.631      9.76699       4.685k             4.99664      425.364Mi/s
bench_sha3_512/32_median               216 ns          216 ns            8    937.625      9.76692       4.685k             4.99667      423.083Mi/s
bench_sha3_512/32_stddev              2.95 ns         2.94 ns            8    1.16119    0.0120958            0            6.19085m      5.88415Mi/s
bench_sha3_512/32_cv                  1.37 %          1.37 %             8      0.12%        0.12%        0.00%               0.12%            1.38%
bench_shake256/2048/64                3032 ns         3031 ns        45925   13.9825k      6.62048      71.045k             5.08101      664.429Mi/s
bench_shake256/2048/64                3198 ns         3197 ns        45925   14.0054k      6.63135      71.045k             5.07268      629.932Mi/s
bench_shake256/2048/64                3244 ns         3244 ns        45925   14.0549k      6.65477      71.045k             5.05483      620.926Mi/s
bench_shake256/2048/64                3244 ns         3244 ns        45925   14.0448k      6.64998      71.045k             5.05847      620.894Mi/s
bench_shake256/2048/64                3240 ns         3240 ns        45925    14.009k      6.63303      71.045k              5.0714      621.719Mi/s
bench_shake256/2048/64                3256 ns         3256 ns        45925        14k       6.6288      71.045k             5.07463      618.625Mi/s
bench_shake256/2048/64                3253 ns         3253 ns        45925   14.0277k      6.64192      71.045k             5.06461      619.157Mi/s
bench_shake256/2048/64                3233 ns         3232 ns        45925   13.7286k      6.50028     69.7653k             5.08175      623.194Mi/s
bench_shake256/2048/64_mean           3212 ns         3212 ns            8   13.9816k      6.62008      70.885k             5.06992       627.36Mi/s
bench_shake256/2048/64_median         3242 ns         3242 ns            8   14.0072k      6.63219      71.045k             5.07204      621.323Mi/s
bench_shake256/2048/64_stddev         75.3 ns         75.2 ns            8    104.986    0.0497091      452.463            9.86957m      15.3841Mi/s
bench_shake256/2048/64_cv             2.34 %          2.34 %             8      0.75%        0.75%        0.64%               0.19%            2.45%
bench_shake128/512/32                  809 ns          809 ns       174500   3.62415k      6.66204      17.971k             4.95868      641.351Mi/s
bench_shake128/512/32                  830 ns          830 ns       174500   3.62043k      6.65519      17.971k             4.96378      624.825Mi/s
bench_shake128/512/32                  827 ns          827 ns       174500   3.61228k      6.64022      17.971k             4.97497      627.294Mi/s
bench_shake128/512/32                  834 ns          834 ns       174500   3.63273k      6.67781      17.971k             4.94697      622.338Mi/s
bench_shake128/512/32                  829 ns          829 ns       174500   3.62108k       6.6564      17.971k             4.96288      625.499Mi/s
bench_shake128/512/32                  836 ns          836 ns       174500   3.62398k      6.66173      17.971k             4.95891      620.724Mi/s
bench_shake128/512/32                  841 ns          841 ns       174500   3.61931k      6.65314      17.971k             4.96531      616.815Mi/s
bench_shake128/512/32                  838 ns          837 ns       174500   3.61764k      6.65007      17.971k             4.96761      619.467Mi/s
bench_shake128/512/32_mean             831 ns          830 ns            8   3.62145k      6.65708      17.971k             4.96239      624.789Mi/s
bench_shake128/512/32_median           832 ns          832 ns            8   3.62075k       6.6558      17.971k             4.96333      623.582Mi/s
bench_shake128/512/32_stddev          9.83 ns         9.84 ns            8     5.9196    0.0108816            0            8.10576m      7.51684Mi/s
bench_shake128/512/32_cv              1.18 %          1.19 %             8      0.16%        0.16%        0.00%               0.16%            1.20%
bench_shake256/256/64                  400 ns          400 ns       354966    1.8662k      5.83188       9.115k             4.88426      763.356Mi/s
bench_shake256/256/64                  412 ns          412 ns       354966   1.85629k      5.80091       9.115k             4.91033       741.45Mi/s
bench_shake256/256/64                  415 ns          415 ns       354966   1.85774k      5.80545       9.115k             4.90649       734.55Mi/s
bench_shake256/256/64                  425 ns          425 ns       354966    1.8656k         5.83       9.115k             4.88583       717.42Mi/s
bench_shake256/256/64                  431 ns          430 ns       354966   1.85186k      5.78705       9.115k             4.92209      708.928Mi/s
bench_shake256/256/64                  429 ns          429 ns       354966   1.85816k      5.80676       9.115k             4.90538      712.036Mi/s
bench_shake256/256/64                  430 ns          430 ns       354966   1.85903k      5.80948       9.115k             4.90309      709.431Mi/s
bench_shake256/256/64                  428 ns          428 ns       354966   1.86874k      5.83983       9.115k             4.87761      713.345Mi/s
bench_shake256/256/64_mean             421 ns          421 ns            8   1.86045k      5.81392       9.115k             4.89938      725.064Mi/s
bench_shake256/256/64_median           427 ns          427 ns            8    1.8586k      5.80812       9.115k             4.90424      715.383Mi/s
bench_shake256/256/64_stddev          11.1 ns         11.1 ns            8    5.78302    0.0180719            0            0.015224      19.6213Mi/s
bench_shake256/256/64_cv              2.64 %          2.64 %             8      0.31%        0.31%        0.00%               0.31%            2.71%
bench_shake256/128/64                  201 ns          201 ns       690703    941.074      4.90143       4.655k             4.94647       910.35Mi/s
bench_shake256/128/64                  202 ns          202 ns       690703    944.023      4.91679       4.655k             4.93103      908.519Mi/s
bench_shake256/128/64                  212 ns          212 ns       690703    943.006      4.91149       4.655k             4.93634        864.4Mi/s
bench_shake256/128/64                  215 ns          215 ns       690703    940.943      4.90075       4.655k             4.94716      852.396Mi/s
bench_shake256/128/64                  216 ns          216 ns       690703     941.24      4.90229       4.655k             4.94561      848.665Mi/s
bench_shake256/128/64                  218 ns          218 ns       690703    947.509      4.93494       4.655k             4.91288      840.362Mi/s
bench_shake256/128/64                  218 ns          218 ns       690703    941.187      4.90202       4.655k             4.94588      838.969Mi/s
bench_shake256/128/64                  217 ns          217 ns       690703      937.9       4.8849       4.655k             4.96321      845.133Mi/s
bench_shake256/128/64_mean             212 ns          212 ns            8     942.11      4.90682       4.655k             4.94107      863.599Mi/s
bench_shake256/128/64_median           215 ns          215 ns            8    941.213      4.90215       4.655k             4.94574      850.531Mi/s
bench_shake256/128/64_stddev          7.02 ns         7.01 ns            8    2.81262    0.0146491            0           0.0147273      29.3749Mi/s
bench_shake256/128/64_cv              3.31 %          3.31 %             8      0.30%        0.30%        0.00%               0.30%            3.40%
bench_sha3_384/512                     963 ns          963 ns       145483    4.4031k      7.86267      22.367k             5.07983      554.438Mi/s
bench_sha3_384/512                     965 ns          965 ns       145483   4.39276k      7.84422      22.367k             5.09179      553.333Mi/s
bench_sha3_384/512                     973 ns          973 ns       145483   4.38593k      7.83202      22.367k             5.09971      549.159Mi/s
bench_sha3_384/512                     994 ns          994 ns       145483   4.38148k      7.82407      22.367k              5.1049      537.131Mi/s
bench_sha3_384/512                    1018 ns         1018 ns       145483   4.39237k      7.84351      22.367k             5.09224      524.652Mi/s
bench_sha3_384/512                    1012 ns         1012 ns       145483   4.38592k        7.832      22.367k             5.09973      527.855Mi/s
bench_sha3_384/512                    1018 ns         1018 ns       145483   4.38354k      7.82776      22.367k             5.10249       524.45Mi/s
bench_sha3_384/512                    1021 ns         1021 ns       145483   4.39864k      7.85471      22.367k             5.08498      523.307Mi/s
bench_sha3_384/512_mean                996 ns          995 ns            8   4.39047k      7.84012      22.367k             5.09446      536.791Mi/s
bench_sha3_384/512_median             1003 ns         1003 ns            8   4.38915k      7.83777      22.367k             5.09598      532.493Mi/s
bench_sha3_384/512_stddev             25.1 ns         25.1 ns            8    7.60714    0.0135842            0            8.82033m      13.6248Mi/s
bench_sha3_384/512_cv                 2.52 %          2.52 %             8      0.17%        0.17%        0.00%               0.17%            2.54%
bench_shake256/4096/64                5908 ns         5907 ns        23467   26.8811k       6.4618     137.403k             5.11151      671.644Mi/s
bench_shake256/4096/64                5957 ns         5957 ns        23467   26.9243k      6.47219     137.403k             5.10331          666Mi/s
bench_shake256/4096/64                5949 ns         5948 ns        23467   27.0091k      6.49257     137.403k             5.08729      666.947Mi/s
bench_shake256/4096/64                6173 ns         6173 ns        23467   26.9646k      6.48187     137.403k             5.09568      642.715Mi/s
bench_shake256/4096/64                6344 ns         6342 ns        23467   25.4741k      6.12357     129.862k             5.09781      625.596Mi/s
bench_shake256/4096/64                6266 ns         6264 ns        23467   26.9237k      6.47204     137.403k             5.10342        633.3Mi/s
bench_shake256/4096/64                6246 ns         6245 ns        23467   26.8988k      6.46607     137.403k             5.10814      635.256Mi/s
bench_shake256/4096/64                6223 ns         6223 ns        23467   26.9274k      6.47294     137.403k             5.10271      637.507Mi/s
bench_shake256/4096/64_mean           6133 ns         6132 ns            8   26.7504k      6.43038      136.46k             5.10123      647.371Mi/s
bench_shake256/4096/64_median         6198 ns         6198 ns            8    26.924k      6.47212     137.403k             5.10301      640.111Mi/s
bench_shake256/4096/64_stddev          169 ns          169 ns            8    517.213      0.12433     2.66615k            7.57504m      17.9576Mi/s
bench_shake256/4096/64_cv             2.76 %          2.75 %             8      1.93%        1.93%        1.95%               0.15%            2.77%
bench_shake128/32/32                   206 ns          206 ns       687200    963.186      15.0498       4.642k             4.81942      296.138Mi/s
bench_shake128/32/32                   205 ns          205 ns       687200    955.941      14.9366       4.642k             4.85595      297.865Mi/s
bench_shake128/32/32                   209 ns          209 ns       687200    958.777      14.9809       4.642k             4.84159      292.533Mi/s
bench_shake128/32/32                   222 ns          222 ns       687200    969.221      15.1441       4.642k             4.78941       275.34Mi/s
bench_shake128/32/32                   219 ns          219 ns       687200    959.245      14.9882       4.642k             4.83923      279.037Mi/s
bench_shake128/32/32                   222 ns          222 ns       687200    962.313      15.0361       4.642k             4.82379      275.497Mi/s
bench_shake128/32/32                   223 ns          223 ns       687200    958.705      14.9798       4.642k             4.84195      273.313Mi/s
bench_shake128/32/32                   220 ns          220 ns       687200    967.328      15.1145       4.642k             4.79879      277.425Mi/s
bench_shake128/32/32_mean              216 ns          216 ns            8    961.839      15.0287       4.642k             4.82627      283.394Mi/s
bench_shake128/32/32_median            219 ns          219 ns            8    960.779      15.0122       4.642k             4.83151      278.231Mi/s
bench_shake128/32/32_stddev           7.69 ns         7.69 ns            8    4.58849    0.0716951     65.2493u           0.0229777      10.2734Mi/s
bench_shake128/32/32_cv               3.57 %          3.57 %             8      0.48%        0.48%        0.00%               0.48%            3.63%
bench_sha3_512/4096                  10466 ns        10465 ns        13439   49.0153k      11.7825     251.904k              5.1393      379.083Mi/s
bench_sha3_512/4096                  10560 ns        10559 ns        13439   49.3812k      11.8705     251.904k             5.10121      375.716Mi/s
bench_sha3_512/4096                  10811 ns        10811 ns        13439    48.978k      11.7736     251.904k             5.14321      366.968Mi/s
bench_sha3_512/4096                  10800 ns        10799 ns        13439   48.9925k       11.777     251.904k             5.14169      367.373Mi/s
bench_sha3_512/4096                  11350 ns        11349 ns        13439    49.122k      11.8082     251.904k             5.12813       349.57Mi/s
bench_sha3_512/4096                  11446 ns        11445 ns        13439   48.9766k      11.7732     251.904k             5.14335      346.632Mi/s
bench_sha3_512/4096                  11395 ns        11395 ns        13439   49.0463k        11.79     251.904k             5.13604      348.169Mi/s
bench_sha3_512/4096                  11428 ns        11427 ns        13439   49.0932k      11.8013     251.904k             5.13114      347.183Mi/s
bench_sha3_512/4096_mean             11032 ns        11031 ns            8   49.0756k       11.797     251.904k             5.13301      360.087Mi/s
bench_sha3_512/4096_median           11081 ns        11080 ns            8   49.0308k      11.7862     251.904k             5.13767      358.269Mi/s
bench_sha3_512/4096_stddev             415 ns          415 ns            8    134.478    0.0323264            0           0.0140072      13.6577Mi/s
bench_sha3_512/4096_cv                3.76 %          3.76 %             8      0.27%        0.27%        0.00%               0.27%            3.79%
bench_sha3_256/512                     770 ns          770 ns       180411   3.59939k      6.61652      17.928k             4.98085      673.714Mi/s
bench_sha3_256/512                     797 ns          797 ns       180411    3.5869k      6.59357      17.928k             4.99819      650.969Mi/s
bench_sha3_256/512                     818 ns          818 ns       180411   3.59222k      6.60335      17.928k             4.99078      633.918Mi/s
bench_sha3_256/512                     827 ns          827 ns       180411   3.58918k      6.59776      17.928k             4.99501      627.341Mi/s
bench_sha3_256/512                     836 ns          836 ns       180411   3.59192k      6.60279      17.928k             4.99121      620.632Mi/s
bench_sha3_256/512                     832 ns          832 ns       180411   3.59122k       6.6015      17.928k             4.99218      623.885Mi/s
bench_sha3_256/512                     828 ns          828 ns       180411   3.59336k      6.60545      17.928k              4.9892      626.456Mi/s
bench_sha3_256/512                     831 ns          831 ns       180411   3.58401k      6.58826      17.928k             5.00222      624.244Mi/s
bench_sha3_256/512_mean                817 ns          817 ns            8   3.59103k      6.60115      17.928k             4.99245      635.145Mi/s
bench_sha3_256/512_median              828 ns          828 ns            8   3.59157k      6.60214      17.928k             4.99169      626.899Mi/s
bench_sha3_256/512_stddev             22.7 ns         22.7 ns            8    4.58682     8.43165m            0            6.37474m      18.2526Mi/s
bench_sha3_256/512_cv                 2.78 %          2.77 %             8      0.13%        0.13%        0.00%               0.13%            2.87%
bench_sha3_256/64                      203 ns          203 ns       697084    946.744      9.86192       4.607k             4.86615      450.602Mi/s
bench_sha3_256/64                      205 ns          205 ns       697084    950.035      9.89619       4.607k              4.8493      446.834Mi/s
bench_sha3_256/64                      205 ns          205 ns       697084    945.455      9.84849       4.607k             4.87278      445.993Mi/s
bench_sha3_256/64                      211 ns          211 ns       697084    949.691      9.89261       4.607k             4.85105      433.766Mi/s
bench_sha3_256/64                      212 ns          212 ns       697084    951.643      9.91294       4.607k              4.8411      432.843Mi/s
bench_sha3_256/64                      215 ns          215 ns       697084    946.164      9.85588       4.607k             4.86913      425.076Mi/s
bench_sha3_256/64                      217 ns          217 ns       697084    945.247      9.84632       4.607k             4.87386      421.613Mi/s
bench_sha3_256/64                      217 ns          217 ns       697084    950.439      9.90041       4.607k             4.84723      422.638Mi/s
bench_sha3_256/64_mean                 211 ns          211 ns            8    948.177      9.87685       4.607k             4.85883      434.921Mi/s
bench_sha3_256/64_median               211 ns          211 ns            8    948.217      9.87726       4.607k              4.8586      433.304Mi/s
bench_sha3_256/64_stddev              5.59 ns         5.59 ns            8    2.53444    0.0264004     65.2493u           0.0129855       11.594Mi/s
bench_sha3_256/64_cv                  2.65 %          2.66 %             8      0.27%        0.27%        0.00%               0.27%            2.67%
bench_shake128/64/32                   218 ns          218 ns       641126    962.767      10.0288       4.642k             4.82152      420.776Mi/s
bench_shake128/64/32                   221 ns          221 ns       641126    965.721      10.0596       4.642k             4.80677      414.614Mi/s
bench_shake128/64/32                   217 ns          217 ns       641126    964.547      10.0474       4.642k             4.81262      421.665Mi/s
bench_shake128/64/32                   221 ns          221 ns       641126    961.766      10.0184       4.642k             4.82654      414.011Mi/s
bench_shake128/64/32                   224 ns          224 ns       641126    964.275      10.0445       4.642k             4.81398      409.034Mi/s
bench_shake128/64/32                   223 ns          223 ns       641126    965.671      10.0591       4.642k             4.80702      410.359Mi/s
bench_shake128/64/32                   222 ns          222 ns       641126    965.189       10.054       4.642k             4.80942      413.077Mi/s
bench_shake128/64/32                   222 ns          222 ns       641126    966.591      10.0687       4.642k             4.80245      412.496Mi/s
bench_shake128/64/32_mean              221 ns          221 ns            8    964.566      10.0476       4.642k             4.81254      414.504Mi/s
bench_shake128/64/32_median            221 ns          221 ns            8    964.868      10.0507       4.642k             4.81102      413.544Mi/s
bench_shake128/64/32_stddev           2.41 ns         2.40 ns            8     1.6126    0.0167979     65.2493u            8.05334m      4.53625Mi/s
bench_shake128/64/32_cv               1.09 %          1.09 %             8      0.17%        0.17%        0.00%               0.17%            1.09%
bench_sha3_384/256                     613 ns          613 ns       227762   2.68522k      8.83297      13.519k             5.03459      473.076Mi/s
bench_sha3_384/256                     618 ns          618 ns       227762   2.68609k      8.83584      13.519k             5.03296      468.806Mi/s
bench_sha3_384/256                     625 ns          625 ns       227762    2.6836k      8.82765      13.519k             5.03763      463.709Mi/s
bench_sha3_384/256                     621 ns          621 ns       227762   2.68281k      8.82503      13.519k             5.03912      466.826Mi/s
bench_sha3_384/256                     622 ns          622 ns       227762   2.68426k      8.82982      13.519k             5.03639      466.378Mi/s
bench_sha3_384/256                     624 ns          624 ns       227762    2.6891k      8.84573      13.519k             5.02733       464.61Mi/s
bench_sha3_384/256                     622 ns          622 ns       227762   2.69202k      8.85531      13.519k             5.02189      466.449Mi/s
bench_sha3_384/256                     624 ns          624 ns       227762   2.68847k      8.84365      13.519k             5.02851      464.772Mi/s
bench_sha3_384/256_mean                621 ns          621 ns            8   2.68645k        8.837      13.519k              5.0323      466.828Mi/s
bench_sha3_384/256_median              622 ns          622 ns            8   2.68566k       8.8344      13.519k             5.03378      466.414Mi/s
bench_sha3_384/256_stddev             3.95 ns         3.94 ns            8    3.15852    0.0103899            0            5.91294m      2.98016Mi/s
bench_sha3_384/256_cv                 0.64 %          0.63 %             8      0.12%        0.12%        0.00%               0.12%            0.64%
bench_shake128/2048/32                2468 ns         2468 ns        57555   11.5247k      5.54073      57.831k               5.018       803.76Mi/s
bench_shake128/2048/32                2518 ns         2518 ns        57555   11.5277k      5.54216      57.831k              5.0167      787.743Mi/s
bench_shake128/2048/32                2652 ns         2651 ns        57555   11.5191k      5.53805      57.831k             5.02043      748.153Mi/s
bench_shake128/2048/32                2652 ns         2652 ns        57555   11.5254k      5.54104      57.831k             5.01772      748.056Mi/s
bench_shake128/2048/32                2643 ns         2643 ns        57555   11.4501k      5.50484      57.831k             5.05072      750.484Mi/s
bench_shake128/2048/32                2653 ns         2653 ns        57555   11.4598k       5.5095      57.831k             5.04644      747.721Mi/s
bench_shake128/2048/32                2667 ns         2667 ns        57555   11.5098k      5.53355      57.831k             5.02451      743.888Mi/s
bench_shake128/2048/32                2680 ns         2679 ns        57555   11.5647k      5.55995      57.831k             5.00065      740.321Mi/s
bench_shake128/2048/32_mean           2617 ns         2616 ns            8   11.5102k      5.53373      57.831k              5.0244      758.766Mi/s
bench_shake128/2048/32_median         2652 ns         2652 ns            8   11.5219k      5.53939      57.831k             5.01921      748.105Mi/s
bench_shake128/2048/32_stddev         78.1 ns         78.1 ns            8    37.7169    0.0181331     1.27862m           0.0164889      23.4326Mi/s
bench_shake128/2048/32_cv             2.99 %          2.99 %             8      0.33%        0.33%        0.00%               0.33%            3.09%
bench_sha3_224/32                      202 ns          201 ns       682786    940.206      15.6701       4.593k              4.8851      283.985Mi/s
bench_sha3_224/32                      209 ns          209 ns       682786    941.276      15.6879       4.593k             4.87955        273.6Mi/s
bench_sha3_224/32                      213 ns          213 ns       682786    949.436      15.8239       4.593k             4.83761      269.133Mi/s
bench_sha3_224/32                      214 ns          214 ns       682786    945.335      15.7556       4.593k             4.85859      267.212Mi/s
bench_sha3_224/32                      216 ns          216 ns       682786    950.508      15.8418       4.593k             4.83215      265.355Mi/s
bench_sha3_224/32                      214 ns          214 ns       682786    944.176      15.7363       4.593k             4.86456      267.393Mi/s
bench_sha3_224/32                      221 ns          221 ns       682786    949.218      15.8203       4.593k             4.83872      258.884Mi/s
bench_sha3_224/32                      218 ns          218 ns       682786    950.722      15.8454       4.593k             4.83107      262.322Mi/s
bench_sha3_224/32_mean                 213 ns          213 ns            8     946.36      15.7727       4.593k             4.85342      268.485Mi/s
bench_sha3_224/32_median               214 ns          214 ns            8    947.277      15.7879       4.593k             4.84866      267.302Mi/s
bench_sha3_224/32_stddev              5.94 ns         5.94 ns            8    4.19871    0.0699786     65.2493u           0.0215646      7.64955Mi/s
bench_sha3_224/32_cv                  2.78 %          2.78 %             8      0.44%        0.44%        0.00%               0.44%            2.85%
bench_shake256/4096/32                6150 ns         6149 ns        22919   27.0123k      6.54367     137.403k             5.08669      640.198Mi/s
bench_shake256/4096/32                6159 ns         6159 ns        22919   27.0444k      6.55146     137.403k             5.08064      639.177Mi/s
bench_shake256/4096/32                6168 ns         6168 ns        22919   26.9526k      6.52921     137.403k             5.09795       638.27Mi/s
bench_shake256/4096/32                6241 ns         6240 ns        22919   26.9924k      6.53886     137.403k             5.09043      630.876Mi/s
bench_shake256/4096/32                6235 ns         6234 ns        22919    27.013k      6.54384     137.403k             5.08656      631.454Mi/s
bench_shake256/4096/32                6238 ns         6237 ns        22919   27.0327k      6.54863     137.403k             5.08284      631.157Mi/s
bench_shake256/4096/32                6260 ns         6259 ns        22919   26.9735k      6.53428     137.403k               5.094      628.959Mi/s
bench_shake256/4096/32                6257 ns         6257 ns        22919   26.9632k      6.53178     137.403k             5.09595      629.215Mi/s
bench_shake256/4096/32_mean           6213 ns         6213 ns            8    26.998k      6.54022     137.403k             5.08938      633.663Mi/s
bench_shake256/4096/32_median         6236 ns         6236 ns            8   27.0023k      6.54127     137.403k             5.08856      631.305Mi/s
bench_shake256/4096/32_stddev         46.1 ns         46.0 ns            8     33.156     8.03197m            0             6.2503m      4.70829Mi/s
bench_shake256/4096/32_cv             0.74 %          0.74 %             8      0.12%        0.12%        0.00%               0.12%            0.74%
bench_shake128/4096/64                4704 ns         4703 ns        29551   22.0009k      5.28868     110.979k             5.04429      843.531Mi/s
bench_shake128/4096/64                4882 ns         4882 ns        29551   21.9979k      5.28796     110.979k             5.04498      812.611Mi/s
bench_shake128/4096/64                5037 ns         5037 ns        29551   21.8413k      5.25031     110.979k             5.08115      787.664Mi/s
bench_shake128/4096/64                5058 ns         5058 ns        29551   21.9395k      5.27391     110.979k             5.05841      784.422Mi/s
bench_shake128/4096/64                5102 ns         5102 ns        29551   22.0137k      5.29176     110.979k             5.04136      777.603Mi/s
bench_shake128/4096/64                5113 ns         5112 ns        29551   21.9809k      5.28387     110.979k             5.04889      776.009Mi/s
bench_shake128/4096/64                5116 ns         5116 ns        29551   22.0537k      5.30137     110.979k             5.03222      775.523Mi/s
bench_shake128/4096/64                5086 ns         5086 ns        29551   21.9572k      5.27816     110.979k             5.05434      780.069Mi/s
bench_shake128/4096/64_mean           5012 ns         5012 ns            8   21.9731k        5.282     110.979k             5.05071      792.179Mi/s
bench_shake128/4096/64_median         5072 ns         5072 ns            8   21.9894k      5.28591     110.979k             5.04693      782.245Mi/s
bench_shake128/4096/64_stddev          146 ns          146 ns            8    63.6387    0.0152978            0           0.0146683      24.0031Mi/s
bench_shake128/4096/64_cv             2.91 %          2.91 %             8      0.29%        0.29%        0.00%               0.29%            3.03%
bench_sha3_224/256                     422 ns          422 ns       332035   1.85956k      6.54775       9.064k             4.87427      642.086Mi/s
bench_sha3_224/256                     423 ns          423 ns       332035    1.8595k      6.54755       9.064k             4.87442      640.507Mi/s
bench_sha3_224/256                     422 ns          422 ns       332035   1.85858k      6.54428       9.064k             4.87685      641.355Mi/s
bench_sha3_224/256                     423 ns          423 ns       332035   1.86226k      6.55725       9.064k             4.86721      640.063Mi/s
bench_sha3_224/256                     428 ns          428 ns       332035   1.86892k       6.5807       9.064k             4.84986      633.501Mi/s
bench_sha3_224/256                     425 ns          425 ns       332035   1.86158k      6.55484       9.064k             4.86899      637.358Mi/s
bench_sha3_224/256                     428 ns          428 ns       332035   1.86786k      6.57697       9.064k             4.85261      632.422Mi/s
bench_sha3_224/256                     427 ns          427 ns       332035   1.86528k      6.56788       9.064k             4.85933      634.377Mi/s
bench_sha3_224/256_mean                425 ns          425 ns            8   1.86294k      6.55965       9.064k             4.86544      637.708Mi/s
bench_sha3_224/256_median              424 ns          424 ns            8   1.86192k      6.55605       9.064k              4.8681      638.711Mi/s
bench_sha3_224/256_stddev             2.55 ns         2.55 ns            8    3.96402    0.0139578     130.499u           0.0103438      3.83054Mi/s
bench_sha3_224/256_cv                 0.60 %          0.60 %             8      0.21%        0.21%        0.00%               0.21%            0.60%
bench_sha3_224/128                     210 ns          210 ns       674799    937.821      6.01167       4.603k             4.90819      707.424Mi/s
bench_sha3_224/128                     214 ns          214 ns       674799    937.816      6.01164       4.603k             4.90821      695.654Mi/s
bench_sha3_224/128                     215 ns          215 ns       674799    939.455      6.02215       4.603k             4.89965      690.673Mi/s
bench_sha3_224/128                     218 ns          218 ns       674799    940.568      6.02928       4.603k             4.89385      681.134Mi/s
bench_sha3_224/128                     219 ns          219 ns       674799    942.647      6.04261       4.603k             4.88306      680.186Mi/s
bench_sha3_224/128                     217 ns          217 ns       674799    941.302      6.03398       4.603k             4.89004      684.673Mi/s
bench_sha3_224/128                     219 ns          219 ns       674799    944.076      6.05177       4.603k             4.87567      680.585Mi/s
bench_sha3_224/128                     218 ns          218 ns       674799    944.418      6.05396       4.603k              4.8739      683.476Mi/s
bench_sha3_224/128_mean                216 ns          216 ns            8    941.013      6.03213       4.603k             4.89157      687.976Mi/s
bench_sha3_224/128_median              218 ns          217 ns            8    940.935      6.03163       4.603k             4.89195      684.074Mi/s
bench_sha3_224/128_stddev             2.96 ns         2.96 ns            8     2.5848    0.0165692            0           0.0134353      9.54487Mi/s
bench_sha3_224/128_cv                 1.37 %          1.37 %             8      0.27%        0.27%        0.00%               0.27%            1.39%
bench_sha3_256/256                     402 ns          402 ns       352978   1.86564k      6.47793       9.077k             4.86535      683.439Mi/s
bench_sha3_256/256                     434 ns          434 ns       352978    1.8743k      6.50799       9.077k             4.84287      633.523Mi/s
bench_sha3_256/256                     432 ns          432 ns       352978   1.86416k      6.47277       9.077k             4.86923      636.302Mi/s
bench_sha3_256/256                     428 ns          428 ns       352978   1.85298k      6.43394       9.077k             4.89861        641.9Mi/s
bench_sha3_256/256                     430 ns          430 ns       352978   1.86658k      6.48118       9.077k              4.8629      638.874Mi/s
bench_sha3_256/256                     434 ns          434 ns       352978   1.87159k      6.49856       9.077k              4.8499      633.214Mi/s
bench_sha3_256/256                     427 ns          427 ns       352978   1.86235k      6.46649       9.077k             4.87395      642.894Mi/s
bench_sha3_256/256                     429 ns          429 ns       352978   1.85637k      6.44572       9.077k             4.88966      640.125Mi/s
bench_sha3_256/256_mean                427 ns          427 ns            8   1.86425k      6.47307       9.077k             4.86906      643.784Mi/s
bench_sha3_256/256_median              430 ns          429 ns            8    1.8649k      6.47535       9.077k             4.86729        639.5Mi/s
bench_sha3_256/256_stddev             10.4 ns         10.4 ns            8    7.12221    0.0247299            0           0.0186163        16.42Mi/s
bench_sha3_256/256_cv                 2.43 %          2.43 %             8      0.38%        0.38%        0.00%               0.38%            2.55%
bench_shake256/64/64                   213 ns          213 ns       669107    955.861      7.46766       4.645k             4.85949      572.467Mi/s
bench_shake256/64/64                   216 ns          216 ns       669107    953.772      7.45135       4.645k             4.87014      564.121Mi/s
bench_shake256/64/64                   217 ns          217 ns       669107    948.437      7.40966       4.645k             4.89753      562.617Mi/s
bench_shake256/64/64                   220 ns          220 ns       669107     951.31      7.43211       4.645k             4.88274      555.779Mi/s
bench_shake256/64/64                   220 ns          220 ns       669107    952.409      7.44069       4.645k             4.87711      554.359Mi/s
bench_shake256/64/64                   220 ns          220 ns       669107    951.399       7.4328       4.645k             4.88229      554.367Mi/s
bench_shake256/64/64                   219 ns          219 ns       669107    952.406      7.44067       4.645k             4.87712      556.247Mi/s
bench_shake256/64/64                   219 ns          219 ns       669107    951.467      7.43333       4.645k             4.88194      558.435Mi/s
bench_shake256/64/64_mean              218 ns          218 ns            8    952.132      7.43854       4.645k             4.87854      559.799Mi/s
bench_shake256/64/64_median            219 ns          219 ns            8    951.936        7.437       4.645k             4.87953      557.341Mi/s
bench_shake256/64/64_stddev           2.42 ns         2.42 ns            8    2.14004    0.0167191            0           0.0109637      6.28883Mi/s
bench_shake256/64/64_cv               1.11 %          1.11 %             8      0.22%        0.22%        0.00%               0.22%            1.12%
bench_shake128/256/32                  405 ns          405 ns       346684   1.86938k      6.49089       9.121k             4.87917       678.81Mi/s
bench_shake128/256/32                  405 ns          405 ns       346684    1.8738k      6.50627       9.121k             4.86764      678.618Mi/s
bench_shake128/256/32                  419 ns          419 ns       346684   1.88111k      6.53163       9.121k             4.84874      656.208Mi/s
bench_shake128/256/32                  431 ns          431 ns       346684   1.88218k      6.53535       9.121k             4.84598      636.599Mi/s
bench_shake128/256/32                  434 ns          434 ns       346684   1.88157k      6.53321       9.121k             4.84756      633.437Mi/s
bench_shake128/256/32                  435 ns          435 ns       346684   1.87475k      6.50955       9.121k             4.86518      631.259Mi/s
bench_shake128/256/32                  436 ns          435 ns       346684   1.87916k      6.52487       9.121k             4.85376      630.674Mi/s
bench_shake128/256/32                  434 ns          434 ns       346684   1.88994k      6.56228       9.121k             4.82609      632.854Mi/s
bench_shake128/256/32_mean             425 ns          425 ns            8   1.87899k      6.52426       9.121k             4.85426      647.307Mi/s
bench_shake128/256/32_median           433 ns          433 ns            8   1.88014k      6.52825       9.121k             4.85125      635.018Mi/s
bench_shake128/256/32_stddev          13.5 ns         13.5 ns            8    6.30647    0.0218975            0           0.0162844      21.0517Mi/s
bench_shake128/256/32_cv              3.18 %          3.18 %             8      0.34%        0.34%        0.00%               0.34%            3.25%
bench_shake256/2048/32                2999 ns         2998 ns        47008   14.0054k      6.73337      71.045k             5.07268       661.56Mi/s
bench_shake256/2048/32                3150 ns         3150 ns        47008   14.0155k      6.73823      71.045k             5.06902      629.748Mi/s
bench_shake256/2048/32                3239 ns         3239 ns        47008   14.0135k      6.73726      71.045k             5.06975      612.394Mi/s
bench_shake256/2048/32                3238 ns         3238 ns        47008   14.0014k      6.73144      71.045k             5.07414      612.602Mi/s
bench_shake256/2048/32                3229 ns         3229 ns        47008   13.9749k      6.71869      71.045k             5.08377      614.334Mi/s
bench_shake256/2048/32                3244 ns         3244 ns        47008   13.9849k      6.72353      71.045k             5.08011      611.458Mi/s
bench_shake256/2048/32                3242 ns         3241 ns        47008   14.0081k      6.73464      71.045k             5.07173      611.973Mi/s
bench_shake256/2048/32                3250 ns         3250 ns        47008   14.0252k       6.7429      71.045k             5.06551      610.407Mi/s
bench_shake256/2048/32_mean           3199 ns         3199 ns            8   14.0036k      6.73251      71.045k             5.07334      620.559Mi/s
bench_shake256/2048/32_median         3239 ns         3239 ns            8   14.0067k        6.734      71.045k             5.07221      612.498Mi/s
bench_shake256/2048/32_stddev         87.1 ns         87.1 ns            8    16.5182     7.94145m            0            5.98826m      17.7034Mi/s
bench_shake256/2048/32_cv             2.72 %          2.72 %             8      0.12%        0.12%        0.00%               0.12%            2.85%
bench_sha3_224/64                      209 ns          209 ns       663777    947.306      10.2968       4.593k             4.84849      420.333Mi/s
bench_sha3_224/64                      213 ns          212 ns       663777    943.104      10.2511       4.593k             4.87009      412.903Mi/s
bench_sha3_224/64                      216 ns          216 ns       663777    947.652      10.3006       4.593k             4.84672      407.034Mi/s
bench_sha3_224/64                      215 ns          215 ns       663777    939.199      10.2087       4.593k             4.89034      408.266Mi/s
bench_sha3_224/64                      215 ns          215 ns       663777    940.885       10.227       4.593k             4.88158      407.748Mi/s
bench_sha3_224/64                      218 ns          218 ns       663777    940.778      10.2259       4.593k             4.88213      402.054Mi/s
bench_sha3_224/64                      220 ns          220 ns       663777    948.578      10.3106       4.593k             4.84198      398.021Mi/s
bench_sha3_224/64                      217 ns          217 ns       663777    947.156      10.2952       4.593k             4.84925      404.091Mi/s
bench_sha3_224/64_mean                 215 ns          215 ns            8    944.332      10.2645       4.593k             4.86382      407.556Mi/s
bench_sha3_224/64_median               215 ns          215 ns            8     945.13      10.2732       4.593k             4.85967      407.391Mi/s
bench_sha3_224/64_stddev              3.58 ns         3.58 ns            8    3.74621    0.0407197            0           0.0193078      6.82278Mi/s
bench_sha3_224/64_cv                  1.66 %          1.66 %             8      0.40%        0.40%        0.00%               0.40%            1.67%
bench_shake256/512/64                  822 ns          822 ns       169724   3.59362k      6.23893      17.966k             4.99941      668.525Mi/s
bench_shake256/512/64                  825 ns          825 ns       169724   3.58855k      6.23012      17.966k             5.00648      666.129Mi/s
bench_shake256/512/64                  825 ns          825 ns       169724   3.59077k      6.23397      17.966k             5.00339      666.007Mi/s
bench_shake256/512/64                  833 ns          833 ns       169724   3.58682k      6.22712      17.966k              5.0089       659.11Mi/s
bench_shake256/512/64                  837 ns          837 ns       169724   3.59941k      6.24898      17.966k             4.99137      656.118Mi/s
bench_shake256/512/64                  835 ns          835 ns       169724   3.60445k      6.25773      17.966k             4.98439      657.998Mi/s
bench_shake256/512/64                  834 ns          833 ns       169724   3.59424k      6.24001      17.966k             4.99855      659.085Mi/s
bench_shake256/512/64                  828 ns          828 ns       169724   3.58133k      6.21759      17.966k             5.01657      663.176Mi/s
bench_shake256/512/64_mean             830 ns          830 ns            8    3.5924k      6.23681      17.966k             5.00113      662.018Mi/s
bench_shake256/512/64_median           831 ns          831 ns            8    3.5922k      6.23645      17.966k              5.0014      661.143Mi/s
bench_shake256/512/64_stddev          5.72 ns         5.69 ns            8     7.2738    0.0126281            0           0.0101223      4.54366Mi/s
bench_shake256/512/64_cv              0.69 %          0.69 %             8      0.20%        0.20%        0.00%               0.20%            0.69%
bench_sha3_384/128                     393 ns          393 ns       353608    1.8252k      10.3704       9.096k             4.98357      427.546Mi/s
bench_sha3_384/128                     410 ns          410 ns       353608   1.82475k      10.3679       9.096k             4.98479      409.318Mi/s
bench_sha3_384/128                     414 ns          414 ns       353608     1.825k      10.3693       9.096k              4.9841      405.502Mi/s
bench_sha3_384/128                     415 ns          415 ns       353608   1.82411k      10.3643       9.096k             4.98654      404.063Mi/s
bench_sha3_384/128                     424 ns          424 ns       353608   1.82846k       10.389       9.096k             4.97468      396.126Mi/s
bench_sha3_384/128                     423 ns          423 ns       353608   1.82482k      10.3683       9.096k              4.9846       396.92Mi/s
bench_sha3_384/128                     420 ns          420 ns       353608   1.82654k       10.378       9.096k             4.97992      400.094Mi/s
bench_sha3_384/128                     422 ns          422 ns       353608   1.82705k      10.3809       9.096k             4.97853      397.767Mi/s
bench_sha3_384/128_mean                415 ns          415 ns            8   1.82574k      10.3735       9.096k             4.98209      404.667Mi/s
bench_sha3_384/128_median              417 ns          417 ns            8    1.8251k      10.3699       9.096k             4.98384      402.079Mi/s
bench_sha3_384/128_stddev             10.3 ns         10.2 ns            8    1.46619     8.33064m            0             3.9986m      10.3345Mi/s
bench_sha3_384/128_cv                 2.47 %          2.47 %             8      0.08%        0.08%        0.00%               0.08%            2.55%
bench_shake128/64/64                   210 ns          210 ns       676173    961.595      7.51246       4.642k              4.8274      582.314Mi/s
bench_shake128/64/64                   214 ns          214 ns       676173    964.244      7.53316       4.642k             4.81414      569.311Mi/s
bench_shake128/64/64                   218 ns          218 ns       676173     965.45      7.54258       4.642k             4.80812      560.166Mi/s
bench_shake128/64/64                   219 ns          219 ns       676173    962.621      7.52047       4.642k             4.82225       557.05Mi/s
bench_shake128/64/64                   223 ns          223 ns       676173    961.398      7.51092       4.642k             4.82839      547.664Mi/s
bench_shake128/64/64                   222 ns          222 ns       676173    961.081      7.50845       4.642k             4.82998      550.269Mi/s
bench_shake128/64/64                   223 ns          223 ns       676173    964.478      7.53499       4.642k             4.81297       547.52Mi/s
bench_shake128/64/64                   222 ns          222 ns       676173    966.753      7.55276       4.642k             4.80164      549.662Mi/s
bench_shake128/64/64_mean              219 ns          219 ns            8    963.452      7.52697       4.642k             4.81811      557.995Mi/s
bench_shake128/64/64_median            220 ns          220 ns            8    963.432      7.52681       4.642k             4.81819       553.66Mi/s
bench_shake128/64/64_stddev           4.75 ns         4.75 ns            8    2.08904    0.0163206     65.2493u           0.0104412      12.3674Mi/s
bench_shake128/64/64_cv               2.17 %          2.17 %             8      0.22%        0.22%        0.00%               0.22%            2.22%
bench_shake256/64/32                   203 ns          203 ns       691754    949.705      9.89276       4.645k             4.89099      450.711Mi/s
bench_shake256/64/32                   216 ns          216 ns       691754    950.038      9.89623       4.645k             4.88928      423.194Mi/s
bench_shake256/64/32                   217 ns          217 ns       691754    955.696      9.95516       4.645k             4.86033      421.101Mi/s
bench_shake256/64/32                   216 ns          216 ns       691754    953.206      9.92923       4.645k             4.87303      424.212Mi/s
bench_shake256/64/32                   220 ns          220 ns       691754    953.281      9.93001       4.645k             4.87265      415.294Mi/s
bench_shake256/64/32                   221 ns          221 ns       691754    954.189      9.93947       4.645k             4.86801      414.364Mi/s
bench_shake256/64/32                   218 ns          218 ns       691754    951.832      9.91492       4.645k             4.88006      419.265Mi/s
bench_shake256/64/32                   220 ns          220 ns       691754     955.09      9.94885       4.645k             4.86342      416.098Mi/s
bench_shake256/64/32_mean              217 ns          217 ns            8     952.88      9.92583       4.645k             4.87472       423.03Mi/s
bench_shake256/64/32_median            218 ns          218 ns            8    953.244      9.92962       4.645k             4.87284      420.183Mi/s
bench_shake256/64/32_stddev           5.75 ns         5.75 ns            8    2.20539    0.0229729            0           0.0112895      11.7627Mi/s
bench_shake256/64/32_cv               2.66 %          2.65 %             8      0.23%        0.23%        0.00%               0.23%            2.78%
bench_shake128/128/32                  214 ns          214 ns       660409    959.267      5.99542       4.652k             4.84954      714.645Mi/s
bench_shake128/128/32                  214 ns          214 ns       660409    961.207      6.00754       4.652k             4.83975      712.025Mi/s
bench_shake128/128/32                  219 ns          219 ns       660409    962.427      6.01517       4.652k             4.83361       696.28Mi/s
bench_shake128/128/32                  221 ns          221 ns       660409    961.745      6.01091       4.652k             4.83704      689.151Mi/s
bench_shake128/128/32                  219 ns          219 ns       660409    956.959      5.98099       4.652k             4.86123      696.428Mi/s
bench_shake128/128/32                  221 ns          221 ns       660409    957.771      5.98607       4.652k             4.85711      689.066Mi/s
bench_shake128/128/32                  223 ns          223 ns       660409    961.461      6.00913       4.652k             4.83847      684.874Mi/s
bench_shake128/128/32                  223 ns          222 ns       660409    962.463       6.0154       4.652k             4.83343      685.795Mi/s
bench_shake128/128/32_mean             219 ns          219 ns            8    960.413      6.00258       4.652k             4.84377      696.033Mi/s
bench_shake128/128/32_median           220 ns          220 ns            8    961.334      6.00834       4.652k             4.83911      692.715Mi/s
bench_shake128/128/32_stddev          3.59 ns         3.59 ns            8    2.13779    0.0133612     65.2493u            0.010796      11.5115Mi/s
bench_shake128/128/32_cv              1.64 %          1.64 %             8      0.22%        0.22%        0.00%               0.22%            1.65%
bench_shake256/32/64                   209 ns          209 ns       678889    947.401      9.86876       4.645k             4.90289      439.088Mi/s
bench_shake256/32/64                   210 ns          210 ns       678889    948.256      9.87766       4.645k             4.89847      435.821Mi/s
bench_shake256/32/64                   216 ns          216 ns       678889    952.231      9.91908       4.645k             4.87802      423.752Mi/s
bench_shake256/32/64                   217 ns          217 ns       678889    949.305      9.88859       4.645k             4.89305      422.504Mi/s
bench_shake256/32/64                   219 ns          219 ns       678889     946.97      9.86427       4.645k             4.90512      418.967Mi/s
bench_shake256/32/64                   220 ns          220 ns       678889    949.658      9.89228       4.645k             4.89123      416.887Mi/s
bench_shake256/32/64                   219 ns          219 ns       678889    949.355      9.88911       4.645k              4.8928      418.462Mi/s
bench_shake256/32/64                   219 ns          219 ns       678889    948.691       9.8822       4.645k             4.89622      417.489Mi/s
bench_shake256/32/64_mean              216 ns          216 ns            8    948.983      9.88524       4.645k             4.89472      424.121Mi/s
bench_shake256/32/64_median            218 ns          218 ns            8    948.998      9.88539       4.645k             4.89464      420.735Mi/s
bench_shake256/32/64_stddev           4.30 ns         4.31 ns            8    1.62216    0.0168975            0            8.35636m      8.60692Mi/s
bench_shake256/32/64_cv               1.99 %          1.99 %             8      0.17%        0.17%        0.00%               0.17%            2.03%
bench_sha3_224/512                     764 ns          764 ns       181541   3.57001k      6.61113      17.919k             5.01932      674.062Mi/s
bench_sha3_224/512                     773 ns          773 ns       181541   3.59054k      6.64914      17.919k             4.99062      666.522Mi/s
bench_sha3_224/512                     823 ns          823 ns       181541   3.57353k      6.61766      17.919k             5.01436      625.748Mi/s
bench_sha3_224/512                     829 ns          829 ns       181541   3.57371k      6.61798      17.919k             5.01412      621.493Mi/s
bench_sha3_224/512                     831 ns          831 ns       181541   3.58342k      6.63596      17.919k             5.00053      619.852Mi/s
bench_sha3_224/512                     826 ns          825 ns       181541   3.57959k      6.62888      17.919k             5.00587      623.858Mi/s
bench_sha3_224/512                     831 ns          831 ns       181541   3.58559k      6.63999      17.919k              4.9975      619.434Mi/s
bench_sha3_224/512                     829 ns          829 ns       181541    3.5769k      6.62389      17.919k             5.00965      621.554Mi/s
bench_sha3_224/512_mean                813 ns          813 ns            8   3.57916k      6.62808      17.919k              5.0065      634.065Mi/s
bench_sha3_224/512_median              827 ns          827 ns            8   3.57825k      6.62638      17.919k             5.00776      622.706Mi/s
bench_sha3_224/512_stddev             27.9 ns         27.8 ns            8    6.96345    0.0128953     260.997u            9.73487m      22.5427Mi/s
bench_sha3_224/512_cv                 3.43 %          3.42 %             8      0.19%        0.19%        0.00%               0.19%            3.56%
bench_sha3_256/1024                   1566 ns         1566 ns        91679   7.06701k      6.69225      35.624k             5.04089       642.99Mi/s
bench_sha3_256/1024                   1629 ns         1629 ns        91679   7.07514k      6.69995      35.624k             5.03509      618.084Mi/s
bench_sha3_256/1024                   1627 ns         1627 ns        91679   7.05666k      6.68244      35.624k             5.04828      618.852Mi/s
bench_sha3_256/1024                   1639 ns         1638 ns        91679   7.06128k      6.68682      35.624k             5.04498      614.656Mi/s
bench_sha3_256/1024                   1643 ns         1643 ns        91679     7.053k      6.67897      35.624k              5.0509      612.979Mi/s
bench_sha3_256/1024                   1638 ns         1638 ns        91679   7.06173k      6.68725      35.624k             5.04465      614.669Mi/s
bench_sha3_256/1024                   1647 ns         1647 ns        91679   7.07764k      6.70232      35.624k             5.03331      611.454Mi/s
bench_sha3_256/1024                   1645 ns         1644 ns        91679   7.07669k      6.70141      35.624k             5.03399      612.393Mi/s
bench_sha3_256/1024_mean              1629 ns         1629 ns            8   7.06615k      6.69143      35.624k             5.04151       618.26Mi/s
bench_sha3_256/1024_median            1639 ns         1638 ns            8   7.06437k      6.68975      35.624k             5.04277      614.663Mi/s
bench_sha3_256/1024_stddev            26.4 ns         26.4 ns            8    9.49519     8.99166m     521.995u            6.77443m      10.3265Mi/s
bench_sha3_256/1024_cv                1.62 %          1.62 %             8      0.13%        0.13%        0.00%               0.13%            1.67%
bench_sha3_384/32                      214 ns          214 ns       662630    936.572      11.7071       4.648k             4.96278      356.248Mi/s
bench_sha3_384/32                      215 ns          215 ns       662630    938.448      11.7306       4.648k             4.95286      355.494Mi/s
bench_sha3_384/32                      214 ns          214 ns       662630    934.927      11.6866       4.648k             4.97151      357.296Mi/s
bench_sha3_384/32                      218 ns          218 ns       662630    935.142      11.6893       4.648k             4.97037      350.146Mi/s
bench_sha3_384/32                      217 ns          217 ns       662630    938.356      11.7294       4.648k             4.95335      351.613Mi/s
bench_sha3_384/32                      217 ns          217 ns       662630    935.776      11.6972       4.648k               4.967      350.854Mi/s
bench_sha3_384/32                      219 ns          219 ns       662630      947.9      11.8488       4.648k             4.90347      347.838Mi/s
bench_sha3_384/32                      219 ns          219 ns       662630    948.466      11.8558       4.648k             4.90054      348.011Mi/s
bench_sha3_384/32_mean                 217 ns          217 ns            8    939.448      11.7431       4.648k             4.94774      352.187Mi/s
bench_sha3_384/32_median               217 ns          217 ns            8    937.464      11.7183       4.648k             4.95806      351.233Mi/s
bench_sha3_384/32_stddev              2.28 ns         2.28 ns            8    5.55111    0.0693888     65.2493u           0.0290797      3.70632Mi/s
bench_sha3_384/32_cv                  1.05 %          1.05 %             8      0.59%        0.59%        0.00%               0.59%            1.05%
bench_shake256/128/32                  202 ns          202 ns       700355    944.377      5.90236       4.655k             4.92918      755.899Mi/s
bench_shake256/128/32                  204 ns          204 ns       700355    943.061      5.89413       4.655k             4.93605      749.169Mi/s
bench_shake256/128/32                  214 ns          214 ns       700355     942.25      5.88906       4.655k              4.9403      713.885Mi/s
bench_shake256/128/32                  215 ns          215 ns       700355    946.433      5.91521       4.655k             4.91847      709.433Mi/s
bench_shake256/128/32                  215 ns          214 ns       700355    939.146      5.86966       4.655k             4.95663      711.402Mi/s
bench_shake256/128/32                  218 ns          218 ns       700355    944.277      5.90173       4.655k              4.9297      699.497Mi/s
bench_shake256/128/32                  219 ns          219 ns       700355    946.009      5.91256       4.655k             4.92067      696.986Mi/s
bench_shake256/128/32                  219 ns          219 ns       700355    950.223      5.93889       4.655k             4.89885      695.453Mi/s
bench_shake256/128/32_mean             213 ns          213 ns            8    944.472      5.90295       4.655k             4.92873      716.466Mi/s
bench_shake256/128/32_median           215 ns          215 ns            8    944.327      5.90204       4.655k             4.92944      710.418Mi/s
bench_shake256/128/32_stddev          6.76 ns         6.77 ns            8    3.26495     0.020406            0           0.0170299      23.3459Mi/s
bench_shake256/128/32_cv              3.17 %          3.17 %             8      0.35%        0.35%        0.00%               0.35%            3.26%
bench_shake256/32/32                   203 ns          203 ns       692424    949.081      14.8294       4.645k             4.89421      300.705Mi/s
bench_shake256/32/32                   204 ns          204 ns       692424    950.677      14.8543       4.645k             4.88599      299.113Mi/s
bench_shake256/32/32                   212 ns          212 ns       692424    950.243      14.8475       4.645k             4.88822      287.525Mi/s
bench_shake256/32/32                   220 ns          220 ns       692424    951.282      14.8638       4.645k             4.88288      277.336Mi/s
bench_shake256/32/32                   220 ns          220 ns       692424    951.365      14.8651       4.645k             4.88246      277.388Mi/s
bench_shake256/32/32                   219 ns          219 ns       692424    949.871      14.8417       4.645k             4.89014      278.798Mi/s
bench_shake256/32/32                   220 ns          220 ns       692424     951.93      14.8739       4.645k             4.87956      277.005Mi/s
bench_shake256/32/32                   220 ns          220 ns       692424    961.073      15.0168       4.645k             4.83314      277.626Mi/s
bench_shake256/32/32_mean              215 ns          215 ns            8     951.94      14.8741       4.645k             4.87958      284.437Mi/s
bench_shake256/32/32_median            219 ns          219 ns            8     950.98      14.8591       4.645k             4.88444      278.212Mi/s
bench_shake256/32/32_stddev           7.46 ns         7.47 ns            8    3.80023    0.0593786            0           0.0193348      10.1637Mi/s
bench_shake256/32/32_cv               3.47 %          3.48 %             8      0.40%        0.40%        0.00%               0.40%            3.57%
bench_sha3_384/4096                   7714 ns         7714 ns        18172   34.4762k      8.31954     177.022k             5.13462      512.347Mi/s
bench_sha3_384/4096                   7888 ns         7887 ns        18172   34.6086k      8.35149     177.022k             5.11498      501.053Mi/s
bench_sha3_384/4096                   7990 ns         7990 ns        18172   34.5932k      8.34777     177.022k             5.11725      494.652Mi/s
bench_sha3_384/4096                   7976 ns         7975 ns        18172   34.5946k      8.34813     177.022k             5.11704      495.555Mi/s
bench_sha3_384/4096                   8057 ns         8056 ns        18172   34.4797k      8.32039     177.022k             5.13409      490.594Mi/s
bench_sha3_384/4096                   8012 ns         8011 ns        18172   34.4825k      8.32108     177.022k             5.13367      493.305Mi/s
bench_sha3_384/4096                   8051 ns         8051 ns        18172   34.4856k      8.32181     177.022k             5.13322      490.891Mi/s
bench_sha3_384/4096                   8049 ns         8049 ns        18172   34.5172k      8.32944     177.022k             5.12851       490.99Mi/s
bench_sha3_384/4096_mean              7967 ns         7967 ns            8   34.5297k      8.33246     177.022k             5.12667      496.173Mi/s
bench_sha3_384/4096_median            8001 ns         8000 ns            8   34.5014k      8.32563     177.022k             5.13087      493.979Mi/s
bench_sha3_384/4096_stddev             116 ns          116 ns            8    58.7438    0.0141756            0            8.71603m      7.38926Mi/s
bench_sha3_384/4096_cv                1.46 %          1.46 %             8      0.17%        0.17%        0.00%               0.17%            1.49%
bench_shake256/256/32                  412 ns          412 ns       339740   1.86214k      6.46578       9.115k              4.8949      666.683Mi/s
bench_shake256/256/32                  421 ns          420 ns       339740   1.86786k      6.48563       9.115k             4.87992      653.194Mi/s
bench_shake256/256/32                  424 ns          424 ns       339740   1.85908k      6.45512       9.115k             4.90298      647.892Mi/s
bench_shake256/256/32                  421 ns          421 ns       339740   1.85995k      6.45814       9.115k             4.90068      651.869Mi/s
bench_shake256/256/32                  424 ns          424 ns       339740   1.85921k      6.45558       9.115k             4.90263      647.905Mi/s
bench_shake256/256/32                  421 ns          421 ns       339740   1.85586k      6.44397       9.115k             4.91146      651.914Mi/s
bench_shake256/256/32                  428 ns          428 ns       339740   1.86595k        6.479       9.115k             4.88491      642.096Mi/s
bench_shake256/256/32                  429 ns          429 ns       339740   1.86589k      6.47878       9.115k             4.88507       639.63Mi/s
bench_shake256/256/32_mean             423 ns          423 ns            8   1.86199k      6.46525       9.115k             4.89532      650.148Mi/s
bench_shake256/256/32_median           423 ns          423 ns            8   1.86104k      6.46196       9.115k             4.89779      649.887Mi/s
bench_shake256/256/32_stddev          5.30 ns         5.30 ns            8    4.19772    0.0145754            0           0.0110343      8.23205Mi/s
bench_shake256/256/32_cv              1.25 %          1.26 %             8      0.23%        0.23%        0.00%               0.23%            1.27%
bench_sha3_256/32                      208 ns          208 ns       686356    946.602      14.7907       4.607k             4.86688      293.439Mi/s
bench_sha3_256/32                      211 ns          211 ns       686356    948.722      14.8238       4.607k             4.85601        289.2Mi/s
bench_sha3_256/32                      215 ns          215 ns       686356    946.466      14.7885       4.607k             4.86758      283.624Mi/s
bench_sha3_256/32                      220 ns          220 ns       686356    948.207      14.8157       4.607k             4.85864      277.731Mi/s
bench_sha3_256/32                      217 ns          217 ns       686356    947.141      14.7991       4.607k             4.86411      280.902Mi/s
bench_sha3_256/32                      216 ns          216 ns       686356    944.833       14.763       4.607k             4.87599      282.549Mi/s
bench_sha3_256/32                      217 ns          217 ns       686356    945.214       14.769       4.607k             4.87403      281.367Mi/s
bench_sha3_256/32                      218 ns          218 ns       686356    946.053      14.7821       4.607k             4.86971      279.937Mi/s
bench_sha3_256/32_mean                 215 ns          215 ns            8    946.655      14.7915       4.607k             4.86662      283.594Mi/s
bench_sha3_256/32_median               216 ns          216 ns            8    946.534      14.7896       4.607k             4.86723      281.958Mi/s
bench_sha3_256/32_stddev              3.89 ns         3.89 ns            8    1.34765     0.021057            0            6.92612m      5.20152Mi/s
bench_sha3_256/32_cv                  1.81 %          1.81 %             8      0.14%        0.14%        0.00%               0.14%            1.83%
bench_sha3_512/64                      198 ns          198 ns       709153    925.817      7.23294       4.685k              5.0604      615.559Mi/s
bench_sha3_512/64                      212 ns          212 ns       709153    926.894      7.24136       4.685k             5.05452      575.091Mi/s
bench_sha3_512/64                      214 ns          214 ns       709153    930.513      7.26963       4.685k             5.03486      569.911Mi/s
bench_sha3_512/64                      215 ns          215 ns       709153    928.004      7.25003       4.685k             5.04847      566.704Mi/s
bench_sha3_512/64                      216 ns          216 ns       709153    927.864      7.24894       4.685k             5.04923      565.461Mi/s
bench_sha3_512/64                      214 ns          214 ns       709153    925.917      7.23373       4.685k             5.05985      570.465Mi/s
bench_sha3_512/64                      214 ns          214 ns       709153    930.704      7.27112       4.685k             5.03383       569.65Mi/s
bench_sha3_512/64                      215 ns          215 ns       709153    927.937      7.24951       4.685k             5.04883      566.953Mi/s
bench_sha3_512/64_mean                 212 ns          212 ns            8    927.956      7.24966       4.685k             5.04875      574.974Mi/s
bench_sha3_512/64_median               214 ns          214 ns            8      927.9      7.24922       4.685k             5.04903      569.781Mi/s
bench_sha3_512/64_stddev              5.83 ns         5.82 ns            8     1.8501    0.0144539     92.2765u           0.0100578      16.6679Mi/s
bench_sha3_512/64_cv                  2.74 %          2.74 %             8      0.20%        0.20%        0.00%               0.20%            2.90%
bench_shake256/512/32                  771 ns          771 ns       182926    3.6066k      6.62977      17.966k             4.98143       673.24Mi/s
bench_shake256/512/32                  815 ns          815 ns       182926   3.59276k      6.60434      17.966k             5.00061      636.943Mi/s
bench_shake256/512/32                  822 ns          822 ns       182926   3.59408k      6.60677      17.966k             4.99877      630.979Mi/s
bench_shake256/512/32                  836 ns          836 ns       182926   3.59471k      6.60792      17.966k              4.9979      620.822Mi/s
bench_shake256/512/32                  834 ns          834 ns       182926   3.60651k      6.62962      17.966k             4.98155       622.28Mi/s
bench_shake256/512/32                  835 ns          835 ns       182926   3.59366k        6.606      17.966k             4.99935      621.443Mi/s
bench_shake256/512/32                  834 ns          834 ns       182926   3.59451k      6.60756      17.966k             4.99817      621.988Mi/s
bench_shake256/512/32                  831 ns          831 ns       182926   3.59371k      6.60609      17.966k             4.99929      624.473Mi/s
bench_shake256/512/32_mean             822 ns          822 ns            8   3.59707k      6.61226      17.966k             4.99463      631.521Mi/s
bench_shake256/512/32_median           832 ns          832 ns            8    3.5943k      6.60717      17.966k             4.99847      623.376Mi/s
bench_shake256/512/32_stddev          22.1 ns         22.1 ns            8     5.8839     0.010816            0            8.15601m      17.7771Mi/s
bench_shake256/512/32_cv              2.69 %          2.68 %             8      0.16%        0.16%        0.00%               0.16%            2.81%
bench_shake128/2048/64                2511 ns         2511 ns        55176   11.5092k      5.44941      57.831k             5.02478      802.174Mi/s
bench_shake128/2048/64                2635 ns         2635 ns        55176   11.5131k      5.45125      57.831k             5.02308      764.484Mi/s
bench_shake128/2048/64                2629 ns         2628 ns        55176   11.4466k      5.41978      57.831k             5.05225      766.303Mi/s
bench_shake128/2048/64                2653 ns         2653 ns        55176   11.5111k      5.45034      57.831k             5.02392      759.217Mi/s
bench_shake128/2048/64                2657 ns         2657 ns        55176   11.5072k      5.44849      57.831k             5.02563      758.117Mi/s
bench_shake128/2048/64                2655 ns         2655 ns        55176   11.4551k      5.42382      57.831k             5.04849      758.697Mi/s
bench_shake128/2048/64                2674 ns         2674 ns        55176    11.504k      5.44699      57.831k             5.02702      753.343Mi/s
bench_shake128/2048/64                2682 ns         2682 ns        55176   11.5646k      5.47566      57.831k             5.00069      750.888Mi/s
bench_shake128/2048/64_mean           2637 ns         2637 ns            8   11.5014k      5.44572      57.831k             5.02823      764.153Mi/s
bench_shake128/2048/64_median         2654 ns         2654 ns            8   11.5082k      5.44895      57.831k             5.02521      758.957Mi/s
bench_shake128/2048/64_stddev         53.9 ns         53.9 ns            8    36.7885    0.0174188     1.04399m           0.0160848      16.1852Mi/s
bench_shake128/2048/64_cv             2.04 %          2.04 %             8      0.32%        0.32%        0.00%               0.32%            2.12%
```

### On 12th Gen Intel(R) Core(TM) i7-1260P ( compiled with GCC-13.1.0 )

```bash
2023-09-16T13:22:54+04:00
Running ./build/perfs/perf.out
Run on (16 X 2336.62 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 0.70, 0.57, 0.53
----------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations     CYCLES CYCLES/ BYTE INSTRUCTIONS INSTRUCTIONS/ CYCLE bytes_per_second
----------------------------------------------------------------------------------------------------------------------------------------------------
bench_sha3_384/2048                   4296 ns         4295 ns        31993   19.6736k      9.38624     114.911k             5.84089      465.365Mi/s
bench_sha3_384/2048                   4368 ns         4367 ns        31993   19.7107k      9.40397     114.911k             5.82987      457.694Mi/s
bench_sha3_384/2048                   4466 ns         4465 ns        31993   19.7266k      9.41153     114.911k             5.82519      447.656Mi/s
bench_sha3_384/2048                   5127 ns         5126 ns        31993   20.0429k      9.56245     114.911k             5.73326      389.916Mi/s
bench_sha3_384/2048                   4565 ns         4565 ns        31993   19.7823k      9.43814     114.911k             5.80877      437.903Mi/s
bench_sha3_384/2048                   4597 ns         4596 ns        31993   19.7472k      9.42137     114.911k             5.81911      434.925Mi/s
bench_sha3_384/2048                   4565 ns         4564 ns        31993   19.7326k       9.4144     114.911k             5.82341      437.945Mi/s
bench_sha3_384/2048                   4580 ns         4579 ns        31993   19.7842k        9.439     114.911k             5.80824      436.517Mi/s
bench_sha3_384/2048_mean              4570 ns         4570 ns            8    19.775k      9.43464     114.911k             5.81109       438.49Mi/s
bench_sha3_384/2048_median            4565 ns         4564 ns            8   19.7399k      9.41789     114.911k             5.82126      437.924Mi/s
bench_sha3_384/2048_stddev             250 ns          250 ns            8    114.169    0.0544698            0           0.0332248      22.5254Mi/s
bench_sha3_384/2048_cv                5.47 %          5.47 %             8      0.58%        0.58%        0.00%               0.57%            5.14%
bench_sha3_384/64                      229 ns          229 ns       610371   1.01663k      9.07709        5.81k             5.71494      467.043Mi/s
bench_sha3_384/64                      231 ns          231 ns       610371    1.0125k      9.04022        5.81k             5.73825      462.354Mi/s
bench_sha3_384/64                      231 ns          231 ns       610371   1.01962k      9.10378        5.81k             5.69818      461.932Mi/s
bench_sha3_384/64                      239 ns          239 ns       610371   1.01081k      9.02513        5.81k             5.74784      447.699Mi/s
bench_sha3_384/64                      234 ns          234 ns       610371   1.01369k      9.05083        5.81k             5.73152      456.459Mi/s
bench_sha3_384/64                      236 ns          236 ns       610371   1.01307k      9.04525        5.81k             5.73505      452.977Mi/s
bench_sha3_384/64                      236 ns          236 ns       610371   1.01545k      9.06651        5.81k             5.72161      453.106Mi/s
bench_sha3_384/64                      235 ns          235 ns       610371   1.01796k      9.08895        5.81k             5.70748      454.934Mi/s
bench_sha3_384/64_mean                 234 ns          234 ns            8   1.01497k      9.06222        5.81k             5.72436      457.063Mi/s
bench_sha3_384/64_median               234 ns          234 ns            8   1.01457k      9.05867        5.81k             5.72656      455.696Mi/s
bench_sha3_384/64_stddev              3.21 ns         3.21 ns            8    2.98085    0.0266147     92.2765u           0.0168021      6.28175Mi/s
bench_sha3_384/64_cv                  1.37 %          1.37 %             8      0.29%        0.29%        0.00%               0.29%            1.37%
bench_sha3_256/32                      249 ns          249 ns       563239   1.14179k      17.8404       5.865k             5.13668      245.461Mi/s
bench_sha3_256/32                      255 ns          255 ns       563239   1.14109k      17.8295       5.865k             5.13984      239.094Mi/s
bench_sha3_256/32                      274 ns          274 ns       563239    949.028      14.8286     4.88121k             5.14338      222.731Mi/s
bench_sha3_256/32                      262 ns          262 ns       563239   1.14035k       17.818       5.865k             5.14314      233.058Mi/s
bench_sha3_256/32                      261 ns          261 ns       563239   1.13992k      17.8112       5.865k             5.14512      233.846Mi/s
bench_sha3_256/32                      262 ns          262 ns       563239   1.13889k      17.7951       5.865k             5.14977      232.557Mi/s
bench_sha3_256/32                      258 ns          258 ns       563239   1.14091k      17.8267       5.865k             5.14065      236.474Mi/s
bench_sha3_256/32                      261 ns          261 ns       563239   1.14045k      17.8195       5.865k             5.14272      233.769Mi/s
bench_sha3_256/32_mean                 260 ns          260 ns            8   1.11655k      17.4461     5.74203k             5.14266      234.624Mi/s
bench_sha3_256/32_median               261 ns          261 ns            8    1.1404k      17.8188       5.865k             5.14293      233.808Mi/s
bench_sha3_256/32_stddev              7.20 ns         7.20 ns            8    67.6949      1.05773      347.821            3.87518m      6.44037Mi/s
bench_sha3_256/32_cv                  2.77 %          2.76 %             8      6.06%        6.06%        6.06%               0.08%            2.74%
bench_sha3_224/128                     249 ns          249 ns       564993    1.1424k       7.3231       5.885k             5.15142      597.661Mi/s
bench_sha3_224/128                     250 ns          250 ns       564993   1.14164k      7.31818       5.885k             5.15489      594.589Mi/s
bench_sha3_224/128                     255 ns          255 ns       564993   1.13853k      7.29827       5.885k             5.16895       583.72Mi/s
bench_sha3_224/128                     258 ns          258 ns       564993   1.14071k      7.31223       5.885k             5.15908      576.008Mi/s
bench_sha3_224/128                     261 ns          261 ns       564993   1.13749k      7.29159       5.885k             5.17368      570.829Mi/s
bench_sha3_224/128                     262 ns          262 ns       564993   1.14146k      7.31706       5.885k             5.15567      567.894Mi/s
bench_sha3_224/128                     264 ns          264 ns       564993   1.13906k      7.30168       5.885k             5.16654      563.509Mi/s
bench_sha3_224/128                     259 ns          259 ns       564993   1.13794k      7.29451       5.885k             5.17161      573.679Mi/s
bench_sha3_224/128_mean                257 ns          257 ns            8    1.1399k      7.30707       5.885k             5.16273      578.486Mi/s
bench_sha3_224/128_median              259 ns          259 ns            8   1.13988k      7.30695       5.885k             5.16281      574.844Mi/s
bench_sha3_224/128_stddev             5.46 ns         5.47 ns            8    1.87448    0.0120159     92.2765u             8.4896m      12.4143Mi/s
bench_sha3_224/128_cv                 2.12 %          2.13 %             8      0.16%        0.16%        0.00%               0.16%            2.15%
bench_shake256/32/64                   255 ns          255 ns       546806   1.15111k      11.9907       5.932k             5.15328      358.663Mi/s
bench_shake256/32/64                   258 ns          258 ns       546806   1.15441k      12.0252       5.932k             5.13853      354.976Mi/s
bench_shake256/32/64                   262 ns          262 ns       546806    1.1524k      12.0042       5.932k             5.14752       349.77Mi/s
bench_shake256/32/64                   264 ns          264 ns       546806   1.15706k      12.0527       5.932k             5.12681      346.894Mi/s
bench_shake256/32/64                   265 ns          265 ns       546806   1.15476k      12.0288       5.932k               5.137      345.313Mi/s
bench_shake256/32/64                   266 ns          266 ns       546806   1.15538k      12.0352       5.932k             5.13426      343.945Mi/s
bench_shake256/32/64                   266 ns          266 ns       546806   1.15573k      12.0389       5.932k             5.13268      344.672Mi/s
bench_shake256/32/64                   260 ns          260 ns       546806   1.14918k      11.9706       5.932k             5.16196        352.1Mi/s
bench_shake256/32/64_mean              262 ns          262 ns            8   1.15375k      12.0183       5.932k              5.1415      349.542Mi/s
bench_shake256/32/64_median            263 ns          263 ns            8   1.15459k       12.027       5.932k             5.13776      348.332Mi/s
bench_shake256/32/64_stddev           3.97 ns         3.97 ns            8    2.63691    0.0274679     130.499u           0.0117652      5.33532Mi/s
bench_shake256/32/64_cv               1.52 %          1.51 %             8      0.23%        0.23%        0.00%               0.23%            1.53%
bench_shake128/4096/64                5726 ns         5725 ns        24322   25.4201k       6.1106     142.914k             5.62208      692.998Mi/s
bench_shake128/4096/64                5797 ns         5797 ns        24322   25.4631k      6.12094     142.914k             5.61259      684.389Mi/s
bench_shake128/4096/64                5860 ns         5860 ns        24322   25.4654k      6.12149     142.914k             5.61209      677.044Mi/s
bench_shake128/4096/64                5871 ns         5870 ns        24322   25.5148k      6.13336     142.914k             5.60122      675.864Mi/s
bench_shake128/4096/64                5889 ns         5888 ns        24322   25.5379k      6.13893     142.914k             5.59615      673.819Mi/s
bench_shake128/4096/64                5902 ns         5902 ns        24322   25.5262k       6.1361     142.914k             5.59872      672.225Mi/s
bench_shake128/4096/64                5927 ns         5927 ns        24322   25.5042k      6.13081     142.914k             5.60355      669.369Mi/s
bench_shake128/4096/64                5815 ns         5815 ns        24322   25.5082k      6.13179     142.914k             5.60266      682.296Mi/s
bench_shake128/4096/64_mean           5848 ns         5848 ns            8   25.4925k        6.128     142.914k             5.60613      678.501Mi/s
bench_shake128/4096/64_median         5866 ns         5865 ns            8   25.5062k       6.1313     142.914k             5.60311      676.454Mi/s
bench_shake128/4096/64_stddev         65.6 ns         65.7 ns            8    39.4962     9.49429m            0            8.69438m      7.68117Mi/s
bench_shake128/4096/64_cv             1.12 %          1.12 %             8      0.15%        0.15%        0.00%               0.16%            1.13%
bench_sha3_512/1024                   3301 ns         3300 ns        42149   14.6532k      13.4681      85.397k             5.82786      314.386Mi/s
bench_sha3_512/1024                   3341 ns         3341 ns        42149   14.6018k      13.4208      85.397k             5.84838      310.601Mi/s
bench_sha3_512/1024                   3346 ns         3346 ns        42149   14.6604k      13.4746      85.397k             5.82502      310.077Mi/s
bench_sha3_512/1024                   3387 ns         3386 ns        42149   14.6577k      13.4722      85.397k             5.82608       306.41Mi/s
bench_sha3_512/1024                   3388 ns         3388 ns        42149   14.5964k      13.4158      85.397k             5.85055      306.264Mi/s
bench_sha3_512/1024                   3396 ns         3396 ns        42149   14.6251k      13.4422      85.397k             5.83908      305.543Mi/s
bench_sha3_512/1024                   3402 ns         3402 ns        42149   14.5739k      13.3951      85.397k             5.85958      305.009Mi/s
bench_sha3_512/1024                   3357 ns         3356 ns        42149   14.6413k      13.4571      85.397k             5.83261       309.15Mi/s
bench_sha3_512/1024_mean              3365 ns         3364 ns            8   14.6262k      13.4432      85.397k             5.83864       308.43Mi/s
bench_sha3_512/1024_median            3372 ns         3371 ns            8   14.6332k      13.4496      85.397k             5.83585       307.78Mi/s
bench_sha3_512/1024_stddev            34.9 ns         34.8 ns            8    32.3765    0.0297578     1.04399m           0.0129353      3.21209Mi/s
bench_sha3_512/1024_cv                1.04 %          1.03 %             8      0.22%        0.22%        0.00%               0.22%            1.04%
bench_shake256/2048/64                3685 ns         3685 ns        38422   16.5489k      7.83566      91.618k             5.53619      546.583Mi/s
bench_shake256/2048/64                3772 ns         3772 ns        38422   16.6051k      7.86228      91.618k             5.51745      534.035Mi/s
bench_shake256/2048/64                3820 ns         3820 ns        38422   16.4827k      7.80429      91.618k             5.55845       527.33Mi/s
bench_shake256/2048/64                3822 ns         3821 ns        38422   16.5059k      7.81532      91.618k             5.55061      527.117Mi/s
bench_shake256/2048/64                3824 ns         3824 ns        38422   16.4973k       7.8112      91.618k             5.55353      526.691Mi/s
bench_shake256/2048/64                3816 ns         3816 ns        38422   16.4702k       7.7984      91.618k             5.56264      527.833Mi/s
bench_shake256/2048/64                3790 ns         3790 ns        38422    16.535k      7.82906      91.618k             5.54086      531.511Mi/s
bench_shake256/2048/64                3770 ns         3769 ns        38422   16.4922k      7.80879      91.618k             5.55524      534.368Mi/s
bench_shake256/2048/64_mean           3787 ns         3787 ns            8   16.5172k      7.82062      91.618k             5.54687      531.933Mi/s
bench_shake256/2048/64_median         3803 ns         3803 ns            8   16.5016k      7.81326      91.618k             5.55207      529.672Mi/s
bench_shake256/2048/64_stddev         46.9 ns         46.9 ns            8    44.0849    0.0208735     1.47642m           0.0147689      6.69547Mi/s
bench_shake256/2048/64_cv             1.24 %          1.24 %             8      0.27%        0.27%        0.00%               0.27%            1.26%
bench_shake128/256/64                  446 ns          446 ns       316650   2.06059k      6.43933      11.686k             5.67121      683.922Mi/s
bench_shake128/256/64                  450 ns          450 ns       316650   2.06136k      6.44175      11.686k             5.66907      677.962Mi/s
bench_shake128/256/64                  451 ns          451 ns       316650   2.05941k      6.43567      11.686k             5.67443      676.863Mi/s
bench_shake128/256/64                  470 ns          470 ns       316650   2.06492k      6.45287      11.686k             5.65931      649.162Mi/s
bench_shake128/256/64                  476 ns          476 ns       316650   2.05895k      6.43422      11.686k             5.67571      641.409Mi/s
bench_shake128/256/64                  478 ns          478 ns       316650    2.0602k      6.43811      11.686k             5.67228      638.076Mi/s
bench_shake128/256/64                  476 ns          476 ns       316650   2.06403k      6.45011      11.686k             5.66173      641.733Mi/s
bench_shake128/256/64                  472 ns          472 ns       316650    2.0602k      6.43811      11.686k             5.67228       646.44Mi/s
bench_shake128/256/64_mean             465 ns          465 ns            8   2.06121k      6.44127      11.686k              5.6695      656.946Mi/s
bench_shake128/256/64_median           471 ns          471 ns            8   2.06039k      6.43872      11.686k             5.67174      647.801Mi/s
bench_shake128/256/64_stddev          13.4 ns         13.4 ns            8    2.15527     6.73521m     260.997u            5.92345m      19.1458Mi/s
bench_shake128/256/64_cv              2.88 %          2.88 %             8      0.10%        0.10%        0.00%               0.10%            2.91%
bench_shake256/64/32                   260 ns          260 ns       537457   1.14315k      11.9079       5.932k             5.18915      352.106Mi/s
bench_shake256/64/32                   262 ns          262 ns       537457   1.14665k      11.9443       5.932k             5.17334      349.645Mi/s
bench_shake256/64/32                   262 ns          262 ns       537457   1.14326k       11.909       5.932k             5.18867      349.515Mi/s
bench_shake256/64/32                   264 ns          264 ns       537457   1.14437k      11.9205       5.932k             5.18365      346.754Mi/s
bench_shake256/64/32                   264 ns          264 ns       537457   1.14438k      11.9206       5.932k              5.1836      346.275Mi/s
bench_shake256/64/32                   263 ns          262 ns       537457   1.13991k      11.8741       5.932k             5.20392      348.776Mi/s
bench_shake256/64/32                   264 ns          264 ns       537457   1.14395k      11.9162       5.932k             5.18552      346.196Mi/s
bench_shake256/64/32                   260 ns          260 ns       537457   1.13966k      11.8715       5.932k             5.20507      351.719Mi/s
bench_shake256/64/32_mean              262 ns          262 ns            8   1.14317k       11.908       5.932k             5.18911      348.873Mi/s
bench_shake256/64/32_median            262 ns          262 ns            8   1.14361k      11.9126       5.932k              5.1871      349.146Mi/s
bench_shake256/64/32_stddev           1.75 ns         1.75 ns            8    2.34744    0.0244525     92.2765u           0.0106633      2.32929Mi/s
bench_shake256/64/32_cv               0.67 %          0.67 %             8      0.21%        0.21%        0.00%               0.21%            0.67%
bench_keccak_permutation               207 ns          207 ns       681043    955.305      4.77652        5.69k             5.95622       922.07Mi/s
bench_keccak_permutation               206 ns          206 ns       681043    955.464      4.77732        5.69k             5.95522      925.223Mi/s
bench_keccak_permutation               213 ns          213 ns       681043    954.183      4.77092        5.69k             5.96321      895.751Mi/s
bench_keccak_permutation               218 ns          218 ns       681043    955.172      4.77586        5.69k             5.95704      874.484Mi/s
bench_keccak_permutation               220 ns          220 ns       681043    954.755      4.77377        5.69k             5.95965      867.804Mi/s
bench_keccak_permutation               218 ns          218 ns       681043    955.464      4.77732        5.69k             5.95522      876.487Mi/s
bench_keccak_permutation               219 ns          219 ns       681043    955.582      4.77791        5.69k             5.95449      869.047Mi/s
bench_keccak_permutation               217 ns          217 ns       681043    955.291      4.77646        5.69k              5.9563      880.404Mi/s
bench_keccak_permutation_mean          215 ns          215 ns            8    955.152      4.77576        5.69k             5.95717      888.909Mi/s
bench_keccak_permutation_median        217 ns          217 ns            8    955.298      4.77649        5.69k             5.95626      878.445Mi/s
bench_keccak_permutation_stddev       5.49 ns         5.48 ns            8   0.465761      2.3288m            0            2.90655m       23.114Mi/s
bench_keccak_permutation_cv           2.55 %          2.55 %             8      0.05%        0.05%        0.00%               0.05%            2.60%
bench_shake256/32/32                   250 ns          250 ns       540221   1.10965k      17.3383       5.932k             5.34584      244.559Mi/s
bench_shake256/32/32                   262 ns          262 ns       540221   1.15256k      18.0087       5.932k             5.14681       232.67Mi/s
bench_shake256/32/32                   262 ns          262 ns       540221    1.1559k       18.061       5.932k             5.13192      233.144Mi/s
bench_shake256/32/32                   262 ns          262 ns       540221   1.15423k      18.0349       5.932k             5.13934      232.651Mi/s
bench_shake256/32/32                   262 ns          262 ns       540221   1.15309k      18.0171       5.932k             5.14442      233.342Mi/s
bench_shake256/32/32                   266 ns          266 ns       540221   1.15564k      18.0569       5.932k             5.13308      229.829Mi/s
bench_shake256/32/32                   261 ns          261 ns       540221   1.14822k       17.941       5.932k             5.16625      234.023Mi/s
bench_shake256/32/32                   262 ns          262 ns       540221   1.15279k      18.0124       5.932k             5.14576      233.296Mi/s
bench_shake256/32/32_mean              261 ns          261 ns            8   1.14776k      17.9338       5.932k             5.16918      234.189Mi/s
bench_shake256/32/32_median            262 ns          262 ns            8   1.15294k      18.0148       5.932k             5.14509       233.22Mi/s
bench_shake256/32/32_stddev           4.72 ns         4.72 ns            8    15.5833     0.243489     92.2765u           0.0721711      4.37292Mi/s
bench_shake256/32/32_cv               1.81 %          1.81 %             8      1.36%        1.36%        0.00%               1.40%            1.87%
bench_sha3_256/128                     263 ns          263 ns       540407   1.14009k      7.12555       5.875k             5.15311      580.003Mi/s
bench_sha3_256/128                     262 ns          262 ns       540407   1.13976k      7.12352       5.875k             5.15458      582.041Mi/s
bench_sha3_256/128                     263 ns          263 ns       540407   1.13904k      7.11901       5.875k             5.15785      580.834Mi/s
bench_sha3_256/128                     261 ns          261 ns       540407   1.13697k      7.10605       5.875k             5.16725      585.057Mi/s
bench_sha3_256/128                     263 ns          263 ns       540407   1.13728k      7.10802       5.875k             5.16582      579.966Mi/s
bench_sha3_256/128                     263 ns          263 ns       540407   1.13591k      7.09946       5.875k             5.17205      580.694Mi/s
bench_sha3_256/128                     259 ns          259 ns       540407   1.13806k      7.11288       5.875k             5.16229       589.71Mi/s
bench_sha3_256/128                     261 ns          261 ns       540407   1.13845k      7.11532       5.875k             5.16052      584.448Mi/s
bench_sha3_256/128_mean                262 ns          262 ns            8    1.1382k      7.11373       5.875k             5.16168      582.844Mi/s
bench_sha3_256/128_median              262 ns          262 ns            8   1.13826k       7.1141       5.875k              5.1614      581.438Mi/s
bench_sha3_256/128_stddev             1.50 ns         1.51 ns            8    1.43365     8.96029m            0            6.50285m      3.38659Mi/s
bench_sha3_256/128_cv                 0.57 %          0.58 %             8      0.13%        0.13%        0.00%               0.13%            0.58%
bench_sha3_384/1024                   2156 ns         2155 ns        64074   9.86173k      9.19937      57.511k             5.83174      474.363Mi/s
bench_sha3_384/1024                   2285 ns         2284 ns        64074   9.88984k       9.2256      57.511k             5.81516      447.554Mi/s
bench_sha3_384/1024                   2290 ns         2289 ns        64074   9.84102k      9.18005      57.511k             5.84401      446.535Mi/s
bench_sha3_384/1024                   2306 ns         2306 ns        64074   9.94675k      9.27869      57.511k             5.78189      443.421Mi/s
bench_sha3_384/1024                   2288 ns         2288 ns        64074   9.90741k      9.24199      57.511k             5.80485      446.793Mi/s
bench_sha3_384/1024                   2301 ns         2300 ns        64074   9.88109k      9.21743      57.511k             5.82031      444.419Mi/s
bench_sha3_384/1024                   2260 ns         2259 ns        64074   9.89717k      9.23244      57.511k             5.81085      452.476Mi/s
bench_sha3_384/1024                   2282 ns         2282 ns        64074   9.86971k      9.20682      57.511k             5.82702      448.023Mi/s
bench_sha3_384/1024_mean              2271 ns         2271 ns            8   9.88684k       9.2228      57.511k             5.81698      450.448Mi/s
bench_sha3_384/1024_median            2287 ns         2286 ns            8   9.88546k      9.22151      57.511k             5.81774      447.173Mi/s
bench_sha3_384/1024_stddev            48.6 ns         48.6 ns            8    32.0389     0.029887     1.04399m           0.0188222      10.0336Mi/s
bench_sha3_384/1024_cv                2.14 %          2.14 %             8      0.32%        0.32%        0.00%               0.32%            2.23%
bench_sha3_224/32                      254 ns          254 ns       551434   1.15226k      19.2044       5.875k             5.09867      224.954Mi/s
bench_sha3_224/32                      258 ns          258 ns       551434   1.14755k      19.1258       5.875k              5.1196      222.106Mi/s
bench_sha3_224/32                      261 ns          261 ns       551434   1.15024k      19.1706       5.875k             5.10764      219.168Mi/s
bench_sha3_224/32                      265 ns          265 ns       551434   1.15064k      19.1774       5.875k             5.10584      216.021Mi/s
bench_sha3_224/32                      264 ns          264 ns       551434   1.15189k      19.1981       5.875k             5.10033      217.069Mi/s
bench_sha3_224/32                      265 ns          265 ns       551434   1.15421k      19.2368       5.875k             5.09006      216.176Mi/s
bench_sha3_224/32                      265 ns          264 ns       551434   1.15342k      19.2237       5.875k             5.09353      216.343Mi/s
bench_sha3_224/32                      264 ns          264 ns       551434   1.15292k      19.2154       5.875k             5.09574      217.105Mi/s
bench_sha3_224/32_mean                 262 ns          262 ns            8   1.15164k       19.194       5.875k             5.10143      218.618Mi/s
bench_sha3_224/32_median               264 ns          264 ns            8   1.15207k      19.2012       5.875k              5.0995      217.087Mi/s
bench_sha3_224/32_stddev              3.86 ns         3.87 ns            8    2.12357    0.0353928            0            9.41907m      3.27732Mi/s
bench_sha3_224/32_cv                  1.47 %          1.48 %             8      0.18%        0.18%        0.00%               0.18%            1.50%
bench_sha3_512/2048                   6393 ns         6391 ns        21870   28.3286k      13.4132     165.002k             5.82458      315.142Mi/s
bench_sha3_512/2048                   6558 ns         6558 ns        21870   28.2719k      13.3863     165.002k             5.83626      307.139Mi/s
bench_sha3_512/2048                   6548 ns         6546 ns        21870   28.2014k       13.353     165.002k             5.85084      307.677Mi/s
bench_sha3_512/2048                   6574 ns         6574 ns        21870   28.2832k      13.3917     165.002k             5.83393      306.388Mi/s
bench_sha3_512/2048                   6565 ns         6564 ns        21870   28.2733k       13.387     165.002k             5.83596      306.858Mi/s
bench_sha3_512/2048                   6545 ns         6545 ns        21870   28.3351k      13.4163     165.002k             5.82323      307.748Mi/s
bench_sha3_512/2048                   6555 ns         6554 ns        21870   28.3252k      13.4115     165.002k             5.82528      307.303Mi/s
bench_sha3_512/2048                   6551 ns         6551 ns        21870   28.3191k      13.4087     165.002k             5.82653       307.48Mi/s
bench_sha3_512/2048_mean              6536 ns         6535 ns            8   28.2922k      13.3959     165.002k             5.83207      308.217Mi/s
bench_sha3_512/2048_median            6553 ns         6552 ns            8   28.3011k      13.4002     165.002k             5.83023      307.392Mi/s
bench_sha3_512/2048_stddev            58.7 ns         59.0 ns            8    44.8239    0.0212234            0            9.25341m      2.83333Mi/s
bench_sha3_512/2048_cv                0.90 %          0.90 %             8      0.16%        0.16%        0.00%               0.16%            0.92%
bench_sha3_256/2048                   3622 ns         3621 ns        38445   16.4378k      7.90281      91.569k             5.57062      547.784Mi/s
bench_sha3_256/2048                   3719 ns         3719 ns        38445   16.4738k      7.92009      91.569k             5.55847      533.399Mi/s
bench_sha3_256/2048                   3789 ns         3788 ns        38445   16.5156k      7.94017      91.569k             5.54441      523.661Mi/s
bench_sha3_256/2048                   3815 ns         3815 ns        38445   16.4634k       7.9151      91.569k             5.56197      519.997Mi/s
bench_sha3_256/2048                   3793 ns         3793 ns        38445   16.4239k      7.89609      91.569k             5.57536      522.993Mi/s
bench_sha3_256/2048                   3798 ns         3798 ns        38445   16.4135k      7.89109      91.569k              5.5789      522.333Mi/s
bench_sha3_256/2048                   3740 ns         3740 ns        38445   16.4106k      7.88972      91.569k             5.57987      530.428Mi/s
bench_sha3_256/2048                   3773 ns         3773 ns        38445   16.4784k       7.9223      91.569k             5.55691      525.762Mi/s
bench_sha3_256/2048_mean              3756 ns         3756 ns            8   16.4521k      7.90967      91.569k             5.56581      528.295Mi/s
bench_sha3_256/2048_median            3781 ns         3780 ns            8   16.4506k      7.90895      91.569k              5.5663      524.712Mi/s
bench_sha3_256/2048_stddev            62.8 ns         62.9 ns            8    36.9017    0.0177412            0           0.0124737      9.03033Mi/s
bench_sha3_256/2048_cv                1.67 %          1.67 %             8      0.22%        0.22%        0.00%               0.22%            1.71%
bench_shake256/64/64                   250 ns          250 ns       564433   1.14523k      8.94711       5.932k             5.17975      489.181Mi/s
bench_shake256/64/64                   251 ns          251 ns       564433   1.11364k      8.70029       5.932k             5.32669      486.299Mi/s
bench_shake256/64/64                   259 ns          259 ns       564433   1.14328k      8.93188       5.932k             5.18858      471.353Mi/s
bench_shake256/64/64                   264 ns          264 ns       564433   1.14754k      8.96518       5.932k             5.16931      463.087Mi/s
bench_shake256/64/64                   264 ns          264 ns       564433    1.1463k       8.9555       5.932k             5.17489      462.525Mi/s
bench_shake256/64/64                   267 ns          267 ns       564433   1.15825k      9.04885       5.932k             5.12151      456.641Mi/s
bench_shake256/64/64                   260 ns          260 ns       564433   1.14294k      8.92923       5.932k             5.19012      469.444Mi/s
bench_shake256/64/64                   262 ns          262 ns       564433   1.14629k      8.95541       5.932k             5.17495      465.849Mi/s
bench_shake256/64/64_mean              260 ns          260 ns            8   1.14294k      8.92918       5.932k             5.19072      470.547Mi/s
bench_shake256/64/64_median            261 ns          261 ns            8   1.14576k      8.95126       5.932k             5.17735      467.646Mi/s
bench_shake256/64/64_stddev           6.27 ns         6.28 ns            8     12.774    0.0997969            0           0.0589931      11.5429Mi/s
bench_shake256/64/64_cv               2.42 %          2.42 %             8      1.12%        1.12%        0.00%               1.14%            2.45%
bench_shake128/32/32                   258 ns          258 ns       540695   1.14036k      17.8182       5.951k             5.21851      236.594Mi/s
bench_shake128/32/32                   266 ns          266 ns       540695   1.15975k       18.121       5.951k             5.13129      229.376Mi/s
bench_shake128/32/32                   268 ns          268 ns       540695   1.16419k      18.1905       5.951k             5.11169      227.774Mi/s
bench_shake128/32/32                   269 ns          269 ns       540695   1.16416k      18.1899       5.951k             5.11186      227.022Mi/s
bench_shake128/32/32                   264 ns          264 ns       540695   1.16211k      18.1579       5.951k             5.12087      231.432Mi/s
bench_shake128/32/32                   262 ns          262 ns       540695   1.15669k      18.0733       5.951k             5.14485      232.781Mi/s
bench_shake128/32/32                   261 ns          261 ns       540695   1.15101k      17.9846       5.951k             5.17022      233.422Mi/s
bench_shake128/32/32                   265 ns          265 ns       540695   1.15939k      18.1155       5.951k             5.13286      230.111Mi/s
bench_shake128/32/32_mean              264 ns          264 ns            8   1.15721k      18.0814       5.951k             5.14277      231.064Mi/s
bench_shake128/32/32_median            264 ns          264 ns            8   1.15957k      18.1183       5.951k             5.13208      230.771Mi/s
bench_shake128/32/32_stddev           3.63 ns         3.60 ns            8    8.05927     0.125926            0           0.0361063      3.16559Mi/s
bench_shake128/32/32_cv               1.37 %          1.36 %             8      0.70%        0.70%        0.00%               0.70%            1.37%
bench_shake256/512/64                  902 ns          902 ns       153182   4.12028k      7.15326      23.095k             5.60521      608.845Mi/s
bench_shake256/512/64                  954 ns          954 ns       153182   4.11796k      7.14924      23.095k             5.60836      575.656Mi/s
bench_shake256/512/64                  954 ns          954 ns       153182   4.12579k      7.16284      23.095k             5.59771      576.036Mi/s
bench_shake256/512/64                  949 ns          949 ns       153182   4.12002k      7.15281      23.095k             5.60556      578.777Mi/s
bench_shake256/512/64                  953 ns          953 ns       153182   4.12533k      7.16203      23.095k             5.59834      576.668Mi/s
bench_shake256/512/64                  957 ns          957 ns       153182   4.12843k      7.16742      23.095k             5.59413       573.86Mi/s
bench_shake256/512/64                  946 ns          946 ns       153182   4.12462k      7.16079      23.095k             5.59931       580.97Mi/s
bench_shake256/512/64                  949 ns          949 ns       153182   4.11205k      7.13897      23.095k             5.61642      578.831Mi/s
bench_shake256/512/64_mean             946 ns          945 ns            8   4.12181k      7.15592      23.095k             5.60313      581.205Mi/s
bench_shake256/512/64_median           951 ns          951 ns            8   4.12245k      7.15702      23.095k             5.60226      577.722Mi/s
bench_shake256/512/64_stddev          17.8 ns         17.8 ns            8    5.28681     9.17849m     521.995u            7.19198m      11.3869Mi/s
bench_shake256/512/64_cv              1.89 %          1.89 %             8      0.13%        0.13%        0.00%               0.13%            1.96%
bench_sha3_256/256                     468 ns          468 ns       298174   2.04252k      7.09209       11.61k             5.68415      586.579Mi/s
bench_sha3_256/256                     473 ns          473 ns       298174   2.05653k      7.14073       11.61k             5.64543      581.282Mi/s
bench_sha3_256/256                     473 ns          473 ns       298174   2.05029k      7.11905       11.61k             5.66262      580.321Mi/s
bench_sha3_256/256                     475 ns          475 ns       298174   2.05297k      7.12838       11.61k             5.65521      578.204Mi/s
bench_sha3_256/256                     475 ns          475 ns       298174   2.04685k       7.1071       11.61k             5.67214      578.131Mi/s
bench_sha3_256/256                     475 ns          475 ns       298174   2.04656k       7.1061       11.61k             5.67294      578.487Mi/s
bench_sha3_256/256                     468 ns          468 ns       298174   2.05585k      7.13835       11.61k             5.64731      586.378Mi/s
bench_sha3_256/256                     475 ns          475 ns       298174   2.05511k      7.13579       11.61k             5.64934      577.716Mi/s
bench_sha3_256/256_mean                473 ns          473 ns            8   2.05083k      7.12095       11.61k             5.66114      580.887Mi/s
bench_sha3_256/256_median              474 ns          474 ns            8   2.05163k      7.12372       11.61k             5.65892      579.404Mi/s
bench_sha3_256/256_stddev             2.96 ns         2.96 ns            8    5.12677    0.0178013     319.655u            0.014164      3.65739Mi/s
bench_sha3_256/256_cv                 0.63 %          0.63 %             8      0.25%        0.25%        0.00%               0.25%            0.63%
bench_shake128/2048/32                2919 ns         2919 ns        48547   13.2741k      6.38177      74.442k             5.60807      679.676Mi/s
bench_shake128/2048/32                3016 ns         3016 ns        48547   13.2529k      6.37159      74.442k             5.61703       657.67Mi/s
bench_shake128/2048/32                3091 ns         3091 ns        48547   13.2571k       6.3736      74.442k             5.61527      641.814Mi/s
bench_shake128/2048/32                3060 ns         3059 ns        48547   13.2758k      6.38257      74.442k             5.60737      648.391Mi/s
bench_shake128/2048/32                3077 ns         3076 ns        48547   13.2824k      6.38576      74.442k             5.60457      644.798Mi/s
bench_shake128/2048/32                3021 ns         3021 ns        48547    13.269k      6.37932      74.442k             5.61023      656.674Mi/s
bench_shake128/2048/32                3017 ns         3017 ns        48547   13.2501k      6.37022      74.442k             5.61824      657.477Mi/s
bench_shake128/2048/32                3048 ns         3048 ns        48547   13.2603k      6.37516      74.442k             5.61389      650.803Mi/s
bench_shake128/2048/32_mean           3031 ns         3031 ns            8   13.2652k       6.3775      74.442k             5.61183      654.663Mi/s
bench_shake128/2048/32_median         3035 ns         3034 ns            8   13.2647k      6.37724      74.442k             5.61206      653.738Mi/s
bench_shake128/2048/32_stddev         53.4 ns         53.3 ns            8    11.7698     5.65856m            0            4.97882m      11.7309Mi/s
bench_shake128/2048/32_cv             1.76 %          1.76 %             8      0.09%        0.09%        0.00%               0.09%            1.79%
bench_shake128/4096/32                5670 ns         5670 ns        24491   25.4794k      6.17234     142.914k               5.609      694.287Mi/s
bench_shake128/4096/32                5756 ns         5755 ns        24491   25.4958k       6.1763     142.914k              5.6054      684.053Mi/s
bench_shake128/4096/32                5873 ns         5873 ns        24491   25.5047k      6.17846     142.914k             5.60345      670.359Mi/s
bench_shake128/4096/32                5898 ns         5898 ns        24491   25.4908k      6.17509     142.914k              5.6065      667.513Mi/s
bench_shake128/4096/32                5903 ns         5903 ns        24491   25.5397k      6.18695     142.914k             5.59576      666.902Mi/s
bench_shake128/4096/32                5900 ns         5899 ns        24491   25.5057k       6.1787     142.914k             5.60323      667.347Mi/s
bench_shake128/4096/32                5918 ns         5917 ns        24491   25.4969k      6.17656     142.914k             5.60516      665.281Mi/s
bench_shake128/4096/32                5858 ns         5858 ns        24491   25.5512k      6.18972     142.914k             5.59325      672.063Mi/s
bench_shake128/4096/32_mean           5847 ns         5847 ns            8    25.508k      6.17926     142.914k             5.60272      673.476Mi/s
bench_shake128/4096/32_median         5886 ns         5885 ns            8   25.5008k      6.17751     142.914k              5.6043      668.936Mi/s
bench_shake128/4096/32_stddev         87.8 ns         87.7 ns            8    24.7046     5.98463m            0            5.42224m       10.283Mi/s
bench_shake128/4096/32_cv             1.50 %          1.50 %             8      0.10%        0.10%        0.00%               0.10%            1.53%
bench_sha3_224/64                      253 ns          253 ns       561410   1.14432k      12.4383       5.875k             5.13405      346.695Mi/s
bench_sha3_224/64                      257 ns          257 ns       561410   1.14613k      12.4579       5.875k             5.12595      341.253Mi/s
bench_sha3_224/64                      263 ns          263 ns       561410   1.14646k      12.4615       5.875k             5.12448      333.522Mi/s
bench_sha3_224/64                      263 ns          263 ns       561410   1.14153k      12.4079       5.875k             5.14661       333.99Mi/s
bench_sha3_224/64                      260 ns          260 ns       561410   1.14269k      12.4205       5.875k             5.14139      337.338Mi/s
bench_sha3_224/64                      261 ns          261 ns       561410    1.1434k      12.4282       5.875k              5.1382      336.566Mi/s
bench_sha3_224/64                      261 ns          261 ns       561410   1.14422k      12.4372       5.875k             5.13449      336.182Mi/s
bench_sha3_224/64                      259 ns          259 ns       561410   1.14512k      12.4469       5.875k             5.13048      338.403Mi/s
bench_sha3_224/64_mean                 260 ns          260 ns            8   1.14423k      12.4373       5.875k             5.13446      337.994Mi/s
bench_sha3_224/64_median               260 ns          260 ns            8   1.14427k      12.4377       5.875k             5.13427      336.952Mi/s
bench_sha3_224/64_stddev              3.25 ns         3.25 ns            8    1.67945    0.0182549            0            7.53798m      4.28082Mi/s
bench_sha3_224/64_cv                  1.25 %          1.25 %             8      0.15%        0.15%        0.00%               0.15%            1.27%
bench_shake256/256/64                  460 ns          460 ns       303034   2.04989k      6.40592      11.673k             5.69444      663.532Mi/s
bench_shake256/256/64                  474 ns          474 ns       303034   2.05253k      6.41414      11.673k             5.68714      643.345Mi/s
bench_shake256/256/64                  476 ns          476 ns       303034   2.04753k      6.39853      11.673k             5.70102      641.595Mi/s
bench_shake256/256/64                  475 ns          475 ns       303034   2.05409k      6.41902      11.673k             5.68282      642.244Mi/s
bench_shake256/256/64                  478 ns          478 ns       303034   2.05102k      6.40944      11.673k             5.69131       638.01Mi/s
bench_shake256/256/64                  470 ns          470 ns       303034   2.05218k      6.41307      11.673k             5.68809      649.222Mi/s
bench_shake256/256/64                  472 ns          472 ns       303034   2.05692k      6.42788      11.673k             5.67498      647.158Mi/s
bench_shake256/256/64                  472 ns          472 ns       303034   2.04667k      6.39584      11.673k             5.70342      646.602Mi/s
bench_shake256/256/64_mean             472 ns          472 ns            8   2.05135k      6.41048      11.673k              5.6904      646.464Mi/s
bench_shake256/256/64_median           473 ns          473 ns            8    2.0516k      6.41126      11.673k              5.6897      644.974Mi/s
bench_shake256/256/64_stddev          5.57 ns         5.59 ns            8    3.36758    0.0105237            0            9.33971m       7.7657Mi/s
bench_shake256/256/64_cv              1.18 %          1.18 %             8      0.16%        0.16%        0.00%               0.16%            1.20%
bench_shake128/1024/32                1537 ns         1537 ns        91841   7.15404k      6.77466      40.205k              5.6199      655.259Mi/s
bench_shake128/1024/32                1612 ns         1612 ns        91841   7.15013k      6.77095      40.205k             5.62298      624.753Mi/s
bench_shake128/1024/32                1652 ns         1652 ns        91841   7.16309k      6.78323      40.205k              5.6128      609.516Mi/s
bench_shake128/1024/32                1655 ns         1655 ns        91841   7.16149k      6.78171      40.205k             5.61406      608.627Mi/s
bench_shake128/1024/32                1634 ns         1634 ns        91841   7.16896k      6.78878      40.205k             5.60821      616.277Mi/s
bench_shake128/1024/32                1651 ns         1650 ns        91841   7.16282k      6.78297      40.205k             5.61302      610.212Mi/s
bench_shake128/1024/32                1636 ns         1636 ns        91841   7.14318k      6.76437      40.205k             5.62845      615.527Mi/s
bench_shake128/1024/32                1643 ns         1642 ns        91841   7.15232k      6.77303      40.205k             5.62126      613.157Mi/s
bench_shake128/1024/32_mean           1627 ns         1627 ns            8     7.157k      6.77746      40.205k             5.61758      619.166Mi/s
bench_shake128/1024/32_median         1639 ns         1639 ns            8   7.15776k      6.77818      40.205k             5.61698      614.342Mi/s
bench_shake128/1024/32_stddev         39.0 ns         39.0 ns            8    8.47739     8.02783m            0            6.65559m      15.4733Mi/s
bench_shake128/1024/32_cv             2.40 %          2.40 %             8      0.12%        0.12%        0.00%               0.12%            2.50%
bench_shake256/4096/32                7074 ns         7074 ns        19918   32.0618k       7.7669     177.281k             5.52936      556.517Mi/s
bench_shake256/4096/32                7359 ns         7358 ns        19918   32.0625k      7.76708     177.281k             5.52923      534.998Mi/s
bench_shake256/4096/32                7402 ns         7401 ns        19918   32.0457k      7.76301     177.281k             5.53213      531.938Mi/s
bench_shake256/4096/32                7423 ns         7422 ns        19918   32.0525k      7.76466     177.281k             5.53096      530.435Mi/s
bench_shake256/4096/32                7266 ns         7265 ns        19918   31.8435k      7.71404     177.281k             5.56725      541.851Mi/s
bench_shake256/4096/32                7308 ns         7308 ns        19918   31.7938k      7.70199     177.281k             5.57596      538.693Mi/s
bench_shake256/4096/32                7365 ns         7364 ns        19918   31.9854k       7.7484     177.281k             5.54256      534.561Mi/s
bench_shake256/4096/32                7303 ns         7303 ns        19918   31.8727k      7.72111     177.281k             5.56215      539.067Mi/s
bench_shake256/4096/32_mean           7313 ns         7312 ns            8   31.9648k       7.7434     177.281k              5.5462      538.507Mi/s
bench_shake256/4096/32_median         7334 ns         7333 ns            8   32.0156k      7.75571     177.281k             5.53735      536.845Mi/s
bench_shake256/4096/32_stddev          110 ns          109 ns            8    110.853    0.0268539            0           0.0192656      8.21234Mi/s
bench_shake256/4096/32_cv             1.50 %          1.50 %             8      0.35%        0.35%        0.00%               0.35%            1.53%
bench_sha3_384/4096                   8865 ns         8864 ns        15730   39.3532k      9.49642     229.701k             5.83691      445.836Mi/s
bench_sha3_384/4096                   9089 ns         9088 ns        15730   39.5636k      9.54719     229.701k             5.80587      434.861Mi/s
bench_sha3_384/4096                   9103 ns         9101 ns        15730   39.4335k      9.51579     229.701k             5.82503      434.232Mi/s
bench_sha3_384/4096                   9213 ns         9212 ns        15730    39.643k      9.56636     229.701k             5.79424      429.029Mi/s
bench_sha3_384/4096                   9150 ns         9149 ns        15730   39.4476k      9.51922     229.701k             5.82294      431.956Mi/s
bench_sha3_384/4096                   9090 ns         9089 ns        15730   39.5298k      9.53905     229.701k             5.81083      434.794Mi/s
bench_sha3_384/4096                   9085 ns         9084 ns        15730   39.3213k      9.48873     229.701k             5.84164      435.036Mi/s
bench_sha3_384/4096                   9010 ns         9010 ns        15730   39.4137k      9.51103     229.701k             5.82795      438.637Mi/s
bench_sha3_384/4096_mean              9076 ns         9075 ns            8   39.4632k      9.52297     229.701k             5.82068      435.548Mi/s
bench_sha3_384/4096_median            9090 ns         9089 ns            8   39.4405k       9.5175     229.701k             5.82398      434.827Mi/s
bench_sha3_384/4096_stddev             103 ns          103 ns            8    108.667    0.0262227            0           0.0160145      4.98318Mi/s
bench_sha3_384/4096_cv                1.14 %          1.13 %             8      0.28%        0.28%        0.00%               0.28%            1.14%
bench_shake128/1024/64                1573 ns         1573 ns        89062    7.1671k      6.58741      40.205k             5.60966      659.703Mi/s
bench_shake128/1024/64                1628 ns         1628 ns        89062   7.15062k      6.57226      40.205k             5.62259      637.291Mi/s
bench_shake128/1024/64                1625 ns         1625 ns        89062   7.15308k      6.57452      40.205k             5.62066      638.425Mi/s
bench_shake128/1024/64                1634 ns         1634 ns        89062   7.16182k      6.58255      40.205k              5.6138      635.153Mi/s
bench_shake128/1024/64                1647 ns         1647 ns        89062   7.17236k      6.59224      40.205k             5.60555      629.842Mi/s
bench_shake128/1024/64                1627 ns         1627 ns        89062   7.15206k      6.57359      40.205k             5.62146      637.851Mi/s
bench_shake128/1024/64                1638 ns         1638 ns        89062    7.1527k      6.57417      40.205k             5.62096      633.406Mi/s
bench_shake128/1024/64                1651 ns         1651 ns        89062   7.16087k      6.58168      40.205k             5.61454      628.655Mi/s
bench_shake128/1024/64_mean           1628 ns         1628 ns            8   7.15883k       6.5798      40.205k             5.61615      637.541Mi/s
bench_shake128/1024/64_median         1631 ns         1631 ns            8   7.15697k       6.5781      40.205k              5.6176      636.222Mi/s
bench_shake128/1024/64_stddev         24.1 ns         24.1 ns            8    8.00443     7.35701m            0            6.27603m      9.65934Mi/s
bench_shake128/1024/64_cv             1.48 %          1.48 %             8      0.11%        0.11%        0.00%               0.11%            1.52%
bench_shake256/1024/32                1841 ns         1841 ns        75448   8.22701k      7.79073      45.939k             5.58392      547.172Mi/s
bench_shake256/1024/32                1852 ns         1852 ns        75448     8.233k       7.7964      45.939k             5.57986      543.677Mi/s
bench_shake256/1024/32                1879 ns         1878 ns        75448   8.24792k      7.81053      45.939k             5.56977      536.281Mi/s
bench_shake256/1024/32                1885 ns         1885 ns        75448   8.28283k      7.84359      45.939k             5.54629      534.173Mi/s
bench_shake256/1024/32                1895 ns         1895 ns        75448   8.23649k       7.7997      45.939k              5.5775      531.353Mi/s
bench_shake256/1024/32                1916 ns         1916 ns        75448   8.25895k      7.82098      45.939k             5.56233      525.706Mi/s
bench_shake256/1024/32                1912 ns         1913 ns        75448   8.25683k      7.81897      45.939k             5.56376      526.538Mi/s
bench_shake256/1024/32                1901 ns         1901 ns        75448   8.26372k      7.82549      45.939k             5.55912      529.655Mi/s
bench_shake256/1024/32_mean           1885 ns         1885 ns            8   8.25084k       7.8133      45.939k             5.56782      534.319Mi/s
bench_shake256/1024/32_median         1890 ns         1890 ns            8   8.25237k      7.81475      45.939k             5.56676      532.763Mi/s
bench_shake256/1024/32_stddev         27.1 ns         27.2 ns            8    18.4832     0.017503            0           0.0124644      7.76378Mi/s
bench_shake256/1024/32_cv             1.44 %          1.44 %             8      0.22%        0.22%        0.00%               0.22%            1.45%
bench_shake128/512/32                  943 ns          943 ns       149184   4.11691k      7.56786      23.089k             5.60833      550.231Mi/s
bench_shake128/512/32                  947 ns          947 ns       149184   4.10657k      7.54884      23.089k             5.62245       548.03Mi/s
bench_shake128/512/32                  946 ns          946 ns       149184   4.10525k      7.54642      23.089k             5.62426      548.506Mi/s
bench_shake128/512/32                  947 ns          947 ns       149184   4.11019k      7.55549      23.089k             5.61751      548.123Mi/s
bench_shake128/512/32                  950 ns          950 ns       149184   4.10159k      7.53968      23.089k             5.62929      546.034Mi/s
bench_shake128/512/32                  947 ns          947 ns       149184   4.09896k      7.53485      23.089k             5.63289       547.61Mi/s
bench_shake128/512/32                  935 ns          935 ns       149184   4.12006k      7.57364      23.089k             5.60404      554.797Mi/s
bench_shake128/512/32                  944 ns          944 ns       149184   4.10802k       7.5515      23.089k             5.62048        549.8Mi/s
bench_shake128/512/32_mean             945 ns          945 ns            8   4.10844k      7.55228      23.089k             5.61991      549.141Mi/s
bench_shake128/512/32_median           946 ns          946 ns            8   4.10729k      7.55017      23.089k             5.62146      548.315Mi/s
bench_shake128/512/32_stddev          4.48 ns         4.49 ns            8    7.17786    0.0131946     260.997u            9.81218m      2.62633Mi/s
bench_shake128/512/32_cv              0.47 %          0.48 %             8      0.17%        0.17%        0.00%               0.17%            0.48%
bench_shake256/4096/64                7159 ns         7158 ns        19531   31.8652k       7.6599     177.281k             5.56347      554.249Mi/s
bench_shake256/4096/64                7190 ns         7189 ns        19531   31.9397k      7.67781     177.281k             5.55049      551.824Mi/s
bench_shake256/4096/64                7297 ns         7297 ns        19531   31.9158k      7.67207     177.281k             5.55465       543.68Mi/s
bench_shake256/4096/64                7300 ns         7300 ns        19531   31.9743k      7.68612     177.281k             5.54449      543.481Mi/s
bench_shake256/4096/64                7352 ns         7352 ns        19531   32.0158k      7.69611     177.281k              5.5373      539.641Mi/s
bench_shake256/4096/64                7422 ns         7421 ns        19531    31.954k      7.68126     177.281k               5.548       534.59Mi/s
bench_shake256/4096/64                7341 ns         7340 ns        19531   31.8188k      7.64874     177.281k             5.57159      540.481Mi/s
bench_shake256/4096/64                7328 ns         7328 ns        19531   31.7956k      7.64317     177.281k             5.57565      541.402Mi/s
bench_shake256/4096/64_mean           7299 ns         7298 ns            8   31.9099k      7.67065     177.281k              5.5557      543.669Mi/s
bench_shake256/4096/64_median         7314 ns         7314 ns            8   31.9278k      7.67494     177.281k             5.55257      542.441Mi/s
bench_shake256/4096/64_stddev         86.2 ns         86.3 ns            8    77.0877    0.0185307     2.08798m           0.0134293      6.46339Mi/s
bench_shake256/4096/64_cv             1.18 %          1.18 %             8      0.24%        0.24%        0.00%               0.24%            1.19%
bench_sha3_256/4096                   7399 ns         7398 ns        18976   32.0102k      7.75442     177.247k              5.5372      532.143Mi/s
bench_sha3_256/4096                   7308 ns         7308 ns        18976   31.9216k      7.73294     177.247k             5.55258      538.711Mi/s
bench_sha3_256/4096                   7376 ns         7375 ns        18976   31.7793k      7.69847     177.247k             5.57744      533.802Mi/s
bench_sha3_256/4096                   7324 ns         7322 ns        18976   31.7943k       7.7021     177.247k             5.57481      537.686Mi/s
bench_sha3_256/4096                   7379 ns         7378 ns        18976   31.8476k      7.71501     177.247k             5.56548      533.595Mi/s
bench_sha3_256/4096                   7285 ns         7284 ns        18976   31.7626k      7.69443     177.247k             5.58037      540.447Mi/s
bench_sha3_256/4096                   7374 ns         7374 ns        18976   31.9691k      7.74444     177.247k             5.54433      533.882Mi/s
bench_sha3_256/4096                   7336 ns         7335 ns        18976   31.8797k       7.7228     177.247k             5.55987      536.706Mi/s
bench_sha3_256/4096_mean              7348 ns         7347 ns            8   31.8705k      7.72058     177.247k             5.56151      535.871Mi/s
bench_sha3_256/4096_median            7355 ns         7354 ns            8   31.8636k       7.7189     177.247k             5.56268      535.294Mi/s
bench_sha3_256/4096_stddev            40.3 ns         40.2 ns            8     91.212    0.0220959            0           0.0159054      2.93522Mi/s
bench_sha3_256/4096_cv                0.55 %          0.55 %             8      0.29%        0.29%        0.00%               0.29%            0.55%
bench_shake128/64/64                   251 ns          251 ns       551487   1.14954k      8.98078       5.951k             5.17686      487.071Mi/s
bench_shake128/64/64                   265 ns          265 ns       551487   1.15483k      9.02212       5.951k             5.15314      461.045Mi/s
bench_shake128/64/64                   264 ns          263 ns       551487   1.15189k      8.99913       5.951k              5.1663      463.306Mi/s
bench_shake128/64/64                   262 ns          262 ns       551487   1.14491k      8.94459       5.951k              5.1978      465.811Mi/s
bench_shake128/64/64                   263 ns          263 ns       551487   1.15025k       8.9863       5.951k             5.17368      464.015Mi/s
bench_shake128/64/64                   269 ns          269 ns       551487   1.17976k      9.21691       5.951k             5.04423      453.183Mi/s
bench_shake128/64/64                   268 ns          268 ns       551487   1.17287k      9.16302       5.951k              5.0739       455.35Mi/s
bench_shake128/64/64                   264 ns          264 ns       551487    1.1488k      8.97497       5.951k             5.18021       462.63Mi/s
bench_shake128/64/64_mean              263 ns          263 ns            8    1.1566k      9.03598       5.951k             5.14576      464.051Mi/s
bench_shake128/64/64_median            264 ns          264 ns            8   1.15107k      8.99271       5.951k             5.16999      462.968Mi/s
bench_shake128/64/64_stddev           5.66 ns         5.66 ns            8    12.6171    0.0985715     92.2765u           0.0555371      10.2709Mi/s
bench_shake128/64/64_cv               2.15 %          2.15 %             8      1.09%        1.09%        0.00%               1.08%            2.21%
bench_shake256/1024/64                1870 ns         1869 ns        76038   8.25761k      7.58972      45.939k             5.56323      555.046Mi/s
bench_shake256/1024/64                1892 ns         1891 ns        76038   8.25669k      7.58887      45.939k             5.56386      548.568Mi/s
bench_shake256/1024/64                1896 ns         1896 ns        76038   8.23698k      7.57075      45.939k             5.57717      547.271Mi/s
bench_shake256/1024/64                1911 ns         1910 ns        76038   8.24304k      7.57633      45.939k             5.57306      543.132Mi/s
bench_shake256/1024/64                1923 ns         1922 ns        76038   8.25897k      7.59097      45.939k             5.56232      539.734Mi/s
bench_shake256/1024/64                1920 ns         1919 ns        76038   8.26618k      7.59759      45.939k             5.55746      540.683Mi/s
bench_shake256/1024/64                1890 ns         1890 ns        76038    8.2646k      7.59614      45.939k             5.55853      548.958Mi/s
bench_shake256/1024/64                1907 ns         1907 ns        76038   8.23725k        7.571      45.939k             5.57699      544.218Mi/s
bench_shake256/1024/64_mean           1901 ns         1901 ns            8   8.25266k      7.58517      45.939k             5.56658      545.951Mi/s
bench_shake256/1024/64_median         1901 ns         1901 ns            8   8.25715k      7.58929      45.939k             5.56354      545.745Mi/s
bench_shake256/1024/64_stddev         17.5 ns         17.5 ns            8    11.8437    0.0108858            0            7.99258m      5.03866Mi/s
bench_shake256/1024/64_cv             0.92 %          0.92 %             8      0.14%        0.14%        0.00%               0.14%            0.92%
bench_sha3_384/32                      233 ns          233 ns       606388   1.01294k      12.6618        5.81k             5.73577      328.115Mi/s
bench_sha3_384/32                      234 ns          234 ns       606388   1.01189k      12.6486        5.81k             5.74176      326.184Mi/s
bench_sha3_384/32                      237 ns          237 ns       606388   1.01629k      12.7036        5.81k             5.71688      321.841Mi/s
bench_sha3_384/32                      236 ns          236 ns       606388    1.0138k      12.6725        5.81k              5.7309      323.374Mi/s
bench_sha3_384/32                      236 ns          236 ns       606388   1.01551k      12.6938        5.81k             5.72128      323.194Mi/s
bench_sha3_384/32                      237 ns          237 ns       606388   1.01651k      12.7064        5.81k             5.71564      322.293Mi/s
bench_sha3_384/32                      236 ns          236 ns       606388   1.01475k      12.6844        5.81k             5.72553      323.302Mi/s
bench_sha3_384/32                      237 ns          237 ns       606388   1.01356k      12.6695        5.81k             5.73227      321.963Mi/s
bench_sha3_384/32_mean                 236 ns          236 ns            8   1.01441k      12.6801        5.81k              5.7275      323.783Mi/s
bench_sha3_384/32_median               236 ns          236 ns            8   1.01428k      12.6785        5.81k             5.72822      323.248Mi/s
bench_sha3_384/32_stddev              1.61 ns         1.61 ns            8    1.64242    0.0205303     92.2765u            9.27467m      2.22263Mi/s
bench_sha3_384/32_cv                  0.68 %          0.68 %             8      0.16%        0.16%        0.00%               0.16%            0.69%
bench_sha3_224/2048                   3461 ns         3461 ns        40353   15.2998k      7.36983      85.569k             5.59283      572.109Mi/s
bench_sha3_224/2048                   3514 ns         3514 ns        40353    15.348k      7.39308      85.569k             5.57524      563.469Mi/s
bench_sha3_224/2048                   3504 ns         3504 ns        40353    15.268k      7.35452      85.569k             5.60448      565.086Mi/s
bench_sha3_224/2048                   3531 ns         3531 ns        40353   15.3302k      7.38448      85.569k             5.58173      560.738Mi/s
bench_sha3_224/2048                   3527 ns         3527 ns        40353   15.2462k      7.34403      85.569k             5.61248      561.311Mi/s
bench_sha3_224/2048                   3482 ns         3482 ns        40353   15.2747k      7.35775      85.569k             5.60201      568.618Mi/s
bench_sha3_224/2048                   3537 ns         3536 ns        40353   15.3036k      7.37167      85.569k             5.59144      559.857Mi/s
bench_sha3_224/2048                   3529 ns         3529 ns        40353   15.2414k      7.34173      85.569k             5.61423      561.054Mi/s
bench_sha3_224/2048_mean              3511 ns         3510 ns            8    15.289k      7.36464      85.569k             5.59681       564.03Mi/s
bench_sha3_224/2048_median            3521 ns         3520 ns            8   15.2872k      7.36379      85.569k             5.59742       562.39Mi/s
bench_sha3_224/2048_stddev            26.8 ns         26.9 ns            8    38.2657    0.0184324            0           0.0140006      4.34678Mi/s
bench_sha3_224/2048_cv                0.76 %          0.77 %             8      0.25%        0.25%        0.00%               0.25%            0.77%
bench_sha3_224/512                     949 ns          948 ns       147993   4.09007k       7.5742      22.978k               5.618      542.959Mi/s
bench_sha3_224/512                     944 ns          944 ns       147993    4.1039k      7.59981      22.978k             5.59906       545.49Mi/s
bench_sha3_224/512                     950 ns          949 ns       147993   4.10355k      7.59917      22.978k             5.59954       542.38Mi/s
bench_sha3_224/512                     952 ns          952 ns       147993   4.09887k       7.5905      22.978k             5.60594      541.068Mi/s
bench_sha3_224/512                     952 ns          951 ns       147993   4.10266k      7.59751      22.978k             5.60076      541.266Mi/s
bench_sha3_224/512                     940 ns          940 ns       147993   4.02471k      7.45317     22.5543k             5.60395      547.786Mi/s
bench_sha3_224/512                     940 ns          940 ns       147993    4.1162k      7.62259      22.978k             5.58234      547.804Mi/s
bench_sha3_224/512                     947 ns          947 ns       147993    4.0988k      7.59037      22.978k             5.60603      543.663Mi/s
bench_sha3_224/512_mean                947 ns          947 ns            8   4.09234k      7.57842      22.925k             5.60195      544.052Mi/s
bench_sha3_224/512_median              948 ns          948 ns            8   4.10076k      7.59401      22.978k             5.60235      543.311Mi/s
bench_sha3_224/512_stddev             4.67 ns         4.68 ns            8    28.2799    0.0523702      149.815            9.95984m      2.69629Mi/s
bench_sha3_224/512_cv                 0.49 %          0.49 %             8      0.69%        0.69%        0.65%               0.18%            0.50%
bench_sha3_256/512                     905 ns          905 ns       156399   4.11906k       7.5718      23.034k             5.59206      573.316Mi/s
bench_sha3_256/512                     938 ns          938 ns       156399   4.11802k      7.56989      23.034k             5.59347      553.022Mi/s
bench_sha3_256/512                     947 ns          947 ns       156399   4.11238k      7.55953      23.034k             5.60113      547.589Mi/s
bench_sha3_256/512                     947 ns          947 ns       156399   4.10485k      7.54569      23.034k             5.61141      547.701Mi/s
bench_sha3_256/512                     949 ns          949 ns       156399    4.1116k      7.55809      23.034k              5.6022      546.561Mi/s
bench_sha3_256/512                     943 ns          943 ns       156399   4.11338k      7.56136      23.034k             5.59977      550.449Mi/s
bench_sha3_256/512                     935 ns          935 ns       156399   4.10538k      7.54666      23.034k             5.61069      554.763Mi/s
bench_sha3_256/512                     951 ns          951 ns       156399   4.10945k      7.55414      23.034k             5.60513      545.544Mi/s
bench_sha3_256/512_mean                940 ns          939 ns            8   4.11177k      7.55839      23.034k             5.60198      552.368Mi/s
bench_sha3_256/512_median              945 ns          945 ns            8   4.11199k      7.55881      23.034k             5.60167      549.075Mi/s
bench_sha3_256/512_stddev             15.0 ns         15.0 ns            8    5.19853     9.55611m     452.061u            7.08232m      9.05187Mi/s
bench_sha3_256/512_cv                 1.60 %          1.60 %             8      0.13%        0.13%        0.00%               0.13%            1.64%
bench_sha3_384/512                    1129 ns         1129 ns       123971   4.94631k      8.83269      28.811k             5.82475      473.071Mi/s
bench_sha3_384/512                    1154 ns         1154 ns       123971   4.97941k      8.89181      28.811k             5.78602       462.95Mi/s
bench_sha3_384/512                    1151 ns         1151 ns       123971   4.97367k      8.88156      28.811k              5.7927      464.015Mi/s
bench_sha3_384/512                    1153 ns         1153 ns       123971   4.95915k      8.85563      28.811k             5.80966      463.266Mi/s
bench_sha3_384/512                    1152 ns         1152 ns       123971   4.96706k      8.86976      28.811k             5.80041      463.751Mi/s
bench_sha3_384/512                    1151 ns         1151 ns       123971   4.94862k      8.83682      28.811k             5.82203       464.18Mi/s
bench_sha3_384/512                    1148 ns         1148 ns       123971   4.95369k      8.84588      28.811k             5.81607      465.371Mi/s
bench_sha3_384/512                    1154 ns         1153 ns       123971   4.96751k      8.87055      28.811k             5.79989      463.003Mi/s
bench_sha3_384/512_mean               1149 ns         1149 ns            8   4.96193k      8.86059      28.811k             5.80644      464.951Mi/s
bench_sha3_384/512_median             1151 ns         1151 ns            8   4.96311k      8.86269      28.811k             5.80504      463.883Mi/s
bench_sha3_384/512_stddev             8.21 ns         8.22 ns            8    11.9396    0.0213207     369.106u           0.0139703      3.37336Mi/s
bench_sha3_384/512_cv                 0.71 %          0.72 %             8      0.24%        0.24%        0.00%               0.24%            0.73%
bench_sha3_384/256                     694 ns          694 ns       202733   2.99087k       9.8384      17.321k             5.79129      417.827Mi/s
bench_sha3_384/256                     692 ns          692 ns       202733   2.98392k      9.81552      17.321k             5.80479      419.235Mi/s
bench_sha3_384/256                     694 ns          694 ns       202733    2.9888k      9.83159      17.321k             5.79529      417.945Mi/s
bench_sha3_384/256                     693 ns          693 ns       202733   2.98876k      9.83146      17.321k             5.79537      418.465Mi/s
bench_sha3_384/256                     694 ns          694 ns       202733   2.99737k      9.85976      17.321k             5.77874       417.86Mi/s
bench_sha3_384/256                     695 ns          695 ns       202733   2.99579k      9.85458      17.321k             5.78177      417.352Mi/s
bench_sha3_384/256                     697 ns          697 ns       202733   2.99579k      9.85458      17.321k             5.78178      415.844Mi/s
bench_sha3_384/256                     698 ns          698 ns       202733   3.00214k      9.87547      17.321k             5.76955       415.54Mi/s
bench_sha3_384/256_mean                694 ns          694 ns            8   2.99293k      9.84517      17.321k             5.78732      417.509Mi/s
bench_sha3_384/256_median              694 ns          694 ns            8   2.99333k      9.84649      17.321k             5.78653      417.844Mi/s
bench_sha3_384/256_stddev             2.08 ns         2.09 ns            8    5.86501    0.0192928            0           0.0113405      1.25253Mi/s
bench_sha3_384/256_cv                 0.30 %          0.30 %             8      0.20%        0.20%        0.00%               0.20%            0.30%
bench_sha3_224/1024                   1808 ns         1808 ns        75807   8.13786k      7.73561      45.731k             5.61954      555.033Mi/s
bench_sha3_224/1024                   1849 ns         1848 ns        75807   8.16359k      7.76006      45.731k             5.60183      542.886Mi/s
bench_sha3_224/1024                   1866 ns         1866 ns        75807   8.15699k      7.75379      45.731k             5.60636      537.752Mi/s
bench_sha3_224/1024                   1891 ns         1891 ns        75807   8.18312k      7.77863      45.731k             5.58846      530.451Mi/s
bench_sha3_224/1024                   1848 ns         1848 ns        75807   8.14668k        7.744      45.731k             5.61345      542.951Mi/s
bench_sha3_224/1024                   1880 ns         1880 ns        75807   8.17702k      7.77283      45.731k             5.59263      533.691Mi/s
bench_sha3_224/1024                   1891 ns         1890 ns        75807   8.16508k      7.76149      45.731k              5.6008      530.707Mi/s
bench_sha3_224/1024                   1877 ns         1877 ns        75807   8.16864k      7.76487      45.731k             5.59836      534.622Mi/s
bench_sha3_224/1024_mean              1864 ns         1863 ns            8   8.16237k      7.75891      45.731k             5.60268      538.512Mi/s
bench_sha3_224/1024_median            1871 ns         1871 ns            8   8.16434k      7.76078      45.731k             5.60131      536.187Mi/s
bench_sha3_224/1024_stddev            28.1 ns         28.2 ns            8    14.9709    0.0142309     738.212u           0.0102813      8.25164Mi/s
bench_sha3_224/1024_cv                1.51 %          1.51 %             8      0.18%        0.18%        0.00%               0.18%            1.53%
bench_shake256/256/32                  446 ns          446 ns       313572   2.04762k       7.1098      11.673k             5.70076      615.612Mi/s
bench_shake256/256/32                  471 ns          471 ns       313572   2.04535k      7.10189      11.673k             5.70711      582.648Mi/s
bench_shake256/256/32                  476 ns          476 ns       313572   2.04809k      7.11142      11.673k             5.69946      576.829Mi/s
bench_shake256/256/32                  474 ns          474 ns       313572   2.05235k      7.12621      11.673k             5.68764       579.05Mi/s
bench_shake256/256/32                  476 ns          476 ns       313572   2.04568k      7.10305      11.673k             5.70618      576.946Mi/s
bench_shake256/256/32                  474 ns          474 ns       313572    2.0495k      7.11632      11.673k             5.69554      579.987Mi/s
bench_shake256/256/32                  475 ns          475 ns       313572   2.04718k      7.10828      11.673k             5.70198      577.913Mi/s
bench_shake256/256/32                  475 ns          475 ns       313572   2.05569k      7.13783      11.673k             5.67837      578.228Mi/s
bench_shake256/256/32_mean             471 ns          471 ns            8   2.04893k      7.11435      11.673k             5.69713      583.402Mi/s
bench_shake256/256/32_median           475 ns          475 ns            8   2.04786k      7.11061      11.673k             5.70011      578.639Mi/s
bench_shake256/256/32_stddev          10.1 ns         10.2 ns            8    3.51827    0.0122162            0            9.76837m      13.1499Mi/s
bench_shake256/256/32_cv              2.15 %          2.16 %             8      0.17%        0.17%        0.00%               0.17%            2.25%
bench_sha3_512/32                      246 ns          246 ns       568328   1.05913k      11.0326       5.758k             5.43655      372.544Mi/s
bench_sha3_512/32                      245 ns          245 ns       568328   1.05944k      11.0358       5.758k             5.43496       373.34Mi/s
bench_sha3_512/32                      247 ns          247 ns       568328   1.06147k      11.0569       5.758k             5.42458      370.406Mi/s
bench_sha3_512/32                      246 ns          246 ns       568328   1.06033k      11.0451       5.758k             5.43036        371.5Mi/s
bench_sha3_512/32                      246 ns          246 ns       568328   1.05814k      11.0223       5.758k             5.44164      371.789Mi/s
bench_sha3_512/32                      243 ns          243 ns       568328   1.05642k      11.0044       5.758k             5.45048      376.092Mi/s
bench_sha3_512/32                      244 ns          244 ns       568328   1.05087k      10.9466       5.758k             5.47925      374.682Mi/s
bench_sha3_512/32                      246 ns          246 ns       568328   1.06326k      11.0757       5.758k             5.41541      372.672Mi/s
bench_sha3_512/32_mean                 246 ns          246 ns            8   1.05863k      11.0274       5.758k             5.43915      372.878Mi/s
bench_sha3_512/32_median               246 ns          246 ns            8   1.05928k      11.0342       5.758k             5.43576      372.608Mi/s
bench_sha3_512/32_stddev              1.20 ns         1.19 ns            8    3.75286    0.0390923            0           0.0193472      1.81841Mi/s
bench_sha3_512/32_cv                  0.49 %          0.49 %             8      0.35%        0.35%        0.00%               0.36%            0.49%
bench_sha3_512/128                     445 ns          444 ns       315947   1.97571k      10.2902       11.48k             5.81056      411.945Mi/s
bench_sha3_512/128                     453 ns          453 ns       315947   1.97017k      10.2613       11.48k              5.8269      404.503Mi/s
bench_sha3_512/128                     453 ns          453 ns       315947   1.96734k      10.2466       11.48k             5.83528      404.043Mi/s
bench_sha3_512/128                     459 ns          459 ns       315947   1.97167k      10.2691       11.48k             5.82249      398.734Mi/s
bench_sha3_512/128                     458 ns          458 ns       315947   1.97564k      10.2898       11.48k             5.81079      399.558Mi/s
bench_sha3_512/128                     460 ns          460 ns       315947   1.96894k      10.2549       11.48k             5.83054      398.352Mi/s
bench_sha3_512/128                     456 ns          456 ns       315947   1.97755k      10.2997       11.48k             5.80516      401.879Mi/s
bench_sha3_512/128                     458 ns          458 ns       315947   1.97098k      10.2655       11.48k             5.82452      400.118Mi/s
bench_sha3_512/128_mean                455 ns          455 ns            8   1.97225k      10.2721       11.48k             5.82078      402.392Mi/s
bench_sha3_512/128_median              457 ns          457 ns            8   1.97132k      10.2673       11.48k              5.8235      400.999Mi/s
bench_sha3_512/128_stddev             4.95 ns         5.02 ns            8    3.64178    0.0189676            0           0.0107444       4.4979Mi/s
bench_sha3_512/128_cv                 1.09 %          1.10 %             8      0.18%        0.18%        0.00%               0.18%            1.12%
bench_sha3_512/4096                  12287 ns        12285 ns        11351   55.5471k      13.3527      324.21k             5.83667      322.945Mi/s
bench_sha3_512/4096                  12697 ns        12695 ns        11351   55.4995k      13.3412      324.21k             5.84167      312.509Mi/s
bench_sha3_512/4096                  12769 ns        12768 ns        11351   55.6213k      13.3705      324.21k             5.82888      310.714Mi/s
bench_sha3_512/4096                  12716 ns        12716 ns        11351   55.7466k      13.4006      324.21k             5.81579      311.994Mi/s
bench_sha3_512/4096                  12762 ns        12761 ns        11351   55.7131k      13.3926      324.21k             5.81928      310.903Mi/s
bench_sha3_512/4096                  12846 ns        12845 ns        11351   55.5099k      13.3437      324.21k             5.84058      308.849Mi/s
bench_sha3_512/4096                  12855 ns        12854 ns        11351   55.6569k      13.3791      324.21k             5.82516      308.638Mi/s
bench_sha3_512/4096                  12856 ns        12855 ns        11351   55.5487k       13.353      324.21k             5.83651      308.623Mi/s
bench_sha3_512/4096_mean             12723 ns        12722 ns            8   55.6054k      13.3667      324.21k             5.83057      311.897Mi/s
bench_sha3_512/4096_median           12765 ns        12764 ns            8    55.585k      13.3618      324.21k             5.83269      310.808Mi/s
bench_sha3_512/4096_stddev             187 ns          187 ns            8    93.6329    0.0225079     4.17596m            9.81293m      4.71257Mi/s
bench_sha3_512/4096_cv                1.47 %          1.47 %             8      0.17%        0.17%        0.00%               0.17%            1.51%
bench_shake128/512/64                  951 ns          951 ns       146450   4.11199k      7.13887      23.089k             5.61504      577.709Mi/s
bench_shake128/512/64                  948 ns          948 ns       146450   4.10705k       7.1303      23.089k             5.62179      579.655Mi/s
bench_shake128/512/64                  949 ns          949 ns       146450   4.10198k       7.1215      23.089k             5.62874       578.96Mi/s
bench_shake128/512/64                  949 ns          949 ns       146450   4.10341k      7.12397      23.089k             5.62679      578.668Mi/s
bench_shake128/512/64                  951 ns          951 ns       146450   4.10349k      7.12412      23.089k             5.62667      577.905Mi/s
bench_shake128/512/64                  940 ns          940 ns       146450   4.11279k      7.14025      23.089k             5.61396      584.159Mi/s
bench_shake128/512/64                  942 ns          942 ns       146450   4.10421k      7.12537      23.089k             5.62569      583.058Mi/s
bench_shake128/512/64                  943 ns          943 ns       146450   4.10792k      7.13181      23.089k              5.6206      582.287Mi/s
bench_shake128/512/64_mean             947 ns          947 ns            8   4.10661k      7.12952      23.089k             5.62241        580.3Mi/s
bench_shake128/512/64_median           948 ns          948 ns            8   4.10563k      7.12783      23.089k             5.62374      579.307Mi/s
bench_shake128/512/64_stddev          4.09 ns         4.07 ns            8    4.07396     7.07285m     521.995u            5.57517m      2.50064Mi/s
bench_shake128/512/64_cv              0.43 %          0.43 %             8      0.10%        0.10%        0.00%               0.10%            0.43%
bench_shake128/64/32                   254 ns          254 ns       546382   1.15041k      11.9834       5.951k             5.17294      360.498Mi/s
bench_shake128/64/32                   255 ns          255 ns       546382    1.1493k      11.9719       5.951k             5.17791      359.014Mi/s
bench_shake128/64/32                   258 ns          258 ns       546382   1.14523k      11.9294       5.951k             5.19636      354.189Mi/s
bench_shake128/64/32                   263 ns          263 ns       546382   1.15275k      12.0078       5.951k             5.16245       348.09Mi/s
bench_shake128/64/32                   265 ns          265 ns       546382   1.15185k      11.9984       5.951k             5.16648      344.992Mi/s
bench_shake128/64/32                   257 ns          257 ns       546382    1.1467k      11.9448       5.951k             5.18969      356.675Mi/s
bench_shake128/64/32                   263 ns          263 ns       546382   1.14764k      11.9545       5.951k             5.18545      348.159Mi/s
bench_shake128/64/32                   262 ns          262 ns       546382   1.15187k      11.9987       5.951k             5.16638      349.253Mi/s
bench_shake128/64/32_mean              260 ns          260 ns            8   1.14947k      11.9736       5.951k             5.17721      352.609Mi/s
bench_shake128/64/32_median            260 ns          260 ns            8   1.14986k      11.9777       5.951k             5.17542      351.721Mi/s
bench_shake128/64/32_stddev           4.23 ns         4.23 ns            8    2.72945    0.0284318     130.499u           0.0123018      5.75442Mi/s
bench_shake128/64/32_cv               1.63 %          1.63 %             8      0.24%        0.24%        0.00%               0.24%            1.63%
bench_shake128/128/64                  251 ns          251 ns       561308   1.15526k      6.01695       5.961k              5.1599      728.734Mi/s
bench_shake128/128/64                  265 ns          265 ns       561308   1.15419k       6.0114       5.961k             5.16467      691.773Mi/s
bench_shake128/128/64                  264 ns          263 ns       561308   1.14981k       5.9886       5.961k             5.18433      694.925Mi/s
bench_shake128/128/64                  260 ns          260 ns       561308   1.14522k      5.96468       5.961k             5.20512      704.042Mi/s
bench_shake128/128/64                  264 ns          264 ns       561308   1.15062k      5.99279       5.961k             5.18071      694.583Mi/s
bench_shake128/128/64                  262 ns          262 ns       561308   1.15152k      5.99748       5.961k             5.17666      699.203Mi/s
bench_shake128/128/64                  266 ns          266 ns       561308   1.15246k      6.00238       5.961k             5.17242       688.71Mi/s
bench_shake128/128/64                  264 ns          264 ns       561308   1.15199k      5.99997       5.961k              5.1745      693.864Mi/s
bench_shake128/128/64_mean             262 ns          262 ns            8   1.15138k      5.99678       5.961k             5.17729      699.479Mi/s
bench_shake128/128/64_median           264 ns          264 ns            8   1.15176k      5.99872       5.961k             5.17558      694.754Mi/s
bench_shake128/128/64_stddev          4.62 ns         4.62 ns            8    3.05805    0.0159274            0           0.0137797      12.6961Mi/s
bench_shake128/128/64_cv              1.76 %          1.76 %             8      0.27%        0.27%        0.00%               0.27%            1.82%
bench_sha3_512/64                      238 ns          238 ns       590630   1.02744k      8.02691       5.758k              5.6042      513.882Mi/s
bench_sha3_512/64                      239 ns          239 ns       590630   1.02585k      8.01446       5.758k              5.6129      511.296Mi/s
bench_sha3_512/64                      240 ns          240 ns       590630   1.02703k      8.02368       5.758k             5.60645      508.794Mi/s
bench_sha3_512/64                      239 ns          239 ns       590630   1.02356k      7.99656       5.758k             5.62547      511.403Mi/s
bench_sha3_512/64                      245 ns          244 ns       590630   1.05111k      8.21178       5.758k             5.47803      499.375Mi/s
bench_sha3_512/64                      240 ns          240 ns       590630   1.02769k       8.0288       5.758k             5.60288      507.831Mi/s
bench_sha3_512/64                      239 ns          239 ns       590630   1.02623k       8.0174       5.758k             5.61084      509.857Mi/s
bench_sha3_512/64                      238 ns          238 ns       590630   1.02177k      7.98254       5.758k             5.63534      513.217Mi/s
bench_sha3_512/64_mean                 240 ns          240 ns            8   1.02883k      8.03777       5.758k             5.59701      509.457Mi/s
bench_sha3_512/64_median               239 ns          239 ns            8   1.02663k      8.02054       5.758k             5.60865      510.576Mi/s
bench_sha3_512/64_stddev              2.18 ns         2.17 ns            8    9.22815    0.0720949            0           0.0493602      4.56342Mi/s
bench_sha3_512/64_cv                  0.91 %          0.91 %             8      0.90%        0.90%        0.00%               0.88%            0.90%
bench_shake256/128/64                  262 ns          262 ns       533318    1.1436k      5.95623       5.942k             5.19589      699.156Mi/s
bench_shake256/128/64                  262 ns          262 ns       533318   1.14583k      5.96786       5.942k             5.18576      699.212Mi/s
bench_shake256/128/64                  263 ns          263 ns       533318   1.14452k      5.96106       5.942k             5.19168      696.529Mi/s
bench_shake256/128/64                  265 ns          265 ns       533318   1.14235k      5.94973       5.942k             5.20157      691.289Mi/s
bench_shake256/128/64                  260 ns          260 ns       533318   1.13944k      5.93459       5.942k             5.21484      703.702Mi/s
bench_shake256/128/64                  261 ns          261 ns       533318    1.1415k       5.9453       5.942k             5.20544      701.869Mi/s
bench_shake256/128/64                  264 ns          264 ns       533318   1.14099k      5.94267       5.942k             5.20775      694.868Mi/s
bench_shake256/128/64                  262 ns          262 ns       533318   1.14141k      5.94486       5.942k             5.20583      697.896Mi/s
bench_shake256/128/64_mean             262 ns          262 ns            8   1.14246k      5.95029       5.942k             5.20109      698.065Mi/s
bench_shake256/128/64_median           262 ns          262 ns            8   1.14192k      5.94752       5.942k              5.2035      698.526Mi/s
bench_shake256/128/64_stddev          1.49 ns         1.47 ns            8    2.07737    0.0108196            0            9.45293m       3.9146Mi/s
bench_shake256/128/64_cv              0.57 %          0.56 %             8      0.18%        0.18%        0.00%               0.18%            0.56%
bench_shake128/128/32                  263 ns          263 ns       534853   1.15522k       7.2201       5.961k             5.16008      579.514Mi/s
bench_shake128/128/32                  262 ns          262 ns       534853   1.15238k      7.20237       5.961k             5.17277      581.548Mi/s
bench_shake128/128/32                  264 ns          264 ns       534853   1.15408k        7.213       5.961k             5.16515      577.894Mi/s
bench_shake128/128/32                  266 ns          266 ns       534853   1.15545k      7.22159       5.961k             5.15901      574.238Mi/s
bench_shake128/128/32                  266 ns          266 ns       534853   1.15742k      7.23387       5.961k             5.15025      573.475Mi/s
bench_shake128/128/32                  267 ns          267 ns       534853   1.15668k      7.22926       5.961k             5.15354       571.64Mi/s
bench_shake128/128/32                  264 ns          264 ns       534853    1.1519k      7.19938       5.961k             5.17493      578.849Mi/s
bench_shake128/128/32                  265 ns          265 ns       534853   1.15421k       7.2138       5.961k             5.16458      575.266Mi/s
bench_shake128/128/32_mean             265 ns          265 ns            8   1.15467k      7.21667       5.961k             5.16254      576.553Mi/s
bench_shake128/128/32_median           265 ns          265 ns            8   1.15471k      7.21695       5.961k             5.16233       576.58Mi/s
bench_shake128/128/32_stddev          1.57 ns         1.57 ns            8    1.92684    0.0120428            0            8.61607m      3.41036Mi/s
bench_shake128/128/32_cv              0.59 %          0.59 %             8      0.17%        0.17%        0.00%               0.17%            0.59%
bench_sha3_256/64                      254 ns          254 ns       549568   1.13734k      11.8473       5.865k             5.15678      361.049Mi/s
bench_sha3_256/64                      257 ns          257 ns       549568   1.13777k      11.8518       5.865k             5.15482      355.715Mi/s
bench_sha3_256/64                      260 ns          260 ns       549568   1.13966k      11.8714       5.865k             5.14629      352.118Mi/s
bench_sha3_256/64                      261 ns          261 ns       549568   1.13834k      11.8577       5.865k             5.15222      350.441Mi/s
bench_sha3_256/64                      261 ns          261 ns       549568   1.13428k      11.8154       5.865k              5.1707      350.473Mi/s
bench_sha3_256/64                      262 ns          262 ns       549568   1.13646k      11.8381       5.865k             5.16076      349.828Mi/s
bench_sha3_256/64                      257 ns          257 ns       549568   1.13717k      11.8455       5.865k             5.15754      356.615Mi/s
bench_sha3_256/64                      260 ns          260 ns       549568   1.13808k       11.855       5.865k             5.15342      352.724Mi/s
bench_sha3_256/64_mean                 259 ns          259 ns            8   1.13739k      11.8478       5.865k             5.15657       353.62Mi/s
bench_sha3_256/64_median               260 ns          260 ns            8   1.13755k      11.8495       5.865k              5.1558      352.421Mi/s
bench_sha3_256/64_stddev              2.82 ns         2.83 ns            8    1.57074    0.0163618            0            7.12779m      3.89144Mi/s
bench_sha3_256/64_cv                  1.09 %          1.09 %             8      0.14%        0.14%        0.00%               0.14%            1.10%
bench_sha3_224/256                     453 ns          453 ns       307815   2.05884k      7.24943      11.596k             5.63231      597.848Mi/s
bench_sha3_224/256                     478 ns          478 ns       307815   2.06341k      7.26552      11.596k             5.61983      566.594Mi/s
bench_sha3_224/256                     478 ns          478 ns       307815   2.05915k      7.25052      11.596k             5.63145      566.115Mi/s
bench_sha3_224/256                     473 ns          473 ns       307815   2.05928k      7.25098      11.596k              5.6311       573.14Mi/s
bench_sha3_224/256                     473 ns          473 ns       307815   2.06495k      7.27096      11.596k             5.61563       572.36Mi/s
bench_sha3_224/256                     472 ns          472 ns       307815   2.06195k       7.2604      11.596k             5.62379      574.353Mi/s
bench_sha3_224/256                     477 ns          477 ns       307815   2.06186k      7.26006      11.596k             5.62406      568.132Mi/s
bench_sha3_224/256                     476 ns          476 ns       307815   2.06538k      7.27248      11.596k             5.61445      569.291Mi/s
bench_sha3_224/256_mean                472 ns          472 ns            8   2.06185k      7.26004      11.596k             5.62408      573.479Mi/s
bench_sha3_224/256_median              475 ns          474 ns            8   2.06191k      7.26023      11.596k             5.62392      570.826Mi/s
bench_sha3_224/256_stddev             8.22 ns         8.23 ns            8    2.60563     9.17476m     260.997u            7.10638m      10.3076Mi/s
bench_sha3_224/256_cv                 1.74 %          1.74 %             8      0.13%        0.13%        0.00%               0.13%            1.80%
bench_shake128/256/32                  463 ns          463 ns       300411   2.06688k      7.17668      11.686k             5.65392      593.762Mi/s
bench_shake128/256/32                  494 ns          493 ns       300411    1.9146k      6.64793     10.8522k             5.66811      556.698Mi/s
bench_shake128/256/32                  479 ns          479 ns       300411    2.0662k       7.1743      11.686k              5.6558      573.813Mi/s
bench_shake128/256/32                  478 ns          478 ns       300411   2.06122k      7.15702      11.686k             5.66946      574.901Mi/s
bench_shake128/256/32                  477 ns          477 ns       300411   2.05966k      7.15161      11.686k             5.67374      575.314Mi/s
bench_shake128/256/32                  479 ns          479 ns       300411   2.07099k      7.19092      11.686k             5.64273      573.607Mi/s
bench_shake128/256/32                  474 ns          474 ns       300411   2.06101k      7.15628      11.686k             5.67004      578.896Mi/s
bench_shake128/256/32                  475 ns          475 ns       300411   2.06286k      7.16271      11.686k             5.66495      578.448Mi/s
bench_shake128/256/32_mean             477 ns          477 ns            8   2.04543k      7.10218     11.5818k             5.66234       575.68Mi/s
bench_shake128/256/32_median           478 ns          478 ns            8   2.06204k      7.15987      11.686k             5.66653      575.107Mi/s
bench_shake128/256/32_stddev          8.43 ns         8.40 ns            8    52.9943     0.184008      294.799           0.0105449      10.0982Mi/s
bench_shake128/256/32_cv              1.77 %          1.76 %             8      2.59%        2.59%        2.55%               0.19%            1.75%
bench_sha3_256/1024                   1862 ns         1862 ns        75004   8.22405k      7.78793      45.882k               5.579      540.891Mi/s
bench_sha3_256/1024                   1908 ns         1908 ns        75004   8.24346k       7.8063      45.882k             5.56587      527.852Mi/s
bench_sha3_256/1024                   1901 ns         1900 ns        75004   8.19905k      7.76425      45.882k             5.59601      529.932Mi/s
bench_sha3_256/1024                   1887 ns         1887 ns        75004   8.22901k      7.79263      45.882k             5.57564      533.585Mi/s
bench_sha3_256/1024                   1870 ns         1870 ns        75004   8.22206k      7.78604      45.882k             5.58036      538.502Mi/s
bench_sha3_256/1024                   1891 ns         1891 ns        75004   8.23644k      7.79966      45.882k             5.57061      532.645Mi/s
bench_sha3_256/1024                   1897 ns         1897 ns        75004    8.2098k      7.77443      45.882k             5.58869      530.856Mi/s
bench_sha3_256/1024                   1897 ns         1897 ns        75004   8.21271k      7.77719      45.882k              5.5867      530.864Mi/s
bench_sha3_256/1024_mean              1889 ns         1889 ns            8   8.22207k      7.78605      45.882k             5.58036      533.141Mi/s
bench_sha3_256/1024_median            1894 ns         1894 ns            8   8.22305k      7.78698      45.882k             5.57968      531.754Mi/s
bench_sha3_256/1024_stddev            15.7 ns         15.6 ns            8    14.5599    0.0137878            0            9.88339m      4.43873Mi/s
bench_sha3_256/1024_cv                0.83 %          0.83 %             8      0.18%        0.18%        0.00%               0.18%            0.83%
bench_shake128/32/64                   254 ns          254 ns       541592   1.15858k      12.0685       5.951k             5.13648      361.025Mi/s
bench_shake128/32/64                   256 ns          256 ns       541592   1.15959k      12.0791       5.951k             5.13197      357.483Mi/s
bench_shake128/32/64                   268 ns          268 ns       541592   1.16444k      12.1296       5.951k             5.11062      341.138Mi/s
bench_shake128/32/64                   266 ns          266 ns       541592   1.15961k      12.0792       5.951k             5.13192       344.43Mi/s
bench_shake128/32/64                   267 ns          267 ns       541592     1.161k      12.0938       5.951k             5.12575      343.516Mi/s
bench_shake128/32/64                   265 ns          265 ns       541592   1.15638k      12.0456       5.951k             5.14623      344.944Mi/s
bench_shake128/32/64                   267 ns          267 ns       541592   1.16016k       12.085       5.951k             5.12948      342.505Mi/s
bench_shake128/32/64                   266 ns          266 ns       541592   1.15624k      12.0442       5.951k             5.14685      344.569Mi/s
bench_shake128/32/64_mean              264 ns          264 ns            8    1.1595k      12.0781       5.951k             5.13241      347.451Mi/s
bench_shake128/32/64_median            266 ns          266 ns            8    1.1596k      12.0792       5.951k             5.13194        344.5Mi/s
bench_shake128/32/64_stddev           5.53 ns         5.53 ns            8    2.62661    0.0273606            0           0.0116145      7.44906Mi/s
bench_shake128/32/64_cv               2.10 %          2.10 %             8      0.23%        0.23%        0.00%               0.23%            2.14%
bench_shake256/2048/32                3755 ns         3755 ns        37500   16.5377k      7.95082      91.618k             5.53995      528.293Mi/s
bench_shake256/2048/32                3721 ns         3720 ns        37500   16.4569k      7.91198      91.618k             5.56714       533.24Mi/s
bench_shake256/2048/32                3813 ns         3812 ns        37500   16.4698k      7.91819      91.618k             5.56277      520.421Mi/s
bench_shake256/2048/32                3722 ns         3722 ns        37500   16.4005k      7.88483      91.618k             5.58631      532.909Mi/s
bench_shake256/2048/32                3798 ns         3798 ns        37500   16.4644k      7.91556      91.618k             5.56462      522.313Mi/s
bench_shake256/2048/32                3800 ns         3800 ns        37500   16.4719k      7.91918      91.618k             5.56208      522.023Mi/s
bench_shake256/2048/32                3770 ns         3770 ns        37500   16.4583k      7.91265      91.618k             5.56667       526.21Mi/s
bench_shake256/2048/32                3802 ns         3801 ns        37500   16.4908k      7.92825      91.618k             5.55572      521.829Mi/s
bench_shake256/2048/32_mean           3773 ns         3772 ns            8   16.4688k      7.91768      91.618k             5.56316      525.905Mi/s
bench_shake256/2048/32_median         3784 ns         3784 ns            8   16.4671k      7.91688      91.618k              5.5637      524.262Mi/s
bench_shake256/2048/32_stddev         36.5 ns         36.6 ns            8    38.1409     0.018337     1.04399m            0.012883      5.11756Mi/s
bench_shake256/2048/32_cv             0.97 %          0.97 %             8      0.23%        0.23%        0.00%               0.23%            0.97%
bench_shake128/2048/64                2973 ns         2972 ns        46785   13.2359k      6.26698      74.442k             5.62427      677.631Mi/s
bench_shake128/2048/64                3053 ns         3052 ns        46785   13.2745k      6.28526      74.442k             5.60791      659.863Mi/s
bench_shake128/2048/64                3073 ns         3073 ns        46785   13.2529k      6.27503      74.442k             5.61705      655.399Mi/s
bench_shake128/2048/64                3012 ns         3012 ns        46785   13.2673k      6.28185      74.442k             5.61095       668.69Mi/s
bench_shake128/2048/64                3017 ns         3017 ns        46785   13.2662k      6.28133      74.442k             5.61142      667.614Mi/s
bench_shake128/2048/64                3024 ns         3024 ns        46785    13.275k      6.28552      74.442k             5.60767      666.145Mi/s
bench_shake128/2048/64                3064 ns         3064 ns        46785   13.2837k      6.28962      74.442k             5.60402      657.454Mi/s
bench_shake128/2048/64                3077 ns         3077 ns        46785   13.2807k      6.28822      74.442k             5.60527      654.494Mi/s
bench_shake128/2048/64_mean           3037 ns         3036 ns            8    13.267k      6.28173      74.442k             5.61107      663.411Mi/s
bench_shake128/2048/64_median         3038 ns         3038 ns            8   13.2709k      6.28356      74.442k             5.60943      663.004Mi/s
bench_shake128/2048/64_stddev         36.2 ns         36.4 ns            8    15.8425     7.50117m            0             6.7075m      7.99112Mi/s
bench_shake128/2048/64_cv             1.19 %          1.20 %             8      0.12%        0.12%        0.00%               0.12%            1.20%
bench_sha3_384/128                     467 ns          467 ns       300583   2.01023k      11.4218       11.58k             5.76054      359.382Mi/s
bench_sha3_384/128                     464 ns          464 ns       300583   2.00356k      11.3839       11.58k             5.77972      361.737Mi/s
bench_sha3_384/128                     467 ns          467 ns       300583   2.00418k      11.3874       11.58k             5.77793      359.337Mi/s
bench_sha3_384/128                     467 ns          467 ns       300583   2.00667k      11.4015       11.58k             5.77075      359.625Mi/s
bench_sha3_384/128                     463 ns          463 ns       300583   2.00194k      11.3746       11.58k              5.7844      362.218Mi/s
bench_sha3_384/128                     467 ns          467 ns       300583     2.009k      11.4148       11.58k             5.76406      359.721Mi/s
bench_sha3_384/128                     464 ns          464 ns       300583   2.00226k      11.3765       11.58k             5.78348      362.001Mi/s
bench_sha3_384/128                     464 ns          465 ns       300583   2.00295k      11.3804       11.58k             5.78147      361.341Mi/s
bench_sha3_384/128_mean                465 ns          465 ns            8    2.0051k      11.3926       11.58k             5.77529       360.67Mi/s
bench_sha3_384/128_median              466 ns          466 ns            8   2.00387k      11.3856       11.58k             5.77883      360.531Mi/s
bench_sha3_384/128_stddev             1.63 ns         1.63 ns            8    3.16087    0.0179595            0            9.09587m      1.26423Mi/s
bench_sha3_384/128_cv                 0.35 %          0.35 %             8      0.16%        0.16%        0.00%               0.16%            0.35%
bench_sha3_512/512                    1765 ns         1765 ns        79253   7.84131k      13.6134      45.597k             5.81498      311.222Mi/s
bench_sha3_512/512                    1790 ns         1790 ns        79253   7.81349k      13.5651      45.597k             5.83568      306.903Mi/s
bench_sha3_512/512                    1861 ns         1853 ns        79253   7.88219k      13.6844      45.597k             5.78481      296.479Mi/s
bench_sha3_512/512                    1788 ns         1788 ns        79253   7.80364k       13.548      45.597k             5.84304      307.171Mi/s
bench_sha3_512/512                    1812 ns         1812 ns        79253   7.84175k      13.6141      45.597k             5.81465      303.228Mi/s
bench_sha3_512/512                    1812 ns         1811 ns        79253   7.80305k       13.547      45.597k             5.84348      303.252Mi/s
bench_sha3_512/512                    1811 ns         1811 ns        79253   7.81176k      13.5621      45.597k             5.83697      303.366Mi/s
bench_sha3_512/512                    1811 ns         1811 ns        79253   7.82915k      13.5923      45.597k               5.824       303.25Mi/s
bench_sha3_512/512_mean               1806 ns         1805 ns            8   7.82829k      13.5908      45.597k              5.8247      304.359Mi/s
bench_sha3_512/512_median             1811 ns         1811 ns            8   7.82132k      13.5787      45.597k             5.82984      303.309Mi/s
bench_sha3_512/512_stddev             27.8 ns         25.5 ns            8    26.7082    0.0463683     521.995u           0.0198107      4.28106Mi/s
bench_sha3_512/512_cv                 1.54 %          1.41 %             8      0.34%        0.34%        0.00%               0.34%            1.41%
bench_sha3_224/4096                   6639 ns         6638 ns        21047   29.5269k      7.15978     165.243k             5.59635      592.501Mi/s
bench_sha3_224/4096                   6615 ns         6615 ns        21047   29.4541k      7.14211     165.243k             5.61019      594.583Mi/s
bench_sha3_224/4096                   6777 ns         6775 ns        21047   29.5562k      7.16688     165.243k              5.5908      580.499Mi/s
bench_sha3_224/4096                   6747 ns         6747 ns        21047   29.5359k      7.16195     165.243k             5.59465      582.925Mi/s
bench_sha3_224/4096                   6799 ns         6798 ns        21047   29.5003k      7.15332     165.243k             5.60141      578.539Mi/s
bench_sha3_224/4096                   6814 ns         6813 ns        21047    29.457k      7.14282     165.243k             5.60964      577.234Mi/s
bench_sha3_224/4096                   6793 ns         6793 ns        21047   29.5835k       7.1735     165.243k             5.58564      579.005Mi/s
bench_sha3_224/4096                   6745 ns         6746 ns        21047   29.5103k      7.15574     165.243k             5.59951      583.003Mi/s
bench_sha3_224/4096_mean              6741 ns         6741 ns            8   29.5155k      7.15701     165.243k             5.59852      583.536Mi/s
bench_sha3_224/4096_median            6762 ns         6761 ns            8   29.5186k      7.15776     165.243k             5.59793      581.712Mi/s
bench_sha3_224/4096_stddev            74.6 ns         74.6 ns            8    45.1796    0.0109553            0            8.57063m      6.51917Mi/s
bench_sha3_224/4096_cv                1.11 %          1.11 %             8      0.15%        0.15%        0.00%               0.15%            1.12%
bench_sha3_512/256                     864 ns          864 ns       161872   3.85472k       12.046     22.4559k             5.82556      353.264Mi/s
bench_sha3_512/256                     907 ns          907 ns       161872   3.92448k       12.264      22.852k             5.82294       336.53Mi/s
bench_sha3_512/256                     906 ns          906 ns       161872   3.90813k      12.2129      22.852k             5.84731      336.837Mi/s
bench_sha3_512/256                     905 ns          905 ns       161872   3.93087k       12.284      22.852k             5.81347      337.098Mi/s
bench_sha3_512/256                     913 ns          913 ns       161872   3.91777k       12.243      22.852k             5.83292        334.4Mi/s
bench_sha3_512/256                     911 ns          911 ns       161872   3.91886k      12.2464      22.852k             5.83129       335.14Mi/s
bench_sha3_512/256                     908 ns          908 ns       161872   3.93629k      12.3009      22.852k             5.80547      336.012Mi/s
bench_sha3_512/256                     912 ns          912 ns       161872   3.93226k      12.2883      22.852k             5.81142      334.511Mi/s
bench_sha3_512/256_mean                903 ns          903 ns            8   3.91542k      12.2357     22.8025k              5.8238      337.974Mi/s
bench_sha3_512/256_median              908 ns          908 ns            8   3.92167k      12.2552      22.852k             5.82425      336.271Mi/s
bench_sha3_512/256_stddev             16.1 ns         16.1 ns            8    26.1728      0.08179      140.049           0.0135831      6.26316Mi/s
bench_sha3_512/256_cv                 1.78 %          1.79 %             8      0.67%        0.67%        0.61%               0.23%            1.85%
bench_shake256/512/32                  937 ns          937 ns       148713   4.11863k      7.57102      23.095k             5.60744      553.771Mi/s
bench_shake256/512/32                  946 ns          946 ns       148713   4.11955k       7.5727      23.095k              5.6062      548.649Mi/s
bench_shake256/512/32                  960 ns          960 ns       148713   4.14004k      7.61038      23.095k             5.57844       540.27Mi/s
bench_shake256/512/32                  956 ns          956 ns       148713   4.12149k      7.57627      23.095k             5.60356       542.66Mi/s
bench_shake256/512/32                  952 ns          952 ns       148713   4.12147k      7.57624      23.095k             5.60358       544.84Mi/s
bench_shake256/512/32                  936 ns          936 ns       148713    4.1224k      7.57794      23.095k             5.60232      554.033Mi/s
bench_shake256/512/32                  952 ns          952 ns       148713   4.13187k      7.59535      23.095k             5.58948      544.768Mi/s
bench_shake256/512/32                  954 ns          954 ns       148713   4.12016k      7.57383      23.095k             5.60536      543.745Mi/s
bench_shake256/512/32_mean             949 ns          949 ns            8   4.12445k      7.58172      23.095k             5.59955      546.592Mi/s
bench_shake256/512/32_median           952 ns          952 ns            8   4.12148k      7.57625      23.095k             5.60357      544.804Mi/s
bench_shake256/512/32_stddev          8.79 ns         8.79 ns            8    7.52419    0.0138312            0           0.0101919      5.08595Mi/s
bench_shake256/512/32_cv              0.93 %          0.93 %             8      0.18%        0.18%        0.00%               0.18%            0.93%
bench_shake256/128/32                  249 ns          249 ns       555937   1.14213k      7.13831       5.942k             5.20256      612.178Mi/s
bench_shake256/128/32                  258 ns          258 ns       555937   1.14504k      7.15649       5.942k             5.18935      591.901Mi/s
bench_shake256/128/32                  261 ns          261 ns       555937   1.14395k      7.14966       5.942k              5.1943      584.883Mi/s
bench_shake256/128/32                  264 ns          264 ns       555937    1.1426k      7.14123       5.942k             5.20043      577.637Mi/s
bench_shake256/128/32                  263 ns          263 ns       555937    1.1417k      7.13565       5.942k              5.2045      579.461Mi/s
bench_shake256/128/32                  265 ns          265 ns       555937   1.14476k      7.15474       5.942k             5.19062      575.239Mi/s
bench_shake256/128/32                  260 ns          260 ns       555937    1.1392k         7.12       5.942k             5.21594      586.985Mi/s
bench_shake256/128/32                  264 ns          265 ns       555937   1.14138k      7.13362       5.942k             5.20599      576.437Mi/s
bench_shake256/128/32_mean             261 ns          261 ns            8   1.14259k      7.14121       5.942k             5.20046       585.59Mi/s
bench_shake256/128/32_median           262 ns          262 ns            8   1.14236k      7.13977       5.942k              5.2015      582.172Mi/s
bench_shake256/128/32_stddev          5.25 ns         5.29 ns            8    1.94521    0.0121576            0             8.8578m      12.1944Mi/s
bench_shake256/128/32_cv              2.02 %          2.03 %             8      0.17%        0.17%        0.00%               0.17%            2.08%
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
