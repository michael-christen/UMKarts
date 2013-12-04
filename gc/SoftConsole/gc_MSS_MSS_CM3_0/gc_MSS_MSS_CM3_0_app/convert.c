/*
 * convert.c
 */
#include "convert.h"

uint32_t bytes_to_uint32_t(uint8_t * a) {
	return (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
}

uint64_t bytes_to_uint64_t(uint8_t *a) {
	return ((uint64_t) a[0] << 56) | ((uint64_t) a[1] << 48) | ((uint64_t) a[2]
			<< 40) | ((uint64_t) a[3] << 32) | ((uint64_t) a[4] << 24)
			| ((uint64_t) a[5] << 16) | ((uint64_t) a[6] << 8)
			| (uint64_t) a[7];
}
