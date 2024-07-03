#!/usr/bin/env python3

from ntru.ntrucore import *
import sys

def genkeypair(params):
    '''
    Key pair generation
    '''
    N = params['N']
    q = params['q']
    d = params['d']

    while True:
        f = random_pol(N, q, d)
        res = invert_pol_p_q(f, q)
        if not res is None:
            break
    fp, fq = res
    g = random_pol(N, q, d)
    h = 3*fq*g
    hq = invert_pol_q(h)

    privkey = pack_3(f) + pack_3(fp) + pack_q(hq)
    pubkey = pack_q(h)
    
    return privkey, pubkey

if __name__ == '__main__':
    argc = len(sys.argv) - 1

    if argc != 1:
        print('Please enter a name for the key pair.')
        sys.exit()

    try:
        name = sys.argv[1]
        privkey, pubkey = genkeypair(params)
        open(name + '.priv', 'wb').write(privkey)
        open(name + '.pub', 'wb').write(pubkey)
        print(f'A key pair was generated:\n  * Private key in file `{name + ".priv"}`\n  * Public key in file `{name + ".pub"}`')

    except:
        print('A problem occured during key pair generation')
