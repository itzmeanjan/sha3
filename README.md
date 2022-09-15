# sha3
SHA3: Permutation-Based Hash and Extendable-Output Functions

## Motivation

SHA3 standard by NIST specifies four permutation-based hash functions and two extendable-output functions, which are built on top of keccak-p[1600, 24] permutation.

These hash functions and extendable output functions are pretty commonly used in various post-quantum cryptography algorithms ( those used for key encapsulation & digital signature generation ) i.e. some of which are already declared as selected candidates ( e.g. Kyber, Falcon, Dilithium etc. ) of NIST PQC standardization effort or some are still competing ( e.g. Bike, Classic McEliece etc. ) in final round of standardization. This is exactly why I decided to implement SHA3 specification as **zero-dependency, header-only and easy-to-use C++ library**.

`sha3` - this project will be used in future implementations of various post-quantum cryptographic algorithms which are already selected or will be selected by NIST.

Here I'm maintaining a zero-dependency, header-only C++ library, using modern C++ features ( such as C++{11, 17, 20} ), which is fairly easy-to-use in your project, implementing SHA3 [specification](https://dx.doi.org/10.6028/NIST.FIPS.202) i.e. NIST FIPS PUB 202. Following algorithms are implemented in `sha3` library

Algorithm | Input | Output | Behaviour | Interface
--- | --- | --- | --- | ---
SHA3-224 | N ( >=0 ) -bytes message | 28 -bytes digest | Given N -bytes input message this routine computes 28 -bytes sha3-224 digest, while consuming message into Keccak[448] sponge | [`sha3_224::hash(...)`](./include/sha3_224.hpp)
SHA3-256 | N ( >=0 ) -bytes message | 32 -bytes digest | Given N -bytes input message this routine computes 32 -bytes sha3-256 digest, while consuming message into Keccak[512] sponge | [`sha3_256::hash(...)`](./include/sha3_256.hpp)
SHA3-384 | N ( >=0 ) -bytes message | 48 -bytes digest | Given N -bytes input message this routine computes 48 -bytes sha3-384 digest, while consuming message into Keccak[768] sponge | [`sha3_384::hash(...)`](./include/sha3_384.hpp)
SHA3-512 | N ( >=0 ) -bytes message | 64 -bytes digest | Given N -bytes input message this routine computes 64 -bytes sha3-512 digest, while consuming message into Keccak[1024] sponge | [`sha3_512::hash(...)`](./include/sha3_512.hpp)

> SHA3 extendable-output functions i.e. SHAKE{128, 256} are yet to be implemented.

---

> During implementation NIST FIPS PUB 202 was followed, which you can find [here](https://dx.doi.org/10.6028/NIST.FIPS.202)

> Learn more about NIST PQC standardization effort [here](https://csrc.nist.gov/projects/post-quantum-cryptography/)

> Find NIST PQC selected algorithms [here](https://csrc.nist.gov/projects/post-quantum-cryptography/selected-algorithms-2022)

## Prerequisites

- A C++ compiler such as `g++`/ `clang++`, which has support for C++20 standard library

```fish
$ g++ --version
g++ (Ubuntu 11.2.0-19ubuntu1) 11.2.0

$ clang++ --version
Ubuntu clang version 14.0.0-1ubuntu1
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

- System development utilities such as `cmake`, `make` & `git`

```fish
$ make --version
GNU Make 3.81

$ cmake --version
cmake version 3.23.2
```

- For ensuring functional correctness of implementations, you'll need `python3`

> Python `hashlib` module computed digests are used as source of truth during testing. More about [hashlib](https://docs.python.org/3/library/hashlib.html).

```fish
$ python3 --version
Python 3.10.7
```

- Install Python dependencies by issuing

```fish
python3 -m pip install --user -r wrapper/python/requirements.txt
```

- For benchmarking implementations on CPU systems, `google-benchmark` is required as system-wide installation. Please follow [this](https://github.com/google/benchmark/tree/60b16f1#installation) guide if you don't have it installed.

## Testing

For ensuring that SHA3 hash function and extendable output function implementations are correct & conformant to the NIST standard ( see [this](https://dx.doi.org/10.6028/NIST.FIPS.202) ), I make use of Python `hashlib` module as source of truth during testing. 

For randomly generated byte arrays of length N ( s.t. 0 <= N <= 1024 ), I compute SHA3-{224, 256, 384, 512} digest using both 

- (i) my implementation ( Python wrapper interface, [here](./wrapper/python/sha3.py) )
- (ii) `hashlib` implementation

and check for their equality.

Issue following command for running test cases

```fish
make
```

## Benchmarking

For benchmarking SHA3 hash functions and extendable output functions, on CPU systems, using `google-benchmark`, issue following command

> For disabling CPU scaling during benchmarking, follow [this](https://github.com/google/benchmark/blob/60b16f1/docs/user_guide.md#disabling-cpu-frequency-scaling)

> During benchmarking a byte array of length N is used as input | 32 <= N <= 4096 and N = 2^i

```fish
make benchmark
```

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( using `clang++` )

```fish
2022-09-15T13:21:41+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.35, 1.68, 1.89
------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------
bench_sha3::keccakf1600          367 ns          360 ns      1965977 bytes_per_second=530.175M/s
bench_sha3::sha3_224/32          372 ns          367 ns      1863958 bytes_per_second=83.0749M/s
bench_sha3::sha3_224/64          366 ns          362 ns      1926729 bytes_per_second=168.652M/s
bench_sha3::sha3_224/128         375 ns          370 ns      1926146 bytes_per_second=329.865M/s
bench_sha3::sha3_224/256         751 ns          740 ns       912813 bytes_per_second=329.742M/s
bench_sha3::sha3_224/512        1453 ns         1438 ns       481318 bytes_per_second=339.666M/s
bench_sha3::sha3_224/1024       2898 ns         2866 ns       245217 bytes_per_second=340.797M/s
bench_sha3::sha3_224/2048       5391 ns         5335 ns       128947 bytes_per_second=366.13M/s
bench_sha3::sha3_224/4096      10444 ns        10332 ns        67750 bytes_per_second=378.072M/s
bench_sha3::sha3_256/32          370 ns          366 ns      1905846 bytes_per_second=83.445M/s
bench_sha3::sha3_256/64          413 ns          402 ns      1842183 bytes_per_second=151.96M/s
bench_sha3::sha3_256/128         382 ns          374 ns      1869154 bytes_per_second=326.226M/s
bench_sha3::sha3_256/256         730 ns          722 ns       933719 bytes_per_second=338.117M/s
bench_sha3::sha3_256/512        1441 ns         1427 ns       483259 bytes_per_second=342.092M/s
bench_sha3::sha3_256/1024       2956 ns         2912 ns       243254 bytes_per_second=335.371M/s
bench_sha3::sha3_256/2048       5815 ns         5743 ns       114568 bytes_per_second=340.058M/s
bench_sha3::sha3_256/4096      11226 ns        11102 ns        61903 bytes_per_second=351.855M/s
bench_sha3::sha3_384/32          374 ns          370 ns      1892613 bytes_per_second=82.5433M/s
bench_sha3::sha3_384/64          375 ns          371 ns      1886213 bytes_per_second=164.679M/s
bench_sha3::sha3_384/128         756 ns          746 ns       930109 bytes_per_second=163.722M/s
bench_sha3::sha3_384/256        1090 ns         1080 ns       643548 bytes_per_second=226.109M/s
bench_sha3::sha3_384/512        1809 ns         1792 ns       389976 bytes_per_second=272.507M/s
bench_sha3::sha3_384/1024       3648 ns         3574 ns       195394 bytes_per_second=273.218M/s
bench_sha3::sha3_384/2048       7186 ns         7112 ns        95378 bytes_per_second=274.634M/s
bench_sha3::sha3_384/4096      14419 ns        14256 ns        48957 bytes_per_second=274.013M/s
bench_sha3::sha3_512/32          374 ns          370 ns      1894534 bytes_per_second=82.4267M/s
bench_sha3::sha3_512/64          367 ns          363 ns      1912093 bytes_per_second=168.115M/s
bench_sha3::sha3_512/128         726 ns          718 ns       949784 bytes_per_second=169.906M/s
bench_sha3::sha3_512/256        1442 ns         1428 ns       484054 bytes_per_second=170.988M/s
bench_sha3::sha3_512/512        2870 ns         2841 ns       245592 bytes_per_second=171.845M/s
bench_sha3::sha3_512/1024       5398 ns         5347 ns       125628 bytes_per_second=182.64M/s
bench_sha3::sha3_512/2048      10405 ns        10299 ns        66911 bytes_per_second=189.645M/s
bench_sha3::sha3_512/4096      21003 ns        20646 ns        34207 bytes_per_second=189.198M/s
```

### On AWS Graviton2 ( using `g++` )

```fish
2022-09-15T09:27:46+00:00
Running ./bench/a.out
Run on (16 X 166.66 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 48 KiB (x16)
  L2 Unified 2048 KiB (x4)
Load Average: 1.17, 0.30, 0.09
------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------
bench_sha3::keccakf1600         2054 ns         2054 ns       340847 bytes_per_second=92.8762M/s
bench_sha3::sha3_224/32         2058 ns         2058 ns       340252 bytes_per_second=14.8261M/s
bench_sha3::sha3_224/64         2062 ns         2062 ns       339524 bytes_per_second=29.606M/s
bench_sha3::sha3_224/128        2050 ns         2050 ns       341396 bytes_per_second=59.5378M/s
bench_sha3::sha3_224/256        4066 ns         4066 ns       172121 bytes_per_second=60.0407M/s
bench_sha3::sha3_224/512        8087 ns         8087 ns        86547 bytes_per_second=60.3753M/s
bench_sha3::sha3_224/1024      16128 ns        16128 ns        43403 bytes_per_second=60.5507M/s
bench_sha3::sha3_224/2048      30176 ns        30176 ns        23197 bytes_per_second=64.725M/s
bench_sha3::sha3_224/4096      58271 ns        58269 ns        12013 bytes_per_second=67.0385M/s
bench_sha3::sha3_256/32         2152 ns         2152 ns       325293 bytes_per_second=14.1818M/s
bench_sha3::sha3_256/64         2154 ns         2154 ns       324964 bytes_per_second=28.334M/s
bench_sha3::sha3_256/128        2142 ns         2142 ns       326814 bytes_per_second=56.9918M/s
bench_sha3::sha3_256/256        4260 ns         4260 ns       164288 bytes_per_second=57.3103M/s
bench_sha3::sha3_256/512        8465 ns         8465 ns        82686 bytes_per_second=57.6825M/s
bench_sha3::sha3_256/1024      16869 ns        16869 ns        41497 bytes_per_second=57.8921M/s
bench_sha3::sha3_256/2048      33683 ns        33682 ns        20783 bytes_per_second=57.987M/s
bench_sha3::sha3_256/4096      65214 ns        65213 ns        10737 bytes_per_second=59.9M/s
bench_sha3::sha3_384/32         2786 ns         2786 ns       251208 bytes_per_second=10.9524M/s
bench_sha3::sha3_384/64         2783 ns         2783 ns       251511 bytes_per_second=21.9303M/s
bench_sha3::sha3_384/128        5527 ns         5526 ns       126752 bytes_per_second=22.0883M/s
bench_sha3::sha3_384/256        8262 ns         8262 ns        84740 bytes_per_second=29.5504M/s
bench_sha3::sha3_384/512       13723 ns        13723 ns        51013 bytes_per_second=35.5819M/s
bench_sha3::sha3_384/1024      27382 ns        27382 ns        25563 bytes_per_second=35.6649M/s
bench_sha3::sha3_384/2048      54705 ns        54703 ns        12796 bytes_per_second=35.704M/s
bench_sha3::sha3_384/4096     109337 ns       109334 ns         6402 bytes_per_second=35.7276M/s
bench_sha3::sha3_512/32         2820 ns         2820 ns       248212 bytes_per_second=10.8212M/s
bench_sha3::sha3_512/64         2817 ns         2817 ns       248500 bytes_per_second=21.6689M/s
bench_sha3::sha3_512/128        5594 ns         5594 ns       125121 bytes_per_second=21.82M/s
bench_sha3::sha3_512/256       11133 ns        11132 ns        62878 bytes_per_second=21.9307M/s
bench_sha3::sha3_512/512       22195 ns        22195 ns        31538 bytes_per_second=21.9997M/s
bench_sha3::sha3_512/1024      41550 ns        41550 ns        16847 bytes_per_second=23.5036M/s
bench_sha3::sha3_512/2048      80262 ns        80262 ns         8720 bytes_per_second=24.3344M/s
bench_sha3::sha3_512/4096     157686 ns       157683 ns         4439 bytes_per_second=24.7729M/s
```

### On AWS Graviton3 ( using `g++` )

```fish
2022-09-15T09:31:49+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.18, 0.05, 0.01
------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------
bench_sha3::keccakf1600          737 ns          737 ns       949302 bytes_per_second=258.684M/s
bench_sha3::sha3_224/32          752 ns          752 ns       932225 bytes_per_second=40.5813M/s
bench_sha3::sha3_224/64          749 ns          749 ns       935098 bytes_per_second=81.5337M/s
bench_sha3::sha3_224/128         746 ns          746 ns       938969 bytes_per_second=163.689M/s
bench_sha3::sha3_224/256        1473 ns         1473 ns       475248 bytes_per_second=165.775M/s
bench_sha3::sha3_224/512        2944 ns         2944 ns       237821 bytes_per_second=165.879M/s
bench_sha3::sha3_224/1024       5862 ns         5862 ns       119379 bytes_per_second=166.59M/s
bench_sha3::sha3_224/2048      10963 ns        10963 ns        63850 bytes_per_second=178.154M/s
bench_sha3::sha3_224/4096      21156 ns        21155 ns        33092 bytes_per_second=184.647M/s
bench_sha3::sha3_256/32          740 ns          740 ns       946017 bytes_per_second=41.2476M/s
bench_sha3::sha3_256/64          735 ns          735 ns       951802 bytes_per_second=82.9915M/s
bench_sha3::sha3_256/128         733 ns          733 ns       955658 bytes_per_second=166.645M/s
bench_sha3::sha3_256/256        1462 ns         1462 ns       477833 bytes_per_second=167.014M/s
bench_sha3::sha3_256/512        2919 ns         2919 ns       239211 bytes_per_second=167.252M/s
bench_sha3::sha3_256/1024       5835 ns         5835 ns       120027 bytes_per_second=167.36M/s
bench_sha3::sha3_256/2048      11582 ns        11582 ns        60409 bytes_per_second=168.641M/s
bench_sha3::sha3_256/4096      22297 ns        22296 ns        31229 bytes_per_second=175.202M/s
bench_sha3::sha3_384/32          741 ns          741 ns       946813 bytes_per_second=41.2061M/s
bench_sha3::sha3_384/64          737 ns          737 ns       950086 bytes_per_second=82.8337M/s
bench_sha3::sha3_384/128        1465 ns         1465 ns       477611 bytes_per_second=83.3197M/s
bench_sha3::sha3_384/256        2179 ns         2179 ns       321873 bytes_per_second=112.033M/s
bench_sha3::sha3_384/512        3613 ns         3613 ns       193661 bytes_per_second=135.136M/s
bench_sha3::sha3_384/1024       7198 ns         7197 ns        97029 bytes_per_second=135.683M/s
bench_sha3::sha3_384/2048      14362 ns        14362 ns        48810 bytes_per_second=135.994M/s
bench_sha3::sha3_384/4096      28649 ns        28649 ns        24466 bytes_per_second=136.351M/s
bench_sha3::sha3_512/32          714 ns          714 ns       983281 bytes_per_second=42.7417M/s
bench_sha3::sha3_512/64          712 ns          712 ns       980398 bytes_per_second=85.7099M/s
bench_sha3::sha3_512/128        1408 ns         1408 ns       498309 bytes_per_second=86.7256M/s
bench_sha3::sha3_512/256        2815 ns         2815 ns       248564 bytes_per_second=86.7227M/s
bench_sha3::sha3_512/512        5605 ns         5605 ns       124932 bytes_per_second=87.1155M/s
bench_sha3::sha3_512/1024      10506 ns        10506 ns        66340 bytes_per_second=92.9542M/s
bench_sha3::sha3_512/2048      20269 ns        20268 ns        34435 bytes_per_second=96.3637M/s
bench_sha3::sha3_512/4096      40012 ns        40011 ns        17465 bytes_per_second=97.6297M/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( using `g++` )

```fish
2022-09-15T09:34:43+00:00
Running ./bench/a.out
Run on (4 X 2300.22 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.22, 0.07, 0.02
------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------
bench_sha3::keccakf1600         1113 ns         1113 ns       633006 bytes_per_second=171.34M/s
bench_sha3::sha3_224/32         1202 ns         1202 ns       581497 bytes_per_second=25.3981M/s
bench_sha3::sha3_224/64         1205 ns         1205 ns       582634 bytes_per_second=50.665M/s
bench_sha3::sha3_224/128        1203 ns         1203 ns       582177 bytes_per_second=101.49M/s
bench_sha3::sha3_224/256        2390 ns         2390 ns       293398 bytes_per_second=102.152M/s
bench_sha3::sha3_224/512        4752 ns         4752 ns       147386 bytes_per_second=102.757M/s
bench_sha3::sha3_224/1024       9477 ns         9477 ns        73809 bytes_per_second=103.047M/s
bench_sha3::sha3_224/2048      17724 ns        17722 ns        39459 bytes_per_second=110.209M/s
bench_sha3::sha3_224/4096      34284 ns        34283 ns        20451 bytes_per_second=113.942M/s
bench_sha3::sha3_256/32         1150 ns         1150 ns       606285 bytes_per_second=26.5364M/s
bench_sha3::sha3_256/64         1148 ns         1148 ns       610131 bytes_per_second=53.1855M/s
bench_sha3::sha3_256/128        1139 ns         1139 ns       613665 bytes_per_second=107.16M/s
bench_sha3::sha3_256/256        2265 ns         2264 ns       309528 bytes_per_second=107.812M/s
bench_sha3::sha3_256/512        4499 ns         4499 ns       155393 bytes_per_second=108.531M/s
bench_sha3::sha3_256/1024       8984 ns         8983 ns        77843 bytes_per_second=108.715M/s
bench_sha3::sha3_256/2048      17960 ns        17957 ns        38994 bytes_per_second=108.767M/s
bench_sha3::sha3_256/4096      34760 ns        34757 ns        20115 bytes_per_second=112.388M/s
bench_sha3::sha3_384/32         1096 ns         1095 ns       637855 bytes_per_second=27.8592M/s
bench_sha3::sha3_384/64         1096 ns         1095 ns       638035 bytes_per_second=55.7159M/s
bench_sha3::sha3_384/128        2175 ns         2175 ns       322616 bytes_per_second=56.1213M/s
bench_sha3::sha3_384/256        3253 ns         3252 ns       215101 bytes_per_second=75.0625M/s
bench_sha3::sha3_384/512        5383 ns         5382 ns       129482 bytes_per_second=90.7186M/s
bench_sha3::sha3_384/1024      10709 ns        10708 ns        65363 bytes_per_second=91.1985M/s
bench_sha3::sha3_384/2048      21444 ns        21444 ns        32735 bytes_per_second=91.0814M/s
bench_sha3::sha3_384/4096      42666 ns        42664 ns        16398 bytes_per_second=91.559M/s
bench_sha3::sha3_512/32         1071 ns         1071 ns       654337 bytes_per_second=28.4828M/s
bench_sha3::sha3_512/64         1065 ns         1065 ns       656514 bytes_per_second=57.3062M/s
bench_sha3::sha3_512/128        2122 ns         2122 ns       330484 bytes_per_second=57.5395M/s
bench_sha3::sha3_512/256        4221 ns         4220 ns       165966 bytes_per_second=57.8543M/s
bench_sha3::sha3_512/512        8416 ns         8415 ns        83210 bytes_per_second=58.0239M/s
bench_sha3::sha3_512/1024      15748 ns        15747 ns        44481 bytes_per_second=62.0153M/s
bench_sha3::sha3_512/2048      30539 ns        30537 ns        22973 bytes_per_second=63.9597M/s
bench_sha3::sha3_512/4096      59782 ns        59779 ns        11710 bytes_per_second=65.3451M/s
```

## Usage

`sha3` C++ library is written such that it's fairly easy for one to start using it in their project. All one needs to do

- Include proper header files
- Properly use API
- When compiling, let your compiler know where it can find respective header files

Algorithm | Header/ Namespace | Example
--- | --- | --:
SHA3-224 | [`sha3_224::`](./include/sha3_224.hpp) | [example/sha3_224.cpp](./example/sha3_224.cpp)
SHA3-256 | [`sha3_256::`](./include/sha3_256.hpp) | [example/sha3_256.cpp](./example/sha3_256.cpp)
SHA3-384 | [`sha3_384::`](./include/sha3_384.hpp) | [example/sha3_384.cpp](./example/sha3_384.cpp)
SHA3-512 | [`sha3_512::`](./include/sha3_512.hpp) | [example/sha3_512.cpp](./example/sha3_512.cpp)

```fish
$ g++ -std=c++20 -Wall -O3 -I include example/sha3_224.cpp && ./a.out
SHA3-224

Input  : 043f0fa310343b6ca42c3d2ab6f168574fd41774d49c9c1e5922c2cd60b43dbb
Output : 3bfbd5e41e850f29daf9c08dbcaca7c43ca939e7d6c0b6d8993c6af4

# ---

$ g++ -std=c++20 -Wall -O3 -I include example/sha3_256.cpp && ./a.out
SHA3-256

Input  : 70a3bf382218c7f4ae25775ab1d21f9d48e2f03af70dcdec790a338e982e6fa8
Output : 57be0ef9634da2d94219c53032809f4ffc145df6782279a8059afe607715d675

# ---

$ g++ -std=c++20 -Wall -O3 -I include example/sha3_384.cpp && ./a.out
SHA3-384

Input  : 314686636dc0499f2ebf0a201fe2d44e2e8888ac1109939998230f2cba5d0e94
Output : 554f4506a1b73724d0ce25cc4a0c0b4fc26478cde43013a59c7e25a22e3e73fbcfa731fe9f5050f77e82530fba7de512

# ---

$ g++ -std=c++20 -Wall -O3 -I include example/sha3_512.cpp && ./a.out
SHA3-512

Input  : 2c3c0ae485204067f1ecbc69a8fefd19a94c9c1552158a8d57a6612b3202f373
Output : 578386bdd6eb816d6d0cbc984351c889f70675a2661ba605aa65ce204b88a6d6553946c52eeac937647929f99a2ddd7cc6645bc1e89df8ab795faba6dd7d0d45
```
