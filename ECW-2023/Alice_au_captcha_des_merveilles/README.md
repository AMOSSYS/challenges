# ECW 2023 - Alice au captcha des merveilles

This challenge was part of the final phase of the [ECW 2023 CTF](https://www.european-cyber-week.eu/challengectf).

**Alice au captcha des merveilles**, is a reverse challenge on **Windows events**. 
It does not contain anti-debug methods, but offuscation methods with on-the-fly execution of opcode.
There is also an integrity check performed on a function and the code loaded using a TLSCallback to prevent these functions from being patched.

## Overview

The content is as follows:
- Folder `bin/`: client and server binaries;
- Folder `src/`: source code;
- `Makefile`: to recompile the binaries.

## Challenge description

This is the description given to players:

> The artificial intelligence Alice has been at it again, creating a graphical captcha for Windows to distinguish bots from humans.
> However, it has decided that this captcha will only be accessible to bots and not to humans.
> No matter how much we explain that that's not the purpose of a captcha and that it wasn't what we'd asked for in the first place, she refused to understand.
> We decided to push you down the rabbit hole with her.
> Your objective: to successfully complete the captcha to prove to her that it doesn't even protect humans and that she's wrong.
> Validate the captcha and we'll reward you handsomely.
> You'll get the binary that generates the captcha and the server binary that validates it.

Also, players have access to the binaries:
- `./bin/client/alice_et_le_captcha_des_merveilles.exe`
- `./bin/server/server.exe`.

## Objective

The goal of the challenge is to recover the flag contained in the server in encrypted form.
To do this, the correct Windows event must be triggered 100 times within 3 seconds.
For each event, the client will send the server a key that will be used to decrypt a binary server resource.
server binary resource.
The key is correct if the event is triggered within three seconds, if the global timer of 5 min 30 is respected and if the event triggered is the correct one.
The server will respond with the decrypted resource (an image), the client writes the image to disk in a dedicated directory.
Once the 100 events have been generated, the client will calculate the hash of each image and calculate the hash of all these hashes. It will then send the global hash to the server.
The server will use this hash as a key to decrypt the flag. The server sends back the decrypted flag and the client displays it in a MessageBox.

The client will display the `Success MessageBox` even if the flag is wrong.
This isn't ideal, but it makes it easy to change the flag.


## Running the challenge

### Dependencies

To generate the challenge, you need: 
1. *mingw* toolchain: 
   - `x86_64-w64-mingw32-gcc`
   - `x86_64-w64-mingw32-g++`
   - `x86_64-w64-mingw32-windres`
2. A Python3 interpreter (if data regeneration)

**Optional**

- Bash to regenerate challenge images;
- For a resolution of the challenge `autohotkeys v2`.

### Build

#### Without modification 

To build the challenge in its current state, it is possible to run `$ make clean && make all` at the project root.

#### With modification

It is possible to change all exchanged data between client and servier (images et encryption keys).
To do this:
1. (OPTIONAL) Regenerate the images if necessary with a modification of script `./src/python_script/generate_img.sh`
2. Execute `$ python ./src/python_script/generate_event_data.py`
3. Execute `$ python ./src/python_script/generate_event_struct.py`
4. Execute `$ python ./src/python_script/encrypt_images.py`

If images were regenerated, you must regenerate the encrypted flag with the new key.
To do this:

5. Execute `$ python ./src/python_script/calc_key_flag.py`
6. Copy the value of `flag enc =` in the file `./src/server/server.h`:
```c 
...
#define FLAG_SIZE <flag size> 
...
```
7. Compile with `$ make clean && make all` at the project root.

#### Change flag only

To change the flag, you need to modify the file `./src/python_script/calc_key_flag.py`.
```python
...

    flag = b"<YOUR_NEW_FLAG>"
    flag_size = len(flag)

...
```
Then:
1. Executee `$ python ./src/python_script/calc_key_flag.py`
2. Copy the value of `flag enc =` and the field `flag size =` in the file `./src/server/server.h`:
```c 
...
#define FLAG_SIZE <flag size> 
#define ENCRYPTED_FLAG <flag enc>
...
```

## Usage

Tu run a captcha session:

1. In a `powershell`, run `.\server.exe`;
2. In another `powershell`, run `.\client.exe`.
