from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import unpad
import json
import sys

def decrypt_flag(key: bytes, file_name: str):
    data = json.loads(open(file_name, "r").read())

    iv = bytes.fromhex(data['iv'])
    ciphertext = bytes.fromhex(data['encrypted'])

    cipher = AES.new(key, AES.MODE_CBC, iv=iv)
    secret_document = unpad(cipher.decrypt(ciphertext), 16)
    print(secret_document.decode('utf-8'))


if __name__ == "__main__":
    argc = len(sys.argv) - 1
    if argc != 2:
        print("Usage: python3 decrypt_flag AES_KEY /path/to/output.txt")
        exit(-1)
    
    key = bytes.fromhex(sys.argv[1])
    file_name = sys.argv[2]
    assert len(key) == 32

    decrypt_flag(key, file_name)
