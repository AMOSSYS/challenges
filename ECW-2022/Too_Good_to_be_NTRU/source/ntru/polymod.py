#!/usr/bin/env python3

#
# This file contains a Python class for calculatation
# with polynomials in the ring Z/3Z[X]/(X^N - 1)
# or Z/qZ[X]/(X^N - 1) with q is a power of 2.
#

from Cryptodome.Util.number import inverse

class PolyMod:
    def __init__(self, N, q, coefficients):        
        self.N = N
        self.q = q
        self.coefs = [0]*self.N
        for i in range(len(coefficients)):
            self.coefs[i % self.N] += coefficients[i]
        self._balance()
    
    def _balance(self):
        '''
        Centers coefficients in [-q/2, q/2].
        '''
        for i in range(self.N):
            self.coefs[i] = ((self.coefs[i] + self.q//2) % self.q) - self.q//2

    def __add__(self, other):
        assert self.N == other.N and self.q == other.q
        coefficients = [self.coefs[i] + other.coefs[i] for i in range(self.N)]
        return PolyMod(self.N, self.q, coefficients)

    def __sub__(self, other):
        assert self.N == other.N and self.q == other.q
        coefficients = [self.coefs[i] - other.coefs[i] for i in range(self.N)]
        return PolyMod(self.N, self.q, coefficients)

    def __mul__(self, other):
        # Multiplication by an integer
        if isinstance(other, int):
            coefficients = [c*other for c in self.coefs]
            return PolyMod(self.N, self.q, coefficients)

        # Multiplication by another polynomial
        # that must share its characteristics
        assert self.N == other.N and self.q == other.q
        coefficients = [0]*self.N
        for i in range(self.N):
            for j in range(self.N):
                coefficients[(i + j) % self.N] += self.coefs[i]*other.coefs[j]
        return PolyMod(self.N, self.q, coefficients)

    def __rmul__(self, other):
        return self*other

    def __eq__(self, other):
        return self.N == other.N and self.q == other.q and self.coefs == other.coefs

    def __str__(self):
       
        return str(self.coefs)

    def __repr__(self):
        return str(self.coefs)
    
    def change_ring(self, m):
        '''
        Changes the ring to Z/mZ
        '''
        return PolyMod(self.N, m, self.coefs)

def invert_pol(pol, m):
    '''
    Returns coefficients of pol^-1 in Z/mZ[x]/(x^n -1)
    '''

    check3 = False
    if m % 3 == 0:
        check3 = True
    
    # We create a matrix of size N*(N+1)
    nrows = pol.N
    ncols = pol.N + 1
    M = [[0]*ncols for i in range(nrows)]
    M[0][nrows] = 1
    for j in range(pol.N):
        for i in range(pol.N):
            M[(i + j) % pol.N][j] = pol.coefs[i]
    
    # Gaussian elimitation algorithm
    pivots_positions = []
    for j in range(nrows):
        # We search for a pivot in the column
        pivot_found = False
        for i in range(nrows):
            if i in pivots_positions:
                continue
            pivot = M[i][j]
            # pivot must be invertible mod m
            if pivot % 2 == 0 or (check3 and pivot % 3 == 0):
                continue
            pivot_found = True
            position = i
            break
        if not pivot_found:
            return None
        pivots_positions.append(position)

        # We multiply row by pivot^-1 mod (3q)
        # to get [0 ... 0 1 * ... *]
        #                 ^
        #               pivot
        pivot_inv = inverse(pivot, m)
        for k in range(j, ncols):
            M[position][k] = (M[position][k]*pivot_inv) % m
        
        # We eliminate other coefficients in the column
        for i in range(nrows):
            if i == position:
                continue
            c = M[i][j]
            for k in range(j, ncols):
                M[i][k] = (M[i][k] - c*M[position][k]) % m

    # If Gaussian elimination is completed, then the last column
    # contains the coefficients of the inverse mod m we are looking for.
    # We use `pivots_positions` to put them in order.
    coefficients = [M[pivots_positions[i]][nrows] for i in range(nrows)]
    return coefficients

def invert_pol_q(pol):
    '''
    Returns inverse of polynomial in Z/qZ[x]/(x^N - 1)
    '''
    coefficients = invert_pol(pol, pol.q)
    return PolyMod(pol.N, pol.q, coefficients)

def invert_pol_p_q(pol, q):
    '''
    pol is a polynomial with coefficients in {-1,0,1}.
    This returns f^-1 mod 3 and f^{-1} mod q.
    It returns None if such inverses do not exist.
    We use a little bit of linear algebra.
    '''
    coefficients = invert_pol(pol, 3*q)
    if coefficients is None:
        return None    
    fp = PolyMod(pol.N, 3, coefficients)
    fq = PolyMod(pol.N, q, coefficients)

    return fp, fq
