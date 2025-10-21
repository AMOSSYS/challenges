# Learning from error

## Description

A communication has been intercepted containing encrypted data by the DOSEAL initiative (Defensive Operation for Satellite Encrypted Analysis Localization).
It seems to be related to a KEM public key that our experts confirmed to be a post-quantum mechanism, and the corresponding public key has been found.
The secret key is nowhere to be found.

However, our informant has provided the DOSEAL key generation source code and a memory dump of one generation.

Can the secret document of the DOSEAL initiative be retrieved?

## Content

- `files/`: filse for challengers
- `solution/`: files for the solution of the challenge

## Solution

Two implementations are provided through the following scripts:
- [solve.py](./solution/solve.py): Python script using Sagemath (might run a few minutes)
- [solve.sh](./solution/solve.sh): C program and Python script (should run in one second)

The write-up is available [here](../Write-Ups/Learning_with_error.md).
