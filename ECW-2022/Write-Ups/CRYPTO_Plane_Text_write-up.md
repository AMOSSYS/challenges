# CRYPTO - Plane Text (write-up)

This is a write-up for the challenge [Plane Text](../Plane_Text/).

## Overview of the challenge

The core principles of this challenge are the following:
- The goal is to exploit the usage of a predictable IV in CBC mode.
- An administrator has created a web server (that in fact also supports TCP only requests).
- A flag is served on `/flag.txt` ; an admin cookie of 10 characters is required to connect to the server.
- The requests are logged using AES-CBC with a hardcoded key.
- Inside a given TCP session, the IV is updated in a deterministic fashion, known to the user.
- The ciphered logs of the requests are shown to the user, providing an encryption oracle.
- A few logs of different pages visited by the admin are available, chosing the
  proper TCP requests allows to recover the admin cookie with a light
  bruteforce of approximately $10^4\ll 100^{10}$.

## Technical details

### Format of the logs

The logged requests are presented to the user under the form of a HTTP header
`<iv>: <ciphertext>` in the reply.
The administrator has performed a request to each page of the website as explained on the homepage:
```
As mentioned before, this site is currently in an unfinished state. It is however functional: I tested every existing page with the following commands:
curl.exe -H "cookie: **********" -H 'User-Agent:' -H 'Accept:' -H 'Host:' http://<ip>:<port>/planes
curl.exe -H "cookie: **********" -H 'User-Agent:' -H 'Accept:' -H 'Host:' http://<ip>:<port>/flag.txt
curl.exe -H "cookie: **********" -H 'User-Agent:' -H 'Accept:' -H 'Host:' http://<ip>:<port>/
curl.exe -H "cookie: **********" -H 'User-Agent:' -H 'Accept:' -H 'Host:' http://<ip>:<port>/randomgenerator.rs
curl.exe -H "cookie: **********" -H 'User-Agent:' -H 'Accept:' -H 'Host:' http://<ip>:<port>/uselessfile_lol.html
curl.exe -H "cookie: **********" -H 'User-Agent:' -H 'Accept:' -H 'Host:' http://<ip>:<port>/logs.txt
I replaced the characters of my cookie for obvious security reasons. 
```
The precise form of the HTTP request issued by these commands is important:
```sh
curl -v -H "cookie: **********" -H 'User-Agent:' -H 'Accept:' -H 'Host:' http://localhost/logs.txt                   
*   Trying 127.0.0.1:80...
* Connected to localhost (127.0.0.1) port 80 (#0)
> GET /logs.txt HTTP/1.1
> cookie: **********
> 
* Mark bundle as not supporting multiuse
< HTTP/1.1 200
< content-length: 850
< 0b5000b73a53f0916c93c68f4b9b6ba8: 5a751dae88750003a78a4bb253efe7d6cc2e12689fe65aef6e8028b0043b4c09e6f26c2c0f614485ff133038d63d5134
< 
[...]
```

The corresponding logs are available at the address `/logs.txt`:
```
af5570f5a1810b7af78caf4bc70a660f: 39a04041fedff232a1619ccef666a45d1f1141caa334ad734e5b63d596501a8c187115d1748e50119378c3e7c9984613
cd2662154e6d76b2b2b92e70c0cac3cc: 56196689d1ccc59a6a56b16e23941c6c1ebab32d530577cd0502aac11d0706a8b3ea4c61aff50b87ea69071defe4e77f
cd04a4754498e06db5a13c5f371f1f04: 8069a7812b66cbb6abed804a93203835a88ef88dc4f6b88d80fc8200ad5a895fad215a6f0fa9c8eed1c9e0d6ec9c10bb
d5688a52d55a02ec4aea5ec1eadfffe1: f62fa1cc918b38338b9d3d038e8140efb459b8e7be47d70d869d972e85949092ee152bde458e10bc812b6c5ee92d13d1d921fee4ab76442026bbecc01260f5a7
8005f02d43fa06e7d0585fb64c961d57: e1021a0642ed805549a755ee63171e4d1583198c5073efd4a10890d2afb0cfc8e524e12698d74879dc6982691bb9d5423705dbf2162c52e0b9af5c9863c17e88
5dee4dd60ff8d0ba9900fe91e90e0dcf: b4290f20356448daf385655892333c4aac63e32f692b43d61554c0774535872c151bcae84189ecc387e3582b3680ceba
```

### Updating the IV

The IV is updated using a function available at the address `/randomgenerator.rs`:
```rust
extern crate sha2;
use sha2::{Digest, Sha256};

// I do not trust random generators made by others.
// I know for sure that the output of a SHA-256 
// hash is indistinguishible from random bytes.
// Thus, inside a given socket, just renew the IV 
// at each request with this function when logging.
pub fn update_iv(iv: &[u8]) -> [u8; IV_LENGTH] {
    let mut hasher = Sha256::new();
    hasher.update(iv);
    hasher.finalize()[..IV_LENGTH]
        .try_into()
        .expect("IV Length should be correct")
}
```

### Principles of the attack

When a message made of $n$ $16$ bytes blocks $M_1, \dots, M_n$ is ciphered
using AES-CBC with the unkown hardcoded key $k$ and initialization vector
$\mathrm{IV}$, the resulting $16$ bytes blocks
$C_1, \dots, C_n$ are given by 
```math
\begin{align}
C_1 &= \mathrm{AES_k}(\mathrm{IV}\oplus M_1), \\
    & \vdots \\
C_n &= \mathrm{AES_k}(C_{n-1}\oplus M_n).
\end{align}
```

Since the attacker can predict the initial vector $\mathrm{IV_{predicted}}$ used
for his next request, and knows the vector $\mathrm{IV_{admin}}$ used when the 
admin performed a request, he can make a guess on the cookie and send the (TCP)
request $\mathrm{IV_{predicted}} \oplus \mathrm{IV_{admin}} \oplus \mathrm{http\_request}$,
where $\mathrm{http\_request}$ is an HTTP request with a guess on the cookie:
`GET <page> HTTP/1.1 cookie: <cookie guess>}` 

Thus, the terms $\mathrm{IV_{predicted}}$ cancel out and the first block is
```math
C_1 = \mathrm{AES_k}(\mathrm{IV_{admin}}\oplus \mathrm{http_{request}}).
```
Hence, the first block of $C_1$ is equal to the first block obtained by the
administrator if, and only if the first $16$ bytes of $\mathrm{http\_request}$ 
coincide with the first $16$ bytes of the administrator. Similarly, the first
two blocks are equal to those of the administrator if, and only if the first
$32$ bytes of the request were the same, and so on.

The bruteforce is then reduced: by using the length of the different filenames,
the bytes of the admin cookie can be bruteforce two by two. The alignment of
the requests over $16$ bytes is indeed reproduced below:
```
0123456789abcdef
GET /planes HTTP
/1.1\rcookie: **

0123456789abcdef
GET /uselessfile
_lol.html HTTP/1
.1\rcookie: **##

0123456789abcdef
GET /randomgener
ator.rs HTTP/1.1
\rcookie: **##$$

0123456789abcdef
GET / HTTP/1.1\r
cookie: **##$$@@

0123456789abcdef
GET /flag.txt HT
TP/1.1\rcookie:
**##$$@@++\r\r
```
(Note: `\r` denotes here `\r\n` in order to be in adequation with the byte length)

Performing the requests in this order significantly reduces the amount of
bruteforce needed and makes it possible to do over the network. If the $100$
alphanumeric characters are used, at most $5\times10^4$ requests are required.

## Step by step solution

In order to perform the attack in python, a few libraries can be used:
```python
import hashlib
import socket
import string
from collections.abc import Generator
from typing import Dict, Optional, Tuple, Callable
```
We can first define a few helpful functions and constants:
```python
SERVER_ADDRESS = ("localhost", 80)
ALPHABET = string.printable


class File:
    A = b"planes"
    FLAG = b"flag.txt"
    WELCOME = b""
    LOGS = b"logs.txt"
    USELESS = b"uselessfile_lol.html"
    GENERATOR = b"randomgenerator.rs"


def request(filename: bytes, cookie: bytes) -> bytes:
    """Builds a TCP GET request to @filename with cookie @cookie."""
    end = b"\r\n"
    return b"GET /" + filename + b" HTTP/1.1" + end + b"cookie: " + cookie + 2 * end


def update_iv(iv: bytes) -> bytes:
    """Predict the next value of the IV."""
    return hashlib.sha256(iv).digest()[:16]


def xor(bb1: bytes, bb2: bytes) -> bytes:
    """
    Xor two byte strings of the same length.

    Raise Exception if the length are different.
    """
    if len(bb1) != len(bb2):
        raise Exception
    return b"".join([(b1 ^ b2).to_bytes(1, "big") for b1, b2 in zip(bb1, bb2)])


def send_and_receive(s: socket.socket, payload: bytes) -> bytes:
    """Send a payload in the given socket and get up to 1024 bytes of reply."""
    s.sendall(payload)
    return s.recv(1024)
```
It is then possible to test connexion to the server, and find the offset of the
IV and ciphertext blocks in the replies:
```ipython
In [13]: r = request(b"planes", b"test_cookie")

In [14]: with socket.socket() as s:
    ...:     s.connect(SERVER_ADDRESS)
    ...:     reply = send_and_receive(s, r)
    ...:     print(reply)
b'HTTP/1.1 200\r\ncontent-length: 76\r\na3eb8db89fc5123ccfd49585059f292b: a4d45103ecfc891ca48bb18ca6bf03481f807862135644e0cc4b3a50de9d3f0e5bfc9ead62c50f1f685c9a59c0168057\r\n\r\nPlanes are really wonderful.\nTODO: populate this file with planes pictures.\n'
```
This leads to the following definitions:
```python
def recover_iv(server_reply: bytes) -> bytes:
    """Get the IV returned by the server"""
    return bytes.fromhex(str(server_reply[34:66])[2:-1])


def recover_firstblock(server_reply: bytes) -> bytes:
    """Recover the first block of the ciphertext returned by the server."""
    return server_reply[68:100]


def recover_secondblock(server_reply: bytes) -> bytes:
    """Recover the second block of the ciphertext returned by the server."""
    return server_reply[100:132]


def recover_thirdblock(server_reply: bytes) -> bytes:
    """Recover the third block of the ciphertext returned by the server."""
    return server_reply[132:164]


def block_recovery(block_number: int) -> Callable[[bytes], bytes]:
    """
    Return the proper function to get the wanted block.

    For the attack, @block_number should be 2 or 3. Raise Exception
    otherwise.
    """
    if block_number not in [2, 3]:
        raise Exception
    if block_number == 2:
        return recover_secondblock
    return recover_thirdblock
```


Similarly, parsing the file located at `/logs.txt` allow to recover the
IVs and relevent ciphertext blocks of the admin requests:
```python
def get_ivs_and_blocks() -> Tuple[Dict[str, bytes], Dict[str, bytes]]:
    """
    Parse the /logs.txt file to extract IVs and ciphertext blocks.

    Return two dictionaries, the first one mapping filenames to
    the corresponding admin IV, the second one mapping the same filenames
    to the relevent block of ciphertext (depending on the length of the
    filename)
    """
    with socket.socket() as s:
        s.connect(SERVER_ADDRESS)
        s.sendall(request(File.LOGS, b""))
        received = s.recv(1024)
        admin_ivs = {
            file: bytes.fromhex(iv.decode("utf-8"))
            for file, iv in {
                "avions": received[169:201],
                "flag": received[300:332],
                "/": received[431:463],
                "generator": received[562:594],
                "useless": received[725:757],
            }.items()
        }

        admin_pertinent_blocks = {
            "avions": received[235:267],
            "flag": received[398:430],
            "/": received[497:529],
            "generator": received[660:692],
            "useless": received[823:855],
        }
        return (admin_ivs, admin_pertinent_blocks)
```

This gives the expected result:
```ipython
In [3]: get_ivs_and_blocks()
Out[3]: 
({'avions': b'\xafUp\xf5\xa1\x81\x0bz\xf7\x8c\xafK\xc7\nf\x0f',
  'flag': b'\xcd&b\x15Nmv\xb2\xb2\xb9.p\xc0\xca\xc3\xcc',
  '/': b'\xcd\x04\xa4uD\x98\xe0m\xb5\xa1<_7\x1f\x1f\x04',
  'generator': b'\xd5h\x8aR\xd5Z\x02\xecJ\xea^\xc1\xea\xdf\xff\xe1',
  'useless': b'\x80\x05\xf0-C\xfa\x06\xe7\xd0X_\xb6L\x96\x1dW'},
 {'avions': b'aee4cd4c0eb213d20dbbbb759a79baed',
  'flag': b'2c29cbdacab30d880bff5f562129969e',
  '/': b'89e5cc1c06499c7a2692653e9b7f245d',
  'generator': b'91dc35c521fb9a0e59670c6c42cef11d',
  'useless': b'9cd964741a266fef95824ae3c32357be'})
```

The core function of the attack can then be implemented:

```python
def bruteforce_two_chars() -> Generator[bytes, None, None]:
    """
    Return a generator of all combinations "ab" for a, b printable characters.
    """
    return (bytes(a + b, "utf-8") for a in ALPHABET for b in ALPHABET)


def attack(
    file: bytes,
    admin_iv: bytes,
    admin_block: str,
    cookie_start: bytes,
    block_number: int,
) -> Optional[bytes]:
    """
    Perform the attack.

    Given a URL path @file, the corresponding @admin_iv and @admin_block that
    serves as a reference, the bytes of the cookie @cookie_start already
    recovered, and the block_number relevent for the test, return the good
    pair of chars to append to the cookie if any, and None otherwise (should
    not happen if used properly).
    """
    with socket.socket() as s:
        s.connect(SERVER_ADDRESS)
        reply = send_and_receive(s, b"\r\n")
        iv = recover_iv(reply)
        for test in bruteforce_two_chars():
            iv = update_iv(iv)
            base_payload = request(file, cookie_start + test)
            trick = xor(admin_iv, iv)
            payload = xor(base_payload[:16], trick) + base_payload[16:]
            reply = send_and_receive(s, payload)
            block = block_recovery(block_number)
            if block(reply) == admin_block:
                return test
        return None
```

According to the order of the URLs to be visited established earlier,
the `attack` function can now be successively called with the proper
block numbers in order to get the full cookie:

```python
def main():
    """Perform the full attack."""
    admin_ivs, admin_pertinent_blocks = get_ivs_and_blocks()

    cookie_start = attack(
        File.A, admin_ivs["avions"], admin_pertinent_blocks["avions"], b"", 2
    )
    if cookie_start is None:
        print("[!] Error: First two bytes of the cookie not found")
        return
    print(f"[*] Found first two characters: {cookie_start}")

    next_bytes = attack(
        File.USELESS,
        admin_ivs["useless"],
        admin_pertinent_blocks["useless"],
        cookie_start,
        3,
    )
    if next_bytes is None:
        print("[!] Error: bytes 2 and 3 of the cookie not found")
        return

    cookie_start += next_bytes
    print(f"[*] Found first four characters: {cookie_start}")

    next_bytes = attack(
        File.GENERATOR,
        admin_ivs["generator"],
        admin_pertinent_blocks["generator"],
        cookie_start,
        3,
    )
    if next_bytes is None:
        print("[!] Error: Bytes 4 and 5 of the cookie not found")
        return

    cookie_start += next_bytes
    print(f"[*] Found first six characters: {cookie_start}")

    next_bytes = attack(
        File.WELCOME,
        admin_ivs["/"],
        admin_pertinent_blocks["/"],
        cookie_start,
        2,
    )
    if next_bytes is None:
        print("[!] Error: Bytes 6 and 7 of the cookie not found")
        return

    cookie_start += next_bytes
    print(f"[*] Found first height characters: {cookie_start}")

    next_bytes = attack(
        File.FLAG,
        admin_ivs["flag"],
        admin_pertinent_blocks["flag"],
        cookie_start,
        3,
    )
    if next_bytes is None:
        print("[!] Error: Bytes 8 and 9 of the cookie not found")
        return
    print(f"[*] Found cookie: {cookie_start}")

    cookie_start += next_bytes

    # The cookie is now fully recovered, we can get the flag.
    with socket.socket() as s:
        s.connect(SERVER_ADDRESS)
        flag_reply = send_and_receive(s, request(File.FLAG, cookie_start))
        flag_start = flag_reply.find(b'ECW{')
        flag = flag_reply[flag_start:]
        flag = str(flag)[2:-1]
        print(f"Flag: {flag}")
```
This gives the expected result:
```ipython
In [5]: main()
[*] Found first two characters: b'tN'
[*] Found first four characters: b'tNvc'
[*] Found first six characters: b'tNvcbl'
[*] Found first height characters: b'tNvcbltU'
[*] Found cookie: b'tNvcbltUdP'
Flag: ECW{iLikePlanes_WXWkm3fWXi}
```
It runs almost instantaneously on localhost. On the remote challenge, it took around 3 minutes to complete. A more naive bruteforce, for instance by groups of four bytes, would have been around $10 000$ times slower and thus almost impractical.
