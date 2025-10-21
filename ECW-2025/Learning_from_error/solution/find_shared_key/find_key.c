#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "api_frodo1344.h"
#include "solution.h"
#include "shake.h"

// Wrapper to fopen and fread to read a file from an offset
void read_file(uint8_t *buffer, const size_t len, const char *file_name, const long offset) {
  FILE *fp = NULL;

  // open file
  if ((fp = fopen(file_name, "r")) == NULL) {
    fprintf(stderr, "Error: cannot open file \"%s\"\n", file_name);
    exit(EXIT_FAILURE);
  }

  // jump to an offset in file
  if (offset != 0) {
    if (fseek(fp, offset, SEEK_SET) != 0) {
      fprintf(stderr, "Error: cannot seek to offset %ld\n", offset);
      exit(EXIT_FAILURE);
    }
  }

  // read the file
  if (fread(buffer, len, 1, fp) != 1) {
    fprintf(stderr, "Error: cannot read file");
    exit(EXIT_FAILURE);
  }

  fclose(fp);
}

// Taken from file "frodokem_keygen.c"
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

// Unpack matrice modulo q (reverse of pack from file "frodokem_keygen.c")
void unpack(uint16_t mat[N*NBAR], const uint8_t *buffer) {
  // buffer must have length equal to 2*N*NBAR
  int i;
  for(i = 0; i < N*NBAR; i++) {
    mat[i] = ((uint16_t)buffer[2*i]) << 8 | buffer[2*i + 1];
  }
}

// Retrieve S^T from A, B, and E by calculating A^(-1)*(B - E)
void get_matrix_st(uint16_t mat_st[NBAR*N], const uint16_t mat_a[N*N],
                   uint16_t mat_b[N*NBAR], const uint16_t mat_e[N*NBAR]) {
  int i, j, k;
  uint16_t mat_a_inv[N*N];

  // Calculate B - E (overwrites B)
  for(i = 0; i < N*NBAR; i++) {
    mat_b[i] -= mat_e[i];
  }

  // Calculate A^-1 (see file pivot.c)
  if (inverse_matrix(mat_a_inv, mat_a) != 1) {
    fprintf(stderr, "Matrix A not invertible\n");
    exit(EXIT_FAILURE);
  }
 
  // Calculate S = A^(-1)*(B - E)
  // The result is S^T directly (S transposed)
  for(i = 0; i < N; i++) {
    for(j = 0; j < NBAR; j++) {
      mat_st[j*N + i] = 0;
      for(k = 0; k < N; k++) {
        mat_st[j*N + i] += mat_a_inv[i*N + k] * mat_b[k*NBAR + j];
      }
    }
  }
}

// Reconstruct the KEM secret key from the public key and a memory dump
// The offset should point to the error matrix location in the dump
void get_secret_key(uint8_t sk[LEN_SK], const long offset_mat_e, char *dump_file_name, const char *pk_file_name) {
  uint8_t *seed_a = &sk[LEN_SEC]; // second part of secret key
  uint8_t *b = &sk[LEN_SEC + LEN_A]; // third part of secret key
  uint16_t *mat_st = (uint16_t *)(&sk[LEN_SEC + LEN_PK]); // fourth part of secret key
  uint8_t *pkh = &sk[LEN_SEC + LEN_PK + 2*N*NBAR]; // last part of secret key
  uint16_t mat_a[N*N];
  uint16_t mat_b[N*NBAR];
  uint16_t mat_e[N*NBAR];

  // public key (seed_a + pack(B))
  read_file(seed_a, LEN_PK, pk_file_name, 0);

  // load matrix A and B
  generate_matrix_a(mat_a, seed_a);
  unpack(mat_b, b);

  // get error matrix E from dump
  read_file((uint8_t *)mat_e, 2*N*NBAR, dump_file_name, offset_mat_e);

  // calculate S^T with A^(-1)*(B - E)
  get_matrix_st(mat_st, mat_a, mat_b, mat_e);

  // calculate pkh = SHAKE256(pk, LEN_SEC)
  shake256(pkh, LEN_SEC, seed_a, LEN_PK);
}

int main(int argc, char *argv[]) {
  char *pk_file_name, *dump_file_name, *ct_file_name;
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  long offset_mat_e;
  uint8_t ss[CRYPTO_BYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  int i;

  if (argc != 5) {
    fprintf(stderr, "Usage: ./solution PK_FILE_NAME CIPHERTEXT_FILE_NAME DUMP_FILE_NAME OFFSET_MAT_E\n");
    exit(EXIT_FAILURE);
  }

  pk_file_name = argv[1];
  ct_file_name = argv[2];
  dump_file_name = argv[3];
  offset_mat_e = strtol(argv[4], NULL, 10);

  // get secret key
  get_secret_key(sk, offset_mat_e, dump_file_name, pk_file_name);

  // load ciphertext
  read_file(ct, CRYPTO_CIPHERTEXTBYTES, ct_file_name, 0);

  // decapsulation
  if (crypto_kem_dec_Frodo1344(ss, ct, sk) != 0) {
    fprintf(stderr, "Error: decapsulation failed\n");
    exit(EXIT_FAILURE);
  }

  for(i = 0; i < CRYPTO_BYTES; i++) {
    printf("%02x", ss[i]);
  }
  printf("\n");

  return 0;
}
