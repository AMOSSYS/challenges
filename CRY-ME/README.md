# CRY-ME: single signature attack

[ANSSI](https://cyber.gouv.fr/) and [CryptoExperts](https://www.cryptoexperts.com/) collaborated to develop a messaging application called CRY.ME (Cryptographic Messaging) based on the *Matrix* protocol for pedagogic purpose.

The original goal was to challenge the French ITSEFs (Amossys being one of them) on their cryptographic skills.
The application source code was publicly released in 2023 and can be found on the [ANSSI Github repository](https://github.com/anssi-fr/cry-me) with all documentation, including the vulnerabilities introduced.

One of the vulnerabilities concerns the signature generation based on the Schnorr signature.
We present here a Python script to run a lattice attack with a single signature to retrieve the private key.

The whole context and write-up is given on the [Amossys blog](https://www.amossys.fr/insights/blog-technique/cry-me-private-key-recovery-with-a-single-signature/).

## Content

Two files are given:
- `break_schnorr.py`: the main file;
- `ec.py`: custom implementation of elliptic curves in Weierstrass form.

## Usage

Given a public key and signature in base64, just run the command:

```
python3 break_schnorr.py <Public key in base64> <Signature in base64>
```

The private key should be found instantly.
Example:

```bash
$ python3 break_schnorr.py IzGmYC1fTu3AEtHWR2oQMP0YiXIUYl1AWBXZ/6eSFYZCdWtgP7CUYuGVnOLPDOFT6uEb8vE5eXxYrc1RVgZEOg== r5ysPYgq//ztVNhMsiXoG3L6gDVwm0eGYvhIB8u8N4wGHP4firfbbMGJM7bxtQ4s94HqlCkcMIsXf8i91sGRnw==
Public key: (0x2331a6602d5f4eedc012d1d6476a1030fd18897214625d405815d9ffa7921586, 0x42756b603fb09462e1959ce2cf0ce153eae11bf2f139797c58adcd515606443a)
Private key: 0xdfd421d217a6fd0db1629b9e1adade3
```