#!/usr/bin/env python3

import sys
from fpylll import IntegerMatrix, LLL
from base64 import b64decode as b64d
# Custom implementation of elliptic curves in Python
from ec import *

VERBOSE = False

curve = CurveJac(CURVES['wei25519'])
n = curve.order

a1 = 7520897724310334953
a0 = 1876011003808476466
b1 = 18108194425815612945
b0 = 7401132627792533940
c1 = 17333513496047876729
c0 = 10346034117385188870

# ---------------
# LLL Attack
# ---------------

def construct_matrix(r, s):
    ell = 64
    K = 2**128
    M = IntegerMatrix(29, 29)

    # Signature equation in first column
    M[0, 0] = n
    M[27, 0] = r
    M[28, 0] = -s
    for i in range(8):
        M[i + 3, 0] = 2**((7 - i)*8) * 2**(3*ell)
        M[i + 11, 0] = 2**((7 - i)*8) * (2**(2*ell) + 2**ell)
        M[i + 19, 0] = 2**((7 - i)*8)

    # Equation X3 = c1*X0 + c0 in second column
    M[1, 1] = 2**ell
    M[28, 1] = c0
    for i in range(8):
        M[i + 3, 1] = -2**(i*8)
        M[i + 19, 1] = 2**(i*8) * c1

    # Equation X2 = b1*X0 + b0
    M[2, 2] = 2**ell
    M[28, 2] = b0
    for i in range(8):
        M[i + 11, 2] = -2**(i*8)
        M[i + 19, 2] = 2**(i*8) * b1

    # Weights on unknown variables
    # To form an integer matrix, we multiply all entries by K = 2**128
    for i in range(M.nrows):
        for j in range(M.ncols):
            M[i, j] = K*M[i, j]
    for i in range(24):
        M[i + 3, i + 3] = K // 2**8
    M[27, 27] = 1
    M[28, 28] = K
    
    return M

def find_key(r, s, curve, pubkey):
    found = False
    M = construct_matrix(r, s)
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

if __name__ == '__main__':
    argv = []
    for arg in sys.argv[1:]:
        if arg == '-v':
            VERBOSE = True
        else:
            argv += [arg]
    argc = len(argv)
    if argc != 2:
        print('Please write the public key and signature in hexadecimal or base64')
        sys.exit()

    try:
        pubkey_bytes = bytes.fromhex(argv[0])
    except:
        pubkey_bytes = b64d(argv[0])
    
    try:
        sig_bytes = bytes.fromhex(argv[1])
    except:
        sig_bytes = b64d(argv[1])
    
    xq = int.from_bytes(pubkey_bytes[:32], 'big')
    yq = int.from_bytes(pubkey_bytes[32:], 'big')
    if not curve.is_on_curve((xq, yq)):
        print('This is not a valid public key.')
        sys.exit()
    pubkey = curve.field(xq), curve.field(yq)

    r = int.from_bytes(sig_bytes[:32], 'big')
    s = int.from_bytes(sig_bytes[32:], 'big')
    
    print(f'Public key: ({hex(xq)}, {hex(yq)})')
    privkey = find_key(r, s, curve, pubkey)
    if privkey is None:
        print('Private key not found')
    else:
        print(f'Private key: {hex(privkey)}')
