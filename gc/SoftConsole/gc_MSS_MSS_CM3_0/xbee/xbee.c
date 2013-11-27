#include "xbee.h"
#include "atomics.h"

static uint8_t _XBeeFrameId = 1;


uint8_t xbee_packet_api_id(struct xbee_packet * xp) {
  return *(xp->payload);
}

uint8_t xbee_packet_frame_id(struct xbee_packet * xp) {
  /* Should check to make sure that this packet is properly formed to have
   * a frameID
   */
  return xp->payload[1];
}

uint8_t xbee_next_frame_id() {
	uint8_t ret_val, new_val;
	do {
		ret_val = (*((volatile uint8_t *) &_XBeeFrameId));
		new_val = ret_val + 1;
		if (new_val == 0) new_val = 1;
	}while (atomic_cmpxchg_1(&_XBeeFrameId, ret_val, new_val) == 0);
	return ret_val;
}

enum XBeeModemStatus xbee_packet_modem_status(struct xbee_packet * xp) {
  /*ASSERT(xbee_packet_api_id(xp) == XBEE_API_MODEM);*/
  return (enum XBeeModemStatus) xp->payload[1];
}

