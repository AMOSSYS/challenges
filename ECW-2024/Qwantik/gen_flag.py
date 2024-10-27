#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import string
import random

FLAG_FILE = 'deploy/flag.txt'

def gen_flag():
    chars = string.digits + string.ascii_letters
    rand_str = ''.join(random.choice(chars) for i in range(16))
    with open(FLAG_FILE, 'w') as f:
        f.write('ECW{hy6r1d1z4t10n_1s_th3_n3w_tr3nd_' + rand_str + '}')

if __name__ == '__main__':
    gen_flag()
