#ifndef __XBEE_WRITER_H_
#define __XBEE_WRITER_H_
#include "xbee.h"

struct xbee_writer {
  struct xbee_packet * xp;
  enum {
    XBEE_WRITE_START_BYTE,
    XBEE_WRITE_FIRST_LEN,
    XBEE_WRITE_FIRST_LEN_ESCAPE,
    XBEE_WRITE_SECOND_LEN,
    XBEE_WRITE_SECOND_LEN_ESCAPE,
    XBEE_WRITE_DATA,
    XBEE_WRITE_DATA_ESCAPE,
    XBEE_WRITE_CHECKSUM,
    XBEE_WRITE_CHECKSUM_ESCAPE,
    XBEE_WRITE_DONE,
  } state;
  uint16_t pos;
  uint8_t checksum;
};

/**
 * Initializes the XBeeWriter object.
 *
 * The XBeeWriter object allows byte by byte copy of an API mode 2 xbee packet
 * into a buffer if any size. The struct xbee_writer stores the current state
 * of the XBee packet write so that we can start from where we left off last
 * time.
 *
 * @xw - Pointer to xw writer object.
 * @xp - Pointer to an initilized xbee_packet. We expect the xbee_packet to be
 *       uinitialized AND have len > 0. This code will break if len == 0.
 * @ret - (void) No possible errors
 */
void XBeeWriterInit(struct xbee_writer * xw, struct xbee_packet * xp);

/**
 * Makes the XBeeWriter try to fill start to end with bytes from the packet.
 *
 * Based on the current state of xbee_writer, we start at the place we left off
 * last time the XBeeWriterWrite function was called on this object. We're
 * writing data between start and end (as if they are iterators). We stop
 * outputting data when we have filled up that memory, or when we have finished
 * writing the output of the packet. We return a pointer to one past the last
 * element written, in case we stop writing before we actually hit the end ptr.
 *
 * @xw - Initialized xbee_writer
 * @start - Start of memory we can start writing too
 * @end - End of memory we can write too
 * @ret - Ptr to one past the last item written too
 */
uint8_t * XBeeWriterWrite(struct xbee_writer *xw, uint8_t * start, uint8_t * end);

/**
 * Reports if the entire packet has been written by this writter.
 *
 * Checks to see if we've made it all the way through the packet. Returns true
 * if we are done, false if not.
 */
uint8_t XBeeWriterDone(struct xbee_writer * xw); 


#endif
