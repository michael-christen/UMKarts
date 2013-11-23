#ifndef __SIMPLE_ALLOCATOR_H_
#define __SIMPLE_ALLOCATOR_H_
#include "inttypes.h"
#include "stddef.h"

#define ESIMPLEALLOCATORTOOBIG 1

typedef struct {
  void *buf;
  size_t num_items;
  size_t item_sz;
  void *next_free;
  volatile uint32_t empty_mask;
} SimpleAllocator;

int SimpleAllocatorInit(SimpleAllocator * sa, void *buf, size_t num_items, size_t item_sz);

void * SimpleAllocatorAlloc(SimpleAllocator * sa);
void SimpleAllocatorFree(SimpleAllocator * sa, const void *);

#endif
