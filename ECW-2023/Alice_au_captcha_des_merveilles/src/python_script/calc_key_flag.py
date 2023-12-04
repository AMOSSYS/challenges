import hashlib 
from pathlib import Path
from binascii import hexlify

class RC4:
    """
    This class implements the RC4 streaming cipher.
    """

    def __init__(self, key, streaming=True):
        assert(isinstance(key, (bytes, bytearray)))

        # key scheduling
        S = list(range(0x100))
        j = 0
        for i in range(0x100):
            j = (S[i] + key[i % len(key)] + j) & 0xff
            S[i], S[j] = S[j], S[i]
        self.S = S

        # in streaming mode, we retain the keystream state between crypt()
        # invocations
        if streaming:
            self.keystream = self._keystream_generator()
        else:
            self.keystream = None

    def crypt(self, data):
        """
        Encrypts/decrypts data (It's the same thing!)
        """
        assert(isinstance(data, (bytes, bytearray)))
        keystream = self.keystream or self._keystream_generator()
        return bytes([a ^ b for a, b in zip(data, keystream)])

    def _keystream_generator(self):
        """
        Generator that returns the bytes of keystream
        """
        S = self.S.copy()
        x = y = 0
        while True:
            x = (x + 1) & 0xff
            y = (S[x] + y) & 0xff
            S[x], S[y] = S[y], S[x]
            i = (S[x] + S[y]) & 0xff
            yield S[i]


def calculate_sha256(data):
    sha256_hash = hashlib.sha256(data)
    return sha256_hash.hexdigest()


if __name__ == "__main__":
    file_hashes = []
    key = ""

    flag = b"ECW{3v1L_C4ptch@_4uT0_M@5t3r}"
    flag_size = len(flag)

    server_dir = Path(__file__).parent.parent.absolute() / "server"
    images_dir = server_dir / "images" 
    for idx in range(100): 
        with open(str(images_dir / f"img_{idx}.jpeg"), "rb") as current_image:
            file_data = current_image.read()
            file_hashes.append(calculate_sha256(file_data))
    val = bytes.fromhex(str("".join(file_hashes).encode())[2:][:-1])
    print(val)
    key = hashlib.sha256(val).hexdigest()

    print(f"key = {bytes.fromhex(key)}")

    cipher = RC4(bytes.fromhex(key), streaming=True)
    encrypted_flag = cipher.crypt(flag)
    hex_enc_flag = str(hexlify(encrypted_flag))[2:][:-1]
    print(f"hex enc = {hex_enc_flag}")
    separator = "\\x"
    flag_enc_to_print = ""
    for i in range(0, flag_size * 2, 2):
        flag_enc_to_print += separator + hex_enc_flag[i:i+2] 

    print(f'flag enc = "{flag_enc_to_print}"')
    print(f"flag size = {flag_size}")
