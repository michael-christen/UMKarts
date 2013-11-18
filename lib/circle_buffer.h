#ifndef __CIRCULAR_BUFFER_
#define __CIRCULAR_BUFFER_

#define EBUFEMPTY 1
#define EBUFFULL  2

typedef struct {
  void * volatile buffer[];
  size_t len;
  size_t read;
  size_t write;
} CircularBuffer;

/**
 * Initializes a new circular buffer.
 *
 * All circular buffers are static, (no dynamic memory allocation), so we must
 * pass in a pointer to a statically allocated buffer. We then pass in the
 * size of the buffer.
 * 
 * @cb - Pointer to an unitialized buffer
 * @buffer - Pointer to statically allocated buffer of size 
 * @size - The size of each item (in bytes) in the buffer
 *
 * @ret - Void, there are no possible errors in this function
 */
void CircularBufferInit(CircularBuffer * cb, void ** buffer, size_t size);

/**
 * Write an item into the buffer.
 *
 * Writes a item into the buffer, or returns an error if the buffer is full.
 * This function is guarenteed to work to matter when it's called. It's safe to
 * call from an interrupt, as adding an item to the buffer has been made an
 * atomic operation. (This will not work with multiple cores though)
 *
 * @cb   - Initialized circular buffer
 * @item - Item to add to the queue
 *
 * @ret - 0 if item was successfully added to the queue, -EBUFFULL if the
 *        buffer is full.
 */
int CircularBufferWrite(CircularBuffer * cb, void *item);

/**
 * Reads an item from the buffer and then destroys it's spot in the buffer, or
 * returns an error if buffer is empty.
 *
 * Trys to read the next item from the buffer, or returns an error if the
 * buffer is empty.
 *
 * @cb   - Initilalized circular buffer
 * @item - Pointer to void * where we can store the item. Will be null if there
 *         is an error
 * 
 * @ret  - 0 if @item is valid, -EBUFEMPTY if the buffer is empty
 */
int CircularBufferRead(CircularBuffer * cb, void **item);

#endif
