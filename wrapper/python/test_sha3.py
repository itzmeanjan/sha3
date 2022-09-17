#!/usr/bin/python3

import random
import hashlib
import sha3
import pytest

FROM_BYTES = 0  # minimum test input message byte length
TO_BYTES = 1024  # maximum test input message byte length


def gen_rand_bytes(n: int) -> bytes:
    return bytes([random.randint(0, 255) for _ in range(n)])


def test_sha3_224_hash():
    """
    Test functional correctness of SHA3-224 hash function implementation
    """
    for i in range(FROM_BYTES, TO_BYTES + 1):
        msg = gen_rand_bytes(i)

        dig0 = sha3.sha3_224_hash(msg)
        dig1 = hashlib.sha3_224(msg).digest()

        assert (
            dig0 == dig1
        ), f"[SHA3-224] Expected {dig1.hex()}, found {dig0.hex()}, for input {msg.hex()}"


def test_sha3_256_hash():
    """
    Test functional correctness of SHA3-256 hash function implementation
    """
    for i in range(FROM_BYTES, TO_BYTES + 1):
        msg = gen_rand_bytes(i)

        dig0 = sha3.sha3_256_hash(msg)
        dig1 = hashlib.sha3_256(msg).digest()

        assert (
            dig0 == dig1
        ), f"[SHA3-256] Expected {dig1.hex()}, found {dig0.hex()}, for input {msg.hex()}"


def test_sha3_384_hash():
    """
    Test functional correctness of SHA3-384 hash function implementation
    """
    for i in range(FROM_BYTES, TO_BYTES + 1):
        msg = gen_rand_bytes(i)

        dig0 = sha3.sha3_384_hash(msg)
        dig1 = hashlib.sha3_384(msg).digest()

        assert (
            dig0 == dig1
        ), f"[SHA3-384] Expected {dig1.hex()}, found {dig0.hex()}, for input {msg.hex()}"


def test_sha3_512_hash():
    """
    Test functional correctness of SHA3-512 hash function implementation
    """
    for i in range(FROM_BYTES, TO_BYTES + 1):
        msg = gen_rand_bytes(i)

        dig0 = sha3.sha3_512_hash(msg)
        dig1 = hashlib.sha3_512(msg).digest()

        assert (
            dig0 == dig1
        ), f"[SHA3-512] Expected {dig1.hex()}, found {dig0.hex()}, for input {msg.hex()}"


def test_shake128_xof():
    """
    Test functional correctness of SHAKE128 extendable output function implementation
    """
    DLEN = 252  # bytes to read from SHAKE128 XoF

    for i in range(FROM_BYTES, TO_BYTES + 1):
        msg = gen_rand_bytes(i)

        dig0 = sha3.shake128_xof(msg, DLEN)
        dig1 = hashlib.shake_128(msg).digest(DLEN)

        assert (
            dig0 == dig1
        ), f"[SHAKE-128] Expected {dig1.hex()}, found {dig0.hex()}, for input {msg.hex()}"


def test_shake256_xof():
    """
    Test functional correctness of SHAKE256 extendable output function implementation
    """
    DLEN = 204  # bytes to read from SHAKE256 XoF

    for i in range(FROM_BYTES, TO_BYTES + 1):
        msg = gen_rand_bytes(i)

        dig0 = sha3.shake256_xof(msg, DLEN)
        dig1 = hashlib.shake_256(msg).digest(DLEN)

        assert (
            dig0 == dig1
        ), f"[SHAKE-256] Expected {dig1.hex()}, found {dig0.hex()}, for input {msg.hex()}"


@pytest.mark.benchmark(group="sha3-224", min_rounds=5, disable_gc=True, warmup=True)
def test_bench_sha3_224_self(benchmark):
    """
    Benchmark my implementation of SHA3-224 hash function
    """
    msg = gen_rand_bytes(56)

    @benchmark
    def compute():
        return sha3.sha3_224_hash(msg)


@pytest.mark.benchmark(group="sha3-224", min_rounds=5, disable_gc=True, warmup=True)
def test_bench_sha3_224_hashlib(benchmark):
    """
    Benchmark SHA3-224 hash function implementation of hashlib
    """
    msg = gen_rand_bytes(56)

    @benchmark
    def compute():
        return hashlib.sha3_224(msg).digest()


@pytest.mark.benchmark(group="sha3-256", min_rounds=5, disable_gc=True, warmup=True)
def test_bench_sha3_256_self(benchmark):
    """
    Benchmark my implementation of SHA3-256 hash function
    """
    msg = gen_rand_bytes(64)

    @benchmark
    def compute():
        return sha3.sha3_256_hash(msg)


@pytest.mark.benchmark(group="sha3-256", min_rounds=5, disable_gc=True, warmup=True)
def test_bench_sha3_256_hashlib(benchmark):
    """
    Benchmark SHA3-256 hash function implementation of hashlib
    """
    msg = gen_rand_bytes(64)

    @benchmark
    def compute():
        return hashlib.sha3_256(msg).digest()


@pytest.mark.benchmark(group="sha3-384", min_rounds=5, disable_gc=True, warmup=True)
def test_bench_sha3_384_self(benchmark):
    """
    Benchmark my implementation of SHA3-384 hash function
    """
    msg = gen_rand_bytes(96)

    @benchmark
    def compute():
        return sha3.sha3_384_hash(msg)


@pytest.mark.benchmark(group="sha3-384", min_rounds=5, disable_gc=True, warmup=True)
def test_bench_sha3_384_hashlib(benchmark):
    """
    Benchmark SHA3-384 hash function implementation of hashlib
    """
    msg = gen_rand_bytes(96)

    @benchmark
    def compute():
        return hashlib.sha3_384(msg).digest()


@pytest.mark.benchmark(group="sha3-512", min_rounds=5, disable_gc=True, warmup=True)
def test_bench_sha3_512_self(benchmark):
    """
    Benchmark my implementation of SHA3-512 hash function
    """
    msg = gen_rand_bytes(128)

    @benchmark
    def compute():
        return sha3.sha3_512_hash(msg)


@pytest.mark.benchmark(group="sha3-512", min_rounds=5, disable_gc=True, warmup=True)
def test_bench_sha3_512_hashlib(benchmark):
    """
    Benchmark SHA3-512 hash function implementation of hashlib
    """
    msg = gen_rand_bytes(128)

    @benchmark
    def compute():
        return hashlib.sha3_512(msg).digest()


if __name__ == "__main__":
    print("Use `pytest` for executing test cases")
