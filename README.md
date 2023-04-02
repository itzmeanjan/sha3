# sha3
SHA3: Permutation-Based Hash and Extendable-Output Functions

## Motivation

SHA3 standard by NIST ( i.e. NIST FIPS PUB 202 ) specifies four ( of different digest length ) permutation-based hash functions and two extendable-output functions, which are built on top of keccak-p[1600, 24] permutation.

These hash functions and extendable output functions are pretty commonly used in various post-quantum cryptography algorithms ( those used for public key encryption, key establishment mechanism & digital signature algorithms ) i.e. some of which are already declared as selected candidates ( e.g. Kyber, Falcon, Dilithium, SPHINCS+ etc. ) of NIST PQC standardization effort - waiting to be standardized or some are still competing ( e.g. Bike, Classic McEliece etc. ) in final round of standardization. I decided to implement SHA3 specification as **zero-dependency, header-only and easy-to-use C++ library**, so that I can make use of it as a modular dependency ( say pinned to specific commit using git submodule ) in libraries where I implement these PQC schemes.

Few of those places, where I've already used `sha3` as ( git submodule based ) dependency

- [Kyber: Post-Quantum Public-key Encryption & Key-establishment Algorithm](https://github.com/itzmeanjan/kyber)
- [Dilithium: Post-Quantum Digital Signature Algorithm](https://github.com/itzmeanjan/dilithium)
- [SPHINCS+: Stateless Hash-based Digital Signature Algorithm](https://github.com/itzmeanjan/sphincs)
- [Falcon: Fast-Fourier Lattice-based Compact Signatures over NTRU - NIST PQC Digital Signature Algorithm](https://github.com/itzmeanjan/falcon)

Here I'm maintaining a zero-dependency, header-only C++ library, using modern C++ features ( such as C++{>=11} ), which is fairly easy-to-use in your project, implementing SHA3 [specification](https://dx.doi.org/10.6028/NIST.FIPS.202) i.e. NIST FIPS PUB 202. Following algorithms are implemented in `sha3` library

Algorithm | Input | Output | Behaviour | Interface
--- | --- | --- | --- | ---
SHA3-224 | N ( >=0 ) -bytes message | 28 -bytes digest | Given N -bytes input message this routine computes 28 -bytes sha3-224 digest, while consuming message into Keccak[448] sponge | [`sha3_224::hash(...)`](./include/sha3_224.hpp)
SHA3-256 | N ( >=0 ) -bytes message | 32 -bytes digest | Given N -bytes input message this routine computes 32 -bytes sha3-256 digest, while consuming message into Keccak[512] sponge | [`sha3_256::hash(...)`](./include/sha3_256.hpp)
SHA3-384 | N ( >=0 ) -bytes message | 48 -bytes digest | Given N -bytes input message this routine computes 48 -bytes sha3-384 digest, while consuming message into Keccak[768] sponge | [`sha3_384::hash(...)`](./include/sha3_384.hpp)
SHA3-512 | N ( >=0 ) -bytes message | 64 -bytes digest | Given N -bytes input message this routine computes 64 -bytes sha3-512 digest, while consuming message into Keccak[1024] sponge | [`sha3_512::hash(...)`](./include/sha3_512.hpp)
SHAKE-128 | N ( >=0 ) -bytes message **[ Supports incremental message consumption ]** | M ( >=0 ) -bytes digest | Given N -bytes input message this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[256] sponge, which has already absorbed input bytes | [`shake128::{hash(...), read(...)}`](./include/shake128.hpp)
SHAKE-256 | N ( >=0 ) -bytes message **[ Supports incremental message consumption ]** | M ( >=0 ) -bytes digest | Given N -bytes input message this routine squeezes arbitrary ( = M ) number of output bytes from Keccak[512] sponge, which has already absorbed input bytes | [`shake256::{hash(...), read(...)}`](./include/shake256.hpp)

> Learn more about NIST PQC standardization effort [here](https://csrc.nist.gov/projects/post-quantum-cryptography/)

> Find NIST PQC selected algorithms [here](https://csrc.nist.gov/projects/post-quantum-cryptography/selected-algorithms-2022)

## Prerequisites

- A C++ compiler such as `g++`/ `clang++`, with support for C++20 standard library.

```bash
$ g++ --version
g++ (Ubuntu 11.2.0-19ubuntu1) 11.2.0

$ clang++ --version
Ubuntu clang version 14.0.0-1ubuntu1
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

- System development utilities such as `cmake` and `make`.

```bash
$ make --version
GNU Make 3.81

$ cmake --version
cmake version 3.23.2
```

- For ensuring functional correctness of these implementations, you'll need `python3`.

> Python `hashlib` module computed digests are used as source of truth during testing. More about [hashlib](https://docs.python.org/3/library/hashlib.html).

```bash
$ python3 --version
Python 3.10.7
```

- Install Python dependencies by issuing.

```bash
python3 -m pip install --user -r wrapper/python/requirements.txt
```

- For benchmarking implementations on CPU systems, `google-benchmark` library and headers are required to be installed system-wide. Please follow [this](https://github.com/google/benchmark/tree/60b16f1#installation) guide if you don't have it installed.

## Testing

For ensuring that SHA3 hash function and extendable output function implementations are correct & conformant to the NIST standard ( see [this](https://dx.doi.org/10.6028/NIST.FIPS.202) ), I make use of Python `hashlib` module as source of truth during testing. 

For randomly generated byte arrays of length N ( s.t. 0 <= N <= 1024 ), I compute SHA3-{224, 256, 384, 512} & SHAKE{128, 256} digest using both

- (i) my implementation ( Python wrapper interface, [here](./wrapper/python/sha3.py) )
- (ii) `hashlib` implementation

and check for their equality.

I also test implementation correctness of incremental message absorption API of SHAKE{128, 256} XOF. Issue following command for running test cases.

```bash
make
```

## Benchmarking

For benchmarking SHA3 hash functions and extendable output functions, on CPU systems, using `google-benchmark`, issue following command.

> **Warning** For disabling CPU scaling during benchmarking, follow [this](https://github.com/google/benchmark/blob/60b16f1/docs/user_guide.md#disabling-cpu-frequency-scaling)

> **Note** During benchmarking hash functions, a byte array ( filled with random data ) of length N is used as input | 32 <= N <= 4096 and N = 2^i

> **Note** When benchmarking extendable output functions ( XOFs ), fixed length output of 32 -bytes are squeezed from sponge ( s.t. all bytes are read in a single call to `read` function ), for each input byte array of length N | 32 <= N <= 4096 and N = 2^i

```bash
make benchmark
```

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( using `clang++` )

```bash
2023-04-02T18:00:36+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 0.87, 1.51, 1.79
----------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations bytes_per_second
----------------------------------------------------------------------------------------
bench_sha3::keccakf1600             331 ns          331 ns      2093771       576.078M/s
bench_sha3::sha3_224/32             361 ns          360 ns      1974300       158.957M/s
bench_sha3::sha3_224/64             354 ns          354 ns      1985827       247.944M/s
bench_sha3::sha3_224/128            351 ns          350 ns      1991612        424.56M/s
bench_sha3::sha3_224/256            699 ns          695 ns       993204       389.446M/s
bench_sha3::sha3_224/512           1358 ns         1358 ns       504439       379.333M/s
bench_sha3::sha3_224/1024          2705 ns         2704 ns       257154       371.059M/s
bench_sha3::sha3_224/2048          5117 ns         5092 ns       136150       388.847M/s
bench_sha3::sha3_224/4096          9788 ns         9780 ns        70389       402.145M/s
bench_sha3::sha3_256/32             364 ns          363 ns      1958491       167.989M/s
bench_sha3::sha3_256/64             357 ns          356 ns      1947945       256.992M/s
bench_sha3::sha3_256/128            350 ns          350 ns      1967303         435.9M/s
bench_sha3::sha3_256/256            698 ns          697 ns       981024       393.926M/s
bench_sha3::sha3_256/512           1391 ns         1385 ns       499362       374.645M/s
bench_sha3::sha3_256/1024          2720 ns         2719 ns       255307       370.446M/s
bench_sha3::sha3_256/2048          5432 ns         5427 ns       127663       365.547M/s
bench_sha3::sha3_256/4096         10449 ns        10444 ns        65402       376.957M/s
bench_sha3::sha3_384/32             355 ns          353 ns      1975587       216.105M/s
bench_sha3::sha3_384/64             352 ns          352 ns      1974590         303.3M/s
bench_sha3::sha3_384/128            687 ns          686 ns       998787       244.597M/s
bench_sha3::sha3_384/256           1055 ns         1049 ns       659662       276.479M/s
bench_sha3::sha3_384/512           1699 ns         1697 ns       407609        314.73M/s
bench_sha3::sha3_384/1024          3393 ns         3388 ns       207417       301.728M/s
bench_sha3::sha3_384/2048          6790 ns         6771 ns        98625       295.215M/s
bench_sha3::sha3_384/4096         13540 ns        13522 ns        51412       292.257M/s
bench_sha3::sha3_512/32             353 ns          353 ns      1983469       259.295M/s
bench_sha3::sha3_512/64             350 ns          350 ns      1966358       348.705M/s
bench_sha3::sha3_512/128            688 ns          687 ns      1000043       266.505M/s
bench_sha3::sha3_512/256           1378 ns         1371 ns       508082       222.639M/s
bench_sha3::sha3_512/512           2704 ns         2701 ns       258862       203.374M/s
bench_sha3::sha3_512/1024          5089 ns         5084 ns       137934       204.105M/s
bench_sha3::sha3_512/2048          9898 ns         9852 ns        70907       204.437M/s
bench_sha3::sha3_512/4096         19178 ns        19158 ns        36315       207.078M/s
bench_sha3::shake128/32/32          357 ns          357 ns      1958508       170.876M/s
bench_sha3::shake128/64/32          360 ns          358 ns      1956067       255.556M/s
bench_sha3::shake128/128/32         353 ns          352 ns      1972598       432.984M/s
bench_sha3::shake128/256/32         698 ns          697 ns       978296       393.994M/s
bench_sha3::shake128/512/32        1377 ns         1373 ns       496391       377.721M/s
bench_sha3::shake128/1024/32       2384 ns         2383 ns       291461       422.677M/s
bench_sha3::shake128/2048/32       4461 ns         4440 ns       158645       446.731M/s
bench_sha3::shake128/4096/32       8473 ns         8467 ns        80810       464.979M/s
bench_sha3::shake256/32/32          360 ns          360 ns      1934380       169.743M/s
bench_sha3::shake256/64/32          363 ns          361 ns      1919291       253.454M/s
bench_sha3::shake256/128/32         354 ns          354 ns      1968200       431.106M/s
bench_sha3::shake256/256/32         701 ns          700 ns       980282       392.429M/s
bench_sha3::shake256/512/32        1389 ns         1384 ns       505627       374.935M/s
bench_sha3::shake256/1024/32       2719 ns         2717 ns       255941       370.627M/s
bench_sha3::shake256/2048/32       5504 ns         5476 ns       127351       362.261M/s
bench_sha3::shake256/4096/32      10469 ns        10462 ns        65923       376.291M/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( using `g++` )

```bash
2023-04-02T14:51:48+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.12, 0.03, 0.01
----------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations bytes_per_second
----------------------------------------------------------------------------------------
bench_sha3::keccakf1600             568 ns          568 ns      1231212        336.01M/s
bench_sha3::sha3_224/32             646 ns          646 ns      1083827       88.5952M/s
bench_sha3::sha3_224/64             643 ns          642 ns      1090111       136.567M/s
bench_sha3::sha3_224/128            644 ns          644 ns      1089500       231.162M/s
bench_sha3::sha3_224/256           1225 ns         1225 ns       570762       221.007M/s
bench_sha3::sha3_224/512           2385 ns         2385 ns       293244       215.885M/s
bench_sha3::sha3_224/1024          4729 ns         4729 ns       148039       212.147M/s
bench_sha3::sha3_224/2048          8777 ns         8777 ns        79754       225.566M/s
bench_sha3::sha3_224/4096         16905 ns        16905 ns        41450        232.65M/s
bench_sha3::sha3_256/32             629 ns          629 ns      1112175       97.0429M/s
bench_sha3::sha3_256/64             630 ns          630 ns      1111404       145.403M/s
bench_sha3::sha3_256/128            631 ns          631 ns      1110022        241.76M/s
bench_sha3::sha3_256/256           1227 ns         1227 ns       570319       223.762M/s
bench_sha3::sha3_256/512           2417 ns         2417 ns       289903       214.674M/s
bench_sha3::sha3_256/1024          4797 ns         4796 ns       145754       209.966M/s
bench_sha3::sha3_256/2048          9494 ns         9494 ns        73812        208.94M/s
bench_sha3::sha3_256/4096         18341 ns        18341 ns        38168       214.643M/s
bench_sha3::sha3_384/32             579 ns          579 ns      1211352       131.879M/s
bench_sha3::sha3_384/64             578 ns          578 ns      1211169       184.796M/s
bench_sha3::sha3_384/128           1136 ns         1136 ns       615841       147.699M/s
bench_sha3::sha3_384/256           1693 ns         1693 ns       413916        171.21M/s
bench_sha3::sha3_384/512           2795 ns         2794 ns       250205       191.118M/s
bench_sha3::sha3_384/1024          5575 ns         5575 ns       125628       183.377M/s
bench_sha3::sha3_384/2048         11107 ns        11107 ns        63059       179.974M/s
bench_sha3::sha3_384/4096         22159 ns        22159 ns        31597       178.345M/s
bench_sha3::sha3_512/32             579 ns          579 ns      1210637       158.152M/s
bench_sha3::sha3_512/64             577 ns          577 ns      1212272        211.57M/s
bench_sha3::sha3_512/128           1140 ns         1140 ns       613610        160.59M/s
bench_sha3::sha3_512/256           2257 ns         2257 ns       310244       135.234M/s
bench_sha3::sha3_512/512           4505 ns         4505 ns       155391       121.929M/s
bench_sha3::sha3_512/1024          8415 ns         8415 ns        83226       123.304M/s
bench_sha3::sha3_512/2048         16265 ns        16266 ns        43042        123.83M/s
bench_sha3::sha3_512/4096         31909 ns        31909 ns        21920       124.332M/s
bench_sha3::shake128/32/32          696 ns          696 ns      1006963       87.6991M/s
bench_sha3::shake128/64/32          696 ns          696 ns      1005666       131.559M/s
bench_sha3::shake128/128/32         696 ns          696 ns      1005782       219.243M/s
bench_sha3::shake128/256/32        1303 ns         1303 ns       537414       210.844M/s
bench_sha3::shake128/512/32        2462 ns         2462 ns       284406       210.729M/s
bench_sha3::shake128/1024/32       4216 ns         4216 ns       165930       238.867M/s
bench_sha3::shake128/2048/32       7711 ns         7711 ns        90869       257.256M/s
bench_sha3::shake128/4096/32      14679 ns        14679 ns        47684        268.19M/s
bench_sha3::shake256/32/32          667 ns          667 ns      1051529        91.571M/s
bench_sha3::shake256/64/32          666 ns          666 ns      1050322       137.454M/s
bench_sha3::shake256/128/32         669 ns          669 ns      1047474       228.245M/s
bench_sha3::shake256/256/32        1274 ns         1274 ns       549185       215.598M/s
bench_sha3::shake256/512/32        2476 ns         2476 ns       283450       209.532M/s
bench_sha3::shake256/1024/32       4846 ns         4846 ns       144322       207.823M/s
bench_sha3::shake256/2048/32       9581 ns         9581 ns        73065       207.041M/s
bench_sha3::shake256/4096/32      18472 ns        18472 ns        37874       213.119M/s
```

### On Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz ( using `clang++` )

```bash
2023-04-02T14:54:00+00:00
Running ./bench/a.out
Run on (4 X 2300.08 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.12, 0.11, 0.04
----------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations bytes_per_second
----------------------------------------------------------------------------------------
bench_sha3::keccakf1600             481 ns          481 ns      1456386       396.831M/s
bench_sha3::sha3_224/32             514 ns          514 ns      1362291       111.316M/s
bench_sha3::sha3_224/64             514 ns          514 ns      1363026       170.766M/s
bench_sha3::sha3_224/128            507 ns          507 ns      1381475       293.201M/s
bench_sha3::sha3_224/256           1002 ns         1002 ns       698750       270.273M/s
bench_sha3::sha3_224/512           1975 ns         1975 ns       354911        260.81M/s
bench_sha3::sha3_224/1024          3922 ns         3921 ns       178421        255.84M/s
bench_sha3::sha3_224/2048          7309 ns         7309 ns        95792       270.877M/s
bench_sha3::sha3_224/4096         14151 ns        14150 ns        49459       277.944M/s
bench_sha3::sha3_256/32             507 ns          507 ns      1384609        120.41M/s
bench_sha3::sha3_256/64             506 ns          506 ns      1384559       181.104M/s
bench_sha3::sha3_256/128            507 ns          507 ns      1381857       301.164M/s
bench_sha3::sha3_256/256           1012 ns         1012 ns       692174        271.44M/s
bench_sha3::sha3_256/512           1985 ns         1985 ns       352718         261.4M/s
bench_sha3::sha3_256/1024          3937 ns         3936 ns       177732       255.832M/s
bench_sha3::sha3_256/2048          7840 ns         7840 ns        89274       253.029M/s
bench_sha3::sha3_256/4096         15129 ns        15129 ns        46294       260.211M/s
bench_sha3::sha3_384/32             506 ns          506 ns      1383488       150.705M/s
bench_sha3::sha3_384/64             506 ns          506 ns      1385882        211.28M/s
bench_sha3::sha3_384/128           1005 ns         1005 ns       696805       167.053M/s
bench_sha3::sha3_384/256           1485 ns         1485 ns       471658       195.274M/s
bench_sha3::sha3_384/512           2444 ns         2444 ns       286540       218.534M/s
bench_sha3::sha3_384/1024          4894 ns         4894 ns       142954       208.917M/s
bench_sha3::sha3_384/2048          9772 ns         9772 ns        71671       204.561M/s
bench_sha3::sha3_384/4096         19513 ns        19513 ns        35881       202.528M/s
bench_sha3::sha3_512/32             503 ns          503 ns      1390447        181.98M/s
bench_sha3::sha3_512/64             498 ns          498 ns      1408215       245.191M/s
bench_sha3::sha3_512/128           1005 ns         1005 ns       697134       182.203M/s
bench_sha3::sha3_512/256           1964 ns         1964 ns       356452       155.367M/s
bench_sha3::sha3_512/512           3909 ns         3909 ns       179073       140.543M/s
bench_sha3::sha3_512/1024          7313 ns         7313 ns        95731       141.877M/s
bench_sha3::sha3_512/2048         14108 ns        14107 ns        49578       142.776M/s
bench_sha3::sha3_512/4096         27659 ns        27657 ns        25318       143.444M/s
bench_sha3::shake128/32/32          508 ns          508 ns      1372543       120.081M/s
bench_sha3::shake128/64/32          508 ns          508 ns      1377174       180.139M/s
bench_sha3::shake128/128/32         511 ns          511 ns      1368739       298.444M/s
bench_sha3::shake128/256/32        1015 ns         1015 ns       690315       270.602M/s
bench_sha3::shake128/512/32        1994 ns         1993 ns       350931       260.252M/s
bench_sha3::shake128/1024/32       3466 ns         3465 ns       202087       290.617M/s
bench_sha3::shake128/2048/32       6413 ns         6413 ns       109159       309.314M/s
bench_sha3::shake128/4096/32      12294 ns        12293 ns        56928       320.235M/s
bench_sha3::shake256/32/32          511 ns          511 ns      1375825       119.462M/s
bench_sha3::shake256/64/32          511 ns          511 ns      1370707       179.276M/s
bench_sha3::shake256/128/32         509 ns          509 ns      1373614        299.85M/s
bench_sha3::shake256/256/32        1010 ns         1010 ns       695152       272.029M/s
bench_sha3::shake256/512/32        1983 ns         1983 ns       353181       261.676M/s
bench_sha3::shake256/1024/32       3935 ns         3935 ns       177885       255.946M/s
bench_sha3::shake256/2048/32       7820 ns         7820 ns        89541       253.649M/s
bench_sha3::shake256/4096/32      15134 ns        15134 ns        46244       260.131M/s
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

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/incremental_shake128.cpp && ./a.out
Incremental SHAKE-128

Input 0  : 8ee149be89652aa3a96bb1cb21c03a
Input 1  : 6c5240ef768e7a0100946e0f83bea78364
Output   : 94f03616a7ed0168833dcec6f51a359b3c3cd42ac0c27409106424f0adb2257f4bfe214f371b3935

# ---

$ g++ -std=c++20 -Wall -O3 -march=native -I include example/incremental_shake256.cpp && ./a.out
Incremental SHAKE-256

Input 0  : 58efcb50a9a8bb61cd25f89be74fe6
Input 1  : 355dc311ebdae75dd8c382dd5d04e9d17a
Output   : 7c342b6f8b03d4ef09e4cbed70280c0ca8bbfbb3180f1acb268d6e1e67585adf18dd6e98fd71211f
```
