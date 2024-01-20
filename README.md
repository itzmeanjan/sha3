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

> [!NOTE]
> If you are on a machine running GNU/Linux kernel and you want to obtain CPU cycles or Cycles/ byte or instruction/ cycle etc., when benchmarking SHA3 algorithms, you should consider building `google-benchmark` library yourself with `libPFM` support, following the step-by-step guide @ https://gist.github.com/itzmeanjan/05dc3e946f635d00c5e0b21aae6203a7. Find more about libPFM @ https://perfmon2.sourceforge.net.

> [!TIP]
> Git submodule based dependencies will generally be imported automatically, but in case that doesn't work, you can manually bring them in by issuing `$ git submodule update --init` from inside the root of this repository.

## Testing

For ensuring that SHA3 hash function and extendable output function implementations are correct & conformant to the NIST standard ( see https://dx.doi.org/10.6028/NIST.FIPS.202 ), I make use of K(nown) A(nswer) T(ests), generated following the gist @ https://gist.github.com/itzmeanjan/448f97f9c49d781a5eb3ddd6ea6e7364.

I also test correctness of

- Incremental message absorption property of SHA3 hash functions and Xofs.
- Incremental output squeezing property of SHA3 Xofs.

Some compile-time executed tests ( using `static_assert` ) are also implemented, which ensure that all Sha3 hash functions and xofs are `constexpr` - meaning they can be evaluated during compilation-time for any statically defined input message.

Issue following command for running all the test cases.

```bash
make -j            # Run tests without any sort of sanitizers
make asan_test -j  # Run tests with AddressSanitizer enabled
make ubsan_test -j # Run tests with UndefinedBehaviourSanitizer enabled
```

```bash
[18/18] Sha3Xof.Shake256IncrementalAbsorptionAndSqueezing (1149 ms)
PASSED TESTS (18/18):
       1 ms: build/tests/test.out Sha3Hashing.CompileTimeEvalSha3_512
       1 ms: build/tests/test.out Sha3Xof.CompileTimeEvalShake128
       1 ms: build/tests/test.out Sha3Hashing.CompileTimeEvalSha3_384
       1 ms: build/tests/test.out Sha3Xof.CompileTimeEvalShake256
       2 ms: build/tests/test.out Sha3Hashing.CompileTimeEvalSha3_224
       2 ms: build/tests/test.out Sha3Hashing.CompileTimeEvalSha3_256
       5 ms: build/tests/test.out Sha3Hashing.Sha3_224KnownAnswerTests
       5 ms: build/tests/test.out Sha3Hashing.Sha3_256KnownAnswerTests
       6 ms: build/tests/test.out Sha3Hashing.Sha3_512KnownAnswerTests
       6 ms: build/tests/test.out Sha3Hashing.Sha3_512IncrementalAbsorption
       6 ms: build/tests/test.out Sha3Hashing.Sha3_384IncrementalAbsorption
       6 ms: build/tests/test.out Sha3Hashing.Sha3_256IncrementalAbsorption
       6 ms: build/tests/test.out Sha3Hashing.Sha3_224IncrementalAbsorption
       7 ms: build/tests/test.out Sha3Xof.Shake128KnownAnswerTests
       7 ms: build/tests/test.out Sha3Hashing.Sha3_384KnownAnswerTests
       7 ms: build/tests/test.out Sha3Xof.Shake256KnownAnswerTests
    1054 ms: build/tests/test.out Sha3Xof.Shake128IncrementalAbsorptionAndSqueezing
    1149 ms: build/tests/test.out Sha3Xof.Shake256IncrementalAbsorptionAndSqueezing
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

### On 12th Gen Intel(R) Core(TM) i7-1260P ( compiled with `Ubuntu clang version 17.0.2 (1~exp1ubuntu2.1)` )

```bash
2024-01-20T16:32:22+04:00
Running ./build/perfs/perf.out
Run on (16 X 4199.2 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 1.03, 0.94, 0.72
-------------------------------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations     CYCLES CYCLES/ BYTE bytes_per_second
-------------------------------------------------------------------------------------------------------------
sha3_512/16384_mean            47078 ns        47073 ns           10   196.189k      11.9278      333.249Mi/s
sha3_512/16384_median          47123 ns        47120 ns           10   196.171k      11.9268      332.894Mi/s
sha3_512/16384_stddev            375 ns          376 ns           10    280.473    0.0170521      2.66302Mi/s
sha3_512/16384_cv               0.80 %          0.80 %            10      0.14%        0.14%            0.80%
sha3_512/16384_min             46537 ns        46536 ns           10     195.8k      11.9042      329.629Mi/s
sha3_512/16384_max             47587 ns        47587 ns           10   196.741k      11.9614      337.072Mi/s
shake256/1024/64_mean           1697 ns         1697 ns           10   7.06365k      6.49232       611.49Mi/s
shake256/1024/64_median         1694 ns         1694 ns           10   7.06479k      6.49337      612.661Mi/s
shake256/1024/64_stddev         22.2 ns         22.2 ns           10    6.53775     6.00896m       7.8507Mi/s
shake256/1024/64_cv             1.31 %          1.31 %            10      0.09%        0.09%            1.28%
shake256/1024/64_min            1670 ns         1670 ns           10   7.05263k       6.4822      591.563Mi/s
shake256/1024/64_max            1754 ns         1754 ns           10    7.0719k      6.49991      621.295Mi/s
sha3_384/1024_mean              2120 ns         2120 ns           10   8.77788k      8.18832      482.406Mi/s
sha3_384/1024_median            2108 ns         2108 ns           10   8.77528k      8.18589       485.01Mi/s
sha3_384/1024_stddev            46.4 ns         46.4 ns           10     16.016    0.0149403      10.3393Mi/s
sha3_384/1024_cv                2.19 %          2.19 %            10      0.18%        0.18%            2.14%
sha3_384/1024_min               2072 ns         2072 ns           10    8.7518k      8.16399       462.86Mi/s
sha3_384/1024_max               2209 ns         2209 ns           10   8.80363k      8.21235      493.522Mi/s
sha3_256/1024_mean              1696 ns         1696 ns           10   7.06628k      6.69155      593.884Mi/s
sha3_256/1024_median            1688 ns         1688 ns           10   7.06231k      6.68779      596.644Mi/s
sha3_256/1024_stddev            29.6 ns         29.6 ns           10    8.20204     7.76708m      10.0777Mi/s
sha3_256/1024_cv                1.74 %          1.74 %            10      0.12%        0.12%            1.70%
sha3_256/1024_min               1665 ns         1665 ns           10   7.05768k      6.68341      568.538Mi/s
sha3_256/1024_max               1771 ns         1771 ns           10   7.07858k       6.7032      605.012Mi/s
sha3_224/256_mean                450 ns          450 ns           10   1.88327k      6.63123      602.425Mi/s
sha3_224/256_median              449 ns          449 ns           10   1.88234k      6.62796      603.486Mi/s
sha3_224/256_stddev             5.76 ns         5.77 ns           10    4.11249    0.0144806      7.58021Mi/s
sha3_224/256_cv                 1.28 %          1.28 %            10      0.22%        0.22%            1.26%
sha3_224/256_min                 444 ns          444 ns           10   1.87915k      6.61674      583.642Mi/s
sha3_224/256_max                 464 ns          464 ns           10   1.89181k      6.66131      610.465Mi/s
sha3_224/1024_mean              1705 ns         1705 ns           10   7.11719k      6.76539      588.434Mi/s
sha3_224/1024_median            1701 ns         1701 ns           10   7.11954k      6.76763      589.703Mi/s
sha3_224/1024_stddev            27.8 ns         27.8 ns           10    13.0981    0.0124506      9.39551Mi/s
sha3_224/1024_cv                1.63 %          1.63 %            10      0.18%        0.18%            1.60%
sha3_224/1024_min               1675 ns         1675 ns           10   7.09816k       6.7473      566.411Mi/s
sha3_224/1024_max               1771 ns         1771 ns           10   7.13747k      6.78467      598.933Mi/s
sha3_384/16384_mean            32770 ns        32768 ns           10   136.423k      8.30227      478.248Mi/s
sha3_384/16384_median          32754 ns        32754 ns           10   136.378k      8.29953      478.439Mi/s
sha3_384/16384_stddev            215 ns          216 ns           10    283.637    0.0172612      3.14624Mi/s
sha3_384/16384_cv               0.66 %          0.66 %            10      0.21%        0.21%            0.66%
sha3_384/16384_min             32385 ns        32382 ns           10   135.995k      8.27622      472.853Mi/s
sha3_384/16384_max             33142 ns        33141 ns           10   136.902k      8.33143      483.934Mi/s
shake256/16384/64_mean         25386 ns        25385 ns           10   104.586k       6.3586      618.132Mi/s
shake256/16384/64_median       25199 ns        25197 ns           10    104.61k      6.36005      622.527Mi/s
shake256/16384/64_stddev         497 ns          497 ns           10    126.374     7.68322m      11.8281Mi/s
shake256/16384/64_cv            1.96 %          1.96 %            10      0.12%        0.12%            1.91%
shake256/16384/64_min          24969 ns        24968 ns           10   104.365k      6.34518      595.478Mi/s
shake256/16384/64_max          26343 ns        26342 ns           10   104.774k      6.37003      628.257Mi/s
shake256/64/64_mean              226 ns          226 ns           10     943.79      7.37336       539.91Mi/s
shake256/64/64_median            225 ns          225 ns           10    943.411       7.3704       542.65Mi/s
shake256/64/64_stddev           3.86 ns         3.87 ns           10    2.08899    0.0163202      8.89311Mi/s
shake256/64/64_cv               1.71 %          1.71 %            10      0.22%        0.22%            1.65%
shake256/64/64_min               224 ns          224 ns           10    941.407      7.35474      515.542Mi/s
shake256/64/64_max               237 ns          237 ns           10    946.997      7.39841      546.137Mi/s
shake128/256/64_mean             451 ns          451 ns           10   1.87769k      5.86778      676.797Mi/s
shake128/256/64_median           449 ns          449 ns           10   1.87731k       5.8666      679.475Mi/s
shake128/256/64_stddev          6.62 ns         6.62 ns           10     6.2563    0.0195509      9.77397Mi/s
shake128/256/64_cv              1.47 %          1.47 %            10      0.33%        0.33%            1.44%
shake128/256/64_min              445 ns          445 ns           10   1.87026k      5.84456      657.905Mi/s
shake128/256/64_max              464 ns          464 ns           10   1.88865k      5.90202      686.472Mi/s
sha3_256/4096_mean              6501 ns         6500 ns           10   26.9396k      6.52608      605.728Mi/s
sha3_256/4096_median            6484 ns         6484 ns           10   26.9337k      6.52463       607.18Mi/s
sha3_256/4096_stddev            95.1 ns         95.1 ns           10    25.8319     6.25772m      8.60066Mi/s
sha3_256/4096_cv                1.46 %          1.46 %            10      0.10%        0.10%            1.42%
sha3_256/4096_min               6407 ns         6407 ns           10   26.9063k      6.51799      582.404Mi/s
sha3_256/4096_max               6760 ns         6760 ns           10   26.9973k      6.54005      614.465Mi/s
shake128/16384/64_mean         20473 ns        20472 ns           10   85.2039k       5.1802      766.287Mi/s
shake128/16384/64_median       20493 ns        20493 ns           10   85.2611k      5.18367      765.451Mi/s
shake128/16384/64_stddev         190 ns          190 ns           10    467.964    0.0284511      7.10767Mi/s
shake128/16384/64_cv            0.93 %          0.93 %            10      0.55%        0.55%            0.93%
shake128/16384/64_min          20201 ns        20201 ns           10   84.4673k      5.13542      754.681Mi/s
shake128/16384/64_max          20787 ns        20785 ns           10   85.7988k      5.21637      776.512Mi/s
sha3_384/64_mean                 228 ns          228 ns           10    939.351      8.38706      469.139Mi/s
sha3_384/64_median               226 ns          226 ns           10    947.031      8.45563      472.125Mi/s
sha3_384/64_stddev              4.31 ns         4.31 ns           10    24.7589     0.221062      8.77855Mi/s
sha3_384/64_cv                  1.89 %          1.89 %            10      2.64%        2.64%            1.87%
sha3_384/64_min                  223 ns          223 ns           10    869.022      7.75913      454.531Mi/s
sha3_384/64_max                  235 ns          235 ns           10    950.115      8.48317      479.098Mi/s
shake128/64/64_mean              230 ns          230 ns           10    956.516      7.47278      531.434Mi/s
shake128/64/64_median            229 ns          229 ns           10    957.289      7.47882      533.223Mi/s
shake128/64/64_stddev           3.38 ns         3.38 ns           10    2.75085     0.021491      7.64794Mi/s
shake128/64/64_cv               1.47 %          1.47 %            10      0.29%        0.29%            1.44%
shake128/64/64_min               226 ns          226 ns           10     950.46      7.42547      512.269Mi/s
shake128/64/64_max               238 ns          238 ns           10    959.436      7.49559      539.297Mi/s
shake128/4096/64_mean           5271 ns         5271 ns           10    21.876k      5.25865      752.773Mi/s
shake128/4096/64_median         5262 ns         5262 ns           10   21.8532k      5.25317      753.976Mi/s
shake128/4096/64_stddev         57.5 ns         57.5 ns           10    123.224    0.0296211      8.09954Mi/s
shake128/4096/64_cv             1.09 %          1.09 %            10      0.56%        0.56%            1.08%
shake128/4096/64_min            5204 ns         5204 ns           10   21.6725k      5.20974      733.432Mi/s
shake128/4096/64_max            5409 ns         5409 ns           10   22.0734k       5.3061      762.394Mi/s
sha3_384/4096_mean              8345 ns         8344 ns           10   34.6399k      8.35905      473.729Mi/s
sha3_384/4096_median            8321 ns         8321 ns           10   34.6334k      8.35748      474.958Mi/s
sha3_384/4096_stddev             131 ns          131 ns           10    61.8703    0.0149301      7.29331Mi/s
sha3_384/4096_cv                1.57 %          1.57 %            10      0.18%        0.18%            1.54%
sha3_384/4096_min               8209 ns         8209 ns           10   34.5658k      8.34117      457.128Mi/s
sha3_384/4096_max               8646 ns         8645 ns           10   34.7457k      8.38459       481.45Mi/s
shake256/4096/64_mean           6567 ns         6566 ns           10   26.9033k      6.46715      604.542Mi/s
shake256/4096/64_median         6500 ns         6500 ns           10   26.9327k       6.4742      610.387Mi/s
shake256/4096/64_stddev          169 ns          169 ns           10    198.957    0.0478261      15.3932Mi/s
shake256/4096/64_cv             2.57 %          2.57 %            10      0.74%        0.74%            2.55%
shake256/4096/64_min            6331 ns         6331 ns           10   26.3655k      6.33785      582.197Mi/s
shake256/4096/64_max            6814 ns         6814 ns           10   27.0824k      6.51018      626.632Mi/s
sha3_224/16384_mean            23908 ns        23907 ns           10   99.2535k      6.04762      654.753Mi/s
sha3_224/16384_median          23853 ns        23853 ns           10   99.1969k      6.04417      656.185Mi/s
sha3_224/16384_stddev            223 ns          223 ns           10    257.736    0.0157041      6.08101Mi/s
sha3_224/16384_cv               0.93 %          0.93 %            10      0.26%        0.26%            0.93%
sha3_224/16384_min             23642 ns        23642 ns           10   98.9093k      6.02664      643.085Mi/s
sha3_224/16384_max             24339 ns        24338 ns           10   99.6413k      6.07125      662.037Mi/s
sha3_512/256_mean                862 ns          862 ns           10   3.56823k      11.1507      354.154Mi/s
sha3_512/256_median              855 ns          855 ns           10   3.56642k       11.145       356.95Mi/s
sha3_512/256_stddev             18.2 ns         18.2 ns           10    6.50433     0.020326      7.34607Mi/s
sha3_512/256_cv                 2.11 %          2.11 %            10      0.18%        0.18%            2.07%
sha3_512/256_min                 843 ns          843 ns           10    3.5613k      11.1291      341.393Mi/s
sha3_512/256_max                 894 ns          894 ns           10   3.58141k      11.1919      362.115Mi/s
shake256/256/64_mean             444 ns          444 ns           10    1.8628k      5.82126      688.092Mi/s
shake256/256/64_median           444 ns          444 ns           10    1.8645k      5.82655      687.267Mi/s
shake256/256/64_stddev          3.00 ns         3.00 ns           10    3.06097     9.56552m      4.66754Mi/s
shake256/256/64_cv              0.68 %          0.68 %            10      0.16%        0.16%            0.68%
shake256/256/64_min              438 ns          438 ns           10   1.85831k      5.80723       680.75Mi/s
shake256/256/64_max              448 ns          448 ns           10   1.86634k       5.8323      696.864Mi/s
sha3_512/1024_mean              3163 ns         3163 ns           10   13.0816k      12.0235      328.133Mi/s
sha3_512/1024_median            3150 ns         3150 ns           10   13.0817k      12.0236      329.422Mi/s
sha3_512/1024_stddev            42.8 ns         42.8 ns           10    23.3898     0.021498      4.32226Mi/s
sha3_512/1024_cv                1.35 %          1.35 %            10      0.18%        0.18%            1.32%
sha3_512/1024_min               3132 ns         3132 ns           10   13.0397k       11.985      316.492Mi/s
sha3_512/1024_max               3279 ns         3278 ns           10   13.1255k      12.0639      331.332Mi/s
sha3_384/256_mean                647 ns          647 ns           10   2.69961k      8.88029      448.373Mi/s
sha3_384/256_median              645 ns          644 ns           10   2.69926k      8.87914      449.836Mi/s
sha3_384/256_stddev             6.36 ns         6.37 ns           10    3.21142    0.0105639      4.39387Mi/s
sha3_384/256_cv                 0.98 %          0.98 %            10      0.12%        0.12%            0.98%
sha3_384/256_min                 639 ns          639 ns           10   2.69471k      8.86419      440.796Mi/s
sha3_384/256_max                 658 ns          658 ns           10   2.70448k      8.89632      453.811Mi/s
sha3_512/64_mean                 226 ns          226 ns           10    936.855      7.31918      540.182Mi/s
sha3_512/64_median               225 ns          225 ns           10    940.248      7.34569      542.557Mi/s
sha3_512/64_stddev              2.98 ns         2.98 ns           10    12.7421    0.0995479      6.98803Mi/s
sha3_512/64_cv                  1.32 %          1.32 %            10      1.36%        1.36%            1.29%
sha3_512/64_min                  223 ns          223 ns           10    901.144      7.04019      522.984Mi/s
sha3_512/64_max                  233 ns          233 ns           10    944.174      7.37636      546.214Mi/s
sha3_224/64_mean                 229 ns          229 ns           10    956.933      10.4014      383.458Mi/s
sha3_224/64_median               228 ns          228 ns           10    956.708       10.399      385.106Mi/s
sha3_224/64_stddev              3.52 ns         3.52 ns           10     3.9546    0.0429848      5.82002Mi/s
sha3_224/64_cv                  1.54 %          1.54 %            10      0.41%        0.41%            1.52%
sha3_224/64_min                  225 ns          225 ns           10     951.42      10.3415      372.426Mi/s
sha3_224/64_max                  236 ns          236 ns           10     963.04      10.4678      390.144Mi/s
sha3_224/4096_mean              6097 ns         6097 ns           10   25.3631k      6.15011       645.17Mi/s
sha3_224/4096_median            6087 ns         6086 ns           10   25.3494k       6.1468      646.198Mi/s
sha3_224/4096_stddev            62.2 ns         62.1 ns           10    76.6335    0.0185823      6.56813Mi/s
sha3_224/4096_cv                1.02 %          1.02 %            10      0.30%        0.30%            1.02%
sha3_224/4096_min               5994 ns         5994 ns           10   25.2631k      6.12588      634.169Mi/s
sha3_224/4096_max               6202 ns         6202 ns           10   25.5052k      6.18458      656.197Mi/s
keccak-p[1600, 24]_mean          201 ns          201 ns           10    836.028      4.18014      947.066Mi/s
keccak-p[1600, 24]_median        202 ns          202 ns           10    835.852      4.17926      944.329Mi/s
keccak-p[1600, 24]_stddev       1.56 ns         1.56 ns           10   0.953858     4.76929m      7.36137Mi/s
keccak-p[1600, 24]_cv           0.77 %          0.77 %            10      0.11%        0.11%            0.78%
keccak-p[1600, 24]_min           198 ns          198 ns           10     834.91      4.17455      936.229Mi/s
keccak-p[1600, 24]_max           204 ns          204 ns           10    837.704      4.18852      962.401Mi/s
sha3_256/16384_mean            25310 ns        25309 ns           10   104.779k      6.38275       618.77Mi/s
sha3_256/16384_median          25196 ns        25194 ns           10   104.793k      6.38362      621.404Mi/s
sha3_256/16384_stddev            476 ns          476 ns           10    209.428    0.0127576      11.4012Mi/s
sha3_256/16384_cv               1.88 %          1.88 %            10      0.20%        0.20%            1.84%
sha3_256/16384_min             24771 ns        24770 ns           10   104.476k      6.36426       592.49Mi/s
sha3_256/16384_max             26424 ns        26423 ns           10    105.07k      6.40045      632.034Mi/s
shake128/1024/64_mean           1511 ns         1511 ns           10   6.24248k      5.73758      686.751Mi/s
shake128/1024/64_median         1504 ns         1504 ns           10   6.23871k      5.73411      689.881Mi/s
shake128/1024/64_stddev         21.5 ns         21.5 ns           10    30.5501    0.0280791      9.64082Mi/s
shake128/1024/64_cv             1.42 %          1.42 %            10      0.49%        0.49%            1.40%
shake128/1024/64_min            1485 ns         1485 ns           10   6.20875k      5.70657      666.979Mi/s
shake128/1024/64_max            1556 ns         1556 ns           10   6.31645k      5.80556      698.568Mi/s
sha3_256/64_mean                 227 ns          227 ns           10    945.557      9.84955      403.273Mi/s
sha3_256/64_median               227 ns          227 ns           10     944.72      9.84083      403.878Mi/s
sha3_256/64_stddev              2.80 ns         2.80 ns           10      3.757    0.0391354       4.8898Mi/s
sha3_256/64_cv                  1.23 %          1.23 %            10      0.40%        0.40%            1.21%
sha3_256/64_min                  224 ns          224 ns           10    939.278      9.78415      391.182Mi/s
sha3_256/64_max                  234 ns          234 ns           10    950.283      9.89879      408.808Mi/s
sha3_512/4096_mean             11925 ns        11924 ns           10   49.2576k      11.8408      332.756Mi/s
sha3_512/4096_median           11876 ns        11875 ns           10   49.2575k      11.8408      334.075Mi/s
sha3_512/4096_stddev             159 ns          158 ns           10    106.902    0.0256976      4.34838Mi/s
sha3_512/4096_cv                1.33 %          1.33 %            10      0.22%        0.22%            1.31%
sha3_512/4096_min              11766 ns        11766 ns           10    49.099k      11.8027      322.616Mi/s
sha3_512/4096_max              12298 ns        12297 ns           10   49.4537k      11.8879      337.189Mi/s
sha3_256/256_mean                448 ns          448 ns           10    1.8696k      6.49166      613.655Mi/s
sha3_256/256_median              447 ns          447 ns           10    1.8684k      6.48751      614.217Mi/s
sha3_256/256_stddev             4.74 ns         4.74 ns           10    4.80987     0.016701      6.48242Mi/s
sha3_256/256_cv                 1.06 %          1.06 %            10      0.26%        0.26%            1.06%
sha3_256/256_min                 441 ns          441 ns           10   1.86248k      6.46695      602.375Mi/s
sha3_256/256_max                 456 ns          456 ns           10    1.8775k      6.51911      622.879Mi/s
```

### On Apple M1 Max ( compiled with `Apple clang version 15.0.0 (clang-1500.1.0.2.5)` )

```bash
2024-01-20T16:13:19+04:00
Running ./build/benchmarks/bench.out
Run on (10 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x10)
Load Average: 2.73, 3.26, 3.72
-------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations bytes_per_second
-------------------------------------------------------------------------------------
sha3_384/256_mean                734 ns          732 ns           10      395.832Mi/s
sha3_384/256_median              731 ns          730 ns           10      397.168Mi/s
sha3_384/256_stddev             8.10 ns         7.83 ns           10      4.12066Mi/s
sha3_384/256_cv                 1.10 %          1.07 %            10            1.04%
sha3_384/256_min                 730 ns          730 ns           10       384.11Mi/s
sha3_384/256_max                 757 ns          755 ns           10      397.216Mi/s
sha3_256/256_mean                517 ns          515 ns           10      533.919Mi/s
sha3_256/256_median              513 ns          512 ns           10      536.355Mi/s
sha3_256/256_stddev             12.6 ns         7.65 ns           10      7.65515Mi/s
sha3_256/256_cv                 2.44 %          1.49 %            10            1.43%
sha3_256/256_min                 512 ns          512 ns           10      512.133Mi/s
sha3_256/256_max                 553 ns          536 ns           10      536.405Mi/s
sha3_224/256_mean                517 ns          517 ns           10      524.214Mi/s
sha3_224/256_median              517 ns          517 ns           10      524.326Mi/s
sha3_224/256_stddev            0.324 ns        0.345 ns           10       357.38Ki/s
sha3_224/256_cv                 0.06 %          0.07 %            10            0.07%
sha3_224/256_min                 517 ns          517 ns           10      523.222Mi/s
sha3_224/256_max                 518 ns          518 ns           10      524.347Mi/s
sha3_256/16384_mean            29774 ns        29740 ns           10       526.42Mi/s
sha3_256/16384_median          29776 ns        29739 ns           10      526.432Mi/s
sha3_256/16384_stddev           7.40 ns         4.47 ns           10      81.0949Ki/s
sha3_256/16384_cv               0.02 %          0.02 %            10            0.02%
sha3_256/16384_min             29763 ns        29734 ns           10      526.301Mi/s
sha3_256/16384_max             29784 ns        29746 ns           10      526.526Mi/s
shake128/256/64_mean             531 ns          530 ns           10      575.569Mi/s
shake128/256/64_median           530 ns          529 ns           10      576.448Mi/s
shake128/256/64_stddev          2.58 ns         2.61 ns           10      2.79422Mi/s
shake128/256/64_cv              0.49 %          0.49 %            10            0.49%
shake128/256/64_min              530 ns          529 ns           10      567.616Mi/s
shake128/256/64_max              538 ns          538 ns           10      576.479Mi/s
shake256/4096/64_mean           7647 ns         7637 ns           10      519.457Mi/s
shake256/4096/64_median         7647 ns         7637 ns           10      519.456Mi/s
shake256/4096/64_stddev         2.35 ns        0.404 ns           10      28.1501Ki/s
shake256/4096/64_cv             0.03 %          0.01 %            10            0.01%
shake256/4096/64_min            7645 ns         7637 ns           10      519.393Mi/s
shake256/4096/64_max            7652 ns         7638 ns           10      519.485Mi/s
sha3_512/64_mean                 253 ns          253 ns           10      483.055Mi/s
sha3_512/64_median               252 ns          251 ns           10      485.423Mi/s
sha3_512/64_stddev              4.10 ns         4.08 ns           10      7.49395Mi/s
sha3_512/64_cv                  1.62 %          1.61 %            10            1.55%
sha3_512/64_min                  252 ns          251 ns           10      461.727Mi/s
sha3_512/64_max                  265 ns          264 ns           10      485.477Mi/s
shake256/64/64_mean              271 ns          271 ns           10      450.256Mi/s
shake256/64/64_median            271 ns          271 ns           10      451.014Mi/s
shake256/64/64_stddev           1.55 ns         1.49 ns           10      2.44808Mi/s
shake256/64/64_cv               0.57 %          0.55 %            10            0.54%
shake256/64/64_min               271 ns          271 ns           10       443.29Mi/s
shake256/64/64_max               276 ns          275 ns           10       451.16Mi/s
shake128/1024/64_mean           1816 ns         1803 ns           10      575.609Mi/s
shake128/1024/64_median         1800 ns         1798 ns           10      577.014Mi/s
shake128/1024/64_stddev         46.9 ns         12.3 ns           10       3.8613Mi/s
shake128/1024/64_cv             2.58 %          0.68 %            10            0.67%
shake128/1024/64_min            1800 ns         1798 ns           10      564.735Mi/s
shake128/1024/64_max            1949 ns         1837 ns           10      577.148Mi/s
sha3_224/1024_mean              2020 ns         2018 ns           10      497.314Mi/s
sha3_224/1024_median            2012 ns         2010 ns           10      499.175Mi/s
sha3_224/1024_stddev            24.7 ns         24.6 ns           10      5.88352Mi/s
sha3_224/1024_cv                1.22 %          1.22 %            10            1.18%
sha3_224/1024_min               2012 ns         2010 ns           10      480.569Mi/s
sha3_224/1024_max               2091 ns         2088 ns           10      499.218Mi/s
sha3_384/16384_mean            37832 ns        37784 ns           10      414.745Mi/s
sha3_384/16384_median          37825 ns        37781 ns           10      414.775Mi/s
sha3_384/16384_stddev           17.4 ns         9.89 ns           10      111.108Ki/s
sha3_384/16384_cv               0.05 %          0.03 %            10            0.03%
sha3_384/16384_min             37816 ns        37779 ns           10      414.441Mi/s
sha3_384/16384_max             37874 ns        37812 ns           10      414.805Mi/s
sha3_224/16384_mean            28393 ns        28349 ns           10      552.113Mi/s
sha3_224/16384_median          28387 ns        28349 ns           10      552.117Mi/s
sha3_224/16384_stddev           22.8 ns         1.44 ns           10      28.6494Ki/s
sha3_224/16384_cv               0.08 %          0.01 %            10            0.01%
sha3_224/16384_min             28377 ns        28347 ns           10       552.07Mi/s
sha3_224/16384_max             28453 ns        28351 ns           10      552.145Mi/s
sha3_512/4096_mean             12512 ns        12497 ns           10      317.471Mi/s
sha3_512/4096_median           12510 ns        12494 ns           10      317.538Mi/s
sha3_512/4096_stddev            7.05 ns         6.11 ns           10      158.695Ki/s
sha3_512/4096_cv                0.06 %          0.05 %            10            0.05%
sha3_512/4096_min              12507 ns        12493 ns           10      317.092Mi/s
sha3_512/4096_max              12531 ns        12511 ns           10      317.561Mi/s
sha3_512/16384_mean            50271 ns        50202 ns           10      312.515Mi/s
sha3_512/16384_median          49954 ns        49894 ns           10      314.385Mi/s
sha3_512/16384_stddev            732 ns          710 ns           10      4.30358Mi/s
sha3_512/16384_cv               1.46 %          1.41 %            10            1.38%
sha3_512/16384_min             49933 ns        49877 ns           10      301.568Mi/s
sha3_512/16384_max             52161 ns        52015 ns           10      314.494Mi/s
keccak-p[1600, 24]_mean          210 ns          209 ns           10      910.792Mi/s
keccak-p[1600, 24]_median        210 ns          209 ns           10      910.799Mi/s
keccak-p[1600, 24]_stddev      0.061 ns        0.014 ns           10      64.3826Ki/s
keccak-p[1600, 24]_cv           0.03 %          0.01 %            10            0.01%
keccak-p[1600, 24]_min           210 ns          209 ns           10      910.691Mi/s
keccak-p[1600, 24]_max           210 ns          209 ns           10      910.854Mi/s
sha3_384/4096_mean              9591 ns         9579 ns           10      412.574Mi/s
sha3_384/4096_median            9590 ns         9578 ns           10      412.594Mi/s
sha3_384/4096_stddev            3.48 ns         1.56 ns           10      68.6457Ki/s
sha3_384/4096_cv                0.04 %          0.02 %            10            0.02%
sha3_384/4096_min               9587 ns         9577 ns           10      412.459Mi/s
sha3_384/4096_max               9597 ns         9582 ns           10      412.639Mi/s
shake128/64/64_mean              278 ns          278 ns           10      438.941Mi/s
shake128/64/64_median            278 ns          278 ns           10      438.957Mi/s
shake128/64/64_stddev          0.152 ns        0.032 ns           10      51.5279Ki/s
shake128/64/64_cv               0.05 %          0.01 %            10            0.01%
shake128/64/64_min               278 ns          278 ns           10      438.871Mi/s
shake128/64/64_max               279 ns          278 ns           10      439.012Mi/s
sha3_256/1024_mean              1995 ns         1992 ns           10      505.488Mi/s
sha3_256/1024_median            1992 ns         1989 ns           10      506.203Mi/s
sha3_256/1024_stddev            8.81 ns         8.81 ns           10      2.20981Mi/s
sha3_256/1024_cv                0.44 %          0.44 %            10            0.44%
sha3_256/1024_min               1990 ns         1989 ns           10      499.201Mi/s
sha3_256/1024_max               2020 ns         2017 ns           10      506.235Mi/s
sha3_256/4096_mean              7659 ns         7651 ns           10      514.586Mi/s
sha3_256/4096_median            7644 ns         7635 ns           10      515.624Mi/s
sha3_256/4096_stddev            42.9 ns         40.6 ns           10      2.69423Mi/s
sha3_256/4096_cv                0.56 %          0.53 %            10            0.52%
sha3_256/4096_min               7641 ns         7634 ns           10      507.011Mi/s
sha3_256/4096_max               7780 ns         7765 ns           10      515.672Mi/s
sha3_256/64_mean                 271 ns          271 ns           10      338.262Mi/s
sha3_256/64_median               271 ns          271 ns           10      338.262Mi/s
sha3_256/64_stddev             0.098 ns        0.030 ns           10      37.8028Ki/s
sha3_256/64_cv                  0.04 %          0.01 %            10            0.01%
sha3_256/64_min                  271 ns          271 ns           10      338.205Mi/s
sha3_256/64_max                  271 ns          271 ns           10      338.336Mi/s
sha3_512/256_mean                925 ns          923 ns           10      330.489Mi/s
sha3_512/256_median              925 ns          923 ns           10      330.494Mi/s
sha3_512/256_stddev            0.613 ns        0.060 ns           10      22.1389Ki/s
sha3_512/256_cv                 0.07 %          0.01 %            10            0.01%
sha3_512/256_min                 924 ns          923 ns           10      330.451Mi/s
sha3_512/256_max                 926 ns          924 ns           10      330.522Mi/s
sha3_224/4096_mean              7273 ns         7263 ns           10      541.606Mi/s
sha3_224/4096_median            7244 ns         7234 ns           10      543.707Mi/s
sha3_224/4096_stddev            89.4 ns         90.2 ns           10      6.52127Mi/s
sha3_224/4096_cv                1.23 %          1.24 %            10            1.20%
sha3_224/4096_min               7241 ns         7233 ns           10       523.05Mi/s
sha3_224/4096_max               7527 ns         7519 ns           10      543.734Mi/s
sha3_224/64_mean                 272 ns          271 ns           10      323.182Mi/s
sha3_224/64_median               272 ns          271 ns           10      323.247Mi/s
sha3_224/64_stddev             0.194 ns        0.157 ns           10       191.57Ki/s
sha3_224/64_cv                  0.07 %          0.06 %            10            0.06%
sha3_224/64_min                  272 ns          271 ns           10       322.66Mi/s
sha3_224/64_max                  272 ns          272 ns           10      323.285Mi/s
sha3_384/64_mean                 252 ns          251 ns           10      425.074Mi/s
sha3_384/64_median               252 ns          251 ns           10      425.092Mi/s
sha3_384/64_stddev             0.071 ns        0.028 ns           10      47.8782Ki/s
sha3_384/64_cv                  0.03 %          0.01 %            10            0.01%
sha3_384/64_min                  251 ns          251 ns           10      424.952Mi/s
sha3_384/64_max                  252 ns          251 ns           10      425.113Mi/s
shake128/16384/64_mean         24833 ns        24796 ns           10        632.6Mi/s
shake128/16384/64_median       24829 ns        24795 ns           10      632.626Mi/s
shake128/16384/64_stddev        15.4 ns         3.05 ns           10      79.7538Ki/s
shake128/16384/64_cv            0.06 %          0.01 %            10            0.01%
shake128/16384/64_min          24817 ns        24793 ns           10      632.438Mi/s
shake128/16384/64_max          24866 ns        24802 ns           10      632.673Mi/s
shake256/256/64_mean             513 ns          513 ns           10      595.316Mi/s
shake256/256/64_median           513 ns          513 ns           10      595.332Mi/s
shake256/256/64_stddev         0.163 ns        0.034 ns           10      40.9232Ki/s
shake256/256/64_cv              0.03 %          0.01 %            10            0.01%
shake256/256/64_min              513 ns          513 ns           10      595.222Mi/s
shake256/256/64_max              514 ns          513 ns           10      595.354Mi/s
shake256/16384/64_mean         29791 ns        29754 ns           10      527.191Mi/s
shake256/16384/64_median       29786 ns        29752 ns           10      527.232Mi/s
shake256/16384/64_stddev        15.4 ns         5.76 ns           10      104.489Ki/s
shake256/16384/64_cv            0.05 %          0.02 %            10            0.02%
shake256/16384/64_min          29778 ns        29748 ns           10      526.944Mi/s
shake256/16384/64_max          29830 ns        29768 ns           10      527.297Mi/s
sha3_384/1024_mean              2407 ns         2404 ns           10      425.212Mi/s
sha3_384/1024_median            2407 ns         2404 ns           10      425.212Mi/s
sha3_384/1024_stddev           0.667 ns        0.064 ns           10      11.5857Ki/s
sha3_384/1024_cv                0.03 %          0.00 %            10            0.00%
sha3_384/1024_min               2406 ns         2404 ns           10      425.197Mi/s
sha3_384/1024_max               2409 ns         2404 ns           10      425.227Mi/s
shake256/1024/64_mean           1993 ns         1990 ns           10      521.301Mi/s
shake256/1024/64_median         1993 ns         1990 ns           10       521.31Mi/s
shake256/1024/64_stddev         1.56 ns        0.101 ns           10      27.1673Ki/s
shake256/1024/64_cv             0.08 %          0.01 %            10            0.01%
shake256/1024/64_min            1992 ns         1990 ns           10      521.241Mi/s
shake256/1024/64_max            1997 ns         1991 ns           10       521.33Mi/s
sha3_512/1024_mean              3332 ns         3328 ns           10      311.783Mi/s
sha3_512/1024_median            3332 ns         3328 ns           10       311.77Mi/s
sha3_512/1024_stddev            1.02 ns        0.431 ns           10      41.3057Ki/s
sha3_512/1024_cv                0.03 %          0.01 %            10            0.01%
sha3_512/1024_min               3331 ns         3327 ns           10      311.746Mi/s
sha3_512/1024_max               3334 ns         3328 ns           10      311.858Mi/s
shake128/4096/64_mean           6361 ns         6352 ns           10      624.596Mi/s
shake128/4096/64_median         6360 ns         6351 ns           10      624.689Mi/s
shake128/4096/64_stddev         4.09 ns         1.91 ns           10       192.71Ki/s
shake128/4096/64_cv             0.06 %          0.03 %            10            0.03%
shake128/4096/64_min            6356 ns         6350 ns           10      624.315Mi/s
shake128/4096/64_max            6369 ns         6355 ns           10      624.792Mi/s
```

### On ARM Cortex-A72 i.e. Raspberry Pi 4B ( compiled with `gcc version 13.2.0 (Ubuntu 13.2.0-4ubuntu3)` )

```bash
2024-01-20T16:20:37+04:00
Running ./build/perfs/perf.out
Run on (4 X 1800 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 48 KiB (x4)
  L2 Unified 1024 KiB (x1)
Load Average: 0.59, 0.74, 1.06
-------------------------------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations     CYCLES CYCLES/ BYTE bytes_per_second
-------------------------------------------------------------------------------------------------------------
sha3_384/256_mean               2809 ns         2809 ns           10   5.04865k      16.6074       103.22Mi/s
sha3_384/256_median             2809 ns         2809 ns           10   5.04856k      16.6071      103.221Mi/s
sha3_384/256_stddev            0.443 ns        0.294 ns           10   0.230906     759.558u      11.0744Ki/s
sha3_384/256_cv                 0.02 %          0.01 %            10      0.00%        0.00%            0.01%
sha3_384/256_min                2808 ns         2808 ns           10   5.04844k      16.6067      103.202Mi/s
sha3_384/256_max                2809 ns         2809 ns           10   5.04919k      16.6092      103.238Mi/s
sha3_384/4096_mean             35057 ns        35057 ns           10   63.0092k      15.2049      112.733Mi/s
sha3_384/4096_median           35056 ns        35056 ns           10    63.009k      15.2049      112.735Mi/s
sha3_384/4096_stddev            7.12 ns         4.96 ns           10    2.59006     625.015u      16.3467Ki/s
sha3_384/4096_cv                0.02 %          0.01 %            10      0.00%        0.00%            0.01%
sha3_384/4096_min              35045 ns        35049 ns           10   63.0054k       15.204      112.707Mi/s
sha3_384/4096_max              35069 ns        35065 ns           10   63.0137k       15.206      112.757Mi/s
sha3_512/256_mean               3665 ns         3665 ns           10   6.58859k      20.5893      83.2633Mi/s
sha3_512/256_median             3665 ns         3665 ns           10   6.58842k      20.5888      83.2666Mi/s
sha3_512/256_stddev            0.699 ns        0.691 ns           10    1.39233     4.35103m      16.0704Ki/s
sha3_512/256_cv                 0.02 %          0.02 %            10      0.02%        0.02%            0.02%
sha3_512/256_min                3664 ns         3664 ns           10   6.58654k      20.5829      83.2405Mi/s
sha3_512/256_max                3666 ns         3666 ns           10   6.59106k      20.5971       83.285Mi/s
sha3_512/64_mean                1044 ns         1044 ns           10   1.87614k      14.6573      116.958Mi/s
sha3_512/64_median              1044 ns         1044 ns           10   1.87613k      14.6573      116.958Mi/s
sha3_512/64_stddev             0.153 ns        0.087 ns           10  0.0163249     127.538u      9.93599Ki/s
sha3_512/64_cv                  0.01 %          0.01 %            10      0.00%        0.00%            0.01%
sha3_512/64_min                 1043 ns         1044 ns           10   1.87612k      14.6572      116.941Mi/s
sha3_512/64_max                 1044 ns         1044 ns           10   1.87617k      14.6576      116.973Mi/s
sha3_224/64_mean                1116 ns         1116 ns           10   2.00537k      21.7975      78.6454Mi/s
sha3_224/64_median              1116 ns         1116 ns           10    2.0053k      21.7968      78.6396Mi/s
sha3_224/64_stddev             0.522 ns        0.476 ns           10   0.876682     9.52915m      34.3417Ki/s
sha3_224/64_cv                  0.05 %          0.04 %            10      0.04%        0.04%            0.04%
sha3_224/64_min                 1115 ns         1115 ns           10   2.00457k      21.7888      78.5768Mi/s
sha3_224/64_max                 1116 ns         1117 ns           10   2.00743k      21.8199      78.6826Mi/s
shake128/256/64_mean            2065 ns         2065 ns           10   3.71232k       11.601      147.754Mi/s
shake128/256/64_median          2065 ns         2065 ns           10   3.71231k       11.601      147.759Mi/s
shake128/256/64_stddev         0.346 ns        0.259 ns           10    0.05968       186.5u      18.9387Ki/s
shake128/256/64_cv              0.02 %          0.01 %            10      0.00%        0.00%            0.01%
shake128/256/64_min             2065 ns         2065 ns           10   3.71223k      11.6007      147.714Mi/s
shake128/256/64_max             2066 ns         2066 ns           10   3.71243k      11.6013      147.775Mi/s
shake256/1024/64_mean           7317 ns         7317 ns           10   13.1525k      12.0887      141.804Mi/s
shake256/1024/64_median         7316 ns         7317 ns           10   13.1524k      12.0886      141.807Mi/s
shake256/1024/64_stddev         1.39 ns        0.839 ns           10   0.780465     717.339u      16.6491Ki/s
shake256/1024/64_cv             0.02 %          0.01 %            10      0.01%        0.01%            0.01%
shake256/1024/64_min            7315 ns         7316 ns           10   13.1515k      12.0877      141.786Mi/s
shake256/1024/64_max            7319 ns         7318 ns           10   13.1539k        12.09      141.833Mi/s
shake256/64/64_mean             1136 ns         1136 ns           10   2.04103k      15.9455      107.497Mi/s
shake256/64/64_median           1135 ns         1135 ns           10   2.04083k       15.944      107.514Mi/s
shake256/64/64_stddev          0.444 ns        0.405 ns           10   0.470208      3.6735m      39.2242Ki/s
shake256/64/64_cv               0.04 %          0.04 %            10      0.02%        0.02%            0.04%
shake256/64/64_min              1135 ns         1135 ns           10   2.04057k       15.942      107.411Mi/s
shake256/64/64_max              1137 ns         1136 ns           10    2.0421k      15.9539       107.53Mi/s
sha3_512/16384_mean           198486 ns       198488 ns           10   356.755k      21.6899      79.0278Mi/s
sha3_512/16384_median         198490 ns       198484 ns           10    356.73k      21.6883      79.0292Mi/s
sha3_512/16384_stddev           87.8 ns         86.8 ns           10    155.444     9.45064m      35.3845Ki/s
sha3_512/16384_cv               0.04 %          0.04 %            10      0.04%        0.04%            0.04%
sha3_512/16384_min            198380 ns       198386 ns           10   356.604k      21.6807      78.9526Mi/s
sha3_512/16384_max            198655 ns       198677 ns           10   357.142k      21.7134      79.0683Mi/s
sha3_224/256_mean               2017 ns         2016 ns           10   3.62396k      12.7604      134.316Mi/s
sha3_224/256_median             2016 ns         2016 ns           10   3.62407k      12.7608       134.32Mi/s
sha3_224/256_stddev             1.32 ns         1.14 ns           10    1.49815     5.27519m       78.095Ki/s
sha3_224/256_cv                 0.07 %          0.06 %            10      0.04%        0.04%            0.06%
sha3_224/256_min                2014 ns         2015 ns           10   3.62128k       12.751      134.183Mi/s
sha3_224/256_max                2019 ns         2018 ns           10   3.62651k      12.7694      134.443Mi/s
shake256/256/64_mean            2027 ns         2027 ns           10   3.64366k      11.3864      150.534Mi/s
shake256/256/64_median          2027 ns         2027 ns           10   3.64369k      11.3865      150.531Mi/s
shake256/256/64_stddev         0.573 ns        0.508 ns           10   0.670622     2.09569m       38.647Ki/s
shake256/256/64_cv              0.03 %          0.03 %            10      0.02%        0.02%            0.03%
shake256/256/64_min             2026 ns         2027 ns           10   3.64274k      11.3836      150.479Mi/s
shake256/256/64_max             2028 ns         2028 ns           10   3.64468k      11.3896      150.587Mi/s
shake128/4096/64_mean          22446 ns        22446 ns           10   40.3432k      9.69789      176.746Mi/s
shake128/4096/64_median        22446 ns        22446 ns           10   40.3431k      9.69786      176.745Mi/s
shake128/4096/64_stddev         5.63 ns         3.78 ns           10    1.29789     311.993u      30.4457Ki/s
shake128/4096/64_cv             0.03 %          0.02 %            10      0.00%        0.00%            0.02%
shake128/4096/64_min           22437 ns        22440 ns           10   40.3411k      9.69739      176.695Mi/s
shake128/4096/64_max           22454 ns        22453 ns           10    40.345k      9.69831      176.798Mi/s
sha3_224/4096_mean             25960 ns        25961 ns           10   46.6643k      11.3153      151.496Mi/s
sha3_224/4096_median           25961 ns        25961 ns           10   46.6632k       11.315      151.495Mi/s
sha3_224/4096_stddev            3.64 ns         2.81 ns           10    4.74013      1.1494m      16.7751Ki/s
sha3_224/4096_cv                0.01 %          0.01 %            10      0.01%        0.01%            0.01%
sha3_224/4096_min              25955 ns        25957 ns           10   46.6591k       11.314      151.464Mi/s
sha3_224/4096_max              25966 ns        25966 ns           10   46.6757k      11.3181      151.516Mi/s
sha3_384/1024_mean              8920 ns         8921 ns           10    16.035k       14.958      114.601Mi/s
sha3_384/1024_median            8920 ns         8920 ns           10   16.0346k      14.9577      114.607Mi/s
sha3_384/1024_stddev            2.74 ns         2.41 ns           10    1.37496     1.28261m      31.6902Ki/s
sha3_384/1024_cv                0.03 %          0.03 %            10      0.01%        0.01%            0.03%
sha3_384/1024_min               8918 ns         8919 ns           10   16.0336k      14.9568      114.516Mi/s
sha3_384/1024_max               8928 ns         8927 ns           10   16.0383k      14.9611      114.624Mi/s
sha3_384/16384_mean           138096 ns       138094 ns           10   248.196k      15.1044      113.479Mi/s
sha3_384/16384_median         138136 ns       138125 ns           10   248.218k      15.1058      113.453Mi/s
sha3_384/16384_stddev            174 ns          177 ns           10    334.625    0.0203642      148.935Ki/s
sha3_384/16384_cv               0.13 %          0.13 %            10      0.13%        0.13%            0.13%
sha3_384/16384_min            137890 ns       137893 ns           10   247.834k      15.0824      113.199Mi/s
sha3_384/16384_max            138420 ns       138435 ns           10   248.857k      15.1446      113.644Mi/s
sha3_384/64_mean                1060 ns         1060 ns           10   1.90527k      17.0114      100.768Mi/s
sha3_384/64_median              1060 ns         1060 ns           10   1.90537k      17.0123      100.775Mi/s
sha3_384/64_stddev             0.320 ns        0.336 ns           10   0.722997     6.45533m      32.7525Ki/s
sha3_384/64_cv                  0.03 %          0.03 %            10      0.04%        0.04%            0.03%
sha3_384/64_min                 1059 ns         1059 ns           10    1.9041k      17.0009      100.711Mi/s
sha3_384/64_max                 1061 ns         1061 ns           10   1.90615k      17.0192       100.82Mi/s
sha3_512/4096_mean             49713 ns        49714 ns           10   89.3619k      21.4812      79.8016Mi/s
sha3_512/4096_median           49709 ns        49713 ns           10    89.362k      21.4813      79.8036Mi/s
sha3_512/4096_stddev            18.5 ns         16.0 ns           10    23.0994     5.55275m      26.2649Ki/s
sha3_512/4096_cv                0.04 %          0.03 %            10      0.03%        0.03%            0.03%
sha3_512/4096_min              49692 ns        49698 ns           10   89.3225k      21.4718      79.7405Mi/s
sha3_512/4096_max              49757 ns        49752 ns           10   89.4112k      21.4931      79.8279Mi/s
shake256/16384/64_mean        107051 ns       107054 ns           10   192.402k      11.6976      146.526Mi/s
shake256/16384/64_median      107085 ns       107083 ns           10   192.414k      11.6983      146.484Mi/s
shake256/16384/64_stddev         204 ns          201 ns           10    354.397    0.0215465      281.878Ki/s
shake256/16384/64_cv            0.19 %          0.19 %            10      0.18%        0.18%            0.19%
shake256/16384/64_min         106814 ns       106825 ns           10   192.022k      11.6745      146.204Mi/s
shake256/16384/64_max         107278 ns       107289 ns           10   192.818k      11.7229      146.838Mi/s
sha3_256/256_mean               2013 ns         2013 ns           10   3.61754k      12.5609      136.463Mi/s
sha3_256/256_median             2013 ns         2013 ns           10   3.61741k      12.5605      136.461Mi/s
sha3_256/256_stddev            0.419 ns        0.485 ns           10   0.982835     3.41262m      33.6599Ki/s
sha3_256/256_cv                 0.02 %          0.02 %            10      0.03%        0.03%            0.02%
sha3_256/256_min                2012 ns         2012 ns           10   3.61602k      12.5556      136.405Mi/s
sha3_256/256_max                2013 ns         2014 ns           10   3.61951k      12.5677      136.521Mi/s
shake128/64/64_mean             1173 ns         1173 ns           10   2.10873k      16.4745      104.053Mi/s
shake128/64/64_median           1173 ns         1173 ns           10    2.1087k      16.4742      104.052Mi/s
shake128/64/64_stddev          0.223 ns        0.145 ns           10  0.0999248     780.663u      13.1417Ki/s
shake128/64/64_cv               0.02 %          0.01 %            10      0.00%        0.00%            0.01%
shake128/64/64_min              1173 ns         1173 ns           10   2.10862k      16.4736      104.033Mi/s
shake128/64/64_max              1173 ns         1173 ns           10   2.10895k      16.4762      104.072Mi/s
sha3_256/16384_mean           108442 ns       108447 ns           10   194.921k      11.8738      144.362Mi/s
sha3_256/16384_median         108380 ns       108382 ns           10   194.809k       11.867      144.448Mi/s
sha3_256/16384_stddev            311 ns          313 ns           10    564.025    0.0343582      425.739Ki/s
sha3_256/16384_cv               0.29 %          0.29 %            10      0.29%        0.29%            0.29%
sha3_256/16384_min            108049 ns       108061 ns           10   194.233k      11.8319      143.676Mi/s
sha3_256/16384_max            108964 ns       108964 ns           10    195.85k      11.9304      144.877Mi/s
shake128/16384/64_mean         87417 ns        87420 ns           10   157.108k      9.55182      179.434Mi/s
shake128/16384/64_median       87360 ns        87366 ns           10   157.032k      9.54721      179.544Mi/s
shake128/16384/64_stddev         148 ns          149 ns           10    250.579    0.0152346      312.577Ki/s
shake128/16384/64_cv            0.17 %          0.17 %            10      0.16%        0.16%            0.17%
shake128/16384/64_min          87169 ns        87162 ns           10   156.637k      9.52314      178.964Mi/s
shake128/16384/64_max          87651 ns        87649 ns           10   157.471k      9.57387      179.964Mi/s
sha3_256/64_mean                1112 ns         1112 ns           10   1.99811k      20.8136      82.3571Mi/s
sha3_256/64_median              1112 ns         1112 ns           10   1.99807k      20.8133      82.3575Mi/s
sha3_256/64_stddev             0.946 ns        0.937 ns           10    1.70455    0.0177557      71.0998Ki/s
sha3_256/64_cv                  0.09 %          0.08 %            10      0.09%        0.09%            0.08%
sha3_256/64_min                 1110 ns         1110 ns           10   1.99573k      20.7889      82.2608Mi/s
sha3_256/64_max                 1113 ns         1113 ns           10   2.00053k      20.8389      82.4637Mi/s
sha3_512/1024_mean             13218 ns        13218 ns           10   23.7582k      21.8365       78.499Mi/s
sha3_512/1024_median           13219 ns        13219 ns           10   23.7595k      21.8378      78.4923Mi/s
sha3_512/1024_stddev            5.95 ns         5.35 ns           10      9.384       8.625m      32.5618Ki/s
sha3_512/1024_cv                0.05 %          0.04 %            10      0.04%        0.04%            0.04%
sha3_512/1024_min              13206 ns        13208 ns           10   23.7442k      21.8237      78.4465Mi/s
sha3_512/1024_max              13227 ns        13227 ns           10   23.7738k      21.8509      78.5595Mi/s
shake128/1024/64_mean           6500 ns         6500 ns           10   11.6832k      10.7382      159.624Mi/s
shake128/1024/64_median         6500 ns         6500 ns           10   11.6832k      10.7382      159.629Mi/s
shake128/1024/64_stddev         1.67 ns         1.11 ns           10   0.150961     138.751u      27.8682Ki/s
shake128/1024/64_cv             0.03 %          0.02 %            10      0.00%        0.00%            0.02%
shake128/1024/64_min            6498 ns         6499 ns           10   11.6829k      10.7379      159.562Mi/s
shake128/1024/64_max            6504 ns         6503 ns           10   11.6834k      10.7384      159.655Mi/s
shake256/4096/64_mean          27567 ns        27568 ns           10   49.5523k      11.9116      143.908Mi/s
shake256/4096/64_median        27565 ns        27567 ns           10   49.5523k      11.9116      143.913Mi/s
shake256/4096/64_stddev         5.84 ns         3.87 ns           10    1.24483     299.238u      20.6705Ki/s
shake256/4096/64_cv             0.02 %          0.01 %            10      0.00%        0.00%            0.01%
shake256/4096/64_min           27561 ns        27563 ns           10   49.5502k      11.9111      143.871Mi/s
shake256/4096/64_max           27580 ns        27575 ns           10   49.5539k       11.912      143.933Mi/s
sha3_256/4096_mean             27886 ns        27885 ns           10   50.1184k      12.1411      141.176Mi/s
sha3_256/4096_median           27890 ns        27888 ns           10   50.1157k      12.1404      141.163Mi/s
sha3_256/4096_stddev            21.3 ns         20.7 ns           10    36.7096     8.89282m      107.092Ki/s
sha3_256/4096_cv                0.08 %          0.07 %            10      0.07%        0.07%            0.07%
sha3_256/4096_min              27854 ns        27857 ns           10   50.0751k      12.1306      141.039Mi/s
sha3_256/4096_max              27915 ns        27913 ns           10   50.1666k      12.1528      141.319Mi/s
sha3_224/1024_mean              7334 ns         7334 ns           10   13.1817k      12.5301      136.794Mi/s
sha3_224/1024_median            7335 ns         7334 ns           10   13.1818k      12.5302      136.796Mi/s
sha3_224/1024_stddev            1.39 ns        0.948 ns           10   0.795119     755.817u      18.0968Ki/s
sha3_224/1024_cv                0.02 %          0.01 %            10      0.01%        0.01%            0.01%
sha3_224/1024_min               7332 ns         7333 ns           10   13.1806k      12.5291       136.77Mi/s
sha3_224/1024_max               7336 ns         7335 ns           10   13.1832k      12.5316      136.816Mi/s
keccak-p[1600, 24]_mean          862 ns          862 ns           10   1.54907k      7.74537      221.318Mi/s
keccak-p[1600, 24]_median        862 ns          862 ns           10   1.54908k      7.74541      221.322Mi/s
keccak-p[1600, 24]_stddev      0.200 ns        0.136 ns           10   0.016987      84.935u      35.8821Ki/s
keccak-p[1600, 24]_cv           0.02 %          0.02 %            10      0.00%        0.00%            0.02%
keccak-p[1600, 24]_min           861 ns          862 ns           10   1.54904k      7.74522      221.258Mi/s
keccak-p[1600, 24]_max           862 ns          862 ns           10   1.54909k      7.74546      221.375Mi/s
sha3_256/1024_mean              7366 ns         7366 ns           10   13.2407k      12.5385      136.712Mi/s
sha3_256/1024_median            7365 ns         7366 ns           10   13.2411k      12.5389      136.721Mi/s
sha3_256/1024_stddev            3.53 ns         3.43 ns           10    6.45902      6.1165m       65.257Ki/s
sha3_256/1024_cv                0.05 %          0.05 %            10      0.05%        0.05%            0.05%
sha3_256/1024_min               7361 ns         7362 ns           10   13.2305k      12.5289       136.62Mi/s
sha3_256/1024_max               7371 ns         7371 ns           10   13.2523k      12.5496      136.792Mi/s
sha3_224/16384_mean           101629 ns       101627 ns           10   182.654k      11.1293      154.011Mi/s
sha3_224/16384_median         101604 ns       101585 ns           10   182.538k      11.1222      154.075Mi/s
sha3_224/16384_stddev            177 ns          180 ns           10    324.501    0.0197722      279.456Ki/s
sha3_224/16384_cv               0.17 %          0.18 %            10      0.18%        0.18%            0.18%
sha3_224/16384_min            101395 ns       101407 ns           10   182.286k      11.1069      153.594Mi/s
sha3_224/16384_max            101893 ns       101903 ns           10   183.169k      11.1607      154.346Mi/s
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

> [!NOTE]
> This library doesn't expose any raw pointer + length -based interfaces, rather everything is wrapped under much safer `std::span`` - which one can easily create from `std::{array, vector}` or even raw pointers and length pair. See https://en.cppreference.com/w/cpp/container/span. I made this choice because this gives us much better type safety and compile-time error reporting.
