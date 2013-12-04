#include "messages.h"
#include "xbee.h"
#include "xbee_interface.h"
#include "player.h"
#include <errno.h>

int message_game_host() {
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
	payload[0] = XBEE_MESSAGE_GAME_HOST;
	payload[1] = 0x01; /* No App Ack */
	xbee_txpt_set_payload_size(xp, 2);
	err = xbee_send(xp);
	if (err < 0) {
		xbee_interface_free_packet(xp);
	}
	return err;
}

int message_game_join(uint64_t host_address) {
	int err;
	err = send_message_address(host_address, XBEE_MESSAGE_GAME_JOIN, 0x00, NULL, 0);
	return err;
}

int message_game_start(uint64_t * players, uint8_t num_players) {
	uint8_t buf[16*sizeof(uint64_t) + sizeof(uint8_t)];
	int err, i;
	buf[0] = num_players;
	for (i = 0; i < num_players; i++) {
		buf[i*sizeof(uint64_t) + 0] = players[i] >> 56 & 0xFF;
		buf[i*sizeof(uint64_t) + 1] = players[i] >> 48 & 0xFF;
		buf[i*sizeof(uint64_t) + 2] = players[i] >> 40 & 0xFF;
		buf[i*sizeof(uint64_t) + 3] = players[i] >> 32 & 0xFF;
		buf[i*sizeof(uint64_t) + 4] = players[i] >> 24 & 0xFF;
		buf[i*sizeof(uint64_t) + 5] = players[i] >> 16 & 0xFF;
		buf[i*sizeof(uint64_t) + 6] = players[i] >> 8 & 0xFF;
		buf[i*sizeof(uint64_t) + 7] = players[i] & 0xFF;
	}
	err = send_message(XBEE_MESSAGE_GAME_START, 0x00, buf, 1 + num_players * sizeof(uint64_t));
	return err;
}

int message_game_over() {
	int err;
	err = send_message(XBEE_MESSAGE_GAME_OVER, 0x00, NULL, 0);
	return err;
}

int message_game_event(uint64_t address, uint8_t subject, uint8_t object, uint8_t action, uint8_t event, uint8_t flags) {
	int err;
	uint8_t buf[4];
	buf[0] = subject;
	buf[1] = object;
	buf[2] = action;
	buf[3] = event;

	if (address != 0) {
		err = send_message_address(address, XBEE_MESSAGE_GAME_EVENT, flags, buf, 4);
	}
	else {
		err = send_message(XBEE_MESSAGE_GAME_EVENT, flags, buf, 4);
	return err;
}
