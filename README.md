# sha3
SHA3: Permutation-Based Hash and Extendable-Output Functions

## Motivation

SHA3 standard by NIST specifies four permutation-based hash functions and two extendable-output functions, which are built on top of keccak-p[1600, 24] permutation.

These hash functions and extendable output functions are pretty commonly used in various post-quantum cryptography algorithms ( those used for public key encryption, key establishment mechanism & digital signature generation ) i.e. some of which are already declared as selected candidates ( e.g. Kyber, Falcon, Dilithium etc. ) of NIST PQC standardization effort or some are still competing ( e.g. Bike, Classic McEliece etc. ) in final round of standardization. This is exactly why I decided to implement SHA3 specification as **zero-dependency, header-only and easy-to-use C++ library**.

> **Note**
> `sha3` - this project will be used in various post-quantum cryptographic algorithm implementations which are already selected or will be selected by NIST.

Few of those places, where I've already used `sha3` as ( git submodule based ) dependency

- [Kyber: Post-Quantum Public-key Encryption & Key-establishment Algorithm](https://github.com/itzmeanjan/kyber)
- [Dilithium: Post-Quantum Digital Signature Algorithm](https://github.com/itzmeanjan/dilithium)

Here I'm maintaining a zero-dependency, header-only C++ library, using modern C++ features ( such as C++{11, 17, 20} ), which is fairly easy-to-use in your project, implementing SHA3 [specification](https://dx.doi.org/10.6028/NIST.FIPS.202) i.e. NIST FIPS PUB 202. Following algorithms are implemented in `sha3` library

Algorithm | Input | Output | Behaviour | Interface
--- | --- | --- | --- | ---
SHA3-224 | N ( >=0 ) -bytes message | 28 -bytes digest | Given N -bytes input message this routine computes 28 -bytes sha3-224 digest, while consuming message into Keccak[448] sponge | [`sha3_224::hash(...)`](./include/sha3_224.hpp)
SHA3-256 | N ( >=0 ) -bytes message | 32 -bytes digest | Given N -bytes input message this routine computes 32 -bytes sha3-256 digest, while consuming message into Keccak[512] sponge | [`sha3_256::hash(...)`](./include/sha3_256.hpp)
SHA3-384 | N ( >=0 ) -bytes message | 48 -bytes digest | Given N -bytes input message this routine computes 48 -bytes sha3-384 digest, while consuming message into Keccak[768] sponge | [`sha3_384::hash(...)`](./include/sha3_384.hpp)
SHA3-512 | N ( >=0 ) -bytes message | 64 -bytes digest | Given N -bytes input message this routine computes 64 -bytes sha3-512 digest, while consuming message into Keccak[1024] sponge | [`sha3_512::hash(...)`](./include/sha3_512.hpp)
SHAKE-128 | N ( >=0 ) -bytes message **[ Supports incremental message consumption ]** | M ( >=0 ) -bytes digest | Given N -bytes input message this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[256] sponge, which has already absorbed input bytes | [`shake128::{hash(...), read(...)}`](./include/shake128.hpp)
SHAKE-256 | N ( >=0 ) -bytes message **[ Supports incremental message consumption ]** | M ( >=0 ) -bytes digest | Given N -bytes input message this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[512] sponge, which has already absorbed input bytes | [`shake256::{hash(...), read(...)}`](./include/shake256.hpp)

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

For randomly generated byte arrays of length N ( s.t. 0 <= N <= 1024 ), I compute SHA3-{224, 256, 384, 512} & SHAKE{128, 256} digest using both 

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

> When benchmarking extendable output functions ( XoFs ), output of 32, 64 and 128 -bytes are squeezed from sponge, for each input byte array of length N | 32 <= N <= 4096 and N = 2^i

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
bench_sha3::shake128/32/32           489 ns          489 ns      1395590 bytes_per_second=124.784M/s
bench_sha3::shake128/32/64           621 ns          620 ns      1113905 bytes_per_second=147.713M/s
bench_sha3::shake128/32/128          871 ns          871 ns       789774 bytes_per_second=175.215M/s
bench_sha3::shake128/64/32           490 ns          490 ns      1393312 bytes_per_second=186.923M/s
bench_sha3::shake128/64/64           615 ns          615 ns      1105793 bytes_per_second=198.607M/s
bench_sha3::shake128/64/128          876 ns          875 ns       789649 bytes_per_second=209.188M/s
bench_sha3::shake128/128/32          487 ns          486 ns      1389523 bytes_per_second=313.768M/s
bench_sha3::shake128/128/64          620 ns          619 ns      1108507 bytes_per_second=295.716M/s
bench_sha3::shake128/128/128         867 ns          867 ns       787384 bytes_per_second=281.592M/s
bench_sha3::shake128/256/32          819 ns          819 ns       820999 bytes_per_second=335.524M/s
bench_sha3::shake128/256/64          946 ns          945 ns       717735 bytes_per_second=322.926M/s
bench_sha3::shake128/256/128        1206 ns         1205 ns       571611 bytes_per_second=303.905M/s
bench_sha3::shake128/512/32         1529 ns         1506 ns       465435 bytes_per_second=344.515M/s
bench_sha3::shake128/512/64         1624 ns         1623 ns       425149 bytes_per_second=338.487M/s
bench_sha3::shake128/512/128        1870 ns         1869 ns       370945 bytes_per_second=326.581M/s
bench_sha3::shake128/1024/32        2501 ns         2499 ns       279109 bytes_per_second=403.008M/s
bench_sha3::shake128/1024/64        2625 ns         2622 ns       265885 bytes_per_second=395.671M/s
bench_sha3::shake128/1024/128       2885 ns         2883 ns       242030 bytes_per_second=381.109M/s
bench_sha3::shake128/2048/32        4532 ns         4528 ns       155489 bytes_per_second=438.057M/s
bench_sha3::shake128/2048/64        4658 ns         4652 ns       150547 bytes_per_second=432.93M/s
bench_sha3::shake128/2048/128       4919 ns         4914 ns       137554 bytes_per_second=422.267M/s
bench_sha3::shake128/4096/32        8524 ns         8520 ns        79844 bytes_per_second=462.05M/s
bench_sha3::shake128/4096/64        8703 ns         8697 ns        77844 bytes_per_second=456.188M/s
bench_sha3::shake128/4096/128       8931 ns         8926 ns        76237 bytes_per_second=451.315M/s
bench_sha3::shake256/32/32           515 ns          514 ns      1302932 bytes_per_second=118.68M/s
bench_sha3::shake256/32/64           671 ns          671 ns      1014081 bytes_per_second=136.444M/s
bench_sha3::shake256/32/128          991 ns          991 ns       692562 bytes_per_second=154.026M/s
bench_sha3::shake256/64/32           521 ns          520 ns      1324604 bytes_per_second=176.054M/s
bench_sha3::shake256/64/64           699 ns          696 ns      1002549 bytes_per_second=175.355M/s
bench_sha3::shake256/64/128         1008 ns         1007 ns       698533 bytes_per_second=181.89M/s
bench_sha3::shake256/128/32          513 ns          513 ns      1324253 bytes_per_second=297.412M/s
bench_sha3::shake256/128/64          674 ns          674 ns      1014390 bytes_per_second=271.75M/s
bench_sha3::shake256/128/128         985 ns          985 ns       689383 bytes_per_second=247.92M/s
bench_sha3::shake256/256/32          851 ns          850 ns       812442 bytes_per_second=323.236M/s
bench_sha3::shake256/256/64         1004 ns         1003 ns       686530 bytes_per_second=304.285M/s
bench_sha3::shake256/256/128        1326 ns         1325 ns       516239 bytes_per_second=276.459M/s
bench_sha3::shake256/512/32         1517 ns         1515 ns       448652 bytes_per_second=342.41M/s
bench_sha3::shake256/512/64         1685 ns         1683 ns       414248 bytes_per_second=326.374M/s
bench_sha3::shake256/512/128        1992 ns         1991 ns       343371 bytes_per_second=306.581M/s
bench_sha3::shake256/1024/32        2858 ns         2855 ns       245762 bytes_per_second=352.716M/s
bench_sha3::shake256/1024/64        3039 ns         3037 ns       231338 bytes_per_second=341.652M/s
bench_sha3::shake256/1024/128       3349 ns         3344 ns       210160 bytes_per_second=328.5M/s
bench_sha3::shake256/2048/32        5541 ns         5538 ns       120996 bytes_per_second=358.186M/s
bench_sha3::shake256/2048/64        5709 ns         5705 ns       119503 bytes_per_second=353.081M/s
bench_sha3::shake256/2048/128       5999 ns         5994 ns       113721 bytes_per_second=346.222M/s
bench_sha3::shake256/4096/32       10528 ns        10518 ns        64769 bytes_per_second=374.271M/s
bench_sha3::shake256/4096/64       10791 ns        10780 ns        64059 bytes_per_second=368.023M/s
bench_sha3::shake256/4096/128      11128 ns        11116 ns        62334 bytes_per_second=362.397M/s
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
bench_sha3::shake128/32/32          2408 ns         2408 ns       290667 bytes_per_second=25.3444M/s
bench_sha3::shake128/32/64          2617 ns         2617 ns       267468 bytes_per_second=34.9832M/s
bench_sha3::shake128/32/128         3035 ns         3035 ns       230667 bytes_per_second=50.2818M/s
bench_sha3::shake128/64/32          2409 ns         2409 ns       290556 bytes_per_second=38.0037M/s
bench_sha3::shake128/64/64          2618 ns         2618 ns       267388 bytes_per_second=46.6297M/s
bench_sha3::shake128/64/128         3036 ns         3036 ns       230588 bytes_per_second=60.3204M/s
bench_sha3::shake128/128/32         2410 ns         2410 ns       290412 bytes_per_second=63.3059M/s
bench_sha3::shake128/128/64         2614 ns         2614 ns       267783 bytes_per_second=70.0493M/s
bench_sha3::shake128/128/128        3037 ns         3037 ns       230503 bytes_per_second=80.3927M/s
bench_sha3::shake128/256/32         4568 ns         4568 ns       153246 bytes_per_second=60.1239M/s
bench_sha3::shake128/256/64         4776 ns         4776 ns       146471 bytes_per_second=63.8986M/s
bench_sha3::shake128/256/128        5197 ns         5197 ns       134778 bytes_per_second=70.4695M/s
bench_sha3::shake128/512/32         8864 ns         8864 ns        78966 bytes_per_second=58.5301M/s
bench_sha3::shake128/512/64         9071 ns         9071 ns        77165 bytes_per_second=60.5594M/s
bench_sha3::shake128/512/128        9489 ns         9489 ns        73768 bytes_per_second=64.3247M/s
bench_sha3::shake128/1024/32       15285 ns        15285 ns        45797 bytes_per_second=65.8883M/s
bench_sha3::shake128/1024/64       15493 ns        15493 ns        45180 bytes_per_second=66.9714M/s
bench_sha3::shake128/1024/128      15911 ns        15911 ns        43993 bytes_per_second=69.0488M/s
bench_sha3::shake128/2048/32       28122 ns        28122 ns        24891 bytes_per_second=70.5378M/s
bench_sha3::shake128/2048/64       28332 ns        28331 ns        24708 bytes_per_second=71.0931M/s
bench_sha3::shake128/2048/128      28748 ns        28748 ns        24347 bytes_per_second=72.1851M/s
bench_sha3::shake128/4096/32       53819 ns        53817 ns        13005 bytes_per_second=73.1512M/s
bench_sha3::shake128/4096/64       54033 ns        54029 ns        12956 bytes_per_second=73.4293M/s
bench_sha3::shake128/4096/128      54442 ns        54442 ns        12857 bytes_per_second=73.9928M/s
bench_sha3::shake256/32/32          2261 ns         2261 ns       309645 bytes_per_second=26.9978M/s
bench_sha3::shake256/32/64          2470 ns         2470 ns       283439 bytes_per_second=37.073M/s
bench_sha3::shake256/32/128         2887 ns         2887 ns       242447 bytes_per_second=52.8482M/s
bench_sha3::shake256/64/32          2263 ns         2263 ns       309331 bytes_per_second=40.4584M/s
bench_sha3::shake256/64/64          2472 ns         2472 ns       283194 bytes_per_second=49.3851M/s
bench_sha3::shake256/64/128         2889 ns         2889 ns       242269 bytes_per_second=63.373M/s
bench_sha3::shake256/128/32         2250 ns         2249 ns       311185 bytes_per_second=67.8322M/s
bench_sha3::shake256/128/64         2458 ns         2458 ns       284753 bytes_per_second=74.4858M/s
bench_sha3::shake256/128/128        2876 ns         2876 ns       243397 bytes_per_second=84.8878M/s
bench_sha3::shake256/256/32         4255 ns         4255 ns       164492 bytes_per_second=64.548M/s
bench_sha3::shake256/256/64         4465 ns         4465 ns       156876 bytes_per_second=68.356M/s
bench_sha3::shake256/256/128        4885 ns         4885 ns       143272 bytes_per_second=74.9691M/s
bench_sha3::shake256/512/32         8247 ns         8247 ns        84882 bytes_per_second=62.9057M/s
bench_sha3::shake256/512/64         8464 ns         8464 ns        82708 bytes_per_second=64.9032M/s
bench_sha3::shake256/512/128        8874 ns         8874 ns        78886 bytes_per_second=68.7784M/s
bench_sha3::shake256/1024/32       16229 ns        16229 ns        43129 bytes_per_second=62.0553M/s
bench_sha3::shake256/1024/64       16439 ns        16438 ns        42582 bytes_per_second=63.1211M/s
bench_sha3::shake256/1024/128      16856 ns        16856 ns        41523 bytes_per_second=65.1778M/s
bench_sha3::shake256/2048/32       32179 ns        32178 ns        21753 bytes_per_second=61.645M/s
bench_sha3::shake256/2048/64       32388 ns        32387 ns        21613 bytes_per_second=62.1895M/s
bench_sha3::shake256/2048/128      32806 ns        32805 ns        21338 bytes_per_second=63.2582M/s
bench_sha3::shake256/4096/32       62079 ns        62078 ns        11274 bytes_per_second=63.4164M/s
bench_sha3::shake256/4096/64       62286 ns        62286 ns        11237 bytes_per_second=63.6948M/s
bench_sha3::shake256/4096/128      62708 ns        62706 ns        11162 bytes_per_second=64.2415M/s
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
bench_sha3::shake128/32/32           818 ns          818 ns       857216 bytes_per_second=74.643M/s
bench_sha3::shake128/32/64           883 ns          883 ns       792880 bytes_per_second=103.714M/s
bench_sha3::shake128/32/128         1012 ns         1012 ns       691865 bytes_per_second=150.792M/s
bench_sha3::shake128/64/32           820 ns          820 ns       852673 bytes_per_second=111.595M/s
bench_sha3::shake128/64/64           885 ns          885 ns       791779 bytes_per_second=137.951M/s
bench_sha3::shake128/64/128         1013 ns         1013 ns       690520 bytes_per_second=180.732M/s
bench_sha3::shake128/128/32          820 ns          820 ns       850859 bytes_per_second=186.149M/s
bench_sha3::shake128/128/64          888 ns          888 ns       788852 bytes_per_second=206.295M/s
bench_sha3::shake128/128/128        1014 ns         1014 ns       692374 bytes_per_second=240.838M/s
bench_sha3::shake128/256/32         1568 ns         1568 ns       445766 bytes_per_second=175.133M/s
bench_sha3::shake128/256/64         1637 ns         1637 ns       429281 bytes_per_second=186.474M/s
bench_sha3::shake128/256/128        1765 ns         1765 ns       397306 bytes_per_second=207.47M/s
bench_sha3::shake128/512/32         3054 ns         3054 ns       229153 bytes_per_second=169.858M/s
bench_sha3::shake128/512/64         3120 ns         3120 ns       224406 bytes_per_second=176.085M/s
bench_sha3::shake128/512/128        3250 ns         3250 ns       215283 bytes_per_second=187.816M/s
bench_sha3::shake128/1024/32        5269 ns         5269 ns       132817 bytes_per_second=191.122M/s
bench_sha3::shake128/1024/64        5334 ns         5334 ns       130948 bytes_per_second=194.531M/s
bench_sha3::shake128/1024/128       5469 ns         5469 ns       128184 bytes_per_second=200.898M/s
bench_sha3::shake128/2048/32        9693 ns         9693 ns        72260 bytes_per_second=204.65M/s
bench_sha3::shake128/2048/64        9758 ns         9758 ns        71842 bytes_per_second=206.415M/s
bench_sha3::shake128/2048/128       9884 ns         9883 ns        70758 bytes_per_second=209.969M/s
bench_sha3::shake128/4096/32       18531 ns        18530 ns        37745 bytes_per_second=212.454M/s
bench_sha3::shake128/4096/64       18607 ns        18607 ns        37448 bytes_per_second=213.215M/s
bench_sha3::shake128/4096/128      18738 ns        18738 ns        37402 bytes_per_second=214.982M/s
bench_sha3::shake256/32/32           808 ns          808 ns       865934 bytes_per_second=75.5768M/s
bench_sha3::shake256/32/64           892 ns          892 ns       784831 bytes_per_second=102.617M/s
bench_sha3::shake256/32/128         1049 ns         1049 ns       667791 bytes_per_second=145.497M/s
bench_sha3::shake256/64/32           802 ns          802 ns       874175 bytes_per_second=114.148M/s
bench_sha3::shake256/64/64           879 ns          879 ns       797421 bytes_per_second=138.904M/s
bench_sha3::shake256/64/128         1041 ns         1041 ns       675595 bytes_per_second=175.857M/s
bench_sha3::shake256/128/32          796 ns          796 ns       879107 bytes_per_second=191.71M/s
bench_sha3::shake256/128/64          866 ns          866 ns       806747 bytes_per_second=211.339M/s
bench_sha3::shake256/128/128        1026 ns         1026 ns       687834 bytes_per_second=238.046M/s
bench_sha3::shake256/256/32         1518 ns         1518 ns       460664 bytes_per_second=180.882M/s
bench_sha3::shake256/256/64         1597 ns         1597 ns       438829 bytes_per_second=191.111M/s
bench_sha3::shake256/256/128        1754 ns         1754 ns       399220 bytes_per_second=208.757M/s
bench_sha3::shake256/512/32         2963 ns         2963 ns       236254 bytes_per_second=175.101M/s
bench_sha3::shake256/512/64         3042 ns         3042 ns       230110 bytes_per_second=180.555M/s
bench_sha3::shake256/512/128        3204 ns         3204 ns       218371 bytes_per_second=190.489M/s
bench_sha3::shake256/1024/32        5785 ns         5785 ns       121035 bytes_per_second=174.08M/s
bench_sha3::shake256/1024/64        5865 ns         5865 ns       119301 bytes_per_second=176.919M/s
bench_sha3::shake256/1024/128       6024 ns         6024 ns       116197 bytes_per_second=182.368M/s
bench_sha3::shake256/2048/32       11436 ns        11436 ns        61204 bytes_per_second=173.46M/s
bench_sha3::shake256/2048/64       11511 ns        11511 ns        60804 bytes_per_second=174.975M/s
bench_sha3::shake256/2048/128      11663 ns        11662 ns        60026 bytes_per_second=177.938M/s
bench_sha3::shake256/4096/32       22028 ns        22027 ns        31775 bytes_per_second=178.722M/s
bench_sha3::shake256/4096/64       22103 ns        22102 ns        31677 bytes_per_second=179.497M/s
bench_sha3::shake256/4096/128      22250 ns        22250 ns        31452 bytes_per_second=181.048M/s
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
bench_sha3::shake128/32/32          1267 ns         1267 ns       552088 bytes_per_second=48.1849M/s
bench_sha3::shake128/32/64          1340 ns         1340 ns       522935 bytes_per_second=68.3447M/s
bench_sha3::shake128/32/128         1485 ns         1485 ns       471090 bytes_per_second=102.733M/s
bench_sha3::shake128/64/32          1259 ns         1259 ns       556037 bytes_per_second=72.735M/s
bench_sha3::shake128/64/64          1330 ns         1330 ns       526904 bytes_per_second=91.78M/s
bench_sha3::shake128/64/128         1473 ns         1473 ns       475675 bytes_per_second=124.349M/s
bench_sha3::shake128/128/32         1257 ns         1257 ns       556183 bytes_per_second=121.37M/s
bench_sha3::shake128/128/64         1331 ns         1331 ns       525757 bytes_per_second=137.601M/s
bench_sha3::shake128/128/128        1472 ns         1472 ns       476248 bytes_per_second=165.835M/s
bench_sha3::shake128/256/32         2397 ns         2397 ns       291503 bytes_per_second=114.601M/s
bench_sha3::shake128/256/64         2472 ns         2472 ns       283291 bytes_per_second=123.455M/s
bench_sha3::shake128/256/128        2612 ns         2611 ns       267549 bytes_per_second=140.239M/s
bench_sha3::shake128/512/32         4668 ns         4668 ns       150141 bytes_per_second=111.147M/s
bench_sha3::shake128/512/64         4744 ns         4744 ns       147200 bytes_per_second=115.798M/s
bench_sha3::shake128/512/128        4888 ns         4888 ns       143015 bytes_per_second=124.88M/s
bench_sha3::shake128/1024/32        8131 ns         8130 ns        86073 bytes_per_second=123.872M/s
bench_sha3::shake128/1024/64        8192 ns         8192 ns        85526 bytes_per_second=126.662M/s
bench_sha3::shake128/1024/128       8328 ns         8327 ns        84066 bytes_per_second=131.935M/s
bench_sha3::shake128/2048/32       14983 ns        14982 ns        46782 bytes_per_second=132.401M/s
bench_sha3::shake128/2048/64       15045 ns        15043 ns        46516 bytes_per_second=133.889M/s
bench_sha3::shake128/2048/128      15203 ns        15202 ns        46084 bytes_per_second=136.508M/s
bench_sha3::shake128/4096/32       28671 ns        28669 ns        24430 bytes_per_second=137.318M/s
bench_sha3::shake128/4096/64       28765 ns        28763 ns        24361 bytes_per_second=137.929M/s
bench_sha3::shake128/4096/128      28933 ns        28930 ns        24212 bytes_per_second=139.245M/s
bench_sha3::shake256/32/32          1264 ns         1263 ns       555064 bytes_per_second=48.3067M/s
bench_sha3::shake256/32/64          1333 ns         1333 ns       525793 bytes_per_second=68.7059M/s
bench_sha3::shake256/32/128         1475 ns         1475 ns       474031 bytes_per_second=103.463M/s
bench_sha3::shake256/64/32          1265 ns         1265 ns       553966 bytes_per_second=72.3586M/s
bench_sha3::shake256/64/64          1334 ns         1334 ns       524205 bytes_per_second=91.5084M/s
bench_sha3::shake256/64/128         1477 ns         1477 ns       474673 bytes_per_second=123.993M/s
bench_sha3::shake256/128/32         1256 ns         1256 ns       556132 bytes_per_second=121.535M/s
bench_sha3::shake256/128/64         1328 ns         1328 ns       528184 bytes_per_second=137.929M/s
bench_sha3::shake256/128/128        1470 ns         1470 ns       474788 bytes_per_second=166.033M/s
bench_sha3::shake256/256/32         2407 ns         2407 ns       290654 bytes_per_second=114.127M/s
bench_sha3::shake256/256/64         2479 ns         2479 ns       282172 bytes_per_second=123.111M/s
bench_sha3::shake256/256/128        2624 ns         2624 ns       266861 bytes_per_second=139.564M/s
bench_sha3::shake256/512/32         4701 ns         4700 ns       148933 bytes_per_second=110.371M/s
bench_sha3::shake256/512/64         4774 ns         4774 ns       146804 bytes_per_second=115.058M/s
bench_sha3::shake256/512/128        4913 ns         4913 ns       142415 bytes_per_second=124.227M/s
bench_sha3::shake256/1024/32        9323 ns         9322 ns        75039 bytes_per_second=108.031M/s
bench_sha3::shake256/1024/64        9401 ns         9401 ns        74520 bytes_per_second=110.374M/s
bench_sha3::shake256/1024/128       9546 ns         9546 ns        73385 bytes_per_second=115.094M/s
bench_sha3::shake256/2048/32       18526 ns        18526 ns        37790 bytes_per_second=107.074M/s
bench_sha3::shake256/2048/64       18596 ns        18595 ns        37675 bytes_per_second=108.317M/s
bench_sha3::shake256/2048/128      18740 ns        18740 ns        37398 bytes_per_second=110.739M/s
bench_sha3::shake256/4096/32       35752 ns        35751 ns        19570 bytes_per_second=110.116M/s
bench_sha3::shake256/4096/64       35834 ns        35834 ns        19492 bytes_per_second=110.714M/s
bench_sha3::shake256/4096/128      36020 ns        36018 ns        19468 bytes_per_second=111.842M/s
```

---

For comparing performance of Python `hashlib` module & Python wrapper interface built on top of this `sha3` C++ library, issue following command

```fish
make benchpy
```

On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz, when wrapper C interface is compiled to shared library object using `clang++`, performance comparison looks like

```fish
cd wrapper/python; python3 -m pytest -k bench -v; cd ..
=========================================================================================== test session starts ============================================================================================
platform darwin -- Python 3.10.6, pytest-7.1.3, pluggy-1.0.0 -- /usr/local/opt/python@3.10/bin/python3.10
cachedir: .pytest_cache
benchmark: 3.4.1 (defaults: timer=time.perf_counter disable_gc=False min_rounds=5 min_time=0.000005 max_time=1.0 calibration_precision=10 warmup=False warmup_iterations=100000)
rootdir: /Users/anjan/Documents/work/sha3/wrapper/python
plugins: benchmark-3.4.1
collected 12 items / 4 deselected / 8 selected

test_sha3.py::test_bench_sha3_224_self PASSED                                                                                                                                                        [ 12%]
test_sha3.py::test_bench_sha3_224_hashlib PASSED                                                                                                                                                     [ 25%]
test_sha3.py::test_bench_sha3_256_self PASSED                                                                                                                                                        [ 37%]
test_sha3.py::test_bench_sha3_256_hashlib PASSED                                                                                                                                                     [ 50%]
test_sha3.py::test_bench_sha3_384_self PASSED                                                                                                                                                        [ 62%]
test_sha3.py::test_bench_sha3_384_hashlib PASSED                                                                                                                                                     [ 75%]
test_sha3.py::test_bench_sha3_512_self PASSED                                                                                                                                                        [ 87%]
test_sha3.py::test_bench_sha3_512_hashlib PASSED                                                                                                                                                     [100%]


--------------------------------------------------------------------------------- benchmark 'sha3-224': 2 tests ---------------------------------------------------------------------------------
Name (time in us)                  Min                Max              Mean            StdDev            Median               IQR            Outliers  OPS (Kops/s)            Rounds  Iterations
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_bench_sha3_224_hashlib     1.1708 (1.0)      11.6266 (1.0)      1.2292 (1.0)      0.2171 (1.0)      1.2187 (1.0)      0.0126 (1.0)     536;21490      813.5219 (1.0)       85419          10
test_bench_sha3_224_self        1.9797 (1.69)     31.0785 (2.67)     2.0877 (1.70)     0.5655 (2.61)     2.0326 (1.67)     0.0280 (2.22)     690;2973      479.0036 (0.59)      50536          10
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------- benchmark 'sha3-256': 2 tests ---------------------------------------------------------------------------------
Name (time in us)                  Min                Max              Mean            StdDev            Median               IQR            Outliers  OPS (Kops/s)            Rounds  Iterations
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_bench_sha3_256_hashlib     1.1701 (1.0)      14.6670 (1.0)      1.2260 (1.0)      0.2296 (1.0)      1.2156 (1.0)      0.0441 (1.51)      577;794      815.6753 (1.0)       83209          10
test_bench_sha3_256_self        1.9464 (1.66)     26.8438 (1.83)     2.0467 (1.67)     0.5406 (2.35)     2.0047 (1.65)     0.0293 (1.0)      532;2165      488.5953 (0.60)      50662          10
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------- benchmark 'sha3-384': 2 tests ---------------------------------------------------------------------------------
Name (time in us)                  Min                Max              Mean            StdDev            Median               IQR            Outliers  OPS (Kops/s)            Rounds  Iterations
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_bench_sha3_384_hashlib     1.1554 (1.0)      17.1553 (1.0)      1.2257 (1.0)      0.2638 (1.0)      1.2055 (1.0)      0.0597 (1.79)      682;852      815.8825 (1.0)       84367          10
test_bench_sha3_384_self        1.9566 (1.69)     32.4185 (1.89)     2.0766 (1.69)     0.5168 (1.96)     2.0375 (1.69)     0.0333 (1.0)      430;3212      481.5450 (0.59)      51196          10
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------- benchmark 'sha3-512': 2 tests ----------------------------------------------------------------------------------
Name (time in us)                  Min                Max              Mean            StdDev            Median               IQR             Outliers  OPS (Kops/s)            Rounds  Iterations
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_bench_sha3_512_hashlib     1.4670 (1.0)      13.7156 (1.0)      1.5416 (1.0)      0.2780 (1.0)      1.5201 (1.0)      0.0161 (1.0)      680;10144      648.6760 (1.0)       68297          10
test_bench_sha3_512_self        2.3273 (1.59)     30.9500 (2.26)     2.4547 (1.59)     0.5565 (2.00)     2.4073 (1.58)     0.0507 (3.15)       506;911      407.3823 (0.63)      43663          10
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Legend:
  Outliers: 1 Standard Deviation from Mean; 1.5 IQR (InterQuartile Range) from 1st Quartile and 3rd Quartile.
  OPS: Operations Per Second, computed as 1 / Mean
===================================================================================== 8 passed, 4 deselected in 16.20s =====================================================================================
```

## Usage

`sha3` C++ library is written such that it's fairly easy for one to start using it in their project. All one needs to do

- Include proper header files ( select what you need by name )
- Properly use API ( see usage examples/ test cases )
- When compiling, let your compiler know where it can find respective header files

Hash Function | Header/ Namespace | Example
--- | --- | --:
SHA3-224 | [`sha3_224::`](./include/sha3_224.hpp) | [example/sha3_224.cpp](./example/sha3_224.cpp)
SHA3-256 | [`sha3_256::`](./include/sha3_256.hpp) | [example/sha3_256.cpp](./example/sha3_256.cpp)
SHA3-384 | [`sha3_384::`](./include/sha3_384.hpp) | [example/sha3_384.cpp](./example/sha3_384.cpp)
SHA3-512 | [`sha3_512::`](./include/sha3_512.hpp) | [example/sha3_512.cpp](./example/sha3_512.cpp)

Extendable Output Function | Header/ Namespace | Absorb API | Squeeze API | Example
--- | --- | --- | --- | --:
SHAKE128 ( default option, one-shot consumption ) | [`shake128::`](./include/shake128.hpp) | `shake128::hash(...)` : Absorbs N (>=0) -bytes input into keccak[256] sponge | `shake128::read(...)` : Squeezes N (>=0) -bytes output from keccak[256] sponge. This routine can be called arbitrary number of times, requesting arbitrary bytes of output. | [example/shake128.cpp](./example/shake128.cpp)
SHAKE256 ( default option, one-shot consumption ) | [`shake256::`](./include/shake256.hpp) | `shake256::hash(...)` : Absorbs N (>=0) -bytes input into keccak[512] sponge | `shake256::read(...)` : Squeezes N (>=0) -bytes output from keccak[512] sponge. This routine can be called arbitrary number of times, requesting arbitrary bytes of output. | [example/shake256.cpp](./example/shake256.cpp)

Extendable Output Function | Header/ Namespace | Incremental Absorption API | Finalize API | Squeeze API | Example
--- | --- | --- | --- | --- | --:
SHAKE128 ( explicit incremental consumption ) | [`shake128::`](./include/shake128.hpp) | `shake128<true>::absorb(...)` : Absorbs N (>=0) -bytes input into keccak[256] sponge, arbitrary many times | `shake128<true>::finalize()` : Finalizes state of keccak[256] sponge, so that it's ready for squeezing | `shake128<true>::read(...)` : Squeezes N (>=0) -bytes output from keccak[256] sponge. This routine can be called arbitrary number of times, requesting arbitrary bytes of output. | [example/incremental_shake128.cpp](./example/incremental_shake128.cpp)
SHAKE256 ( explicit incremental consumption ) | [`shake256::`](./include/shake256.hpp) | `shake256<true>::absorb(...)` : Absorbs N (>=0) -bytes input into keccak[512] sponge, arbitrary many times | `shake256<true>::finalize()` : Finalizes state of keccak[512] sponge, so that it's ready for squeezing | `shake256<true>::read(...)` : Squeezes N (>=0) -bytes output from keccak[512] sponge. This routine can be called arbitrary number of times, requesting arbitrary bytes of output. | [example/incremental_shake256.cpp](./example/incremental_shake256.cpp)

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

# ---

g++ -std=c++20 -Wall -O3 -I include example/shake128.cpp && ./a.out
SHAKE-128

Input  : 8814e9f091cd4ee6ac6795be43b25b4d741143f4d7f7e9858731447359eaa1e8
Output : d32991406e38740f9b9b2674e59259891bfd23f9d6ea71a816c3133466163dacb3b1cefc341cbda8

# ---

g++ -std=c++20 -Wall -O3 -I include example/shake256.cpp && ./a.out
SHAKE-256

Input  : a6506638e34127e0a8415241479c968c20422f46497663eaf244f205a756f0b3
Output : ce679163b642380365c3c11dcbca7a36ddd01cefba35b8ec18ad937268f584999c6e8ae061c251dd

# ---

g++ -std=c++20 -Wall -O3 -I include example/incremental_shake128.cpp && ./a.out
Incremental SHAKE-128

Input 0  : 8ee149be89652aa3a96bb1cb21c03a
Input 1  : 6c5240ef768e7a0100946e0f83bea78364
Output   : 94f03616a7ed0168833dcec6f51a359b3c3cd42ac0c27409106424f0adb2257f4bfe214f371b3935

# ---

g++ -std=c++20 -Wall -O3 -I include example/incremental_shake256.cpp && ./a.out
Incremental SHAKE-256

Input 0  : 58efcb50a9a8bb61cd25f89be74fe6
Input 1  : 355dc311ebdae75dd8c382dd5d04e9d17a
Output   : 7c342b6f8b03d4ef09e4cbed70280c0ca8bbfbb3180f1acb268d6e1e67585adf18dd6e98fd71211f
```
