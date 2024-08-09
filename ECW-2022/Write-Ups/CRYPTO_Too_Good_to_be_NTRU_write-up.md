# CRYPTO - Too Good to be NTRU (write-up)

This is a write-up for the challenge [Too Good to be NTRU](../Too_Good_to_be_NTRU/).

We have an encrypted file and a public key for the post-quantum algorithm NTRU.
The goal is to decrypt the file using only the public key and the source code which (as we will see), is not even secure against classical computers.

## Too long; I don't like maths

The attack is a little bit maths heavy, so this is a summary of the attack.

NTRU is a public key algorithm that is used like this in this challenge:
- A **public key** is used to *encapsulate* a symmetric key: it outputs a symmetric key and a version that is encapsulated;
- The corresponding **private key** is used to *decapsulate* the symmetric key.

Only the private key can be used to retrieve the symmetric key.
So Alice uses Bob's public key to get a symmetric key and its encapsulation, and encrypts a file.
Bob receives the encrypted file and the encapsulated key, uses its private key to decapsulate the symmetric key, and decrypts the file.

The flaw is that this implementation uses **very small parameters** which can be found in the file `ntrucore.py`:
```python
params = {
    'N' : 101,
    'q' : 4096,
    'd' : 67,
    'pack3_size': 21,
    'packq_size': 152
}
```

Basically, the value `N` should be larger than 500 to be secure enough.
This value means that the private key consists of 101 integers that can take three values: $-1$, $0$ or $1$.

Running through all possible private keys is still too much, but it is possible to create an alternative private key using lattices and the LLL algorithm.

The basic idea is that the private key is composed of small integers which is important for decapsulation to work.
From the public key, we can use LLL that is good at finding small things.
In the particular context of the challenge, the original private key cannot be found, but another with small enough values can be found that makes decapsulation works!

## Details on NTRU

To understand the attack, we need to explain a little bit of NTRU.

Elements manipulated are polynomials $f = a_0 + a_1X + \cdots + a_{N-1}X^{N-1}$ of degree $N - 1$, so they always can be represented with a list a $N$ coefficients.

> This is possible because they are reduced by the polynomial $X^N - 1$, so they always can be represented with a polynomial of degree strictly less than $N$.

From now on, polynomials will be written as a list of coefficients (it is possible to use the class `PolyMod` defined in the source code to play with it and print values).

A private key is composed of two polynomials $f$ and $f_3$:
```python
# coefficients of f
[-1, -1, 1, 0, 1, 0, 1, -1, 1, 0, 0, 0, 1, -1, -1, -1, 0, 1, -1, 0, 1, 1, 0, 1, -1, -1, -1, -1, 0, 1, 0, 0, 0, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 1, 0, -1, -1, -1, 1, 1, 0, 1, 0, -1, 0, 0, 1, 1, 0, 1, -1, 1, 0, 1, 1, -1, -1, 0, 0, -1, -1, 1, 0, -1, -1, -1, -1, 1, 0, 1, 0, 1, 1, 0, -1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0]

# coeffcients de f_3
[1, 1, 1, -1, -1, -1, 1, -1, 0, 0, 0, 1, 1, -1, 0, -1, -1, 0, -1, -1, 0, -1, -1, 1, 0, 0, 1, -1, -1, -1, 0, -1, 0, -1, 1, -1, 1, 1, 0, 1, 0, -1, 1, 0, 1, 1, -1, 0, 1, 1, 1, 1, 0, 1, 0, -1, -1, 1, 0, 1, 1, 1, 0, -1, -1, 0, -1, 0, 1, -1, 1, 1, 1, -1, -1, 0, 1, -1, -1, 0, 1, 1, -1, -1, 1, -1, 1, -1, 0, -1, -1, 0, 1, 1, 1, -1, -1, 1, -1, -1, 0]
```

These polynomials satisfy $f\cdot f_3 \equiv 1 \bmod 3$, so $f_3$ is the inverse of $f$ when coefficients are reduced modulo $3$.

The public key is $h = 3f_q \cdot g$ where $f_q$ is the inverse of $f$ when reduced modulo $q$ (we have $q = 4096$ in the challenge), and $g$ (randomly generated) has coefficients in $\{-1,0,1\}$.
The public key has much bigger coefficients (between $-2048$ and $2048$).

```python
# coefficients de h = 3*f_q*g
[-680, -864, 966, -902, 147, 980, -1605, -1649, 1427, -1968, 1092, 471, 1056, 1718, -1617, 1503, 379, -1886, -427, 68, -1791, -58, -22, -1694, 25, -1662, 1353, 1612, 1152, -1552, 551, 321, -1439, -993, -301, 1958, -1493, 1664, -1933, -584, -1443, 419, 1567, 1242, -442, 669, -1196, 61, 1208, 1471, -796, -287, 235, 1671, 266, 786, 332, 612, 94, 1022, 1086, 27, 745, 1114, -378, 992, 89, -1786, -1420, -810, -1490, 8, -69, 342, 942, -1742, -1606, 23, -537, 681, 34, -1861, -650, 1883, -1200, -1721, 1258, 1999, 1534, 1565, 97, 1356, -645, -1825, 959, 847, 1967, 1390, -966, -1146, 569]
```

Encapsulation is quite simple:
- Two polynomials $m$ and $r$ are randomly generated;
- Encapsulation is $c = h\cdot r + m$;
- The symmetric key is derived from $r$ and $m$ (using a hash function).

```python
# Coefficients of r
[-1, -1, -1, 1, 1, -1, 0, 1, 0, 0, 1, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, 1, 0, 1, 0, 1, -1, 0, -1, -1, 0, -1, 0, -1, 1, 1, -1, 1, -1, -1, -1, 1, 1, 1, 0, 0, 0, 1, 0, 1, -1, -1, 0, 1, -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, -1, 0, 0, -1, 1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, -1, 0, 0, -1, -1, 0, -1]

# Coefficients of m
[1, 0, 1, 0, 1, -1, 0, -1, 1, 0, 0, 1, -1, 1, 1, -1, 1, 1, -1, 0, 0, 1, 1, -1, 1, 0, -1, 0, 1, 1, 1, 0, 0, 1, -1, -1, -1, -1, 1, 0, 0, 0, 1, 0, 1, -1, 0, -1, -1, 1, -1, 0, 0, 1, -1, -1, -1, 1, -1, -1, 1, -1, 0, 1, 1, -1, 0, 0, 1, 1, 1, 0, -1, 1, 0, 1, -1, -1, 1, -1, 1, 1, -1, 0, 0, 0, -1, 0, -1, 1, -1, 0, -1, 1, 1, 1, -1, 0, 1, 1, 0]

# Coefficients of c = h*r + m
[1172, -1000, -36, 448, 326, 1116, 1455, 766, -29, 550, 486, 622, -1800, 258, -1373, 526, -886, -1299, -1929, 668, 1265, -1028, 1780, 175, -1987, -1957, -1537, -1767, -833, 1742, 526, -1090, -1656, 992, -411, -1048, 41, -1472, 1431, 966, 448, -808, 59, -1195, 1734, 1930, 969, 1782, -9, 1398, 714, 1561, -1833, -665, 191, -379, 1201, -1135, -1136, -263, -897, -826, -1189, -490, -875, 232, 969, -1135, 387, 745, -1554, -1785, -744, -660, -1593, -305, 1568, 213, -1332, -1866, -1269, 1136, -1355, 622, 464, -12, 153, 76, -509, -37, 1276, -2035, -1307, -1317, -1580, 802, -1055, 472, -789, -604, -1262]
```

Decapsulation requires the private key polynomial $f$.
First, we compute $f\cdot c \bmod q$.

```python
# Coefficients of f*c
[13, 6, -10, -17, 4, 11, 0, 4, 20, 22, 27, 7, 10, 26, 3, -29, -43, -19, -22, -24, -18, 21, -19, -24, 37, 15, 6, 28, -5, -20, 7, 21, -14, 35, 2, -22, -8, -3, -7, 10, -2, -16, 19, -20, 2, 4, 13, -8, 3, -1, 25, 7, 14, 16, -11, 1, -19, -23, 30, 10, 4, 4, 25, -2, 20, 51, 36, 0, 38, 23, 25, -21, -8, -26, -15, -40, -26, 11, -40, -12, 21, -7, -21, 8, 53, 21, 21, -7, -2, 7, 14, 2, 24, 0, -12, 3, 28, 4, -5, 0, -24]
```
We see that it has small coefficients.
Indeed, if we develop $f\cdot c$, we have:
$$
\begin{align*}
f\cdot c & \equiv f\cdot (r\cdot h + m) & \mod q \\
         & \equiv f\cdot (r\cdot 3f_q\cdot g + m) & \mod q \\
         & \equiv 3r\cdot g + f\cdot m & \mod q
\end{align*}
$$

On the second line, we use that $f_q$ is the inverse of $f$ modulo $q$ so it disappears from the equation in the last line.

Now, since all these polynomials have small coefficients, if we chose all coefficients of $f\cdot c$ centered around 0, then it becomes an equality over the integers (and not modulo $q$ anymore).
It means we can get rid of the first term $3r\cdot g$ by reducing modulo $3$, and only $f\cdot m$ remains.

```python
# Coefficients of f*m mod 3
[1, 0, -1, 1, 1, -1, 0, 1, -1, 1, 0, 1, 1, -1, 0, 1, -1, -1, -1, 0, 0, 0, -1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, -1, -1, -1, 1, 0, -1, 1, 1, -1, 1, 1, -1, 1, 1, 1, 0, -1, 1, 1, -1, 1, 1, 1, -1, 1, 0, 1, 1, 1, 1, 1, -1, 0, 0, 0, -1, -1, 1, 0, 1, 1, 0, -1, 1, -1, -1, 0, 0, -1, 0, -1, -1, 0, 0, -1, 1, 1, -1, -1, 0, 0, 0, 0, 1, 1, 1, 0, 0]
```

To get back $m$, we multiply by $f_3$:
```python
# Coefficients of m = f_3*f*m modulo 3
[1, 0, 1, 0, 1, -1, 0, -1, 1, 0, 0, 1, -1, 1, 1, -1, 1, 1, -1, 0, 0, 1, 1, -1, 1, 0, -1, 0, 1, 1, 1, 0, 0, 1, -1, -1, -1, -1, 1, 0, 0, 0, 1, 0, 1, -1, 0, -1, -1, 1, -1, 0, 0, 1, -1, -1, -1, 1, -1, -1, 1, -1, 0, 1, 1, -1, 0, 0, 1, 1, 1, 0, -1, 1, 0, 1, -1, -1, 1, -1, 1, 1, -1, 0, 0, 0, -1, 0, -1, 1, -1, 0, -1, 1, 1, 1, -1, 0, 1, 1, 0]
```
As we can see it is the original value of $m$.

To complete decapsulation, the polynomial $r$ can be deduced with the equation $r = (c - m)\cdot h_q \bmod q$ where $h_q$ is the inverse of the public key $h$ modulo $q$.
Finally, the symmetric key is calculated from $r$ and $m$.


## Lattice attack on NTRU

A good resource for the lattice attack can be found on [latticehacks.cr.yp.to/ntru.html](https://latticehacks.cr.yp.to/ntru.html) with a script using SageMath.
We give an explanation below on how it works.

A first observation is that if we multiply the public key $h$ by the private key $f$, we have:
$$
f\cdot h \equiv 3g \mod q.
$$
Since $g$ has very small coefficients, then $f\cdot h$ has very small coefficients too.

A second observation is that polynomial multiplication can be rewritten as a matrix multiplication.
If $a_0, \ldots, a_{N-1}$ are coefficients of $f$, and $h_0, \ldots, h_{N-1}$ are coefficients of $h$, then $f\cdot h$ is:
$$
\begin{pmatrix} a_0 & a_1 & \cdots & a_{N-1}\end{pmatrix}\cdot
\begin{bmatrix}
h_0 & h_1 & \cdots & h_{N-1} \\
h_{N-1} & h_0 & \cdots & h_{N-2} \\
\vdots & & & \vdots \\
h_1 & h_2 & \cdots & h_0
\end{bmatrix}.
$$

Since we want to find a polynomial $f$ such that $f\cdot g$ has small coefficients, the problem can be restated as finding a short vector in a lattice generated by rows of a matrix containing the coefficients of the public key $h$.

Before constructing the matrix, we hide the factor $3$ in $h$ with $h' = h/3 \bmod q$, se we work with $h'$ instead of $h$.
Then the matrix is:
$$
M =\begin{bmatrix}
1 &   &        &   & h'_0     & h'_1    & \cdots & h'_{N-1} \\
  & 1 &        &   & h'_{N-1} & h'_0    & \cdots & h'_{N-2} \\
  &   & \ddots &   & \vdots   & \vdots  &        & \vdots \\
  &   &        & 1 & h'_1     & h'_2    & \cdots & h'_0 \\
  &   &        &   & q        &         &        &     \\
  &   &        &   &          & q       &        &     \\
  &   &        &   &          &         & \ddots &     \\
  &   &        &   &          &         &        & q    
\end{bmatrix}
$$

Rows of this matrix generate a lattice: any vector of this lattice is a linear combination of the rows (with integer coefficients).

If the polynomial $g$ (that appears in the generation of public key $h$) has coefficients $b_0, \ldots, b_{N-1}$, then the vector
$$
v = (a_0, \ldots, a_{N-1}, b_0, \ldots, b_{N-1})
$$
is a short vector of this lattice (all its coefficients are $-1$, $0$ or $1$).

> This vector is obtained with a multiplication on the left by vector $u = (a_0, \ldots, a_{N-1}, k_0, \ldots, k_{N-1})$ where polynomial $k$ is such that $f\cdot h' = g - kq$.
> 
> Indeed, with a simpler notation by denoting $u$ as $(f, k)$, we have:
> $$
> (f, k) \cdot M = (f, f\cdot h' + qk) = (f, g).
> $$

If we apply a lattice reduction algorithm such as LLL, we find the short vector $v$ that contains coefficients of the polynomial $f$.

However, with the parameters used in the challenge we do not get the polynomial $f$, but another one with small coefficients:
```python
# First row of reduced matrix with LLL
(-6, 0, 2, -3, 2, -7, 0, -7, 2, -7, -2, 1, 0, 1, 8, 13, -2, -6, -3, 4, -2, -8, 6, -2, -5, -7, -1, 5, -4, -4, -3, 1, -3, 0, 9, 4, -4, -13, 7, -7, 5, 9, -2, 7, -2, -2, 13, 2, 1, 1, -2, -9, 0, -3, 0, 7, -1, 3, 1, 0, -3, -13, 7, 0, 11, -9, 7, 6, 4, -2, 10, -7, -1, -2, -4, 0, 4, 2, -3, -5, 7, 4, 3, 0, -1, 1, -11, -6, 1, -2, 1, 5, -4, -8, -4, -9, -3, 3, -3, 14, -2, 0, -6, 10, -7, -3, -6, 0, -5, 0, 4, 11, 1, -17, -1, 9, -5, 6, -1, -2, 1, -2, 2, -1, -9, 2, 5, -8, -6, 0, -6, -7, -6, 1, 6, -11, 11, 7, 13, 6, -5, -6, 14, -5, -5, -6, 6, -10, -14, -2, -2, 4, -8, 6, 2, -9, 7, 0, 2, 11, -1, -12, 0, -1, -8, 3, -4, -3, 4, 1, 9, 8, -4, -1, -3, -6, -1, -6, 9, -8, -10, -1, -6, 5, -4, -1, 2, 2, -3, -6, 5, 3, 7, -15, 4, 12, -5, -11, 4, -10, 1, -10)
```

It will be used as an alternative private key $\varphi$.
We need its inverse $\varphi_3$ for decapsulation:

```python
# Coefficients of phi_3
[0, 0, 0, 1, 1, -1, 1, 1, 1, 1, 0, 1, 1, -1, -1, 0, -1, -1, -1, -1, 0, 1, -1, 0, -1, -1, -1, 1, -1, 0, -1, 0, 0, -1, 0, 0, 1, -1, 0, 1, 1, 0, -1, 0, 1, 1, 1, 1, 0, 1, 0, -1, -1, 1, 1, 1, 0, 1, 0, 1, 0, 0, -1, -1, 0, 1, 1, -1, 0, 1, -1, -1, -1, 1, 0, 1, 0, -1, 1, 0, 0, 0, 1, -1, 1, 0, -1, -1, 1, 1, 1, 0, 1, -1, 0, 0, 1, 1, 1, 1, 1]
```

We run the decapsulation process exactly as above.
First, we compute $\varphi\cdot c$.

```python
# Coefficients of phi*c
[176, -105, 31, -123, -178, 147, -89, -212, 125, -280, -247, -36, 374, 330, -64, 159, -56, 226, 149, 158, 264, -62, -157, -116, -63, 130, -172, 162, -33, 120, -204, 14, -34, 34, -139, -191, 219, -129, -227, 286, -35, 62, -61, 175, 296, 244, -9, 482, 120, -74, -67, -161, -78, 27, -315, -165, -64, -102, -136, 42, 259, 234, 125, -59, 213, 273, -82, -332, 231, -94, -215, 16, 86, -15, -74, -8, -24, 109, 11, 55, 127, 127, 82, -262, 71, -128, -69, 78, -212, 41, -285, -136, 192, 79, 78, 245, -39, 162, 120, -165, 67]
```

Those are not as small as with the real private key, but it should be enough.
Next step is to reduce modulo $3$, and multiply by $\varphi_3$.

```python
# Coefficients of phi_3*phi*m modulo 3
[1, 0, 1, 0, 1, -1, 0, -1, 1, 0, 0, 1, -1, 1, 1, -1, 1, 1, -1, 0, 0, 1, 1, -1, 1, 0, -1, 0, 1, 1, 1, 0, 0, 1, -1, -1, -1, -1, 1, 0, 0, 0, 1, 0, 1, -1, 0, -1, -1, 1, -1, 0, 0, 1, -1, -1, -1, 1, -1, -1, 1, -1, 0, 1, 1, -1, 0, 0, 1, 1, 1, 0, -1, 1, 0, 1, -1, -1, 1, -1, 1, 1, -1, 0, 0, 0, -1, 0, -1, 1, -1, 0, -1, 1, 1, 1, -1, 0, 1, 1, 0]
```
We find the polynomial $m$, so decapsulation can be completed (we derive the symmetric key, decrypt the file and get the flag)!


## Application to the challenge

Functions from the source code can be used to make the necessary calculations explained above.
We rely on `fpylll` for LLL, and `pycryptodome` for AES decryption.
The decrypted file is a PDF called `confidential.pdf` where the flag can be read from the first letter of each section in the table of contents such as:
```
ECW{NTRU_HOLHHCXZFUQOSHP}
```

Source code of the complete attack:
```python
#!/usr/bin/env python3

from fpylll import IntegerMatrix, LLL
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
from Crypto.Util.number import inverse
from source.ntru.ntrucore import *

# Parameters
N = 101
q = 4096
packq_size = 152

# Read public key (copy/paste from email exchanges), get polynomials h, h_3, and h_q
pubkey = bytes.fromhex('4618acf14758463ac414b61b3d3784d80411ec218091a9f79830434a82384f2900a4acd84623acd3ed55827ba6861f87550cb00add60993b177de2e9779618241e5d9f8a91b3ccc5f4f74f374418a3290a5ad78558ecea62b88b4c7f70d9f3a8828b4d723fcbe8f4e0537f17f0123995248a3384df339dc8d2bb5feb13672b38691627e99da9d41bdabd6bea64ef91179e98b1bbf9a22606')
h = unpack_q(pubkey, N, q)
inv3q = inverse(3, q)
h3 = h*inverse(3, q)
hq = invert_pol_q(h)

# Read encrypted file, get encapsulated key and ciphertext
data = open('confidential.enc', 'rb').read()
encaps_key = data[:packq_size]
c = unpack_q(encaps_key, N, q)
ciphertext = data[packq_size:]

# Building the matrix
M = IntegerMatrix(2*N, 2*N)
for i in range(N):
    M[i, i] = 1
    M[i + N, i + N] = q
    for j in range(N):
        M[j, ((i + j) % N) + N] = h3.coefs[i]

# Lattice reduction (wait a few seconds)
print('Lattice reduction in progress...')
Mreduced = LLL.reduction(M)
print('Lattice reduction finished.')

# Loop over rows of the reduced matrix until we find a polynomial
# that could be a good private key candidate
for i in range(Mreduced.nrows):
    row = list(Mreduced[i])
    f = PolyMod(N, q, row[:N])
    res = invert_pol_p_q(f, q)
    if res is not None:
        f3 = res[0]
        break

# The alternative private key has larger coefficients,
# so we cannot use directly the NTRUPrivKey class.
# So we rewrite the steps of decapsulation.

# We recover the polynomial m
a = (f*c).change_ring(3)
m = (a*f3).change_ring(q)

# We recover the random polynomial r
r = c - m
r = r*hq

# Symmetric key is hash of (r, m)
packed_r = pack_3(r)
packed_m = pack_3(m)
key = sha3_256(packed_r + packed_m).digest()

# Decryption
iv = ciphertext[:16]
ciphertext = ciphertext[16:]
cipher = AES.new(key, AES.MODE_CBC, iv)
plaintext = unpad(cipher.decrypt(ciphertext), 16)

# Write the decrypted PDF file
open('confidential.pdf', 'wb').write(plaintext)
```
