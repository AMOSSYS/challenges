# Kyber functions https://github.com/GiacomoPope/kyber-py in folder Kyber/
from Kyber.kyber import *

# RSA functions
import rsa

# Utils
from os import urandom, chdir, path

def xor(buf1: bytes, buf2: bytes):
    return bytes([x ^ y for x, y in zip(buf1, buf2)])

# Sizes in bytes of parameters of Kyber-512
PK_LEN = 800
C_KYBER_LEN = 768

def Keygen_Kyber():
    return Kyber512.keygen() #pk, sk in bytes

def Encaps_Kyber(pk) :      #pk should be in bytes
    return Kyber512.enc(pk) #c, k in bytes

def Decaps_Kyber(sk, c):    #sk and c should be in bytes
    return Kyber512.dec(c, sk) #k in bytes

# Sizes in bytes of parameters of RSA-1024
N_LEN = 128 # modulus
E_LEN = 3   # public exponent
D_LEN = 128 # private exponent
P_LEN = 68  # prime factor p
Q_LEN = 60  # prime factor q
M_LEN = 32  # message
C_LEN = 128 # chiphertext

def Keygen_RSA():
    return rsa.newkeys(1024) #pk, sk not in bytes

def Encaps_RSA(pk): #pk should be a key object from rsa.newkeys()
    k_rnd = os.urandom(M_LEN)
    return k_rnd, rsa.encrypt(k_rnd, pk) #k, c in bytes

def Decaps_RSA(sk, c):  #sk should be a key object from rsa.newkeys(), c should 
                        #be in bytes
    return rsa.decrypt(c, sk) #k in bytes

def keys_to_bytes(pk):
    """
    pk should be a key object from rsa.newkeys().
    Function that converts the key object pk in a bytes object.
    """
    # Extract the modulus n and the public exponent e
    n, e = pk.n, pk.e
    # Convert n and e in bytes
    n_bytes = n.to_bytes((n.bit_length() + 7) // 8, 'big')
    e_bytes = e.to_bytes((e.bit_length() + 7) // 8, 'big')
    # Concatenate n and e
    pk_bytes = n_bytes + e_bytes
    return pk_bytes #pk in bytes

def bytes_to_keys(pk):
    """
    pk should be in bytes.
    Function that converts a RSA public key in bytes in a key object from
    rsa.newkeys().
    """
    # Extract the modulus n and the public exponent e
    n_bytes = pk[:N_LEN]
    e_bytes = pk[N_LEN:N_LEN+E_LEN]
    # Convert bytes into int
    n = int.from_bytes(n_bytes, 'big')
    e = int.from_bytes(e_bytes, 'big')
    # Create a new key object from integers n and e
    return rsa.PublicKey(n, e) #pk key object from rsa.newkeys()

def bytes_to_hex(b): #b should be bytes
    return b.hex()  #string in hex

def hex_to_bytes(s): #s should be a string of hex characters
    return bytes.fromhex(s) #bytes object

def Keygen():
    """
    Generate combined keys for the XOR combiner.
    """
    rsa_pk, rsa_sk = Keygen_RSA()
    kyber_pk, kyber_sk = Keygen_Kyber()
    return rsa_pk, rsa_sk, kyber_pk, kyber_sk


def Encaps(pk1, pk2):
    """
    pk1 should be a public key object from rsa.newkeys().
    pk2 should be in bytes.
    Encapsulate a xored key from two secrets k1 (resp. k2) from RSA (resp. Kyber).
    """
    k1, c_rsa = Encaps_RSA(pk1)
    c_kyber, k2 = Encaps_Kyber(pk2)
    return c_rsa, c_kyber, xor(k1, k2)

def Decaps(sk1, sk2, c1, c2):
    """
    sk2, c1 and c2 should be in bytes.
    sk1 should be a private key object from rsa.newkeys().
    Decapsulate a xored key from two ciphertexts with the correct private keys.
    """
    k1 = Decaps_RSA(sk1, c1)
    k2 = Decaps_Kyber(sk2, c2)
    return xor(k1, k2)

def Oracle(cbis1, cbis2):
    """
    cbis1 and cbis2 should be in bytes.
    Decapsulation Oracle that decapsulates a xored key corresponding to the 
    input ciphertexts.
    """
    # List of the former queries, including the challenge
    global cyphered_list
    
    # Challenge
    global c1, c2
    
    # Check if input is different from the challenge
    if cbis1 + cbis2 == c1 + c2:
        return None
    
    # Check if input is different from previous queries
    for i in range(len(cyphered_list)):  
        if cyphered_list[i] == cbis1 + cbis2:
            return None
            
    # If not, append the current query to the list
    cyphered_list.append(cbis1 + cbis2)
    
    # No more than the 1000 previous queries in the list
    if len(cyphered_list) > 1000:
        cyphered_list = cyphered_list[len(cyphered_list)-1000:]
    
    # The decapsulated key
    kbis = Decaps(sk1, sk2, cbis1, cbis2)
    return kbis

def test_key(kbis):
    """
    kbis should be in bytes.
    Function that tests if the input is the challenge key.
    """
    # The challenge key
    global k
    
    if(kbis == k):
        with open(FLAG_FILE, 'r') as f:
            FLAG = f.read().encode()
        return f"Well done! The flag is {FLAG}\n"
    return f"It's the wrong session key, try again!\n"

### Begin of the script ###

# The file containing the flag
FLAG_FILE = 'flag.txt'

"""
Generation of the keys.
pk1 is RSA public key.
pk2 is Kyber public key.
sk1 is RSA private key.
sk2 is Kyber private key.
"""
pk1, sk1, pk2, sk2 = Keygen()

# Encapsulation of the challenge key k in the ciphertext c1 || c2
c1, c2, k = Encaps(pk1, pk2)

# List of the previous queries
cyphered_list = []

def main():
    print(f"[DEBUG]: Warning, XOR combiner is unsafe. Consider using a safer key encapsulation mechanism combiners.\n")

    # Make sure the working dir is the file root
    chdir(path.dirname(path.abspath(__file__)))

    while(True):
        print("Please choose an action:\n 1- Encapsulation with RSA\n 2- Encapsulation with Kyber\n 3- Decapsulation Oracle\n 4- Test a session key\n 5- Read the latest communication\n\nCommand:")
        
        # Option selection
        num = input()
        
        # RSA encapsulation
        if(num == "1"):
            pk = input("Please enter a public key:\n")
            try :
                k_rsa, c = Encaps_RSA(bytes_to_keys(hex_to_bytes(pk)))
            except Exception as e:
                print(f"[{type(e).__name__}]: {e}\n")
            else :
                print(f"\nciphertext:\n{bytes_to_hex(c)}\n")
                print(f"key:\n{bytes_to_hex(k_rsa)}\n")
                
        # Kyber encapsulation 
        elif(num == "2"):
            pk = input("Please enter a public key:\n")
            try :
                c, k_kyber = Encaps_Kyber(hex_to_bytes(pk))
            except Exception as e:
                print(f"[{type(e).__name__}]: {e}\n")
            else :
                print(f"\nciphertext:\n{bytes_to_hex(c)}\n")
                print(f"key:\n{bytes_to_hex(k_kyber)}\n")
                
        # Oracle query        
        elif(num == "3"):
            cbis = input("Please enter a ciphertext:\n")
            if len(cbis) != 2 * (C_KYBER_LEN + C_LEN):
                print("Bad request.\n")
                continue
            cbis = hex_to_bytes(cbis)
            c1bis = cbis[:C_LEN]
            c2bis = cbis[C_LEN:]
            try:
                k_guess = Oracle(c1bis, c2bis)
            except Exception as e:
                print(f"[{type(e).__name__}]: {e}\n")
            else :
                if(k_guess is None):
                    print("[ERROR]: You can't decapsulate this combined ciphertext.\n")
                else : 
                    print(f"\nsession key:\n{bytes_to_hex(k_guess)}\n")
                    
        # Test a session key
        elif(num == "4"):
            k_test = input("Please enter a key:\n")
            try:
                print(test_key(hex_to_bytes(k_test)))
            except Exception as e:
                print(f"[{type(e).__name__}]: {e}\n")
                
        # Print the challenge instance
        elif(num == "5"):
            print(f"(Beginning of communication between A and B)\n(A --> B: pk_rsa + pk_kyber)\n{bytes_to_hex(keys_to_bytes(pk1))}{bytes_to_hex(pk2)}\n\n(B --> A: c_rsa + c_kyber)\n{bytes_to_hex(c1 + c2)}\n")

        else:
            print("Bad request.\n")

if __name__ == '__main__':
    main()
        
        
   
