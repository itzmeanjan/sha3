#pragma once
#include "sponge.hpp"

// SHAKE256 Extendable Output Function : Keccak[512](M || 1111, d)
namespace shake256 {

// Capacity of sponge, in bits
constexpr size_t capacity = 512;
// Rate of sponge, in bits
constexpr size_t rate = 1600 - capacity;

// SHAKE256 Extendable Output Function
//
// See SHA3 extendable output function definition in section 6.2 of SHA3
// specification https://dx.doi.org/10.6028/NIST.FIPS.202
template<const bool incremental = false>
struct shake256
{
private:
  uint64_t state[25];
  bool absorbed = false;
  size_t readable = 0;
  size_t offset = 0;
  size_t abytes = 0;

public:
  // Given N -bytes input message, this routine consumes it into keccak[512]
  // sponge state
  //
  // Once you call this function on some object, calling it again doesn't do
  // anything !
  void hash(const uint8_t* const __restrict msg, const size_t mlen)
    requires(!incremental)
  {
    if (!absorbed) {
      sponge::absorb<0b00001111, 4, rate>(state, msg, mlen);

      absorbed = true;
      readable = rate >> 3;
    }
  }

  // Given N -many bytes input message, this routine consumes those into
  // keccak[512] sponge state
  //
  // Note, this routine can be called arbitrary number of times, each time with
  // arbitrary bytes of input message, until keccak[512] state is finalized ( by
  // calling routine with similar name ).
  //
  // This function is only enabled, when you decide to use SHAKE256 in
  // incremental mode ( compile-time decision ). By default one uses SHAKE256
  // API in non-incremental mode.
  void absorb(const uint8_t* const __restrict msg, const size_t mlen)
    requires(incremental)
  {
    constexpr size_t rbytes = rate >> 3;   // # -of bytes
    constexpr size_t rwords = rbytes >> 3; // # -of 64 -bit words

    if (!absorbed) {
      uint8_t blk_bytes[rbytes]{};
      uint64_t blk_words[rwords]{};

      const size_t blk_cnt = (offset + mlen) / rbytes;
      size_t moff = 0;

      for (size_t i = 0; i < blk_cnt; i++) {
        std::memcpy(blk_bytes + offset, msg + moff, rbytes - offset);

        if constexpr (std::endian::native == std::endian::little) {
          std::memcpy(blk_words, blk_bytes, rbytes);
        } else {
          for (size_t j = 0; j < rwords; j++) {
            const size_t boff = j << 3;
            blk_words[j] = (static_cast<uint64_t>(blk_bytes[boff ^ 7]) << 56) |
                           (static_cast<uint64_t>(blk_bytes[boff ^ 6]) << 48) |
                           (static_cast<uint64_t>(blk_bytes[boff ^ 5]) << 40) |
                           (static_cast<uint64_t>(blk_bytes[boff ^ 4]) << 32) |
                           (static_cast<uint64_t>(blk_bytes[boff ^ 3]) << 24) |
                           (static_cast<uint64_t>(blk_bytes[boff ^ 2]) << 16) |
                           (static_cast<uint64_t>(blk_bytes[boff ^ 1]) << 8) |
                           (static_cast<uint64_t>(blk_bytes[boff ^ 0]) << 0);
          }
        }

        for (size_t j = 0; j < rwords; j++) {
          state[j] ^= blk_words[j];
        }

        moff += (rbytes - offset);
        offset += (rbytes - offset);

        keccak::permute(state);
        offset %= rbytes;
      }

      const size_t rm_bytes = mlen - moff;

      std::memset(blk_bytes, 0, rbytes);
      std::memcpy(blk_bytes + offset, msg + moff, rm_bytes);

      if constexpr (std::endian::native == std::endian::little) {
        std::memcpy(blk_words, blk_bytes, rbytes);
      } else {
        for (size_t i = 0; i < rwords; i++) {
          const size_t boff = i << 3;
          blk_words[i] = (static_cast<uint64_t>(blk_bytes[boff ^ 7]) << 56) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 6]) << 48) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 5]) << 40) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 4]) << 32) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 3]) << 24) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 2]) << 16) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 1]) << 8) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 0]) << 0);
        }
      }

      for (size_t i = 0; i < rwords; i++) {
        state[i] ^= blk_words[i];
      }

      offset += rm_bytes;

      if (offset == rbytes) {
        keccak::permute(state);
        offset %= rbytes;
      }

      abytes += mlen;
    }
  }

  // After consuming N -many bytes ( by invoking absorb routine arbitrary many
  // times, each time with arbitrary input bytes ), this routine is invoked when
  // no more input bytes remaining to be consumed by keccak[512] state.
  //
  // Note, once this routine is called, calling absorb() or finalize() again, on
  // same SHAKE256 object, doesn't do anything. After finalization, one might
  // intend to read arbitrary many bytes by squeezing sponge, which is done by
  // calling read() function, as many times required.
  //
  // This function is only enabled, when you decide to use SHAKE256 in
  // incremental mode ( compile-time decision ). By default one uses SHAKE256
  // API in non-incremental mode.
  void finalize()
    requires(incremental)
  {
    constexpr size_t rbytes = rate >> 3;   // # -of bytes
    constexpr size_t rwords = rbytes >> 3; // # -of 64 -bit words

    if (!absorbed) {
      uint8_t pad[rbytes]{};
      uint8_t blk_bytes[rbytes]{};
      uint64_t blk_words[rwords]{};

      const size_t mblen = abytes << 3;
      const size_t tot_mblen = mblen + 4;
      const size_t plen = sponge::pad101<0b00001111, 4, rate>(tot_mblen, pad);
      const size_t read = (plen + 4) >> 3; // in bytes

      std::memcpy(blk_bytes + offset, pad, read);

      if constexpr (std::endian::native == std::endian::little) {
        std::memcpy(blk_words, blk_bytes, rbytes);
      } else {
        for (size_t i = 0; i < rwords; i++) {
          const size_t boff = i << 3;
          blk_words[i] = (static_cast<uint64_t>(blk_bytes[boff ^ 7]) << 56) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 6]) << 48) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 5]) << 40) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 4]) << 32) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 3]) << 24) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 2]) << 16) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 1]) << 8) |
                         (static_cast<uint64_t>(blk_bytes[boff ^ 0]) << 0);
        }
      }

      for (size_t i = 0; i < rwords; i++) {
        state[i] ^= blk_words[i];
      }

      offset += read;

      if (offset == rbytes) {
        keccak::permute(state);
        offset %= rbytes;
      }

      absorbed = true;
      readable = rate >> 3;
    }
  }

  // Given that N -bytes input message is already absorbed into sponge state
  // using `hash( ... )` routine, this routine is used for squeezing M -bytes
  // out of consumable part of state ( i.e. rate portion of state )
  //
  // This routine can be used for squeezing arbitrary number of bytes from
  // sponge keccak[512]
  //
  // Make sure you absorb ( see hash(...) routine ) message bytes first, then
  // only call this function, otherwise it can't squeeze out anything.
  void read(uint8_t* const __restrict dig, const size_t dlen)
  {
    if (!absorbed) {
      return;
    }

    constexpr size_t rbytes = rate >> 3;

    size_t doff = 0;
    while (doff < dlen) {
      const size_t read = std::min(readable, dlen - doff);
      const size_t soff = rbytes - readable;

      if constexpr (std::endian::native == std::endian::little) {
        std::memcpy(dig + doff, reinterpret_cast<uint8_t*>(state) + soff, read);
      } else {
        const size_t till = soff + read;

        for (size_t i = soff; i < till; i++) {
          const size_t woff = i >> 3;
          const size_t boff = (i & 7ul) << 3;

          dig[doff + i] = static_cast<uint8_t>(state[woff] >> boff);
        }
      }

      readable -= read;
      doff += read;

      if (readable == 0) {
        keccak::permute(state);
        readable = rbytes;
      }
    }
  }
};

}
