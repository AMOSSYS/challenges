#!/usr/bin/env python3

from random import randrange
from hashlib import sha3_256
from .polymod import *

params = {
    'N' : 101,
    'q' : 4096,
    'd' : 67,
    'pack3_size': 21,
    'packq_size': 152
}

# ---- Packing and unpacking of private/public keys

def pack_q(pol):
    '''
    Converts a polynomial of degree < N with coefficients
    in [-q/2, q/2 - 1] to a byte array.
    '''
    a = 0
    logq = pol.q.bit_length() - 1
    for i, coef in enumerate(pol.coefs):
        c = coef % pol.q
        for j in range(logq):
            a |= (((c >> j) & 1) << (logq*i + j))
    out = int.to_bytes(a, (pol.N*logq + 7)//8, 'little')
    return out

def unpack_q(arr, N, q):
    '''
    Converts a byte array to a polynomial of degree < N
    with coefficients in [-q/2, q/2 - 1].
    '''
    logq = q.bit_length() - 1
    expected_size = (N*logq + 7)//8
    assert len(arr) == expected_size
    a = int.from_bytes(arr, 'little')
    coefficients = [0]*N

    for i in range(N):
        for j in range(logq):
            coefficients[i] |= ((a & 1) << j)
            a >>= 1
    
    return PolyMod(N, q, coefficients)

def pack_3(pol):
    '''
    Converts a polynomial of degree < N with coefficients
    in {-1, 0, 1} to a byte array.
    '''
    outlen = (pol.N + 4)//5
    L = []
    for i in range(outlen):
        chunk = pol.coefs[i*5:i*5 + 5]
        s = sum((chunk[j] % 3)*3**j for j in range(len(chunk)))
        L.append(s)
    
    return bytes(L)

def unpack_3(arr, N):
    '''
    Converts a byte array to a polynomial of degree < N
    with coefficients in {-1, 0, 1}.
    '''
    expected_size = (N + 4)//5
    assert len(arr) == expected_size
    coefficients = []
    ncoefs_lastbyte = N % 5
    for i in range(expected_size - 1):
        b = arr[i]
        for j in range(5):
            coefficients.append(b % 3)
            b //= 3
    # Last byte has less coefficients coded in it
    last_byte = arr[-1]
    for j in range(ncoefs_lastbyte):
        coefficients.append(last_byte % 3)
        b //= 3

    return PolyMod(N, 3, coefficients)

# ---- Random polynomials

def random_pol(N, q, d):
    '''
    Returns a random polynomial of degree < N with coefficients in {-1, 0, 1}.
    The base ring is Z/qZ, and the number of nonzero coefficients is d.
    '''
    coefficients = [0]*N
    for j in range(d):
        while True:
            r = randrange(N)
            if not coefficients[r]:
                break
        coefficients[r] = 1 - 2*randrange(2)
    
    return PolyMod(N, q, coefficients)

def random_message(N, q):
    '''
    Returns a random polynomial of degree < N with coefficients in {-1,0,1}.
    The base ring is Z/qZ.
    '''
    coefficients = [randrange(-1,2) for i in range(N)]
    return PolyMod(N, q, coefficients)

# ----- Encapsulation / Decapsulation

class NTRUPubKey:
    def __init__(self, params, pubkey):
        self.N = params['N']
        self.q = params['q']
        self.d = params['d']
        self.h = unpack_q(pubkey, self.N, self.q)
 
    def encaps(self):
        '''
        Encapsulation
        '''
        r = random_pol(self.N, self.q, self.d)
        m = random_message(self.N, self.q)
        ciphertext = self.h*r + m
        packed_ciphertext = pack_q(ciphertext)
        packed_r = pack_3(r)
        packed_m = pack_3(m)
        key = sha3_256(packed_r + packed_m).digest()
        return key, packed_ciphertext


class NTRUPrivKey:
    def __init__(self, params, privkey):
        self.N = params['N']
        self.q = params['q']
        pack3_size = (self.N + 4)//5
        self.f = unpack_3(privkey[:pack3_size], self.N).change_ring(self.q)
        self.fp = unpack_3(privkey[pack3_size:2*pack3_size], self.N)
        self.hq = unpack_q(privkey[2*pack3_size:], self.N, self.q)

    def decaps(self, packed_ciphertext):
        '''
        Decapsulation
        '''
        # We decrypt c
        c = unpack_q(packed_ciphertext, self.N, self.q)
        a = (c*self.f).change_ring(3)
        m = (a*self.fp).change_ring(self.q)
        # We recover the random polynomial r
        r = c - m
        r = r*self.hq
        # Shared key is hash of (r, m)
        packed_r = pack_3(r)
        packed_m = pack_3(m)
        key = sha3_256(packed_r + packed_m).digest()
        return key
