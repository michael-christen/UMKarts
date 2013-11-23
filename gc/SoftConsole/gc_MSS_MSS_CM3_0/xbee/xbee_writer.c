#include "xbee_writer.h"

static uint8_t _check_escape(uint8_t data); 

void XBeeWriterInit(struct xbee_writer * xw, const struct xbee_packet * xp) {
  xw->xp = xp;
  xw->state = XBEE_WRITE_START_BYTE;
  xw->pos = 0;
  xw->checksum = 0;
}

uint8_t XBeeWriterDone(struct xbee_writer * xw) {
  return xw->state == XBEE_WRITE_DONE;
}

uint8_t * XBeeWriterWrite(struct xbee_writer * xw, uint8_t * start, uint8_t * end) {
  while (start != end) {
    switch (xw->state) {
    case XBEE_WRITE_START_BYTE:
      *start++ = XBEE_START_BYTE;
      xw->state = XBEE_WRITE_FIRST_LEN;
      break;
    case XBEE_WRITE_FIRST_LEN:
      if (_check_escape(HIGHBYTE(xw->xp->len))) {
        *start++ = XBEE_ESCAPE_BYTE;
        xw->state = XBEE_WRITE_FIRST_LEN_ESCAPE;
      }
      else {
        *start++ = HIGHBYTE(xw->xp->len);
        xw->state = XBEE_WRITE_SECOND_LEN;
      }
      break;
    case XBEE_WRITE_FIRST_LEN_ESCAPE:
      *start++ = XBEE_XOR_BYTE ^ HIGHBYTE(xw->xp->len);
      xw->state = XBEE_WRITE_SECOND_LEN;
      break;
    case XBEE_WRITE_SECOND_LEN:
      if (_check_escape(LOWBYTE(xw->xp->len))) {
        *start++ = XBEE_ESCAPE_BYTE;
        xw->state = XBEE_WRITE_SECOND_LEN_ESCAPE;
      }
      else {
        *start++ = LOWBYTE(xw->xp->len);
        xw->state = XBEE_WRITE_DATA;
      }
      break;
    case XBEE_WRITE_SECOND_LEN_ESCAPE:
      *start++ = XBEE_XOR_BYTE ^ HIGHBYTE(xw->xp->len);
      xw->state = XBEE_WRITE_DATA;
      break;
    case XBEE_WRITE_DATA:
      /* Assuming packet size > 1 */
      if (_check_escape(xw->xp->payload[xw->pos])) {
        *start++ = XBEE_ESCAPE_BYTE;
        xw->state = XBEE_WRITE_DATA_ESCAPE;
      }
      else {
        *start++ = xw->xp->payload[xw->pos];
        xw->checksum += xw->xp->payload[xw->pos];
        ++(xw->pos);
        if (xw->pos == xw->xp->len) {
          xw->state = XBEE_WRITE_CHECKSUM;
        }
      }
      break;
    case XBEE_WRITE_DATA_ESCAPE:
      xw->checksum += xw->xp->payload[xw->pos];
      *start++ = XBEE_XOR_BYTE ^ xw->xp->payload[xw->pos];
      ++(xw->pos);
      if (xw->pos == xw->xp->len) {
        xw->state = XBEE_WRITE_CHECKSUM;
      }
      else {
        xw->state = XBEE_WRITE_DATA;
      }
      break;
    case XBEE_WRITE_CHECKSUM:
      xw->checksum = 0xFF - xw->checksum;
      if (_check_escape(xw->checksum)) {
        *start++ = XBEE_ESCAPE_BYTE;
        xw->state = XBEE_WRITE_CHECKSUM_ESCAPE;
      }
      else {
        *start++ = xw->checksum;
        xw->state = XBEE_WRITE_DONE;
      }
      break;
    case XBEE_WRITE_CHECKSUM_ESCAPE:
      *start++ = XBEE_XOR_BYTE ^ xw->checksum;
      xw->state = XBEE_WRITE_DONE;
      break;
    case XBEE_WRITE_DONE:
      return start;
    }
  }
  return start;
}
      



static uint8_t _check_escape(uint8_t data) {
	if (
	  data  ==  XBEE_START_BYTE   ||
	  data  ==  XBEE_ESCAPE_BYTE  ||
	  data  ==  XBEE_XON_BYTE     ||
	  data  ==  XBEE_XOFF_BYTE)  {
    return 1;
  }
  return 0;
}
