#ifndef FIPS202_H
#define FIPS202_H

#include <stddef.h>
#include <stdint.h>

#define SHAKE128_RATE 168
#define SHAKE256_RATE 136

#define PQC_SHAKEINCCTX_BYTES (sizeof(uint64_t)*26)
#define PQC_SHAKECTX_BYTES (sizeof(uint64_t)*25)

// Context for incremental API
typedef struct {
    uint64_t *ctx;
} shake128incctx;

// Context for non-incremental API
typedef struct {
    uint64_t *ctx;
} shake128ctx;

// Context for incremental API
typedef struct {
    uint64_t *ctx;
} shake256incctx;

// Context for non-incremental API
typedef struct {
    uint64_t *ctx;
} shake256ctx;

/* One-stop SHAKE128 call */
void shake128(uint8_t *output, size_t outlen,
              const uint8_t *input, size_t inlen);

/* One-stop SHAKE256 call */
void shake256(uint8_t *output, size_t outlen,
              const uint8_t *input, size_t inlen);

#endif