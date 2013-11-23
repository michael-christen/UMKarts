#include "xbee.h"


uint8_t xbee_packet_api_id(struct xbee_packet * xp) {
  return *(xp->payload);
}

uint8_t xbee_packet_frame_id(struct xbee_packet * xp) {
  /* Should check to make sure that this packet is properly formed to have
   * a frameID
   */
  return xp->payload[1];
}

enum XBeeModemStatus xbee_packet_modem_status(struct xbee_packet * xp) {
  /*ASSERT(xbee_packet_api_id(xp) == XBEE_API_MODEM);*/
  return (enum XBeeModemStatus) xp->payload[1];
}

