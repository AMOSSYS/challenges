#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from ctypes import *
from Cryptodome.Cipher import AES

SO_FILE = './generate_key.so'
FLAG_FILE = '../flag.txt'

with open(FLAG_FILE, 'r') as f:
    FLAG = f.read().encode()

def encrypt(plaintext, key):
    return AES.new(key, AES.MODE_CBC, b'FEDCBA9876543210').encrypt(plaintext)

if __name__ == '__main__':
    print("""Un message chiffré a été envoyé par l'IA ALICE à son centre de contrôle. Vous avez réussi à mettre la main sur certains extraits de code utilisés par ALICE pour chiffrer son message ainsi que sur le texte chiffré. Votre mission est de retrouver le message en clair.""")
    
    generate_256bits_encryption_key = CDLL(SO_FILE).generate_256bits_encryption_key
    generate_256bits_encryption_key.restype = c_char_p
    key = generate_256bits_encryption_key(b'Control_center').hex().encode()
    
    enc = encrypt(FLAG, key)
    print('Enc:', enc.hex())
    
    # Enc: f1af6a49ffea965613e35f4a85d4653dafb88682db267d76c8b78666c7a03c0e
