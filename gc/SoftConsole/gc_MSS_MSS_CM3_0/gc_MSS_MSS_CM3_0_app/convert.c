/*
 * convert.c
 */
#include "convert.h"

uint32_t bytes_to_uint32_t(uint8_t * a) {
	return (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
}
