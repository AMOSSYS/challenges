#!/bin/bash

FILES=../files

make -C find_shared_key
KEY=$(./find_shared_key/find_key $FILES/doseal_tepley.pub $FILES/doseal_ciphertext.bin $FILES/doseal_dump.bin 2080)
python3 decrypt_flag.py $KEY $FILES/output.txt | grep -oP "ECW\{[^}]*}"
