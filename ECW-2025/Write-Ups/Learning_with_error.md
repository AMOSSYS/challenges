# Learning from error - Write-up

This is a write-up for the challenge [Learning from error](../Learning_from_error/).

> A communication has been intercepted containing encrypted data by the DOSEAL initiative (Defensive Operation for Satellite Encrypted Analysis Localization).
> It seems to be related to a KEM public key that our experts confirmed to be a post-quantum mechanism, and the corresponding public key has been found.
> The secret key is nowhere to be found.
> 
> However, our informant has provided the DOSEAL key generation source code and a memory dump of one generation.
> 
> Can the secret document of the DOSEAL initiative be retrieved?

This challenge is based on FrodoKEM post-quantum mechanism where a KEM
shared secret is used to encrypt a secret document (that contains the
flag).

Five files are given:
- `doseal_ciphertext.bin`: a FrodoKEM-1344 ciphertext
- `doseal_dump.bin`: a memory dump of a FrodoKEM-1344 key generation
- `doseal_teplay.pub`: a FrodoKEM-1344 public key
- `frodokem_keygen.c`: source code of FrodoKEM-1344 key generation
- `output.txt`: a secret document encrypted with AES-256-CBC where the key is the secret key encapsulated in the file `doseal_ciphertext.bin`

The goal seems to be clear: the AES key is needed to decrypt the secret document, so **the FrodoKEM-1344 private key must be reconstructed** to execute a decapsulation.

## Overview of FrodoKEM key generation

FrodoKEM is quite simple. Its key generation uses matrices:
- $S$: secret matrix of dimension $1344 \times 8$ (part of the secret key)
- $E$: error matrix of dimension $1344 \times 8$ (secret, but not kept after generation)
- $A$: public key of dimension $1344 \times 1344$
- $B$: public key calculatedd as $B = AS + E$

## Source code analysis

The analysis of the source code shows that several values MUST be cleaned from memory according to the comments. It is the case for the buffer mat_e:

```c
  uint16_t mat_e[N*NBAR]; // must be CLEANED
```

However, it is not explicitely erased using the function `memory_cleanse()` contrary to the matrix `mat_s` which is the matrix of the secret key.

## Dump analysis

Since the error matrix is not erased, it is highly probable to find it in the memory dump.
In FrodoKEM, each coefficient of this matrix is sampled according to a distribution and is an integer between $-6$ and $6$ (for FrodoKEM-1344).
The coefficients are integers reduced modulo $2^{16}$ so they are represented in memory as 16-bit integers.

As a consequence, the error matrix should be represented in memory as a buffer of bytes `0xFFFA`, `0xFFFB`, `0xFFFC`, `0xFFFD`, `0xFFFE`, `0xFFFF`, `0x0000`, `0x0001`, `0x0002`, `0x0003`, `0x0004`, `0x0005`, `0x0006`, which is easily located in a memory dump.

Indeed, a buffer of size $2 \times 1344 \times 8$ bytes is present from offset `0x0820` which corresponds exactly to the number of bytes to represent this matrix.

Thus, the matrix $E$ can be reconstructed.

## Reconstructing the secret key

The matrix $A$ is a square matrix and it might be possible to invert.
Then, the secret matrix $S$ can be retrived by calculating: $S = A^{- 1}(B - E)$.

One condition for the matrix $A$ to be invertible is that its determinant must be coprime with the modulus $2^{16}$, so it must be odd.

Fortunately, this is the case, then the KEM secret key can be reconstructed.
It is the concatenation of the following elements (this can be found at the end of the key generation in the source code):
- `s`: used for rejection in case of decapsulation failure, not necessary here for a correct decapsulation in the challenge context
- public key: seed for matrix $A$ and packed version of $B$
- $S^{T}$: secret matrix transposed
- `pkh`: hash of the public key with SHAKE256

Then it can be used for a decapsulation.

## Decapsulation and flag

For this part, an external implementation can be used.
Since the source code points to https://frodokem.org, a reference implementation can be
found there that contains a C version and a Python version:
https://github.com/Microsoft/PQCrypto-LWEKE.

## Code of the solution

The first implemenation of the solution uses PARI for the matrix inversion through Sagemath.
However, it is quite slow (a few minutes).
An alternative is the implementation of Gaussian elimination in C which is much faster.
This implementation is provided [here](../Learning_from_error/solution/find_shared_key/).

The Sagemath solution is given below:

```py
from hashlib import shake_128, shake_256
from sage.all import Matrix, Zmod, pari, ZZ
import json
try:
    from Cryptodome.Cipher import AES
    from Cryptodome.Util.Padding import unpad
except:
    from Crypto.Cipher import AES
    from Crypto.Util.Padding import unpad

# challenge files
PUBLIC_KEY_PATH = "../files/doseal_tepley.pub"
DUMP_PATH = "../files/doseal_dump.bin" 
CIPHERTEXT_PATH = "../files/doseal_ciphertext.bin"
OUTPUT_PATH = "../files/output.txt"

# FrodoKEM-1344 parameters
N = 1344
NBAR = 8
LEN_A = 16
LEN_SE = 64
LEN_SEC = 32

Zq = Zmod(2**16)


# ===================
# retrieve secret key
# ===================

# load public key
pk_raw = open(PUBLIC_KEY_PATH, 'rb').read()
pkh = shake_256(pk_raw).digest(32) # public key hash
seed_a = pk_raw[:LEN_A]
b = pk_raw[LEN_A:]

# generate matrix A
A = Matrix(Zq, N)
for i in range(N):
    buffer = shake_128(bytes([i % 256, i >> 8]) + seed_a).digest(2*N)
    for j in range(N):
        A[i,j] = buffer[2*j] | (buffer[2*j + 1] << 8)

# unpack matrix B
B = Matrix(Zq, N, NBAR)
for i in range(N):
    for j in range(NBAR):
        B[i,j] = (b[2*(i*NBAR + j)] << 8) | b[2*(i*NBAR + j) + 1]

# get error matrix E from dump (present at offset 2080 of size 2*N*NBAR bytes)
data = open(DUMP_PATH, 'rb').read()[2080:2080 + 2*N*NBAR]
E = Matrix(Zq, N, NBAR)
for i in range(N):
    for j in range(NBAR):
        E[i,j] = data[2*(i*NBAR + j)] | (data[2*(i*NBAR + j) + 1] << 8)

# calculate A^-1 through PARI since it's faster
# A must be converted as an integer matrix to be used with pari.matinvmod
A_ZZ = A.change_ring(ZZ)
A_pari = pari(A_ZZ)
A_inv = pari.matinvmod(A_pari, 2**16).sage()

# calculate S = A^(-1)*(B - E)
S = A_inv*(B - E)


# =============
# decapsulation
# =============

# load ciphertext
ct = open(CIPHERTEXT_PATH, 'rb').read()
c1 = ct[:2*N*NBAR]
c2 = ct[2*N*NBAR:2*N*NBAR + 2*NBAR*NBAR]
salt = ct[2*N*NBAR + 2*NBAR*NBAR:]

# unpack matrix B'
Bp = Matrix(Zq, NBAR, N)
for i in range(NBAR):
    for j in range(N):
        Bp[i,j] = (c1[2*(i*N + j)] << 8) | c1[2*(i*N + j) + 1]

# unpack matrix C
C = Matrix(Zq, NBAR, NBAR)
for i in range(NBAR):
    for j in range(NBAR):
        C[i,j] = (c2[2*(i*NBAR + j)] << 8) | c2[2*(i*NBAR + j) + 1]

# calculate M = C - B'*S
M = C - Bp*S

# decode M by rounding the four msb of each coefficient into a 32-byte buffer
buffer = []
for i in range(NBAR):
    for j in range(NBAR):
        if j % 2 == 0:
            # 4 lsb of a new byte
            buffer.append(round(ZZ(M[i,j]) / 2**12))
        else:
            # 4 msb of current byte
            buffer[-1] |= (round(ZZ(M[i,j]) / 2**12) << 4)
u = bytes(buffer)

# derive secret value k
buffer = shake_256(pkh + u + salt).digest(LEN_SE + LEN_SEC)
k = buffer[LEN_SE:]

# derive shared secret
ss = shake_256(ct + k).digest(LEN_SEC)


# ==========================
# secret document decryption
# ==========================

data = json.loads(open(OUTPUT_PATH, "r").read())
iv = bytes.fromhex(data['iv'])
ciphertext = bytes.fromhex(data['encrypted'])

cipher = AES.new(ss, AES.MODE_CBC, iv=iv)
secret_document = unpad(cipher.decrypt(ciphertext), 16)
print(secret_document.decode('utf-8'))
```
