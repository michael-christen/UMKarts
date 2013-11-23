#ifndef __XBEE_AT_H_
#define __XBEE_AT_H_

struct xp_at_command {
  uint8_t cmdId[2]; /* Commands are two bytes always */
  uint8_t frameId;
  uint8_t param[MAX_XBEE_PAYLOAD_SIZE - 5];
  uint16_t param_len;
};

struct xp_at_response {
  uint8_t cmdId[2];
  uint8_t frameeId;
  uint8_t param[MAX_XBEE_PAYLOAD_SIZE - 5];
  uint16_t param_len;
};

/* Set the passed in packet as an AT command to the xbee to be interpreted and
 * is saved immediately by the XBee. */
void xbpt_create_at_command(const struct xp_at_command * xpac, struct xbee_packet * xp); 
void xbpt_create_at_command_queue(const struct xp_at_command * xpac, struct xbee_packet * xp); 
void xbpt_get_at_command_response(const struct xbee_packet *xp, struct xp_at_response *);

#endif
