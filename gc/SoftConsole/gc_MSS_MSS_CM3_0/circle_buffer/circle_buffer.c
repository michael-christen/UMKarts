#include "circle_buffer.h"
#include "atomics.h"

void CircularBufferInit(CircularBuffer * cb, void ** buffer, size_t capacity) {
  size_t i;
  cb->buffer = buffer;
  for (i = 0; i < capacity; i++) cb->buffer[i] = NULL;
  cb->len = capacity;
  cb->read = 0;
  cb->write = 0;
}


int CircularBufferWrite(CircularBuffer *cb, void *item) {
  size_t old_write, new_write;
  do {
    old_write = *((volatile size_t *) &(cb->write));
    new_write = old_write + 1;
    if (new_write >= cb->len) new_write = 0;
    /* If were at new_write == cb->read, then we're out of room */
    if (new_write == cb->read) {
      return -EBUFFULL;
    }
  }while(atomic_cmpxchg_4((int *) &(cb->write), (int) new_write, (int) old_write) != 0);

  cb->buffer[old_write] = item;

  return 0;
}

void * CircularBufferRead(CircularBuffer *cb) {
  void *item;
  if (cb->read == cb->write) {
    return NULL;
  }
  item = cb->buffer[cb->read];
  /* Can occur if we interrupt the write functionality before the item was copied
   * into the buffer. Race condition, so safe call is to return empty buffer
   */
  if (item == NULL) {
    return NULL;
  }

  cb->buffer[cb->read] = NULL;

  cb->read = cb->read + 1;
  if (cb->read >= cb->len) cb->read = 0;

  return item;
}
