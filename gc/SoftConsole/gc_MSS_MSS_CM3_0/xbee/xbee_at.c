#include "xbee.h"

static void _xbpt_create_at_command(uint8_t api_id, const struct xp_at_command * xpac, struct xbee_packet * xp); 

void xbpt_create_at_command_queue(struct xp_at_command * xpac, struct xbee_packet * xp) {
  _xbpt_create_at_command(XBEE_API_AT_COMMAND, xpac, xp);
}

void xbpt_create_at_command(const struct xp_at_command * xpac, struct xbee_packet * xp) {
  _xbpt_create_at_command(XBEE_API_AT_QUEUE_COMMAND, xpac, xp);
}

void xbpt_get_at_command_response(const struct xbee_packet *xp, struct xp_at_response * xpatr) {



static void _xbpt_create_at_command(uint8_t api_id, const struct xp_at_command * xpac, struct xbee_packet * xp) {
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
