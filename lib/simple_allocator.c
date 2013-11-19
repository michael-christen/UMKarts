#include "simple_allocator.h"


int SimpleAllocatorInit(SimpleAllocator * sa, void *buf, size_t num_items, size_t item_sz) {
  /* sizeof(uint32_t)*8 = number of bits we have */
  if (num_items > sizeof(sa->empty_mask)*8) return -ESIMPLEALLOCATORTOOBIG;
  sa->buf = buf;
  sa->num_items = num_items;
  sa->item_sz = item_sz;
  sa->next_free = sa->buf;
  sa->empty_mask = 0;
  return 0;
}

void * SimpleAllocatorAlloc(SimpleAllocator * sa) {
  size_t i;
  uint32_t old_empty, new_empty;
  for (i = 0; i < sa->num_items; i++) {
    old_empty = sa->empty_mask;
    if (!(old_empty & (0x1 << i))) {
      new_empty = old_empty | (0x1 << i);
      if (__sync_bool_compare_and_swap(&(sa->empty_mask), old_empty, new_empty)) {
        return sa->buf + i*sa->item_sz;
      }
    }
  }
  return NULL;
}

void SimpleAllocatorFree(SimpleAllocator * sa, void *data) {
  size_t i;
  for (i = 0; i < sa->num_items; i++) {
    if (sa->buf + i*sa->item_sz == data) {
      __sync_fetch_and_and(&(sa->empty_mask), ~(0x1 << i));
      return;
    }
  }
}
