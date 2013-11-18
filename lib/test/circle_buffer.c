#include "circle_buffer.h"
#include <stdlib.h>
#include <stdio.h>

void *void_ptr_buf[10];

int main(int argc, char **argv) {
  int a;
  int * a_ptr, *z_ptr;
  size_t i;
  CircularBuffer cb;
  CircularBufferInit(&cb, void_ptr_buf, sizeof(void_ptr_buf) / sizeof(void *));
  a = 1;
  if (CircularBufferRead(&cb, (void **) &z_ptr) == -EBUFEMPTY) {
    printf("Success: Returns error when reading from empty buffer\n");
  }
  else {
    printf("Error: Should be error when reading from empty buffer\n");
  }
  if (CircularBufferWrite(&cb, (void *) &a) == 0) {
    printf("Success: No errors writing to buffer\n");
  }
  else {
    printf("ERROR: Can't write to empty buffer\n");
  }

  if (CircularBufferRead(&cb, (void **) &a_ptr) == 0) {
    printf("Success: Can read back value from buffer\n");
    if (*a_ptr == a) {
      printf("Success: Read back correct value\n");
    }
    else {
      printf("ERROR: Expected %d but got %d\n", a, *a_ptr);
    }
  }
  else {
    printf("ERROR: Should be able to read back value from queue, but can't\n");
  }

  /* DO NOT EVER DO THIS!! This is just for a test, but notice we are always
   * writing the same memory location to multiple places in the buffer.
   */
  for (i = 0; i < (sizeof(void_ptr_buf) / sizeof(void *) - 1); i++) {
    if (CircularBufferWrite(&cb, (void *) &i) != 0) {
      printf("ERROR: Unabled to write to buffer with empty space\n");
    }
  }
  if (CircularBufferWrite(&cb, (void *) &i) != -EBUFFULL) {
    printf("ERROR:Expected BUFFULL error, but didn't get that\n");
  }

  return 0;
}
