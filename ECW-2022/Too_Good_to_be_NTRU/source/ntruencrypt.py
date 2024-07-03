#!/usr/bin/env python3

from ntru.ntrucore import *
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from os import urandom, path
import argparse, sys

def encrypt(params, pubkey, data):
    ntrupub = NTRUPubKey(params, pubkey)
    key, encaps_key = ntrupub.encaps()
    iv = urandom(16)
    cipher = AES.new(key, AES.MODE_CBC, iv)
    ciphertext = cipher.encrypt(pad(data, 16))
    return encaps_key, iv + ciphertext


def decrypt(params, privkey, data, encaps_key):
    ntrupriv = NTRUPrivKey(params, privkey)
    key = ntrupriv.decaps(encaps_key)
    iv = data[:16]
    ciphertext = data[16:]
    cipher = AES.new(key, AES.MODE_CBC, iv)
    plaintext = unpad(cipher.decrypt(ciphertext), 16)
    return plaintext


def print_instructions():
    print('Instructions [TODO]')

if __name__ == "__main__":
    try:
        parser = argparse.ArgumentParser(description='Encryption/decryption with NTRU')
        
        parser.add_argument('-e', action='store_true',
                            help='Encryption mode', required=False)

        parser.add_argument('-d', action='store_true',
                            help='Decryption mode', required=False)

        parser.add_argument('-k', action='store', type=str, dest='key',
                            help='Public or private key depending of the mode', required=True)

        parser.add_argument('-i', action='store', dest='fname_in', type=str,
                            help='File to encrypt/decrypt', required=True)
        
        parser.add_argument('-o', action='store', dest='fname_out', type=str,
                            help='Encrypted/decrypted file', required=True)

        args = parser.parse_args()

        if (args.e and args.d) or (not args.e and not args.d):
            print('You should select either encryption mode or decryption mode.')
            print_instructions()
            sys.exit()

        key = open(args.key, 'rb').read()
        data = open(args.fname_in, 'rb').read()

        if args.e:
            encaps_key, ciphertext = encrypt(params, key, data)
            open(args.fname_out, 'wb').write(encaps_key + ciphertext)
            print(f'File `{path.basename(args.fname_in)}` has been encrypted using the following public key:\nPublic key: {key.hex()}')
        else:
            encaps_key = data[:params['packq_size']]
            ciphertext = data[params['packq_size']:]
            plaintext = decrypt(params, key, ciphertext, encaps_key)
            open(args.fname_out, 'wb').write(plaintext)
            print(f'File `{args.fname_in}` has been successfully decrypted.')
            print(f'Decrypted file: `{args.fname_out}`')

    except:
        print_instructions()
