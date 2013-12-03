#ifndef __MESSAGES_H_
#define __MESSAGES_H_

/* Messages have to following fields:
 *  - Message ID (uint8_t)
 *  - Message payload
 * Message length will be stored in the xbee protocol. (Sad face)
 */

#define XBEE_MESSAGE_PRINTF ((uint8_t) 0x00)

/**
 * Sends a new message off to all available XBee's.
 */
int send_message(uint8_t message_type, uint8_t * data, uint16_t data_len);

#endif /* __MESSAGES_H_ */
