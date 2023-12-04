#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import string
import random

FLAG_FILE = '../flag.txt'

def gen_flag():
	chars = string.digits + string.ascii_letters
	rand_str = ''.join(random.choice(chars) for i in range(16))
	with open(FLAG_FILE, 'w') as f:
		f.write('ECW{random_key_' + rand_str + '}')

if __name__ == '__main__':
	gen_flag()
