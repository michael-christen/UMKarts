#ifndef __MESSAGES_H_
#define __MESSAGES_H_
#include <inttypes.h>
#include "xbee.h"

/* Messages have to following fields:
 *  - Message type (uint8_t)
 *  - Message ID   (uint8_t)
 *  - Message Flags (uint8_t)
 *  - Message payload
 * Message length will be stored in the xbee protocol. (Sad face)
 * Message flags may contain:
 *  - 0x01 - No Application level ACK required
 */
#define XBEE_MESSAGE_MAX_SIZE      MAX_XBEE_TX_PAYLOAD_SIZE - 3

#define XBEE_MESSAGE_PRINTF        (0x00)
#define XBEE_MESSAGE_GAME_HOST     (0x02)
#define XBEE_MESSAGE_GAME_JOIN     (0x03)
#define XBEE_MESSAGE_GAME_START    (0x04)
#define XBEE_MESSAGE_GAME_OVER     (0x05)
#define XBEE_MESSAGE_GAME_EVENT    (0x06)
#define XBEE_MESSAGE_PLAYER_LEFT   (0x07)
#define XBEE_MESSAGE_ACK           (0x80)

#define XBEE_APP_OPT_NO_ACK        (0x01)
#define XBEE_APP_OPT_ACK           (0x00)

void send_message_init();

/**
 * Prints over xbee a message to be printed by someone else, probably the listener.
 *
 * This function prints a message over the network, to the specified listener. It should
 * be used for debugging purposes, and can be called from wherever you want whenver you want,
 * it should deal with interrupts no problem. The constraint is that the final length of the
 * message MUST be less than max payload size of one xbee packet. If it is great, the message
 * will be truncated, and no error will be returned.
 * TODO: Return an error if message is truncated.
 */
int xbee_printf(const char * format_string, ...);

int message_game_host();
int message_game_join(uint64_t host_address); 
int message_game_start(uint64_t * players, uint8_t num_players); 
int message_game_over(); 
int message_game_event(uint64_t address, uint8_t subject, uint8_t object, uint8_t action, uint8_t item, uint8_t flags);

int message_ack(uint64_t address, uint8_t frame_id, uint8_t msg_id, uint8_t failure);

int message_player_left(uint64_t address);
/**
 * Sends a new message off to all available XBee's.
 */
int send_message(uint8_t message_type, uint8_t app_flags, uint8_t * data, uint16_t data_len);
int send_message_address(uint64_t address, uint8_t message_type, uint8_t app_flags, uint8_t * data, uint16_t data_len);

void send_message_ack(uint8_t frame_id);
int send_message_retry(uint8_t frame_id);

#endif /* __MESSAGES_H_ */
