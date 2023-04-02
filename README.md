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
