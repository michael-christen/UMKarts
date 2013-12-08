#include "messages.h"
#include "xbee.h"
#include "xbee_interface.h"
#include "player.h"
#include <errno.h>
#include <string.h>

#define MAX_FRAME_ID 256

static struct {
	uint8_t payload[MAX_XBEE_TX_PAYLOAD_SIZE - 1];
	uint16_t payload_len;
	uint64_t address;
	uint8_t msg_type;
	uint8_t app_flags;
	uint8_t valid;
	uint8_t retries;
} _sent_messages[MAX_FRAME_ID];

static void _add_sent_message(uint8_t frame_id, uint8_t msg_type, uint8_t app_flags, uint8_t * data, uint16_t len, uint64_t address); 

void send_message_init() {
	memset(_sent_messages, 0, sizeof(_sent_messages));
}

int send_message(uint8_t message_type, uint8_t app_flags, uint8_t *data, uint16_t data_len) {
	int err, i;
	for (i = 0; i < g_player_table.size; i++) {
		/* Don't send a message to ourselves!! */
		if (g_player_table.players[i] != player_get_address_from_driver(DRIVER)) {
			err = send_message_address(g_player_table.players[i], message_type, app_flags, data, data_len);
			if (err < 0)  return err;
		}
	}
	return 0;
}


int send_message_address(uint64_t address, uint8_t message_type, uint8_t app_flags, uint8_t * data, uint16_t data_len) {
	struct xbee_packet *xp = xbee_interface_create_packet();
	uint8_t frameId;
	uint8_t *payload;
	int i, err;
	if (!xp) {
		err = -ENOMEM;
		goto send_message_address_error;
	}
	if (data_len > XBEE_MESSAGE_MAX_SIZE) {
		err = -EINVAL;
		goto send_message_address_error;
	}

	xbee_txpt_init(xp);
	frameId = xbee_interface_next_frame_id();
	if (address == XBEE_LISTENER_ADDRESS) {
		frameId = 0;
	}

	xbee_txpt_set_frame_id(xp, frameId);
	xbee_txpt_set_options(xp, 0);
	xbee_txpt_set_radius(xp, 0);
	xbee_txpt_set_destaddress(xp, address);
	payload = xbee_txpt_payload_start(xp);
	payload[0] = message_type;
	payload[1] = app_flags;
	payload[2] = frameId;
	for (i = 0; i < data_len; i++) {
		payload[i + 3] = data[i];
	}
	xbee_txpt_set_payload_size(xp, data_len + 3);
	err = xbee_send(xp);
	if (err < 0) {
		goto send_message_address_error;
	}
	if (!(app_flags & 0x1)) {
		_add_sent_message(frameId, message_type, app_flags, data, data_len, address);
	}

	return err;

	/* Error, be sure to free packet */
	send_message_address_error:
	xbee_interface_free_packet(xp); /* Never fails */
	return err;
}

int send_message_retry(uint8_t frame_id) {
	if (_sent_messages[frame_id].valid == 1) {
		if (_sent_messages[frame_id].retries++ < 3) {
			return send_message_address(_sent_messages[frame_id].address,
																 _sent_messages[frame_id].msg_type,
																 _sent_messages[frame_id].app_flags,
																 _sent_messages[frame_id].payload,
																 _sent_messages[frame_id].payload_len);
		}
		else {
			return -ETIMEDOUT;
		}
	}
	else {
		return -EINVAL;
	}
}



static void _add_sent_message(uint8_t frame_id, uint8_t msg_type, uint8_t app_flags, uint8_t * data, uint16_t len, uint64_t address) {
	int i;
	for (i = 0; i < len; i++) {
		_sent_messages[frame_id].payload[i] = data[i];
	}
	_sent_messages[frame_id].payload_len = len;
	_sent_messages[frame_id].address = address;
	_sent_messages[frame_id].msg_type = msg_type;
	_sent_messages[frame_id].app_flags = app_flags;
	_sent_messages[frame_id].valid = 1;
	_sent_messages[frame_id].retries = 0;
}

void send_message_ack(uint8_t frame_id) {
	_sent_messages[frame_id].valid = 0;
}
