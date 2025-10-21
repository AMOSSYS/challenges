#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <stdint.h>
#include <stddef.h>

#define LEN_SK  43088
#define LEN_PK  21520
#define N       1344
#define NBAR    8
#define LEN_A   16
#define LEN_SEC 32

int inverse_matrix(uint16_t mat_inv[N*N], const uint16_t mat[N*N]);

#endif
