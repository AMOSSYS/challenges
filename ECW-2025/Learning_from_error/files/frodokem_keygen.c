#include <string.h>
#include "rand.h"
#include "shake.h"

// FrodoKEM – Preliminary Standardization Proposal
// Source: https://frodokem.org/ (PDF)

// Clean memory (from OpenSSL)
typedef void *(*memset_t)(void *, int, size_t);
static volatile memset_t memset_func = memset;
void memory_cleanse(void *ptr, size_t len) {
    memset_func(ptr, 0, len);
}

// Parameters
const size_t N = 1344;
const size_t NBAR = 8;
const size_t LEN_SK = 43088;
const size_t LEN_PK = 21520;
const size_t LEN_SEC = 32;
const size_t LEN_SE = 64;
const size_t LEN_A = 16;

// Distribution table for FrodoKEM-1344 (annex A, table A.4)
const uint16_t DIST[6] = {
  9142,
  23462,
  30338,
  32361,
  32725,
  32765
};

// Packing matrices modulo q (section 7.3)
void pack(uint8_t *buffer, const uint16_t *mat, const size_t n) {
  // buffer must have length equal to 2*n
  int i;
  for(i = 0; i < n; i++) {
    buffer[2*i] = mat[i] >> 8;
    buffer[2*i + 1] = mat[i];
  }
}

// Sampling from the error distribution (section 7.4)
uint16_t sample_coefficient(uint16_t a) {
  // Constant-time operations from:
  // https://github.com/microsoft/PQCrypto-LWEKE/blob/master/FrodoKEM/src/noise.c
  uint16_t sign = a & 1;
  uint16_t t = a >> 1;
  uint16_t e = 0;
  int i;
  for(i = 0; i < 6; i++) {
    e += (uint16_t)(DIST[i] - t) >> 15;
  }
  return ((-sign) ^ e) + sign;
}

// Matrix sampling from the error distribution (section 7.5)
void sample_matrix(uint16_t *m, const size_t mlen, const uint8_t *r) {
  // r must be of size 2*mlen
  int i;
  uint16_t a;
  for(i = 0; i < mlen; i++) {
    a = r[2*i] | (uint16_t)(r[2*i + 1]) << 8;
    m[i] = sample_coefficient(a);
  }
}

// Matrix A generation with SHAKE128 (section 7.6.2)
void generate_matrix_a(uint16_t mat_a[N*N], const uint8_t seed_a[LEN_A]) {
  uint8_t input[LEN_A + 2];
  int i;
  memcpy(&input[2], seed_a, LEN_A);
  for(i = 0; i < N; i++) {
    input[0] = i;
    input[1] = i >> 8;
    shake128((uint8_t *)(&mat_a[N*i]), 2*N, input, LEN_A + 2);
  }
}

// Calculate A*S^T
void matrix_mul_a_st(uint16_t res[N*NBAR], const uint16_t mat_a[N*N],
                     const uint16_t mat_st[N*NBAR]) {
  int i, j, k;
  for(i = 0; i < N; i++) {
    for(j = 0; j < NBAR; j++) {
      res[i*NBAR + j] = 0;
      for(k = 0; k < N; k++) {
        res[i*NBAR + j] += mat_a[i*N + k] * mat_st[j*N + k];
      }
    }
  }
}

// Key generation derandomized (section 8.1, steps 2-10)
void frodokem_keygen_derand(uint8_t sk[LEN_SK], uint8_t pk[LEN_PK],
                            const uint8_t seeds[LEN_SEC + LEN_SE + LEN_A]) {
  // separation of `seeds` in three parts
  const uint8_t *s = seeds;
  const uint8_t *seed_se = &seeds[LEN_SEC];
  const uint8_t *z = &seeds[LEN_SEC + LEN_SE];

  uint8_t *seed_a = &sk[LEN_SEC]; // second part of secret key
  uint8_t *b = &sk[LEN_SEC + LEN_A]; // third part of secret key
  uint16_t *mat_st = (uint16_t *)(&sk[LEN_SEC + LEN_PK]); // fourth part of secret key
  uint8_t *pkh = &sk[LEN_SEC + LEN_PK + 2*N*NBAR]; // last part of secret key
  uint8_t seed_se_with_prefix[LEN_SE + 1]; // must be CLEANED
  uint8_t r[4*N*NBAR]; // must be CLEANED
  uint16_t mat_a[N*N];
  uint16_t mat_b[N*NBAR];
  uint16_t mat_e[N*NBAR]; // must be CLEANED

  int i;

  // 2. Generate pseudorandom seed `seed_A ← SHAKE(z, len_A)`
  shake256(seed_a, LEN_A, z, LEN_A);

  // 3. Generate the matrix `A ← Gen(seed_A)`
  generate_matrix_a(mat_a, seed_a);

  // 4. Generate pseudorandom bit string
  //    `(r(0), r(1), ... , 𝑟(2nnbar - 1)) ← SHAKE(0x5F||seed_SE, 32nnbar)`
  seed_se_with_prefix[0] = 0x5f;
  memcpy(&seed_se_with_prefix[1], seed_se, LEN_SE);
  shake256(r, 4*N*NBAR, seed_se_with_prefix, LEN_SE + 1);
  memory_cleanse(&seed_se_with_prefix[1], LEN_SE);

  // 5. Sample error matrix
  //    `S^T ← SampleMatrix(r(0), r(1), ..., r(nnbar-1), nbar, n)`
  sample_matrix(mat_st, N*NBAR, r);

  // 6. Sample error matrix
  //    `E = SampleMatrix(r(nnbar), r(nnbar+1), ..., r(2nnbar - 1)), n, nbar)`
  sample_matrix(mat_e, N*NBAR, &r[2*N*NBAR]);
  memory_cleanse(r, 4*N*NBAR);

  // 7. Compute `B ← AS + E`
  matrix_mul_a_st(mat_b, mat_a, mat_st);
  for(i = 0; i < N*NBAR; i++) {
    mat_b[i] += mat_e[i];
  }

  // 8. Compute `b ← Pack(B)`
  pack(b, mat_b, N*NBAR);

  // 9. Compute `pkh ← SHAKE(seed_a || b, len_sec)`
  shake256(pkh, LEN_SEC, seed_a, LEN_PK);

  // 10. Return public key `pk ← seed_a || b` and secret key `sk ← s || seed_a || b || S^T || pkh.
  memcpy(pk, seed_a, LEN_PK); // copy from `pk`
  memcpy(sk, s, LEN_SEC); // first part of secret key
}

// Key generation (section 8.1)
void frodokem_keygen(uint8_t sk[LEN_SK], uint8_t pk[LEN_PK]) {
  uint8_t seeds[LEN_SEC + LEN_SE + LEN_A]; // must be CLEANED

  // 1. Choose uniformly random seeds `s`, `seed_SE` and `z`
  //    of bitlengths `len_sec`, `len_SE` and `len_A`(resp.)
  randombytes(seeds, LEN_SEC + LEN_SE + LEN_A);

  // 2-10.
  frodokem_keygen_derand(sk, pk, seeds);

  // cleaning
  memory_cleanse(seeds, LEN_SEC + LEN_SE + LEN_A);
}
