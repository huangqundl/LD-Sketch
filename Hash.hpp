#ifndef __Hash_hpp__
#define __Hash_hpp__

/**
 * hash function
 * @param data the binary to be hashed
 * @param n the length of binary to be hashed
 */
static unsigned int AwareHash(const unsigned char* data, unsigned int n) {
	unsigned int hash = 388650253;
	unsigned int scale = 388650319;
	unsigned int hardener  = 1176845762;
	while( n ) {
		hash *= scale;
		hash += *data++;
		n--;
	}
	return hash ^ hardener;
}

#endif
