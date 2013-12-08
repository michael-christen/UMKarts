#ifndef _XBEE_READER_H_
#define _XBEE_READER_H_
#include "xbee.h"
#include <inttypes.h>

struct xbee_reader {
  struct xbee_packet *xp;
  enum {
    XBEE_READ_START_BYTE,
    XBEE_READ_FIRST_LEN,
    XBEE_READ_FIRST_LEN_ESCAPED,
    XBEE_READ_SECOND_LEN,
    XBEE_READ_SECOND_LEN_ESCAPED,
    XBEE_READ_DATA,
    XBEE_READ_DATA_ESCAPED,
    XBEE_READ_CHECKSUM,
    XBEE_READ_CHECKSUM_ESCAPED,
    XBEE_READ_DONE_GOOD,
    XBEE_READ_DONE_BAD_CHECKSUM,
    XBEE_READ_DONE_UNEXPECTED_START_BYTE,
  } state;
  uint16_t pos;
  uint8_t checksum;
};

void XBeeReaderInit(struct xbee_reader * xr, struct xbee_packet *xp);
uint8_t *XBeeReaderRead(struct xbee_reader * xr, uint8_t * start, uint8_t * end);
uint8_t XBeeReaderDone(struct xbee_reader * xr);
uint8_t XBeeReaderGood(struct xbee_reader * xr);
uint8_t XBeeReaderGoodOrBadCheckSum(struct xbee_reader * xr);
uint8_t XBeeReaderCheckCheckSum(struct xbee_reader *xr);

#endif
