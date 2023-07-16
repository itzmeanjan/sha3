# sha3
SHA3: Permutation-Based Hash and Extendable-Output Functions.

## Overview

SHA3 standard by NIST ( i.e. NIST FIPS PUB 202 ) specifies four ( of different digest length ) permutation-based hash functions and two extendable-output functions, which are built on top of keccak-p[1600, 24] permutation.

These hash functions and extendable output functions are pretty commonly used in various post-quantum cryptography algorithms ( those used for public key encryption, key establishment mechanism & digital signature ), some of which are already declared as selected candidates ( e.g. Kyber, Falcon, Dilithium, SPHINCS+ etc. ) of NIST PQC standardization effort - waiting to be standardized or some are still competing ( e.g. Bike, Classic McEliece etc. ) in final round of standardization. I decided to implement SHA3 specification as a **header-only C++ library**, so that I can make use of it as a modular dependency ( say pinned to specific commit using git submodule ) in libraries where I implement various PQC schemes.

Few of those places, where I've already used `sha3` as ( git submodule based ) dependency

- [Kyber: Post-Quantum Public-key Encryption & Key-establishment Algorithm](https://github.com/itzmeanjan/kyber)
- [Dilithium: Post-Quantum Digital Signature Algorithm](https://github.com/itzmeanjan/dilithium)
- [SPHINCS+: Stateless Hash-based Digital Signature Algorithm](https://github.com/itzmeanjan/sphincs)
- [Falcon: Fast-Fourier Lattice-based Compact Signatures over NTRU](https://github.com/itzmeanjan/falcon)
- [FrodoKEM: Practical Quantum-secure Key Encapsulation from Generic Lattices](https://github.com/itzmeanjan/frodokem)

> **Warning** Above list may not be up-to-date !

Here I'm maintaining a zero-dependency, header-only C++ library, using modern C++ features ( such as C++{>=11} ), which is fairly easy-to-use in your project, implementing SHA3 [specification](https://dx.doi.org/10.6028/NIST.FIPS.202) i.e. NIST FIPS PUB 202. Following algorithms ( with flexible interfaces ) are implemented in `sha3` library.

Algorithm | Input | Output | Behaviour | Namespace + Header
--- | :-: | :-: | :-: | --:
SHA3-224 | N ( >=0 ) -bytes message | 28 -bytes digest | Given N -bytes input message, this routine computes 28 -bytes sha3-224 digest, while *(incrementally)* consuming message into Keccak[448] sponge. | [`sha3_224::sha3_224`](./include/sha3_224.hpp)
SHA3-256 | N ( >=0 ) -bytes message | 32 -bytes digest | Given N -bytes input message, this routine computes 32 -bytes sha3-256 digest, while *(incrementally)* consuming message into Keccak[512] sponge. | [`sha3_256::sha3_256`](./include/sha3_256.hpp)
SHA3-384 | N ( >=0 ) -bytes message | 48 -bytes digest | Given N -bytes input message, this routine computes 48 -bytes sha3-384 digest, while *(incrementally)* consuming message into Keccak[768] sponge. | [`sha3_384::sha3_384`](./include/sha3_384.hpp)
SHA3-512 | N ( >=0 ) -bytes message | 64 -bytes digest | Given N -bytes input message, this routine computes 64 -bytes sha3-512 digest, while *(incrementally)* consuming message into Keccak[1024] sponge. | [`sha3_512::sha3_512`](./include/sha3_512.hpp)
SHAKE-128 | N ( >=0 ) -bytes message | M ( >=0 ) -bytes output | Given N -bytes input message, this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[256] sponge, which has already *(incrementally)* absorbed input bytes. | [`shake128::shake128`](./include/shake128.hpp)
SHAKE-256 | N ( >=0 ) -bytes message | M ( >=0 ) -bytes digest | Given N -bytes input message, this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[512] sponge, which has already *(incrementally)* absorbed input bytes. | [`shake256::shake256`](./include/shake256.hpp)

> **Note** Learn more about NIST PQC standardization effort [here](https://csrc.nist.gov/projects/post-quantum-cryptography/).

## Prerequisites

- A C++ compiler such as `g++`/ `clang++`, with support for C++20 standard library.

```bash
$ g++ --version
g++ (Ubuntu 12.2.0-17ubuntu1) 12.2.0

$ clang++ --version
Ubuntu clang version 15.0.7
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

Issue following command for running all the test cases.

```bash
$ make -j8

[==========] Running 12 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 8 tests from Sha3Hashing
[ RUN      ] Sha3Hashing.Sha3_224IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_224IncrementalAbsorption (1 ms)
[ RUN      ] Sha3Hashing.Sha3_224KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_224KnownAnswerTests (2 ms)
[ RUN      ] Sha3Hashing.Sha3_256IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_256IncrementalAbsorption (1 ms)
[ RUN      ] Sha3Hashing.Sha3_256KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_256KnownAnswerTests (2 ms)
[ RUN      ] Sha3Hashing.Sha3_384IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_384IncrementalAbsorption (1 ms)
[ RUN      ] Sha3Hashing.Sha3_384KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_384KnownAnswerTests (2 ms)
[ RUN      ] Sha3Hashing.Sha3_512IncrementalAbsorption
[       OK ] Sha3Hashing.Sha3_512IncrementalAbsorption (2 ms)
[ RUN      ] Sha3Hashing.Sha3_512KnownAnswerTests
[       OK ] Sha3Hashing.Sha3_512KnownAnswerTests (3 ms)
[----------] 8 tests from Sha3Hashing (17 ms total)

[----------] 4 tests from Sha3Xof
[ RUN      ] Sha3Xof.Shake128IncrementalAbsorptionAndSqueezing
[       OK ] Sha3Xof.Shake128IncrementalAbsorptionAndSqueezing (971 ms)
[ RUN      ] Sha3Xof.Shake128KnownAnswerTests
[       OK ] Sha3Xof.Shake128KnownAnswerTests (2 ms)
[ RUN      ] Sha3Xof.Shake256IncrementalAbsorptionAndSqueezing
[       OK ] Sha3Xof.Shake256IncrementalAbsorptionAndSqueezing (1060 ms)
[ RUN      ] Sha3Xof.Shake256KnownAnswerTests
[       OK ] Sha3Xof.Shake256KnownAnswerTests (2 ms)
[----------] 4 tests from Sha3Xof (2038 ms total)

[----------] Global test environment tear-down
[==========] 12 tests from 2 test suites ran. (2055 ms total)
[  PASSED  ] 12 tests.
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
2023-07-11T09:18:28+04:00
Running ./benchmarks/perf.out
Run on (16 X 601.957 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 18432 KiB (x1)
Load Average: 0.20, 0.49, 0.56
***WARNING*** There are 65 benchmarks with threads and 2 performance counters were requested. Beware counters will reflect the combined usage across all threads.
-------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations     CYCLES CYCLES/ BYTE INSTRUCTIONS INSTRUCTIONS/ CYCLE bytes_per_second
-------------------------------------------------------------------------------------------------------------------------------------------------
bench_sha3::keccakf1600             204 ns          204 ns      3382741    952.497      4.76249       5.689k             5.97272       935.552M/s
bench_sha3::sha3_224/32             238 ns          238 ns      2944147   1.11184k      18.5306       5.906k             5.31194       240.608M/s
bench_sha3::sha3_224/64             237 ns          237 ns      2956947   1.10706k      12.0333       5.906k             5.33485       370.651M/s
bench_sha3::sha3_224/128            235 ns          235 ns      2979980       1096      7.02566       5.916k              5.3978       634.041M/s
bench_sha3::sha3_224/256            438 ns          438 ns      1598391   2.04883k       7.2142      11.649k             5.68568       617.827M/s
bench_sha3::sha3_224/512            881 ns          881 ns       785228   4.12282k      7.63485      23.071k             5.59593       584.467M/s
bench_sha3::sha3_224/1024          1768 ns         1768 ns       396226   8.26518k      7.85664      45.904k              5.5539       567.618M/s
bench_sha3::sha3_224/2048          3323 ns         3323 ns       210738   15.5442k      7.48756      85.882k             5.52503       595.884M/s
bench_sha3::sha3_224/4096          6446 ns         6446 ns       108536   30.1042k      7.29976     165.836k             5.50873        610.16M/s
bench_sha3::sha3_256/32             240 ns          240 ns      2916048   1.12198k       17.531       5.953k             5.30579       254.407M/s
bench_sha3::sha3_256/64             239 ns          239 ns      2918469   1.11947k      11.6612       5.953k             5.31767        382.34M/s
bench_sha3::sha3_256/128            237 ns          237 ns      2950765   1.10777k      6.92358       5.963k             5.38287       643.691M/s
bench_sha3::sha3_256/256            467 ns          466 ns      1499310   2.18082k      7.57231      12.336k             5.65658       588.802M/s
bench_sha3::sha3_256/512            974 ns          974 ns       712676   4.54354k       8.3521      25.048k             5.51288       532.626M/s
bench_sha3::sha3_256/1024          2001 ns         2001 ns       350507   9.28836k       8.7958      50.472k              5.4339       503.398M/s
bench_sha3::sha3_256/2048          4077 ns         4077 ns       171979   18.7672k       9.0227     101.311k              5.3983       486.539M/s
bench_sha3::sha3_256/4096          7951 ns         7950 ns        87327   36.5089k      8.84421     196.649k             5.38633       495.208M/s
bench_sha3::sha3_384/32             237 ns          237 ns      2950914    1096.34      13.7042       5.938k             5.41621       321.547M/s
bench_sha3::sha3_384/64             237 ns          237 ns      2942018    1090.68      9.73821       5.938k             5.44431       450.048M/s
bench_sha3::sha3_384/128            436 ns          436 ns      1604990   1.98857k      11.2987      11.758k             5.91278       384.784M/s
bench_sha3::sha3_384/256            654 ns          654 ns      1074396   2.97678k      9.79203      17.569k             5.90202       443.049M/s
bench_sha3::sha3_384/512           1090 ns         1090 ns       647333   4.96156k      8.85992      29.199k             5.88505       489.793M/s
bench_sha3::sha3_384/1024          2176 ns         2176 ns       320268    9.9223k      9.25588      58.249k             5.87051       469.873M/s
bench_sha3::sha3_384/2048          4366 ns         4366 ns       160521   19.8368k      9.46412     116.349k             5.86531       457.868M/s
bench_sha3::sha3_384/4096          8739 ns         8739 ns        80166   39.6558k      9.56944     232.539k             5.86394       452.254M/s
bench_sha3::sha3_512/32             239 ns          239 ns      2922842    1084.61       11.298       5.915k             5.45357        383.06M/s
bench_sha3::sha3_512/64             239 ns          239 ns      2937449    1081.56      8.44968       5.915k             5.46896       510.621M/s
bench_sha3::sha3_512/128            439 ns          439 ns      1589147   1.98287k      10.3275      11.696k             5.89851       416.669M/s
bench_sha3::sha3_512/256            872 ns          872 ns       799908   3.94609k      12.3315      23.234k             5.88785       349.834M/s
bench_sha3::sha3_512/512           1748 ns         1748 ns       400630   7.88841k      13.6952      46.311k             5.87076       314.308M/s
bench_sha3::sha3_512/1024          3267 ns         3266 ns       213289   14.7807k      13.5852      86.692k             5.86523       317.684M/s
bench_sha3::sha3_512/2048          6347 ns         6346 ns       110975   28.6202k      13.5513     167.459k             5.85107       317.368M/s
bench_sha3::sha3_512/4096         12418 ns        12417 ns        56733   56.0995k      13.4854     328.991k             5.86442       319.508M/s
bench_sha3::shake128/32/32          241 ns          241 ns      2908442    1092.45      17.0695       5.798k             5.30735        253.49M/s
bench_sha3::shake128/64/32          241 ns          241 ns      2910706    1091.36      11.3683       5.798k             5.31265       379.296M/s
bench_sha3::shake128/128/32         242 ns          242 ns      2910558    1091.42      6.82136       5.808k             5.32152        631.03M/s
bench_sha3::shake128/256/32         459 ns          459 ns      1522674   2.07959k       7.2208       11.78k             5.66458       597.995M/s
bench_sha3::shake128/512/32         943 ns          943 ns       744327    4.2413k      7.79651      23.681k             5.58343       549.972M/s
bench_sha3::shake128/1024/32       1659 ns         1659 ns       421970   7.47771k      7.08116      41.544k             5.55571       607.027M/s
bench_sha3::shake128/2048/32       3118 ns         3118 ns       224275   13.9865k      6.72428      77.275k             5.52497        636.24M/s
bench_sha3::shake128/4096/32       5992 ns         5991 ns       117529   26.9722k      6.53397     148.735k             5.51438       657.067M/s
bench_sha3::shake128/32/64          240 ns          240 ns      2914777     1092.4      11.3792       5.798k             5.30758       380.773M/s
bench_sha3::shake128/64/64          240 ns          240 ns      2898743    1091.47      8.52711       5.798k              5.3121       507.998M/s
bench_sha3::shake128/128/64         241 ns          241 ns      2905659     1091.6      5.68542       5.808k             5.32063       759.543M/s
bench_sha3::shake128/256/64         460 ns          460 ns      1521896   2.07749k      6.49215       11.78k             5.67031        663.37M/s
bench_sha3::shake128/512/64         944 ns          944 ns       742232   4.23709k      7.35607      23.681k             5.58897       581.721M/s
bench_sha3::shake128/1024/64       1674 ns         1674 ns       420158   7.48884k      6.88313      41.544k             5.54745       619.997M/s
bench_sha3::shake128/2048/64       3123 ns         3122 ns       224317   13.9777k      6.61823      77.275k             5.52845       645.096M/s
bench_sha3::shake128/4096/64       6022 ns         6021 ns       117146   26.9551k       6.4796     148.735k             5.51787        658.88M/s
bench_sha3::shake256/32/32          249 ns          249 ns      2812374    1.1233k      17.5516       5.869k             5.22477       244.991M/s
bench_sha3::shake256/64/32          249 ns          249 ns      2803444   1.12052k      11.6721       5.869k             5.23775       367.748M/s
bench_sha3::shake256/128/32         247 ns          247 ns      2818505   1.10777k      6.92355       5.879k             5.30707       618.551M/s
bench_sha3::shake256/256/32         468 ns          468 ns      1493478   2.10194k      7.29841      11.916k             5.66904       587.218M/s
bench_sha3::shake256/512/32         955 ns          955 ns       730431   4.27319k      7.85514      23.954k             5.60564       543.478M/s
bench_sha3::shake256/1024/32       1927 ns         1927 ns       361182    8.6278k      8.17026       48.03k             5.56689       522.707M/s
bench_sha3::shake256/2048/32       3892 ns         3892 ns       180625   17.3318k      8.33258      96.173k             5.54894       509.653M/s
bench_sha3::shake256/4096/32       7502 ns         7501 ns        92328   33.6069k       8.1412     186.456k             5.54815       524.856M/s
bench_sha3::shake256/32/64          249 ns          249 ns      2793855   1.12242k      11.6919       5.869k             5.22887       367.932M/s
bench_sha3::shake256/64/64          248 ns          248 ns      2814934   1.12097k      8.75759       5.869k             5.23564       491.418M/s
bench_sha3::shake256/128/64         247 ns          247 ns      2836927   1.10692k       5.7652       5.879k             5.31114       740.755M/s
bench_sha3::shake256/256/64         470 ns          469 ns      1493341   2.10326k      6.57269      11.916k             5.66549       650.027M/s
bench_sha3::shake256/512/64         956 ns          956 ns       732281   4.27517k      7.42216      23.954k             5.60306       574.727M/s
bench_sha3::shake256/1024/64       1946 ns         1945 ns       362132   8.62615k      7.92844       48.03k             5.56796       533.376M/s
bench_sha3::shake256/2048/64       3910 ns         3910 ns       179003   17.3289k      8.20496      96.173k             5.54987       515.158M/s
bench_sha3::shake256/4096/64       7614 ns         7612 ns        91960   33.6347k      8.08525     186.456k             5.54357       521.159M/s
```

## Usage

`sha3` - C++ header-only library is written such that it's fairly easy for one to start using it in their project. All one needs to do

- Include proper header files ( select what you need by name ).
- Use proper struct(s)/ API(s)/ constant(s) ( see [usage examples](./example) or [test cases](./include/tests) ).
- When compiling, let your compiler know where it can find respective header files, which is `./include` directory.

Scheme | Header | Namespace | Example
--- | --- | --- | --:
SHA3-224 | ./include/sha3_224.hpp | `sha3_224::` | [example/sha3_224.cpp](./example/sha3_224.cpp)
SHA3-256 | ./include/sha3_256.hpp | `sha3_256::` | [example/sha3_256.cpp](./example/sha3_256.cpp)
SHA3-384 | ./include/sha3_384.hpp | `sha3_384::` | [example/sha3_384.cpp](./example/sha3_384.cpp)
SHA3-512 | ./include/sha3_512.hpp | `sha3_512::` | [example/sha3_512.cpp](./example/sha3_512.cpp)
SHAKE128 | ./include/shake128.hpp | `shake128::` | [example/shake128.cpp](./example/shake128.cpp)
SHAKE256 | ./include/shake256.hpp | `shake256::` | [example/shake256.cpp](./example/shake256.cpp)

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
