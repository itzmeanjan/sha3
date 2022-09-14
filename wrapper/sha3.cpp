#include "sha3_224.hpp"

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
}
