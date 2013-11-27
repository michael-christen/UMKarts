#include "xbee.h"

void xbee_txpt_init(struct xbee_packet * xp) {
	xp->payload[0] = XBEE_API_TX_REQUEST;
	xp->payload[10] = 0xFF; /* Reserved by spec */
	xp->payload[11] = 0xFE; /* Reserved by spec */
}

uint8_t * xbee_txpt_payload_start(struct xbee_packet *xp) {
	return xp->payload + 14;
}

void xbee_txpt_set_frame_id(struct xbee_packet * xp, uint8_t frameId) {
	xp->payload[1] = frameId;
}

void xbee_txpt_set_payload_size(struct xbee_packet *xp, uint16_t payload_size) {
	xp->len = payload_size + 14; /* Packet length is payload_size + 14 for extra data */
}

void xbee_txpt_set_options(struct xbee_packet * xp, uint8_t opts) {
	xp->payload[13] = opts; /* 13th slot is the options slot */
}

void xbee_txpt_set_radius(struct xbee_packet *xp, uint8_t rad) {
	xp->payload[12] = rad; /* 12th slot is the radius slot */
}

void xbee_txpt_set_destaddress(struct xbee_packet *xp, uint64_t address) {
	int i;
	for (i = 0; i < sizeof(uint64_t); i++) {
		xp->payload[2 + i] = (uint8_t) (((uint64_t) address >> (sizeof(uint64_t) - (i+1))*8) & 0xFF);
	}
}

