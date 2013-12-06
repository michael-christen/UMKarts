#include "xbee.h"
#include "xbee_interface.h"
#include "mario_xbee.h"
#include "player.h"
#include "convert.h"
#include "game.h"
#include "messages.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define MAX_PRINTF_SIZE MAX_XBEE_TX_PAYLOAD_SIZE - 1

static void _mario_xbee_interpret_at_response(struct xbee_packet *xp);
static int _mario_xbee_interpret_rx_packet(struct xbee_packet *xp);
static int _mario_xbee_interpret_tx_status(struct xbee_packet *xp);

int mario_xbee_interpret_packet(struct xbee_packet * xp) {
	switch (xbee_packet_api_id(xp)) {
	case XBEE_API_AT_COMMAND_RESPONSE:
		_mario_xbee_interpret_at_response(xp);
		break;
	case XBEE_API_MODEM:
		xbee_printf("XBee Modem Status: %d\n", xp->payload[1]);
		break;
	case XBEE_API_TX_STATUS:
		_mario_xbee_interpret_tx_status(xp);
		break;
	case XBEE_API_RX:
		_mario_xbee_interpret_rx_packet(xp);
		break;
	}
	return 0;
}

static inline uint8_t _mario_xbee_at_cmd_is(const struct xbee_packet *xp, const char * cmd) {
	return (xp->payload[2] == cmd[0] && xp->payload[3] == cmd[1]);
}

static void _mario_xbee_interpret_at_response(struct xbee_packet *xp) {
	uint64_t address;
	const struct xbee_packet *sent_xp;
	sent_xp = xbee_interface_tx_get_packet_by_frame_id(xp->payload[1]);
	xbee_interface_free_packet(sent_xp);
	if (xp->payload[4] != 0x00) {
		xbee_printf("Invalid xbee packet: %c%c, %d\r\n", xp->payload[2],
				xp->payload[3], xp->payload[4]);
	} else if (_mario_xbee_at_cmd_is(xp, "SH")) {
		player_set_high_address(bytes_to_uint32_t(&(xp->payload[5])));
	} else if (_mario_xbee_at_cmd_is(xp, "SL")) {
		player_set_low_address(bytes_to_uint32_t(&(xp->payload[5])));
	} else if (_mario_xbee_at_cmd_is(xp, "ND")) {
		address = bytes_to_uint64_t(xp->payload + 7);
		if (player_get_driver_from_address(address) != DRIVER_INVALID) {
			player_add_player(address);
		}
	} else {
		/* Should we print an error in this case? */
	}
}

static int _mario_xbee_interpret_rx_packet(struct xbee_packet *xp) {
	int err, i;
	uint64_t sender = bytes_to_uint64_t(xp->payload + 1);
	uint8_t msg_type = xp->payload[12];
	/* uint8_t msg_opts = xp->payload[13]; */
	/* uint8_t msg_id   = xp->payload[14]; */
	uint8_t *data = xp->payload + 15;
	/*uint16_t data_len = xp->len - 15; */
	switch (msg_type) {
	case XBEE_MESSAGE_PRINTF:
		/* We ignore PRINTF calls */
		break;
	case XBEE_MESSAGE_GAME_HOST_ANNOUNCE:
		if (g_game_state == GAME_HOST) {
			if (sender < player_get_address_from_driver(DRIVER)) {
				err = game_trans_host_to_join(sender);
				if (err == 0) {
					message_game_join(sender);
				}
				else {
					xbee_printf("Unable to switch into GAME JOIN state\n");
				}
			}
		}
		else if (g_game_state == GAME_JOIN) {
			g_game_host = sender;
			message_game_join(sender);
		}
		else {
			xbee_printf("Ignoring GAME_HOST packet because in state %s\n", g_game_state_str[g_game_state]);
		}
		break;
	case XBEE_MESSAGE_GAME_JOIN:
		if (g_game_state == GAME_HOST) {
			err = player_add_player(sender);
			if (err < 0) {
				xbee_printf("Unable to find driver for player: %llx\n", sender);
			}
		}
		else {
			xbee_printf("Ignoring GAME_JOIN packet because in state %s\n", g_game_state_str[g_game_state]);
		}
		break;
	case XBEE_MESSAGE_LEAVE_GAME:
		if (g_game_state == GAME_HOST) {
			player_remove_player(sender);
		}
		else if (g_game_state == GAME_IN_GAME) {
			player_remove_player(sender);
			message_player_left(sender);
		}
		else {
			xbee_printf("Ignoring LEAVE_GAME packet because in state %s\n", g_game_state_str[g_game_state]);
		}
		break;
	case XBEE_MESSAGE_GAME_START:
		if (g_game_state == GAME_JOIN) {
			for (i = 0; i < data[0]; i++) {
				err = player_add_player(bytes_to_uint64_t(data + (i*sizeof(uint64_t) + 1)));
				if (err < 0) {
					xbee_printf("Game table: invalid player address: %llx\n", bytes_to_uint64_t(data + (i*sizeof(uint64_t) + 1)));
				}
			}
			err = game_trans_join_to_in_game();
			if (err < 0) {
				xbee_printf("Unable to transition into game start?\n");
			}
		}
		else {
			xbee_printf("Ignoring GAME_START packet because in state %s\n", g_game_state_str[g_game_state]);
		}
		break;
	case XBEE_MESSAGE_GAME_OVER:
		if (g_game_state == GAME_IN_GAME) {
			err = game_trans_in_game_to_over();
			if (err < 0) {
				xbee_printf("Unable to transition into game start\n");
			}
		}
		else {
			xbee_printf("Ignoring GAME_OVER packet because in state %s\n", g_game_state_str[g_game_state]);
		}
		break;
	case XBEE_MESSAGE_GAME_EVENT:
		/* For now do nothing */
		break;
	case XBEE_MESSAGE_PLAYER_LEFT:
		player_remove_player(bytes_to_uint64_t(data));
		break;
	case XBEE_MESSAGE_ACK:
		if (data[2] == 0) {
			send_message_ack(data[0]);
		}
		else {
			err = send_message_retry(data[0]);
			if (err < 0) {
				xbee_printf("Error, receiving bad ack. Error: %d\n", err);
			}
		}
		break;
	}
	return 0;
}

static int _mario_xbee_interpret_tx_status(struct xbee_packet *xp) {
	const struct xbee_packet *sent_xp;
	sent_xp = xbee_interface_tx_get_packet_by_frame_id(xp->payload[1]);
	switch (xp->payload[5]) {
		case 0x00: /* Success */
			/* Nothing to do, successful */
			break;
		case 0x01: /* MAC ACK Failure */
		case 0x21: /* Network ACK Failure */
		case 0x25: /* No route found */
			/* Here we need to check if we are master, and remove them from
			 * the game if we are. If we aren't, then we should check to
			 * see if we can't communicate with master, and get into a game
			 * join state if that is the case
			 */
			if (sent_xp->payload[0] == XBEE_API_TX_REQUEST) {
				if (g_game_host == player_get_address_from_driver(DRIVER)) {
					player_remove_player(bytes_to_uint64_t(sent_xp->payload + 2));
					message_player_left(bytes_to_uint64_t(sent_xp->payload + 2));
					xbee_printf("Player %llx left game", bytes_to_uint64_t(sent_xp->payload + 2));
				}
				else {
					if (g_game_host == (bytes_to_uint64_t(sent_xp->payload + 2))) {
						/* Need to send that we are leaving the game */
						/* Then leave the game */
					}
				}
			}
			else {
				xbee_printf("MAJOR ISSUE: Expected a tx packet in sent buffer, got %d", sent_xp->payload[0]);
			}
			break;
		case 0x15: /* Invalid destinattion endpoint */
			xbee_printf("Trying to send to invalid destination");
			break;
		case 0x74:
			xbee_printf("Trying to send a packet with too big of payload");
			break;
	}
	xbee_interface_free_packet(sent_xp);
	/* TX status packet gets freed later */
	return 0;
}

