#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include "ecc.h"

#define RANDOM_BUFFER_LENGTH 144
#define MAX_N_SIG 16

const char *messages[16] = {
  "Lorem ipsum dolor sit amet elit ullamco incididunt deserunt aliq",
  "Labore magna amet enim cillum elit est aute ea qui reprehenderit",
  "Laboris nulla minim incididunt nostrud nisi laboris consectetur ",
  "Pariatur reprehenderit minim enim dolore tempor cupidatat qui Lo",
  "Ex officia nostrud esse Lorem dolor dolore officia minim proiden",
  "Laboris id aliquip nisi in proident commodo labore veniam. Dolor",
  "Velit fugiat sunt, nisi labore irure dolor id cupidatat est. Ess",
  "Excepteur laborum nisi eu ut sunt duis veniam in qui eiusmod fug",
  "Ea aute ex anim laboris velit voluptate est laborum ut. Cillum s",
  "Non laborum dolor et cillum nisi adipisicing voluptate aute anim",
  "Tempor officia cillum duis esse exercitation ut. Sit exercitatio",
  "Ad enim, consectetur aute laborum id consectetur laborum Lorem n",
  "Ea in aliqua reprehenderit nostrud anim dolore sint dolor veniam",
  "Sit Lorem duis sint incididunt aute, non cupidatat aute enim. Al",
  "Eu eiusmod nisi do irure enim, occaecat. Fugiat et amet laborum ",
  "Dolore dolor et proident, laborum officia ad aliquip aute aliqui",
};

int main(int argc, char *argv[]) {
  long n_sig;
  uint8_t sk[32];
  uint8_t pk[64];
  uint8_t sig[64];
  uint8_t random_buffer[RANDOM_BUFFER_LENGTH];
  rnd_engine_t rnd;
  int i, j;
  FILE *fp = NULL;
  
  /* parameters */
  if (argc != 3) {
    fprintf(stderr, "Usage: gen_sig <NUMBER OF SIGNATURES> <OUTPUT FILE NAME>\n");
    exit(EXIT_FAILURE);
  }

  n_sig = strtol(argv[1], NULL, 10);
  if (n_sig < 1 || n_sig > 16) {
    fprintf(stderr, "Error: the number of signatures must be between 1 and 16.\n");
    exit(EXIT_FAILURE);
  }

  fp = fopen(argv[2], "w");
  if (fp == NULL) {
    fprintf(stderr, "Cannot write to file %s.\n", argv[2]);
    exit(EXIT_FAILURE);
  }

  /* generate a key pair */
  if (getrandom(random_buffer, RANDOM_BUFFER_LENGTH, GRND_NONBLOCK) != RANDOM_BUFFER_LENGTH) {
    fprintf(stderr, "Cannot fill random buffer.\n");
    return EXIT_FAILURE;
  }
  rnd.random_buffer = random_buffer;
  rnd.random_buffer_length = RANDOM_BUFFER_LENGTH;
  rnd.random_index = 0;

  /* 16 bytes are taken from rnd */
  if (wei25519_keypair(pk, sk, &rnd) != 0) {
    fprintf(stderr, "Error: while generating key pair.\n");
    exit(EXIT_FAILURE);
  }

  /* write keypair in output file */
  fprintf(fp, "# secret key: ");
  for (j = 0; j < 32; j++) {
    fprintf(fp, "%02x", sk[j]);
  }
  fprintf(fp, "\n\n# public key: ");
  for (j = 0; j < 64; j++) {
    fprintf(fp, "%02x", pk[j]);
  }
  fprintf(fp, "\n\n# Signatures\n");

  /* rnd still has 144 - 16 = 128 bytes, enough for 16 signatures */
  for (i = 0; i < n_sig; i++) {
    wei25519_sign(sig, (unsigned char *)messages[i], 64, sk, &rnd);
    for (j = 0; j < 64; j++) {
      fprintf(fp, "%02x", sig[j]);
    }
    fprintf(fp, "\n");
  }

  if (fp != NULL) {
    fclose(fp);
  }

  return EXIT_SUCCESS;
}