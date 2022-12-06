# sha3
SHA3: Permutation-Based Hash and Extendable-Output Functions

## Motivation

SHA3 standard by NIST ( i.e. NIST FIPS PUB 202 ) specifies four permutation-based hash functions and two extendable-output functions, which are built on top of keccak-p[1600, 24] permutation.

These hash functions and extendable output functions are pretty commonly used in various post-quantum cryptography algorithms ( those used for public key encryption, key establishment mechanism & digital signature algorithms ) i.e. some of which are already declared as selected candidates ( e.g. Kyber, Falcon, Dilithium, SPHINCS+ etc. ) of NIST PQC standardization effort or some are still competing ( e.g. Bike, Classic McEliece etc. ) in final round of standardization. I decided to implement SHA3 specification as **zero-dependency, header-only and easy-to-use C++ library**, so that I can make use of it as modular dependency ( say pinned to specific commit using git submodule ) in other ( *future* ) projects.

> **Note**
> `sha3` - this project will be used in various post-quantum cryptographic algorithm implementations which are already selected or will be selected by NIST.

Few of those places, where I've already used `sha3` as ( git submodule based ) dependency

- [Kyber: Post-Quantum Public-key Encryption & Key-establishment Algorithm](https://github.com/itzmeanjan/kyber)
- [Dilithium: Post-Quantum Digital Signature Algorithm](https://github.com/itzmeanjan/dilithium)
- [SPHINCS+: Stateless Hash-based Digital Signature Algorithm](https://github.com/itzmeanjan/sphincs)

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
make       # by default USE_AVX2=0
```

If target CPU has AVX2 flag, this SHA3 implementation can make use of AVX2 intrinsics for keccak-p[1600, 24] permutation. All one needs to do is define `USE_AVX2` preprocessor constant with non-zero value, which is used in [keccak.hpp](./include/keccak.hpp) for determining whether to use AVX2 for keccak permutation or not. For testing functional correctness of AVX2 intrinsic based keccak permutation, you may issue

```fish
AVX2=1 make # trigger AVX2 based keccak permutation code compilation
```

## Benchmarking

Find micro-benchmarking ( using `google-benchmark` ) results [here](./bench).

## Usage

`sha3` C++ library is written such that it's fairly easy for one to start using it in their project. All one needs to do

- Include proper header files ( select what you need by name )
- Properly use API ( see usage examples/ test cases )
- When compiling, let your compiler know where it can find respective header files

> **Note**

> When compiling you can ask your compiler to check if AVX2 is available on target CPU by issuing

> $ `echo | g++ -march=native -dM -E -x c++ - | grep -i avx`

> If AVX2 is available on target CPU and you want to make use of it for keccak-p[1600, 24] permutation, consider passing `-march=native -DUSE_AVX2=1` when compiling [include/keccak.hpp](./include/keccak.hpp). By default it won't trigger compilation of AVX2 intrinsic based code path, you've to explicitly ask for it.

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
$ g++ -std=c++20 -Wall -O3 -march=native -DUSE_AVX2=1 -I include example/sha3_224.cpp && ./a.out # for keccak with AVX2
SHA3-224

Input  : 043f0fa310343b6ca42c3d2ab6f168574fd41774d49c9c1e5922c2cd60b43dbb
Output : 3bfbd5e41e850f29daf9c08dbcaca7c43ca939e7d6c0b6d8993c6af4

# ---

$ g++ -std=c++20 -Wall -O3 -I include example/sha3_256.cpp && ./a.out
$ g++ -std=c++20 -Wall -O3 -march=native -DUSE_AVX2=1 -I include example/sha3_256.cpp && ./a.out # for keccak with AVX2
SHA3-256

Input  : 70a3bf382218c7f4ae25775ab1d21f9d48e2f03af70dcdec790a338e982e6fa8
Output : 57be0ef9634da2d94219c53032809f4ffc145df6782279a8059afe607715d675

# ---

$ g++ -std=c++20 -Wall -O3 -I include example/sha3_384.cpp && ./a.out
$ g++ -std=c++20 -Wall -O3 -march=native -DUSE_AVX2=1 -I include example/sha3_384.cpp && ./a.out # for keccak with AVX2
SHA3-384

Input  : 314686636dc0499f2ebf0a201fe2d44e2e8888ac1109939998230f2cba5d0e94
Output : 554f4506a1b73724d0ce25cc4a0c0b4fc26478cde43013a59c7e25a22e3e73fbcfa731fe9f5050f77e82530fba7de512

# ---

$ g++ -std=c++20 -Wall -O3 -I include example/sha3_512.cpp && ./a.out
$ g++ -std=c++20 -Wall -O3 -march=native -DUSE_AVX2=1 -I include example/sha3_512.cpp && ./a.out # for keccak with AVX2
SHA3-512

Input  : 2c3c0ae485204067f1ecbc69a8fefd19a94c9c1552158a8d57a6612b3202f373
Output : 578386bdd6eb816d6d0cbc984351c889f70675a2661ba605aa65ce204b88a6d6553946c52eeac937647929f99a2ddd7cc6645bc1e89df8ab795faba6dd7d0d45

# ---

$ g++ -std=c++20 -Wall -O3 -I include example/shake128.cpp && ./a.out
$ g++ -std=c++20 -Wall -O3 -march=native -DUSE_AVX2=1 -I include example/shake128.cpp && ./a.out # for keccak with AVX2
SHAKE-128

Input  : 8814e9f091cd4ee6ac6795be43b25b4d741143f4d7f7e9858731447359eaa1e8
Output : d32991406e38740f9b9b2674e59259891bfd23f9d6ea71a816c3133466163dacb3b1cefc341cbda8

# ---

$ g++ -std=c++20 -Wall -O3 -I include example/shake256.cpp && ./a.out
$ g++ -std=c++20 -Wall -O3 -march=native -DUSE_AVX2=1 -I include example/shake256.cpp && ./a.out # for keccak with AVX2
SHAKE-256

Input  : a6506638e34127e0a8415241479c968c20422f46497663eaf244f205a756f0b3
Output : ce679163b642380365c3c11dcbca7a36ddd01cefba35b8ec18ad937268f584999c6e8ae061c251dd

# ---

$ g++ -std=c++20 -Wall -O3 -I include example/incremental_shake128.cpp && ./a.out
$ g++ -std=c++20 -Wall -O3 -march=native -DUSE_AVX2=1 -I include example/incremental_shake128.cpp && ./a.out # for keccak with AVX2
Incremental SHAKE-128

Input 0  : 8ee149be89652aa3a96bb1cb21c03a
Input 1  : 6c5240ef768e7a0100946e0f83bea78364
Output   : 94f03616a7ed0168833dcec6f51a359b3c3cd42ac0c27409106424f0adb2257f4bfe214f371b3935

# ---

$ g++ -std=c++20 -Wall -O3 -I include example/incremental_shake256.cpp && ./a.out
$ g++ -std=c++20 -Wall -O3 -march=native -DUSE_AVX2=1 -I include example/incremental_shake256.cpp && ./a.out # for keccak with AVX2
Incremental SHAKE-256

Input 0  : 58efcb50a9a8bb61cd25f89be74fe6
Input 1  : 355dc311ebdae75dd8c382dd5d04e9d17a
Output   : 7c342b6f8b03d4ef09e4cbed70280c0ca8bbfbb3180f1acb268d6e1e67585adf18dd6e98fd71211f
```
