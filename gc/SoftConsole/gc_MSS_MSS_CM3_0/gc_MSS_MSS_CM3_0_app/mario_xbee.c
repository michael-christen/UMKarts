#include "xbee.h"
#include "xbee_interface.h"
#include "mario_xbee.h"
#include "player.h"
#include "convert.h"
#include "game.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define MAX_PRINTF_SIZE MAX_XBEE_TX_PAYLOAD_SIZE - 1

static void _mario_xbee_interpret_at_response(struct xbee_packet *xp);
static int _mario_xbee_interpret_rx_packet(struct xbee_packet *xp);

int mario_xbee_interpret_packet(struct xbee_packet * xp) {
	switch (xbee_packet_api_id(xp)) {
	case XBEE_API_AT_COMMAND_RESPONSE:
		_mario_xbee_interpret_at_response(xp);
		break;
	case XBEE_API_MODEM:
		xbee_printf("XBee Modem Status: %d\n", xp->payload[1]);
		break;
	case XBEE_API_TX_STATUS:
		break;
	XBEE_API_RX:
		_mario_xbee_interpret_rx_packet(xp);
	}
	return 0;
}

static inline uint8_t _mario_xbee_at_cmd_is(const struct xbee_packet *xp, const char * cmd) {
	return (xp->payload[2] == cmd[0] && xp->payload[3] == cmd[1]);
}

static void _mario_xbee_interpret_at_response(struct xbee_packet *xp) {
	uint64_t address;
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
	int err;
	uint64_t sender = bytes_to_uint64_t(xp->payload + 1);
	uint8_t msg_type = xp->payload[12];
	uint8_t msg_opts = xp->payload[13];
	uint8_t msg_id   = xp->payload[14];
	uint8_t *data = xp->payload + 15;
	uint16_t data_len = xp->len - 15;
	switch (msg_type) {
	case XBEE_MESSAGE_PRINTF:
		/* We ignore PRINTF calls */
		break;
	case XBEE_MESSAGE_GAME_HOST:
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
			xbee_printf("Ingoring GAME_START packet because in state %s\n", g_game_state_str[g_game_state]);
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
			xbee_printf("Ignoring GAME_OVER packet because in state %s\n", g_game_start_str[g_game_state]);
		}
		break;
	case XBEE_MESSAGE_GAME_EVENT:
		/* For now do nothing */
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
}

