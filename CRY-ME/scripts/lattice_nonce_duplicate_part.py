#!/usr/bin/env python3

import sys
from fpylll import IntegerMatrix, LLL, BKZ
from base64 import b64decode as b64d
# Custom implementation of elliptic curves in Python
from ec import *

VERBOSE = False

curve = CurveJac(CURVES['wei25519'])
n = curve.order

# ---------------
# LLL Attack
# ---------------

def construct_matrix(signatures, sk_bitlen: int):
    n_sig = len(signatures)
    
    ell = 64
    K = 2**sk_bitlen
    dim = n_sig + 3*n_sig + 2
    M = IntegerMatrix(dim, dim)

    # Signature equations in first columns
    for i in range(n_sig):
        M[i,       i] = n
        M[dim - 2, i] = signatures[i][0]
        M[dim - 1, i] = -signatures[i][1]

        M[n_sig + i*3,     i] = 2**(3*ell)
        M[n_sig + i*3 + 1, i] = 2**ell + 2**(2*ell)
        M[n_sig + i*3 + 2, i] = 1

    # Weights on unknown variables
    # To form an integer matrix, we multiply all entries by K
    for i in range(dim):
        for j in range(n_sig):
            M[i, j] = K*M[i, j]
    for i in range(3*n_sig):
        M[n_sig + i, n_sig + i] = K // 2**ell
    M[dim - 2, dim - 2] = 1
    M[dim - 1, dim - 1] = K
    
    return M

def find_key(signatures, curve, pubkey, sk_bitlen: int):
    found = False
    M = construct_matrix(signatures, sk_bitlen)
    LLL.reduction(M)
    res = []
    for i in range(M.nrows):
        privkey = M[i, -2]
        Q = curve.scalar_mult(privkey, curve.base)
        if Q[0] == pubkey[0]:
            found = True
            if Q[1] == -pubkey[1]:
                privkey = -privkey
            if VERBOSE:
                print(f'Row: {M[i]}')
            return privkey
    return None

# ---------------
# Load file
# ---------------

def load_signatures(file_name):
    lines = open(file_name, 'r').readlines()
    signatures = []
    for line in lines:
        if line.startswith('#') or len(line.strip()) == 0:
            continue
        sig_bytes = bytes.fromhex(line.strip())
        r = int.from_bytes(sig_bytes[:32], 'big')
        s = int.from_bytes(sig_bytes[32:], 'big')
        signatures.append((r, s))
    return signatures

if __name__ == '__main__':
    argv = []
    for arg in sys.argv[1:]:
        if arg == '-v':
            VERBOSE = True
        else:
            argv += [arg]
    argc = len(argv)
    if argc != 3:
        print('Arguments:\n- public key in hexadecimal\n- file name of signatures\n- bit length of secret key')
        sys.exit()

    pubkey_bytes = bytes.fromhex(argv[0])
    xq = int.from_bytes(pubkey_bytes[:32], 'big')
    yq = int.from_bytes(pubkey_bytes[32:], 'big')
    if not curve.is_on_curve((xq, yq)):
        print('This is not a valid public key.')
        sys.exit()
    pubkey = curve.field(xq), curve.field(yq)

    signatures = load_signatures(argv[1])
    sk_bitlen = int(argv[2])

    print(f'Public key: ({hex(xq)}, {hex(yq)})')
    privkey = find_key(signatures, curve, pubkey, sk_bitlen)
    if privkey is None:
        print('Private key not found')
    else:
        print(f'Private key: {hex(privkey)}')
