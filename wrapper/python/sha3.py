#!/usr/bin/python3

"""
  Before using `sha3` library module, make sure you've run
  `make lib` and generated shared library object, which is loaded
  here; then all function calls are forwarded to respective C++
  implementation, executed on host CPU.

  Author: Anjan Roy <hello@itzmeanjan.in>
  
  Project: https://github.com/itzmeanjan/sha3
"""

from ctypes import c_size_t, CDLL, c_char_p, create_string_buffer
from posixpath import exists, abspath

SO_PATH: str = abspath("../libsha3.so")
assert exists(SO_PATH), "Use `make lib` to generate shared library object !"

SO_LIB: CDLL = CDLL(SO_PATH)


def sha3_224_hash(msg: bytes) -> bytes:
    """
    Given a N ( >= 0 ) -bytes input message, this function computes 28 -bytes
    SHA3-224 digest
    """
    mlen = len(msg)
    dig = create_string_buffer(28)

    args = [c_char_p, c_size_t, c_char_p]
    SO_LIB.sha3_224_hash.argtypes = args

    SO_LIB.sha3_224_hash(msg, mlen, dig)

    return dig.raw


def sha3_256_hash(msg: bytes) -> bytes:
    """
    Given a N ( >= 0 ) -bytes input message, this function computes 32 -bytes
    SHA3-256 digest
    """
    mlen = len(msg)
    dig = create_string_buffer(32)

    args = [c_char_p, c_size_t, c_char_p]
    SO_LIB.sha3_256_hash.argtypes = args

    SO_LIB.sha3_256_hash(msg, mlen, dig)

    return dig.raw


def sha3_384_hash(msg: bytes) -> bytes:
    """
    Given a N ( >= 0 ) -bytes input message, this function computes 48 -bytes
    SHA3-384 digest
    """
    mlen = len(msg)
    dig = create_string_buffer(48)

    args = [c_char_p, c_size_t, c_char_p]
    SO_LIB.sha3_384_hash.argtypes = args

    SO_LIB.sha3_384_hash(msg, mlen, dig)

    return dig.raw


def sha3_512_hash(msg: bytes) -> bytes:
    """
    Given a N ( >= 0 ) -bytes input message, this function computes 64 -bytes
    SHA3-512 digest
    """
    mlen = len(msg)
    dig = create_string_buffer(64)

    args = [c_char_p, c_size_t, c_char_p]
    SO_LIB.sha3_512_hash.argtypes = args

    SO_LIB.sha3_512_hash(msg, mlen, dig)

    return dig.raw


def shake128_xof(msg: bytes, dlen: int) -> bytes:
    """
    Given a N ( >= 0 ) -bytes input message, this function computes `dlen` -bytes
    SHAKE128 output | dlen >= 0
    """
    mlen = len(msg)
    dig = create_string_buffer(dlen)

    args = [c_char_p, c_size_t, c_char_p, c_size_t]
    SO_LIB.shake128_xof.argtypes = args

    SO_LIB.shake128_xof(msg, mlen, dig, dlen)

    return dig.raw


def shake256_xof(msg: bytes, dlen: int) -> bytes:
    """
    Given a N ( >= 0 ) -bytes input message, this function computes `dlen` -bytes
    SHAKE256 output | dlen >= 0
    """
    mlen = len(msg)
    dig = create_string_buffer(dlen)

    args = [c_char_p, c_size_t, c_char_p, c_size_t]
    SO_LIB.shake256_xof.argtypes = args

    SO_LIB.shake256_xof(msg, mlen, dig, dlen)

    return dig.raw


if __name__ == "__main__":
    print("Use `sha3` as a library module")
