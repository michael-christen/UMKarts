#include "xbee.h"

void xbpt_create_tx_request(const struct xp_tx_request * xptx, struct xbee_packet * xp) {
  uint16_t i, j;
  xp->len = xptx->data_len + 14;
  xp->payload[0] = XBEE_API_TX_REQUEST;
  xp->payload[1] = xptx->frameID;
  for (i = 7, j = 2; i >= 0; i--,j++) {
    xp->payload[i] = (xptx->dest_addres >> (8)*i) & 0xFF;
  }
  xp->payload[10] = 0xFF; /* Reserved, by standard */
  xp->payload[11] = 0xFE;
  xp->payload[12] = xptx->broadcast_radius;
  xp->payload[13] = xptx->txoptions;
  for (i = 0; i < xptx->data_len; i++) {
    xp->payload[14 + i] xptx->refdata[i];
  }
}


