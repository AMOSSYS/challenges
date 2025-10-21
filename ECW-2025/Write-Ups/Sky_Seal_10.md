# Sky Seal 10 - Write-up

This is a write-up for the challenge [Sky Seal 10](../Sky_Seal_10/).

> The year is 2039. Humanity has grown heavily reliant on an ultra-secure satellite network known as SkyNet Quantum Link.
> But disaster strikes: a critical node, Satellite #10 -- Sky Seal, suddenly goes dark.
> 
> A cybersecurity analyst team uncovers that an automatic lockdown has been triggered.
> To restore access, a valid authentication key must be regenerated.
> 
> Your objective is to analyze the binary file, understand the internal logic, and reconstruct the legitimate key that will restore access to the satellite's data systems.
> 
> Time is critical.
> The longer Sky Seal 10 stays offline, the greater the risk of downstream system collapse.

A binary is given and a key is asked when executed.

```
$ ./sky_seal_10 
Welcome to the sky_seal_10 challenge!

Key:
```

The challenger has to find the correct key.
It is now time to reverse the binary using IDA Pro or Ghidra to understand the logic.

Before to start, the binary validates the input type.
A 16-bytes string composed of hexadecimal chars is required.

```rust
fn validate_input(input: &str) {
    let re: Regex = Regex::new(r"^[0-9a-fA-F]{32}$").unwrap();
    if !re.is_match(input) {
        println!("Key must be 32 hex chars!");
        println!("Exiting.");
        process::exit(0);
    }
}
```

The entered key is then splitted into two parts.
Each part is checked in a dedicated function.

In the first function, the first part of the key is checked.

```rust
fn check_key_part_0(internal_state: &mut InternalState, key_part_0: &[u8]) -> i32 {
    let mut tmp: [u8; 8] = [0xe4, 0x7f, 0x85, 0xa2, 0x5c, 0x31, 0x56, 0x19];
    let key: [u8; 4] = [0x89, 0x9F, 0x22, 0x76];
    for i in 4..8 {
        tmp[i] ^= key[i % 4];
        internal_state.tick += 1;
    }
    for i in 0..8 {
        if key_part_0[i] != tmp[i] {
            return -1;
        }
        internal_state.tick += 2;
    }
    0
}
```

An array (*tmp* on the picture) is xored with an internal key array (*key*) modulo 4.
Only the second part of the array is xored, not the first part.
Also, a tick value is incremented from 0 to 4.
After that, the first part of the entered key is checked byte by byte against the obtained value.
During this phase, the tick value is incremented by 2 each round.
So, at the end, if the entered key is good, the tick value is 20.

In the second function, the second part of the key is checked.

```rust
fn check_key_part_1(tick: u8, key_part_1: &[u8]) -> i32 {
    let mut tmp: [u8; 8] = [0; 8];
    let tmp_final: [u8; 8] = [0xea, 0xc6, 0xd5, 0x9a, 0x08, 0x3f, 0x27, 0xea];
    for i in 0..8 {
        tmp[i] = key_part_1[i].rotate_left(tick.into());
        tmp[i] ^= tick - i as u8;
        tmp[i] = tmp[i].rotate_right(tick.into());
    }
    for i in 0..8 {
        if tmp[i] != tmp_final[i] {
            return -1;
        }
    }
    0
}
```

For each byte of the key, the following operations are performed:
- rotation left by *tick* value
- the result is xored with the *tick value - i* (i being the iteration number from 0 to 7)
- rotation right by *tick value* is applied on the result

At the end, the obtained array is compared byte by byte with the *tmp_final* array.
By following these instructions, the following Python3 script can be implemented in order to solve the challenge.

```py
#!/usr/bin/python3
# -*- coding: utf-8 -*-

def ror(value, nb):
	nb %= 8
	return value >> nb | (value << (8 - nb)) & 0xff

def rol(value, nb):
	nb %= 8
	return ((value << nb) & 0xff) | (value >> (8 - nb))

def compute_key_part_0(tick):
	tmp = [0xe4, 0x7f, 0x85, 0xa2, 0x5c, 0x31, 0x56, 0x19]
	key = [0x89, 0x9F, 0x22, 0x76]
	for i in range(4, 8):
		tmp[i] ^= key[i % 4]
		tick += 1
	for i in range(8):
		tick += 2
	return tick, tmp

def compute_key_part_1(tick):
	tmp_final = [0xea, 0xc6, 0xd5, 0x9a, 0x08, 0x3f, 0x27, 0xea]
	tmp = [0] * 8
	for i in range(8):
		tmp[i] = ror(tmp_final[i], tick)
		tmp[i] ^= tick - i
		tmp[i] = rol(tmp[i], tick)
	return 0, tmp

if __name__ == '__main__':
	tick, key_part_0 = compute_key_part_0(0)
	print('key_part_0:', ''.join('{:02x}'.format(x) for x in key_part_0))
	
	tick, key_part_1 = compute_key_part_1(tick)
	print('key_part_1:', ''.join('{:02x}'.format(x) for x in key_part_1))
	
	key = key_part_0 + key_part_1
	print('key:', ''.join('{:02x}'.format(x) for x in key))
```



If the two functions succeed, the encrypted flag is decrypted and printed.

```
$ python3 solve.py 
key_part_0: e47f85a2d5ae746f
key_part_1: abf7f48b09cfc73a
key: e47f85a2d5ae746fabf7f48b09cfc73a
```


To finish, the flag is obtained by executing the binary with the good
key.

```
$ ./sky_seal_10 
Welcome to the sky_seal_10 challenge!

Key: e47f85a2d5ae746fabf7f48b09cfc73a

Good key, congratulations!
You can validate the challenge using the following flag: ECW{UNL0CK3D_XXXXXXXXXXXXXXXXXX}
```
