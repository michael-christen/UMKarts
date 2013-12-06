/*
 * convert.h
 */

#include <inttypes.h>

#ifndef CONVERT_H_
#define CONVERT_H_

uint32_t bytes_to_uint32_t(const uint8_t * bytes);
uint64_t bytes_to_uint64_t(const uint8_t * bytes);
void uint64_t_to_bytes(uint64_t in, uint8_t *buffer);

#endif /* CONVERT_H_ */
