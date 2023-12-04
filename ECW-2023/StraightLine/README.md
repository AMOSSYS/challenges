# ECW 2023 - Crypto - StraightLine

This challenge was part of the pre-qualification phase of the ECW 2023 CTF.

This repository contains everything to run the challenge, so it can be played again.

## Overview

The content is as follows:
- Folder `genflag/`: flag generation;
- Folders `static/` and `templates/`: auxiliary folders for the challenge (for web pages);
- Folder `straightline/`: main folder of the challenge for the server;
- A `Dockerfile`, and TOML configuration files to run the challenge;

## Challenge description

This is the description given to players:
> You got access to a web site created by **Alice**, the artificial intelligence.
> Your goal is to find an administrator access in order to obtain information to stop **Alice**.
>
> To your disposal is the source of the cryptographic algorithm used for the authenfication.

File: [internals.rs](./straightline/src/internals.rs).

## Running the challenge

With Docker (from the current directory):
```
docker build -t straightline .
docker run -p 8000:8000 straightline
```

Without Docker, Rust and its toolchain are necessary:
```
cargo build --release
cargo run --release --bin genflag
cargo run --release --bin straightline
```

The binary `genflag` will generate the file `flag.txt` that contains the flag with the following form:
```
ECW{straight_line_XXXXXXXXXXXXXXXXXXXXXXXX}
```
There are 24 `X` that are replaced with random alphanumeric characters.

The binary `straightline` runs the server which can be accessed on the web page http://localhost:8000 (or http://127.0.0.1:8000).
