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
g++ (Ubuntu 14.2.0-4ubuntu2) 14.2.0
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
# Shows help message - which targets are available and what do each of them do
make
# or
make help

make test -j
make debug_asan_test -j
make debug_ubsan_test -j
make release_asan_test -j
make release_ubsan_test -j

# Specify which compiler to use
CXX=clang++ make test -j
```

```bash
PASSED TESTS (18/18):
       3 ms: build/test/test.out Sha3Xof.CompileTimeEvalShake256
       4 ms: build/test/test.out Sha3Xof.CompileTimeEvalShake128
      12 ms: build/test/test.out Sha3Hashing.CompileTimeEvalSha3_224
      13 ms: build/test/test.out Sha3Hashing.CompileTimeEvalSha3_256
      13 ms: build/test/test.out Sha3Hashing.CompileTimeEvalSha3_512
      16 ms: build/test/test.out Sha3Hashing.CompileTimeEvalSha3_384
      18 ms: build/test/test.out Sha3Hashing.Sha3_224IncrementalAbsorption
      18 ms: build/test/test.out Sha3Hashing.Sha3_256IncrementalAbsorption
      19 ms: build/test/test.out Sha3Hashing.Sha3_384IncrementalAbsorption
      19 ms: build/test/test.out Sha3Hashing.Sha3_512IncrementalAbsorption
      21 ms: build/test/test.out Sha3Hashing.Sha3_384KnownAnswerTests
      21 ms: build/test/test.out Sha3Hashing.Sha3_224KnownAnswerTests
      21 ms: build/test/test.out Sha3Xof.Shake128KnownAnswerTests
      21 ms: build/test/test.out Sha3Hashing.Sha3_256KnownAnswerTests
      22 ms: build/test/test.out Sha3Hashing.Sha3_512KnownAnswerTests
      22 ms: build/test/test.out Sha3Xof.Shake256KnownAnswerTests
    1078 ms: build/test/test.out Sha3Xof.Shake128IncrementalAbsorptionAndSqueezing
    1159 ms: build/test/test.out Sha3Xof.Shake256IncrementalAbsorptionAndSqueezing
```

## Benchmarking

For benchmarking SHA3 hash and extendable output functions, targeting CPU systems, using `google-benchmark`, issue following command.

> [!CAUTION]
> You must disable CPU frequency scaling during benchmarking, following [this](https://github.com/google/benchmark/blob/4931aefb51d1e5872b096a97f43e13fa0fc33c8c/docs/reducing_variance.md) guide.

> [!NOTE]
> When benchmarking extendable output functions ( Xofs ), fixed length output of 32/ 64 -bytes are squeezed from sponge ( s.t. all output bytes are requested in a single call to the `squeeze` function ), for input message byte array of length N s.t. N = 2^i (i.e. power of 2).

```bash
make perf -j      # You must issue this if you built your google-benchmark library with libPFM support.
make benchmark -j # Else you have to issue this one.
```

### On 12th Gen Intel(R) Core(TM) i7-1260P

Compiled with `g++ (Ubuntu 14.2.0-4ubuntu2) 14.2.0` while running on `Linux 6.11.0-9-generic x86_64`.

I maintain benchmark results in JSON format @ [bench_result_on_Linux_6.11.0-9-generic_x86_64_with_g++_14](./bench_result_on_Linux_6.11.0-9-generic_x86_64_with_g++_14.json).

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

// Eval SHA3-256 hash on statically defined input message during compilation-time.
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

I maintain examples of using Sha3 hash function and xof API, inside [examples](./examples/) directory. Run them all by issuing

```bash
make example -j
```

```bash
SHA3-224

Input  : 2c85bd27e447424ffecd668a690d385304553230e898e50e1bbda2035a852a3c
Output : a39d989bab91337bcc2af474aa6235a3ab05680c0d4cd02e5243b7cd
--- --- ---
SHA3-256

Input  : 5753f01c245b2cc4417850703e6cbeed822870a2b8f8144c2c30b35d2f9be2db
Output : c54d90ded2536dce8a73d06f474cdfd3cf14ddaaf54c0c2598347304e5fcb208
--- --- ---
SHA3-384

Input  : 0633acdbe5f78361622e4fefa816a41af0705f0d6e5e14cf339127e035275e11
Output : 18094d6c42a25e8f64357f834f8f7d64ac456b32fc1bc09fd3084565f7b4e9eae8ada85adb6be35f89effedcdf84831a
--- --- ---
SHA3-512

Input  : 9c3679d1a7ba47a5fb7cd05fa60fe05939e6bf267b7e4b95c1b7a306ee83d5d0
Output : 31e4c24bf38c287ba66f992b7bddebf99bfe6dca03830e9845880af5019a02aa32e1e5aef47cec902731d0b83815a5faed3ecabc44b68082bbd199a12b40579c
--- --- ---
SHAKE-128

Input  : 09ad8b662f1e7c1fc151f2b896a33d22695a6e1e67e316bd4953c3475a8bd6b3
Output : b7e3784fa89e4c9edd7eacd6e3ffa67c7032b22fbf31c58a7f0b268b0040c9099607a7ac68cc8722
--- --- ---
SHAKE-256

Input  : 5f6b6c5c905cca71d9f83cd90b7db3a4f4ca5303e97e25e343e2c829ae0ce7c3
Output : be24aea2c7d7488d28d638ee46baba5474994171b6bc42fce59679f5578e57ac59f4521cbeb88bd5
--- --- ---
```

> [!NOTE]
> This library doesn't expose any raw pointer + length -based interfaces, rather everything is wrapped under much safer `std::span` - which one can easily create from `std::{array, vector}` or even raw pointers and length pair. See https://en.cppreference.com/w/cpp/container/span. I made this choice because this gives us much better type safety and compile-time error reporting.
