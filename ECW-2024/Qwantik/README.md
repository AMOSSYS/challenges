# Qwantik

## Description

We have intercepted a communication between two enemy submarines.
It appears to be a key exchange.
According to our intelligence, the enemy is using a hybrid key exchange mechanism combining RSA-1024 and Kyber-512, making it impossible to use our quantum computers...

Qwantik, our best spy, managed to get his hands on a development interface that performs the cryptographic operations of the submarines, but we do not have access to the source code or the private keys.
However, our national pride Qwantik is convinced that it is possible to decipher the session key with this tool...

Qwantik has also indicated a [reliable source](https://www.amossys.fr/insights/blog-technique/why-you-should-not-put-all-your-eggs-in-the-same-basket-part-3/). 

<img src="images/qwantik.jpg"/>

> The challenge can be accessed with `nc localhost 1337`.

## Content

- `deploy/`: files for the server
- `gen_flag.py`: script to generate a new flag
- `README.md`: this README

## Running the challenge locally

From this folder:
```
docker build -t qwantik deploy/
docker run --rm -d -p 1337:1337 qwantik
```

## Generate a new flag

The script `gen_flag.py` generates a new flag.
