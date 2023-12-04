#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// gcc -static -c -fPIC challenge.c -o generate_key.o -lcrypto
// gcc generate_key.o -shared -o generate_key.so -lcrypto

unsigned char key[32] = { 0 };

void md5(unsigned char *in, unsigned char *out)
{
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, in, strlen(in));
	MD5_Final(out, &ctx);
}

unsigned char *generate_256bits_encryption_key(unsigned char *recipient_name)
{
	int i = 0;
	FILE *f = NULL;
	time_t now1 = 0L;
	time_t now2 = 0L;
	time_t delta = 0L;
	
	now1 = time(NULL);
	
	f = fopen("/dev/urandom", "rb");
	fread(&key, 1, 32, f);
	fclose(f);
	
	md5(recipient_name, key);
	
	now2 = time(NULL);
	delta = now2 - now1;
	
	key[8] = delta;
	
	return key;
}
