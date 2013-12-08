/*
 * convert.c
 */
#include "convert.h"

uint32_t bytes_to_uint32_t(const uint8_t * a) {
	return (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
}

uint64_t bytes_to_uint64_t(const uint8_t *a) {
	return ((uint64_t) a[0] << 56) | ((uint64_t) a[1] << 48) | ((uint64_t) a[2]
			<< 40) | ((uint64_t) a[3] << 32) | ((uint64_t) a[4] << 24)
			| ((uint64_t) a[5] << 16) | ((uint64_t) a[6] << 8)
			| (uint64_t) a[7];
}

void uint64_t_to_bytes(uint64_t in, uint8_t *buffer) {
	int i, j;
	for (i = 56, j = 0; i >= 0; i -= 8, j++) {
		buffer[j] = (in >> i) & 0xFF;
	}
}
