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
