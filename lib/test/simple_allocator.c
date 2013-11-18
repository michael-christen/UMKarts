#include "simple_allocator.h"
#include <stdio.h>
#include <stddef.h>

struct TS {
  int x;
  int y;
  char hi[2];
};

struct TS test_array[32];
struct TS *test_array_ptr[32];

SimpleAllocator sa;

int main(int argc, char **argv) {
  size_t i;
  if (SimpleAllocatorInit(&sa, (void *) test_array, 32, sizeof(struct TS)) == 0) {
    printf("Success: Able to allocate 32 of something\n");
  }
  else {
    printf("ERROR: Should be able to initialize this allocator\n");
  }
  if (SimpleAllocatorInit(&sa, (void *) test_array, 33, sizeof(struct TS)) != -ESIMPLEALLOCATORTOOBIG) {
    printf("ERROR: Should not be able to initalize an allocator of this size\n");
  }

  for (i = 0; i < 32; i++) {
    if ((test_array_ptr[i] = SimpleAllocatorAlloc(&sa)) == NULL) {
      printf("ERROR: Should be able to allocate 32 items. Allocated %ld\n", i);
    }
  }
  if (SimpleAllocatorAlloc(&sa) != NULL) {
    printf("ERROR: Should not be able to allocate more than 32 items\n");
  }
  SimpleAllocatorFree(&sa, test_array_ptr[15]);
  if ((test_array_ptr[15] = SimpleAllocatorAlloc(&sa)) == NULL) {
    printf("ERROR: Should be able to write back into allocator after freeing\n");
  }

  return 0;
}
