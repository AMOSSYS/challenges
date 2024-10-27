#!/usr/bin/env python3

import string
import random
import json

FLAG_FILE = "deploy/flag.txt"

def gen_flag():
    chars = string.digits + string.ascii_letters
    rand_str = ''.join(random.choice(chars) for i in range(24))
    flag = "ECW{authentifiction_" + rand_str + "}"
    open(FLAG_FILE, "w").write(flag)

if __name__ == '__main__':
    gen_flag()
