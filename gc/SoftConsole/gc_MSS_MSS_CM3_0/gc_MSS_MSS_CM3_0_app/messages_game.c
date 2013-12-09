#include "messages.h"
#include "xbee.h"
#include "xbee_interface.h"
#include "player.h"
#include "convert.h"
#include <errno.h>

int message_game_host_announce() {
	/* Special implementation because need to send broadcast packet */
	int err;
	uint8_t *payload;
	struct xbee_packet *xp = xbee_interface_create_packet();
	if (!xp) {
		return -ENOMEM;
	}
	xbee_txpt_init(xp);
	xbee_txpt_set_frame_id(xp, 0);
	xbee_txpt_set_options(xp, 0x1); /* Set No Ack */
	xbee_txpt_set_radius(xp, 0);
	xbee_txpt_set_destaddress(xp, XBEE_BROADCAST_ADDRESS);
	payload = xbee_txpt_payload_start(xp);
	payload[0] = XBEE_MESSAGE_GAME_HOST_ANNOUNCE;
	payload[1] = XBEE_APP_OPT_NO_ACK; /* No App Ack */
	payload[2] = 0; /* Frame id 0 */
	payload[3] = DRIVER;
	xbee_txpt_set_payload_size(xp, 4);
	err = xbee_send(xp);
	if (err < 0) {
		xbee_interface_free_packet(xp);
	}
	return err;
}

int message_game_join(uint64_t host_address) {
	int err;
	err = send_message_address(host_address, XBEE_MESSAGE_GAME_JOIN, XBEE_APP_OPT_ACK, 0, 0);
	return err;
}

int message_game_leave() {
	int err;
	err = send_message(XBEE_MESSAGE_LEAVE_GAME, XBEE_APP_OPT_NO_ACK, 0, 0);
	return err;
}

int message_game_start(uint64_t * players, uint8_t num_players) {
	uint8_t buf[16*sizeof(uint8_t) + sizeof(uint8_t)];
	int err, i;
	buf[0] = num_players;
	for (i = 0; i < num_players; i++) {
		buf[i + 1] = player_get_driver_from_address(players[i]);
	}
	err = send_message(XBEE_MESSAGE_GAME_START, XBEE_APP_OPT_ACK, buf, 1 + num_players * sizeof(uint8_t));
	if (err == 0) {
		err = send_message_address(XBEE_LISTENER_ADDRESS,
				XBEE_MESSAGE_GAME_START, XBEE_APP_OPT_NO_ACK, buf, 1 + num_players * sizeof(uint8_t));
	}
	return err;
}

int message_game_over() {
	int err;
	err = send_message(XBEE_MESSAGE_GAME_OVER, XBEE_APP_OPT_ACK, 0, 0);
	if (err == 0) {
		err = send_message_address(XBEE_LISTENER_ADDRESS,
				XBEE_MESSAGE_GAME_OVER, XBEE_APP_OPT_NO_ACK, 0, 0);
	}
	return err;
}

int message_game_event(uint64_t address, uint8_t subject, uint8_t object, uint8_t action, uint8_t item, uint8_t flags) {
	int err;
	uint8_t buf[4];
	buf[0] = subject;
	buf[1] = object;
	buf[2] = action;
	buf[3] = item;
	err = send_message_address(address, XBEE_MESSAGE_GAME_EVENT, flags, buf, 4);

	return err;
}

int message_game_event_all(uint8_t subject, uint8_t object, uint8_t action, uint8_t item, uint8_t flags) {
	int err;
	uint8_t buf[4];
	buf[0] = subject;
	buf[1] = object;
	buf[2] = action;
	buf[3] = item;
	err = send_message(XBEE_MESSAGE_GAME_EVENT, flags, buf, 4);
	if (err == 0) {
		err = send_message_address(XBEE_LISTENER_ADDRESS, XBEE_MESSAGE_GAME_EVENT, XBEE_APP_OPT_NO_ACK, buf, 4);
	}

	return err;
}


int message_player_left(uint64_t address) {
	int err;
	uint8_t buf;
	buf = player_get_driver_from_address(address);
	err = send_message(XBEE_MESSAGE_PLAYER_LEFT, XBEE_APP_OPT_ACK, &buf, 1);
	if (err == 0) {
		err = send_message_address(XBEE_LISTENER_ADDRESS,
				XBEE_MESSAGE_PLAYER_LEFT, XBEE_APP_OPT_NO_ACK, &buf, 1);
	}
	return err;
}
