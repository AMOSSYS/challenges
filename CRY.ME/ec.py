#!/usr/bin/env python3

from random import randint

# FLT inversion when m is prime
invmod = lambda a, m : pow(a, m - 2, m)

# ------------
# Finite field
# ------------

class FieldElement:
    def __init__(self, a, field):
        self.field = field
        self.a = a % self.field.p

    def __add__(self, other):
        return FieldElement(self.a + other.a, self.field)

    def __radd__(self, other):
        return self + other

    def __sub__(self, other):
        return FieldElement(self.a - other.a, self.field)

    def __rsub__(self, other):
        return self - other

    def __neg__(self):
        return FieldElement(-self.a, self.field)

    def __mul__(self, other):
        if isinstance(other, int):
            return FieldElement(self.a * other, self.field)
        return FieldElement(self.a * other.a, self.field)

    def __rmul__(self, other):
        return self * other

    def invert(self):
        if self.a == 0:
            raise ZeroDivisionError
        return FieldElement(invmod(self.a, self.field.p), self.field)

    def __truediv__(self, other):
        return self * invert(other)

    def __pow__(self, exp : int):
        if exp < 0:
            return self.invert()**(-exp)
        return FieldElement(pow(self.a, exp, self.field.p), self.field)

    def __eq__(self, other):
        if isinstance(other, int):
            return self == FieldElement(other, self.field)
        return self.a == other.a and self.field.p == other.field.p

    def __neq__(self, other):
        return not self == other

    def __str__(self):
        return str(self.a)

    def __repr__(self):
        return str(self.a)

    def hex(self):
        return hex(self.a)

    def to_int(self):
        return self.a

    def legendre_symbol(self):
        return self**((self.field.p - 1) // 2)

    def sqrt(self):
        '''
        https://eli.thegreenplace.net/2009/03/07/computing-modular-square-roots-in-python
        '''

        if self.legendre_symbol() != 1:
            return 0
        elif self == 0:
            return self
        elif self.field.p % 4 == 3:
            return self**((self.field.p + 1)//4)

        s = self.field.p - 1
        e = 0
        while s % 2 == 0:
            s //= 2
            e += 1

        n = self.field(2)
        while n.legendre_symbol() != -1:
            n = n + 1

        x = a**((s + 1)//2)
        b = a**s
        g = n**s
        r = e

        while True:
            t = b
            m = 0
            for m in xrange(r):
                if t == 1:
                    break
                t = t**2

            if m == 0:
                return x

            gs = g**(2**(r - m - 1))
            g = gs * gs
            x = x * gs
            b = b * g
            r = m

    def __hash__(self):
        return hash(self.a)

class PrimeField:
    def __init__(self, prime):
        self.p = prime

    def __call__(self, a):
        return FieldElement(a, self)


# --------------
# Elliptic curve
# --------------


def conditional_swap(bit, R0, R1):
    if bit == 1:
        R0, R1 = R1, R0
    return R0, R1

class Curve:
    '''
    Generic class for elliptic curves in Weierstrass form
    '''

    def __init__(self, params):
        self.name = params['name']
        self.field = PrimeField(params['p'])
        self.A = self.field(params['A'])
        self.B = self.field(params['B'])
        self.order = params['order']
        self.base = self.field(params['x0']), self.field(params['y0'])

    def is_on_curve(self, P):
        '''
        Verify that y0^2 = x0^3 + Ax0 + B with P=(x0,y0)
        '''
        x = self.field(P[0])
        y = self.field(P[1])
        t0 = x**2
        t0 = t0 + self.A
        t0 = t0 * x
        t0 = t0 + self.B
        t1 = y**2
        return t0 == t1

    def lift_x(self, r):
        '''
        Find (x,y) on curve such that x mod curve.order = r
        and 0 <= r < curve.order
        '''
        assert 0 <= r and r < self.order

        # find potential x
        Lx = []
        while r < self.field.p:
            Lx.append(self.field(r))
            r += self.order

        # find y for each possible x
        lifted_points = []
        for x in Lx:
            ysqr = x**2
            ysqr = ysqr + self.A
            ysqr = ysqr * x
            ysqr = ysqr + self.B
            y = ysqr.sqrt()
            # case ysqr is not a square
            if y == 0 and ysqr != 0:
                continue
            # We add the points
            lifted_points += [(x, y), (x, -y)]
            
        return lifted_points

    def neg(self, P):
        '''
        Point negation (P in affine form)
        '''
        if P == self.infty:
            return self.infty
        return P[0], -P[1]
          
    def dbl_aff(self, P):
        '''
        https://hyperelliptic.org/EFD/g1p/auto-shortw-projective.html#doubling-mdbl-2007-bl
        '''
        if P == self.infty:
            return P
        X1, Y1 = P
        XX = X1**2
        t0 = 3 * XX
        w = self.A + t0
        Y1Y1 = Y1**2
        R = 2 * Y1Y1
        t1 = Y1 * R
        sss = 4 * t1
        RR = R**2
        t2 = X1 + R
        t3 = t2**2
        t4 = t3 - XX
        B = t4 - RR
        t5 = w**2
        t6 = 2 * B
        h = t5 - t6
        t7 = h * Y1
        X3 = 2 * t7
        t8 = B - h
        t9 = 2 * RR
        t10 = w * t8
        Y3 = t10 - t9
        Z3 = sss
        t = Z3**-1
        return X3 * t, Y3 * t
        
    def add_aff(self, P, Q):
        '''
        https://hyperelliptic.org/EFD/g1p/auto-shortw-projective.html#addition-mmadd-1998-cmo
        '''
        if P == self.infty:
            return Q
        if Q == self.infty:
            return P
        X1, Y1 = P
        X2, Y2 = Q
        if X1 == X2:
            if Y1 == Y2:
                return self.dbl_aff(P)
            return self.infty
        u = Y2 - Y1
        uu = u**2
        v = X2 - X1
        vv = v**2
        vvv = v * vv
        R = vv * X1
        t0 = 2 * R
        t1 = uu - vvv
        A = t1 - t0
        X3 = v * A
        t2 = R - A
        t3 = vvv * Y1
        t4 = u * t2
        Y3 = t4 - t3
        Z3 = vvv
        t = Z3**-1        
        return X3 * t, Y3 * t

class CurveJac(Curve):
    '''
    Class for elliptic curve calculation with Jacobian coordinates
    '''

    def __init__(self, params):
        Curve.__init__(self, params)
        self.type = 'Jac'
        self.infty = self.field(1), self.field(1), self.field(0)
     
    def to_affine(self, P):
        X, Y, Z = P
        if Z == 0:
            return self.infty
        tcub = Z**-1
        tsqr = tcub**2
        tcub = tsqr*tcub
        return X*tsqr, Y*tcub
 
    def add_jac(self, P1, P2):
        '''
        https://hyperelliptic.org/EFD/g1p/auto-shortw-jacobian.html#addition-add-2007-bl
        '''
        X1, Y1, Z1 = P1
        X2, Y2, Z2 = P2
        if Z1 == 0:
            return P2
        if Z2 == 0:
            return P1
        Z1Z1 = Z1**2
        Z2Z2 = Z2**2
        U1 = X1 * Z2Z2
        U2 = X2 * Z1Z1
        t0 = Z2 * Z2Z2
        S1 = Y1 * t0
        t1 = Z1 * Z1Z1
        S2 = Y2 * t1
        H = U2 - U1
        t2 = 2 * H
        I = t2**2
        J = H * I
        t3 = S2 - S1
        if H == 0:
            if t3 == 0:
                return self.dbl_jac(P1)
            else:
                return self.infty
        r = 2 * t3
        V = U1*I
        t4 = r**2
        t5 = 2 * V
        t6 = t4 - J
        X3 = t6 - t5
        t7 = V - X3
        t8 = S1 * J
        t9 = 2 * t8
        t10 = r * t7
        Y3 = t10 - t9
        t11 = Z1 + Z2
        t12 = t11**2
        t13 = t12 - Z1Z1
        t14 = t13 - Z2Z2
        Z3 = t14 * H
        return X3, Y3, Z3
      
    def dbl_jac(self, P1):
        '''
        https://hyperelliptic.org/EFD/g1p/auto-shortw-jacobian.html#doubling-dbl-2007-bl
        '''
        X1, Y1, Z1 = P1
        if Z1 == 0:
            return P1
        XX = X1**2
        YY = Y1**2
        YYYY = YY**2
        ZZ = Z1**2
        t0 = X1 + YY
        t1 = t0**2
        t2 = t1 - XX
        t3 = t2 - YYYY
        S = 2 * t3
        t4 = ZZ**2
        t5 = self.A * t4
        t6 = 3 * XX
        M = t6 + t5
        t7 = M**2
        t8 = 2 * S
        T = t7 - t8
        X3 = T
        t9 = S - T
        t10 = 8 * YYYY
        t11 = M * t9
        Y3 = t11 - t10
        t12 = Y1 + Z1
        t13 = t12**2
        t14 = t13 - YY
        Z3 = t14 - ZZ
        return X3, Y3, Z3

    def scalar_mult(self, k, P):
        '''
        Computer scalar multiplication [k]P (Montgomery ladder)
        '''

        k = int(k) % self.order
        if k == 0:
            return self.infty
        if k == 1:
            return P
        n = k.bit_length()
        R0 = P[0], P[1], self.field(1)
        R1 = self.dbl_jac(R0)

        condition = 0
        for i in range(n - 2, -1, -1):
            ki = (k >> i) & 1
            condition ^= ki
            R0, R1 = conditional_swap(condition, R0, R1)
            R1 = self.add_jac(R0, R1)
            R0 = self.dbl_jac(R0)
            condition = ki
        R0, R1 = conditional_swap(k & 1, R0, R1)

        return self.to_affine(R0)

def generate_keypair(curve):
    '''
    Generate a private/public key pair on the curve
    '''
    privkey = randint(1, curve.order - 1)
    pubkey = curve.ladder(privkey, curve.base)
    return privkey, pubkey

SECP192R1 = {
    'name'  : 'secp192r1',
    'p'     : 0xfffffffffffffffffffffffffffffffeffffffffffffffff,
    'order' : 0xffffffffffffffffffffffff99def836146bc9b1b4d22831,
    'A'     : -3,
    'B'     : 0x64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1,
    'x0'    : 0x188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012,
    'y0'    : 0x07192b95ffc8da78631011ed6b24cdd573f977a11e794811
}

SECP224R1 = {
    'name'  : 'secp224r1',
    'p'     : 0xffffffffffffffffffffffffffffffff000000000000000000000001,
    'order' : 0xffffffffffffffffffffffffffff16a2e0b8f03e13dd29455c5c2a3d,
    'A'     : -3,
    'B'     : 0xb4050a850c04b3abf54132565044b0b7d7bfd8ba270b39432355ffb4,
    'x0'    : 0xb70e0cbd6bb4bf7f321390b94a03c1d356c21122343280d6115c1d21,
    'y0'    : 0xbd376388b5f723fb4c22dfe6cd4375a05a07476444d5819985007e34
}

SECP256R1 = {
    'name'  : 'secp256r1',
    'p'     : 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff,
    'order' : 0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551,
    'A'     : -3,
    'B'     : 0x5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b,
    'x0'    : 0x6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296,
    'y0'    : 0x4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5
}

SECP256K1 = {
    'name'  : 'secp256k1',
    'p'     : 0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f,
    'order' : 0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141,
    'A'     : 0,
    'B'     : 7,
    'x0'    : 0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798,
    'y0'    : 0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8
}

SECP384R1 = {
    'name'   : 'secp384r1',
    'p'      : 0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff0000000000000000ffffffff,
    'order'  : 0xffffffffffffffffffffffffffffffffffffffffffffffffc7634d81f4372ddf581a0db248b0a77aecec196accc52973,
    'A'      : -3,
    'B'      : 0xb3312fa7e23ee7e4988e056be3f82d19181d9c6efe8141120314088f5013875ac656398d8a2ed19d2a85c8edd3ec2aef,
    'x0'     : 0xaa87ca22be8b05378eb1c71ef320ad746e1d3b628ba79b9859f741e082542a385502f25dbf55296c3a545e3872760ab7,
    'y0'     : 0x3617de4a96262c6f5d9e98bf9292dc29f8f41dbd289a147ce9da3113b5f0b8c00a60b1ce1d7e819d7a431d7c90ea0e5f
}

SECP521R1 = {
    'name'  : 'secp521r1',
    'p'     : 0x1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff,
    'order' : 0x1fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffa51868783bf2f966b7fcc0148f709a5d03bb5c9b8899c47aebb6fb71e91386409,
    'A'     : -3,
    'B'     : 0x51953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00,
    'x0'    : 0xc6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe75928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66,
    'y0'    : 0x11839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef42640c550b9013fad0761353c7086a272c24088be94769fd16650
}

# Parameters of Weierstrass form of Curve25519
# as used in CRY-ME.
# The base point generates the whole curve (so its order is not prime).
WEI25519 = {
    'name'  : 'wei25519',
    'p'     : 57896044618658097711785492504343953926634992332820282019728792003956564819949,
    'order' : 57896044618658097711785492504343953926856930875039260848015607506283634007912,
    'A'     : 19298681539552699237261830834781317975544997444273427339909597334573241639236,
    'B'     : 55751746669818908907645289078257140818241103727901012315294400837956729358436,
    'x0'    : 1,
    'y0'    : 18909347336615151728670064226692766338825600391524661473633862702332996581084
}

CURVES = {
    'secp192r1' : SECP192R1,
    'secp224r1' : SECP224R1,
    'secp256r1' : SECP256R1,
    'secp256k1' : SECP256K1,
    'secp384r1' : SECP384R1,
    'secp521r1' : SECP521R1,
    'wei25519'  : WEI25519
}
