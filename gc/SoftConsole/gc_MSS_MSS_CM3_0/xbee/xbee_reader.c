#include "xbee_reader.h"

void XBeeReaderInit(struct xbee_reader * xr, struct xbee_packet *xp) {
	xr->xp = xp;
	xr->state = XBEE_READ_START_BYTE;
}

uint8_t *XBeeReaderRead(struct xbee_reader * xr, uint8_t * start, uint8_t * end) {
	while (start != end) {
		if (*start == XBEE_START_BYTE && xr->state != XBEE_READ_START_BYTE) {
			xr->state = XBEE_READ_DONE_UNEXPECTED_START_BYTE;
			return start;
		}
		switch (xr->state) {
		case XBEE_READ_START_BYTE:
			if (*start++ == XBEE_START_BYTE) {
				xr->state = XBEE_READ_FIRST_LEN;
				xr->xp->len = 0;
				xr->checksum = 0;
				xr->pos = 0;
			}
			break;
		case XBEE_READ_FIRST_LEN:
			if (*start == XBEE_ESCAPE_BYTE) {
				xr->state = XBEE_READ_FIRST_LEN_ESCAPED;
			} else {
				xr->xp->len = ((uint16_t) (((uint16_t) (*start)) << 8));
				xr->state = XBEE_READ_SECOND_LEN;
			}
			start++;
			break;
		case XBEE_READ_FIRST_LEN_ESCAPED:
			xr->xp->len = ((uint16_t) (((uint16_t) ((*start++) ^ XBEE_XOR_BYTE)
					<< 8)));
			xr->state = XBEE_READ_SECOND_LEN;
			break;
		case XBEE_READ_SECOND_LEN:
			if (*start == XBEE_ESCAPE_BYTE) {
				xr->state = XBEE_READ_SECOND_LEN_ESCAPED;
			} else {
				xr->xp->len += ((uint16_t) ((*start)));
				xr->state = XBEE_READ_DATA;
			}
			start++;
			break;
		case XBEE_READ_SECOND_LEN_ESCAPED:
			xr->xp->len += ((uint16_t) (*start++) ^ XBEE_XOR_BYTE);
			xr->state = XBEE_READ_DATA;
			break;
		case XBEE_READ_DATA:
			if (*start == XBEE_ESCAPE_BYTE) {
				xr->state = XBEE_READ_DATA_ESCAPED;
			} else {
				xr->checksum += *start;
				xr->xp->payload[xr->pos++] = *start;
				if (xr->pos == xr->xp->len) {
					xr->state = XBEE_READ_CHECKSUM;
				}
			}
			start++;
			break;
		case XBEE_READ_DATA_ESCAPED:
			xr->checksum += (*start ^ XBEE_XOR_BYTE);
			xr->xp->payload[xr->pos++] = *start++ ^ XBEE_XOR_BYTE;
			if (xr->pos == xr->xp->len) {
				xr->state = XBEE_READ_CHECKSUM;
			} else {
				xr->state = XBEE_READ_DATA;
			}
			break;
		case XBEE_READ_CHECKSUM:
			if (*start == XBEE_ESCAPE_BYTE) {
				xr->state = XBEE_READ_CHECKSUM_ESCAPED;
				start++;
			} else {
				if (*start + xr->checksum == 0xFF) {
					xr->state = XBEE_READ_DONE_GOOD;
				} else {
					xr->state = XBEE_READ_DONE_BAD_CHECKSUM;
				}
				start++;
				return start;
			}
			break;
		case XBEE_READ_CHECKSUM_ESCAPED:
			if ((*start ^ XBEE_XOR_BYTE) + xr->checksum == 0xFF) {
				xr->state = XBEE_READ_DONE_GOOD;
			} else {
				xr->state = XBEE_READ_DONE_BAD_CHECKSUM;
			}
			start++;
			return start;
			break;
		case XBEE_READ_DONE_UNEXPECTED_START_BYTE:
		case XBEE_READ_DONE_BAD_CHECKSUM:
		case XBEE_READ_DONE_GOOD:
			return start;
		}
	}
	return start;
}

uint8_t XBeeReaderDone(struct xbee_reader * xr) {
	if (xr->state == XBEE_READ_DONE_UNEXPECTED_START_BYTE || xr->state
			== XBEE_READ_DONE_BAD_CHECKSUM || xr->state == XBEE_READ_DONE_GOOD) {
		return 1;
	}
	return 0;
}

uint8_t XBeeReaderGood(struct xbee_reader * xr) {
	return xr->state == XBEE_READ_DONE_GOOD;
}

uint8_t XBeeReaderGoodOrBadCheckSum(struct xbee_reader * xr) {
	return xr->state == XBEE_READ_DONE_BAD_CHECKSUM || XBeeReaderGood(xr);
}

uint8_t XBeeReaderCheckCheckSum(struct xbee_reader *xr) {
	return XBeeReaderGood(xr);
}
