#include "sha3_224.hpp"
#include "sha3_256.hpp"
#include "sha3_384.hpp"
#include "sha3_512.hpp"
#include "shake128.hpp"
#include "shake256.hpp"

// Thin C wrapper on top of underlying C++ implementation of SHA3 hash functions
// and XOFs, which can be used for producing shared library object with
// conformant C-ABI & used from other languages such as Rust, Python

// Function prototype(s)
extern "C"
{
  void sha3_224_hash(const uint8_t* const __restrict, // input message
                     const size_t,                    // input byte length
                     uint8_t* const __restrict        // output digest
  );

  void sha3_256_hash(const uint8_t* const __restrict, // input message
                     const size_t,                    // input byte length
                     uint8_t* const __restrict        // output digest
  );

  void sha3_384_hash(const uint8_t* const __restrict, // input message
                     const size_t,                    // input byte length
                     uint8_t* const __restrict        // output digest
  );

  void sha3_512_hash(const uint8_t* const __restrict, // input message
                     const size_t,                    // input byte length
                     uint8_t* const __restrict        // output digest
  );

  void shake128_xof(const uint8_t* const __restrict, // input message
                    const size_t,                    // input byte length
                    uint8_t* const __restrict,       // output digest
                    const size_t                     // output byte length
  );

  void shake256_xof(const uint8_t* const __restrict, // input message
                    const size_t,                    // input byte length
                    uint8_t* const __restrict,       // output digest
                    const size_t                     // output byte length
  );
}

// Function implementation(s)
extern "C"
{

  // Given N (>=0) -bytes input message, this routines computes 28 -bytes output
  // digest, using SHA3-224 hashing algorithm
  void sha3_224_hash(
    const uint8_t* const __restrict in, // input message
    const size_t ilen,                  // len(in) | >= 0
    uint8_t* const __restrict out       // 28 -bytes digest, to be computed
  )
  {
    sha3_224::hash(in, ilen, out);
  }

  // Given N (>=0) -bytes input message, this routines computes 32 -bytes output
  // digest, using SHA3-256 hashing algorithm
  void sha3_256_hash(
    const uint8_t* const __restrict in, // input message
    const size_t ilen,                  // len(in) | >= 0
    uint8_t* const __restrict out       // 32 -bytes digest, to be computed
  )
  {
    sha3_256::hash(in, ilen, out);
  }

  // Given N (>=0) -bytes input message, this routines computes 48 -bytes output
  // digest, using SHA3-384 hashing algorithm
  void sha3_384_hash(
    const uint8_t* const __restrict in, // input message
    const size_t ilen,                  // len(in) | >= 0
    uint8_t* const __restrict out       // 48 -bytes digest, to be computed
  )
  {
    sha3_384::hash(in, ilen, out);
  }

  // Given N (>=0) -bytes input message, this routines computes 64 -bytes output
  // digest, using SHA3-512 hashing algorithm
  void sha3_512_hash(
    const uint8_t* const __restrict in, // input message
    const size_t ilen,                  // len(in) | >= 0
    uint8_t* const __restrict out       // 64 -bytes digest, to be computed
  )
  {
    sha3_512::hash(in, ilen, out);
  }

  // Given N (>=0) -bytes input message, this routine computes arbitrary number
  // of output bytes, using SHAKE-128 extendable output function
  void shake128_xof(const uint8_t* const __restrict in, // input message
                    const size_t ilen,                  // len(in) | >= 0
                    uint8_t* const __restrict out,      // output digest
                    const size_t olen                   // len(out) | >= 0
  )
  {
    shake128::shake128 hasher{};
    hasher.hash(in, ilen);
    hasher.read(out, olen);
  }

  // Given N (>=0) -bytes input message, this routine computes arbitrary number
  // of output bytes, using SHAKE-256 extendable output function
  void shake256_xof(const uint8_t* const __restrict in, // input message
                    const size_t ilen,                  // len(in) | >= 0
                    uint8_t* const __restrict out,      // output digest
                    const size_t olen                   // len(out) | >= 0
  )
  {
    shake256::shake256 hasher{};
    hasher.hash(in, ilen);
    hasher.read(out, olen);
  }
}
