#!/usr/bin/python3

"""
  Before using `sha3` library module, make sure you've run
  `make lib` and generated shared library object, which is loaded
  here; then all function calls are forwarded to respective C++
  implementation, executed on host CPU.

  Author: Anjan Roy <hello@itzmeanjan.in>
  
  Project: https://github.com/itzmeanjan/sha3
"""

from ctypes import c_size_t, CDLL
import numpy as np
from posixpath import exists, abspath

SO_PATH: str = abspath("../libsha3.so")
assert exists(SO_PATH), "Use `make lib` to generate shared library object !"

SO_LIB: CDLL = CDLL(SO_PATH)

u8 = np.uint8
len_t = c_size_t
uint8_tp = np.ctypeslib.ndpointer(dtype=u8, ndim=1, flags="CONTIGUOUS")


def sha3_224_hash(msg: bytes) -> bytes:
    """
    Given a N ( >= 0 ) -bytes input message, this function computes 28 -bytes
    SHA3-224 digest
    """
    mlen = len(msg)
    msg_ = np.frombuffer(msg, dtype=u8)
    digest = np.empty(28, dtype=u8)

    args = [uint8_tp, len_t, uint8_tp]
    SO_LIB.sha3_224_hash.argtypes = args

    SO_LIB.sha3_224_hash(msg_, mlen, digest)

    digest_ = digest.tobytes()
    return digest_


def sha3_256_hash(msg: bytes) -> bytes:
    """
    Given a N ( >= 0 ) -bytes input message, this function computes 32 -bytes
    SHA3-256 digest
    """
    mlen = len(msg)
    msg_ = np.frombuffer(msg, dtype=u8)
    digest = np.empty(32, dtype=u8)

    args = [uint8_tp, len_t, uint8_tp]
    SO_LIB.sha3_256_hash.argtypes = args

    SO_LIB.sha3_256_hash(msg_, mlen, digest)

    digest_ = digest.tobytes()
    return digest_


def sha3_384_hash(msg: bytes) -> bytes:
    """
    Given a N ( >= 0 ) -bytes input message, this function computes 48 -bytes
    SHA3-384 digest
    """
    mlen = len(msg)
    msg_ = np.frombuffer(msg, dtype=u8)
    digest = np.empty(48, dtype=u8)

    args = [uint8_tp, len_t, uint8_tp]
    SO_LIB.sha3_384_hash.argtypes = args

    SO_LIB.sha3_384_hash(msg_, mlen, digest)

    digest_ = digest.tobytes()
    return digest_


if __name__ == "__main__":
    print("Use `sha3` as a library module")
