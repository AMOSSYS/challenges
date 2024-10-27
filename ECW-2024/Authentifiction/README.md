# Authentifiction

## Description

An authentication server's API has been found.
The protocol seems unknown, but the cryptography was reversed:
it is based on well known primitives.

A client interface has been provided to facilitate the authentication.
The account for user `SugarColts` has a weak password: `101lol`.
It gives access to an interesting file.
Unfortunatly, all other users seem to have strong passwords...

> The authentication server can be accessed with `nc localhost 5000`.
> The file `client.py` can be also be used.

## Content

- `deploy/`: files for the server
- `files/`: files for challengers
- `gen_flag.py`: script to generate a new flag
- `README.md`: this README

## Running the challenge locally

From this folder:
```
docker build -t authentifiction deploy/
docker run --rm -d -p5000:5000 authentifiction
```

## Generate a new flag

The script `gen_flag.py` generates a new flag.
