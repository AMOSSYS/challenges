# ECW 2023 - Crypto - Random key

This challenge was part of the pre-qualification phase of the [ECW 2023 CTF](https://www.european-cyber-week.eu/challengectf).

This repository contains everything to run the challenge, so it can be played again.

## Overview

The content is as follows:
- Folder `genflag/`: flag generation;
- Folder `random_key/`: source code of the challenge;

## Challenge description

An encrypted message sent by ALICE to its control center has been intercepted.

You have managed to get your hands on some code snippets used by ALICE to encrypt its message, as well as the ciphertext.
Your mission is to recover the message in clear text.

Enc: `f1af6a49ffea965613e35f4a85d4653dafb88682db267d76c8b78666c7a03c0e`.

Files:
- [challenge.py](./random_key/challenge.py)
- [generate_key.c](./random_key/generate_key.c)

## Running the challenge

To generate a new flag:
```
python3 genflag.py
```

To generate the encrypted flag:
```
python3 challenge.py
```

