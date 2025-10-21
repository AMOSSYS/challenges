#include <stdlib.h>
#include <string.h>
#include "inverses.h"
#include "solution.h"

// This is an implementation of Gaussian elimination
// to invert the matrix A of dimension 1344*1344
// (much faster than Sagemath)

// Find a pivot in the given column
int find_pivot_index(const uint16_t mat[2*N*N], const int col) {
  int i;
  int index = -1;
  for(i = col; i < N; i++) {
    if (mat[i*(2*N) + col] & 1) {
      index = i;
      break;
    }
  }
  return index;
}

// Multiply a line by a scalar from a given index
// (the first coefficients of the line are supposed to be zero)
void matrix_mul_line(uint16_t row[2*N], const uint16_t value, const int start) {
  int i;
  for(i = start; i < 2*N; i++) {
    row[i] *= value;
  }
}

// Swap two lines of the matrix
void matrix_swap_lines(uint16_t row1[2*N], uint16_t row2[2*N]) {
  int i;
  for(i = 0; i < 2*N; i++) {
    row1[i] ^= row2[i];
    row2[i] ^= row1[i];
    row1[i] ^= row2[i];
  }
}

// Calculate a^-1 mod 2^16 using a table look-up
uint16_t inverse_mod_q(const uint16_t a) {
  return INVERSE_MOD_Q[a >> 1];
}

// Add a multiple of row2 to row1
void matrix_muladd_line(uint16_t row1[2*N], const uint16_t row2[2*N], int start) {
  uint16_t coef = row1[start];
  int i;
  for(i = start; i < 2*N; i++) {
    row1[i] -= coef*row2[i];
  }
}

// Calculate the inverse of a matrix
// Returns 1 if it is inverted, or 0 if not invertible
int inverse_matrix(uint16_t mat_inv[N*N], const uint16_t mat[N*N]) {
  uint16_t *m;
  uint16_t pivot_inverse;
  int i, j, k;
  int invertible = 1;
  
  m = (uint16_t *)malloc((N*N*2)*sizeof(uint16_t));

  // initialization
  memset((uint8_t *)m, 0, N*N*2*2);
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++) {
      m[i*(2*N) + j] = mat[i*N + j];
    }
    m[i*(2*N) + (i + N)] = 1;
  }


  for(j = 0; j < N; j++) {    
    // find pivot
    i = find_pivot_index(m, j);

    // not invertible, stop here
    if (i == -1) {
      invertible = 0;
      break;
    }
    pivot_inverse = inverse_mod_q(m[i*(2*N) + j]);

    // invert line
    matrix_mul_line(&m[i*(2*N)], pivot_inverse, j);

    // zero in column
    for(k = 0; k < i; k++) {
      matrix_muladd_line(&m[k*(2*N)], &m[i*(2*N)], j);
    }
    for(k = i + 1; k < N; k++) {
      matrix_muladd_line(&m[k*(2*N)], &m[i*(2*N)], j);
    }

    // swap lines
    if (i != j) {
      matrix_swap_lines(&m[i*(2*N)], &m[j*(2*N)]);
    }

  }

  if (invertible) {
    for(i = 0; i < N; i++) {
      for(j = 0; j < N; j++) {
        mat_inv[i*N + j] = m[i*(2*N) + (j + N)];
      }
    }
  }

  free(m);
  return invertible;
}
