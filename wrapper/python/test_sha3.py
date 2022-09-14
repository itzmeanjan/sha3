#!/usr/bin/python3

import random
import hashlib
import sha3

FROM_BYTES = 0
TO_BYTES = 1024


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

        flg = dig0 == dig1
        assert flg, f"Expected {dig1.hex()}, found {dig0.hex()}, for input {msg.hex()}"


if __name__ == "__main__":
    print("Use `pytest` for executing test cases")
