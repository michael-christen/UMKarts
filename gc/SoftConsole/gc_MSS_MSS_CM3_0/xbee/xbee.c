#include "xbee.h"

static void _xbee_packet_at_command(uint8_t api_id, struct xp_at_command * xpac, struct xbee_packet * xp); 

uint8_t xbee_packet_api_id(struct xbee_packet * xp) {
  return *(xp->payload);
}

enum XBeeModemStatus xbee_packet_modem_status(struct xbee_packet * xp) {
  ASSERT(xbee_packet_api_id(xp) == XBEE_API_MODEM);

  return (enum XBeeModemStatus) xp->payload[1];
}

void xbee_packet_at_command_queue(struct xp_at_command * xpac, struct xbee_packet * xp) {
  _xbee_packet_at_command(XBEE_API_AT_COMMAND, xpac, xp);
}

void xbee_packet_at_command(struct xp_at_command * xpac, struct xbee_packet * xp) {
  _xbee_packet_at_command(XBEE_API_AT_QUEUE_COMMAND, xpac, xp);
}

static void _xbee_packet_at_command(uint8_t api_id, struct xp_at_command * xpac, struct xbee_packet * xp) {
  uint16_t i;
  xp->len = xpac->param_len + 4;
  xp->payload[0] = api_id;
  xp->payload[1] = xpac->frameId;
  xp->payload[2] = xpac->cmdId[0];
  xp->payload[3] = xpac->cmdId[1];
  for (i = 0; i < xpac->param_len; i++) {
    xp->payload[4 + i] = xpac->param[i];
  }
}
