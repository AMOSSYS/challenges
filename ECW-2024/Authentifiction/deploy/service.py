from database import USER_KEYS
import json
import os
import sys

# --- Cryptography ---

from Cryptodome.Protocol import KDF
from Cryptodome.Cipher import AES

def zero_pad(data : bytes):
    pad_length = (-len(data) % 16)
    return data + b'\x00'*pad_length, pad_length

class CipherCTS:
    def __init__(self, key: bytes):
        assert len(key) == 32
        self.key = key
    
    def encrypt(self, data : bytes):
        assert len(data) > 16
        iv = os.urandom(16)
        cipher = AES.new(self.key, mode=AES.MODE_CBC, iv=iv)
        padded_data, pad_length = zero_pad(data)
        ciphertext = cipher.encrypt(padded_data)
        last_block = ciphertext[-16:]
        penultimate_block = ciphertext[-32:-16]
        ciphertext = ciphertext[:-32] + last_block + penultimate_block[:16-pad_length]
        return iv + ciphertext

    def decrypt(self, data : bytes):
        assert len(data) > 32
        iv, data = data[:16], data[16:]
        cipher = AES.new(self.key, mode=AES.MODE_ECB)
        l = 16 - (-len(data) % 16)
        penultimate_block = data[-(l + 16):-l]
        tmp = cipher.decrypt(penultimate_block)
        last_block = data[-l:] + tmp[l:]
        ciphertext = data[:-(l + 16)] + last_block + penultimate_block
        cipher = AES.new(self.key, mode=AES.MODE_CBC, iv=iv)
        plaintext = cipher.decrypt(ciphertext)
        return plaintext[:len(data)]


# --- Service ---

CHALLENGE_LENGTH = 16

def get_user_key(name: str):
    if name not in USER_KEYS.keys():
        return None
    userdata = USER_KEYS[name]
    salt = bytes.fromhex(userdata["salt"])
    key = bytes.fromhex(userdata["key"])
    return salt, key

class ServerAuth:
    def __init__(self):
        self.auth_initialized = False

    def auth_init_response(self, user_request: dict):
        # Step 1: get user name and client challenge from request
        try:
            user_name = user_request["user_name"]
            client_challenge = bytes.fromhex(user_request["client_challenge"])
            assert len(client_challenge) == CHALLENGE_LENGTH
        except:
            return {
                "error": "missing or bad format for expected fields 'user_name' (string), and 'client_challenge' (16 bytes in hexadecimal representation)"
            }

        # Step 2: retrieve secret key derived from user's password
        #         (password is not stored by the server)
        try:
            salt, key = get_user_key(user_name)
        except:
            return {"error": "This user does not exist"}

        # Step 3: calculate server proof
        self.cipher = CipherCTS(key)
        payload = client_challenge + user_name.encode() + b"server"
        server_proof = self.cipher.encrypt(
            client_challenge + user_name.encode() + b"server"
        )

        # Step 4: send response with server challenge
        self.user_name = user_name
        self.server_challenge = os.urandom(CHALLENGE_LENGTH)

        self.auth_initialized = True
        return {
            "salt" : salt.hex(),
            "server_proof": server_proof.hex(),
            "server_challenge": self.server_challenge.hex()
        }

    def auth_proof_response(self, user_request: dict):
        if not self.auth_initialized:
            return {
                "error": "authentication must be initialized first with 'auth_init'"
            }

        # Step 1: get client proof
        try:
            client_proof = bytes.fromhex(user_request["client_proof"])
            assert len(client_proof) > 32

        except:
            return {
                "error": "missing field 'client_proof' (at least 33 bytes in hexadecimal representation)"
            }
        
        # Step 2: decrypt proof and verify
        proof_decrypted = self.cipher.decrypt(client_proof)

        if proof_decrypted[:CHALLENGE_LENGTH] != self.server_challenge:
            return {
                "authenticated": False,
                "message": "authentication error (client proof has an incorrect server challenge)"
            }
        
        if proof_decrypted[-6:] != b'client':
            return {
                "authenticated": False,
                "message": "authentication error (client proof does not terminate with 'client')"
            }

        # Step 3: successful authentication
        return {
            "authenticated": True,
            "message": f"Welcome {self.user_name}"
        }

def get_file(user_name: str):
    try:
        return open(f'user_files/{user_name}.txt', 'r').read()
    except:
        return {"error": f"cannot retrieve file for {user_name}"}

class Service:
    def __init__(self):
        self.auth = None
        self.status = {
            "user_name": None,
            "authenticated": False
        }

    def handle_request(self, user_request: str):
        try:
            user_request = json.loads(user_request)
        except:
            return {
                "error": "JSON expected with 'option' set (available: 'auth_init', 'auth_proof', 'get_file')"
            }

        if "option" not in user_request:
            return {
                "error": "JSON expected with 'option' set (available: 'auth_init', 'auth_proof', 'get_file')"
            }

        if user_request["option"] == "auth_init":
            self.auth = ServerAuth()
            return json.dumps(self.auth.auth_init_response(user_request))

        if user_request["option"] == "auth_proof":
            response = self.auth.auth_proof_response(user_request)
            if "authenticated" in response:
                self.status["user_name"] = self.auth.user_name
                self.status["authenticated"] = response["authenticated"]
            return json.dumps(response)

        if user_request["option"] == "get_file":
            if self.status["authenticated"]:
                return get_file(self.auth.user_name)
            else:
                return {"error": "you must be authenticated for access"}

        return {
            "error": "JSON expected with 'option' set (available: 'auth_init', 'auth_proof', 'get_file')"
        }

    def run_service(self):
        while True:
            try:
                user_request = input()
                response = self.handle_request(user_request)
                print(response)
                if "get_file" in user_request and not "error" in response:
                    break

            except:
                print(json.dumps({
                    "error": "JSON expected with 'option' set (available: 'auth_init', 'auth_proof', 'get_file')"
                }))
                break

if __name__ == "__main__":
    service = Service()
    service.run_service()
