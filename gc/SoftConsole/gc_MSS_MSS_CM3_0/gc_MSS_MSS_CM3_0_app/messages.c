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
	uint8_t noack;
} _sent_messages[MAX_FRAME_ID];

static void _add_sent_message(uint8_t frame_id, uint8_t msg_type, uint8_t * data, uint16_t len, uint64_t address); 
static void _ack_sent_message(uint8_t frame_id);

void send_message_init() {
	memset(_sent_messages, 0, sizeof(_sent_messages));
}

int send_message(uint8_t message_type, uint8_t *data, uint16_t data_len) {
	int err, i;
	for (i = 0; i < g_player_table.size; i++) {
		err = send_message_address(g_player_table.players[i], message_type, data, data_len);
		if (err < 0)  return err;
	}
	return 0;
}


int send_message_address(uint64_t address, uint8_t message_type, uint8_t * data, uint16_t data_len) {
	struct xbee_packet *xp = xbee_interface_create_packet();
	uint8_t frameId;
	uint8_t *payload;
	int i, err;
	if (!xp) {
		err = -ENOMEM;
		goto send_message_address_error;
	}
	if (data_len > MAX_XBEE_TX_PAYLOAD_SIZE - 1) {
		err = -EINVAL;
		goto send_message_address_error;
	}

	xbee_txpt_init(xp);
	frameId = xbee_interface_next_frame_id();

	xbee_txpt_set_frame_id(xp, frameId);
	xbee_txpt_set_options(xp, 0);
	xbee_txpt_set_radius(xp, 3);
	xbee_txpt_set_destaddress(xp, address);
	payload = xbee_txpt_payload_start(xp);
	payload[0] = message_type;
	for (i = 0; i < data_len; i++) {
		payload[i + 1] = data[i];
	}
	xbee_txpt_set_payload_size(xp, data_len + 1);
	err = xbee_send(xp);
	if (err < 0) {
		goto send_message_address_error;
	}
	return err;

	/* Error, be sure to free packet */
	send_message_address_error:
	xbee_interface_free_packet(xp); /* Never fails */
	return err;
}

int send_message_retry(uint8_t frame_id) {
	if (_sent_messages[frame_id].noack == 1) {
		return send_message_address(_sent_messages[frame_id].address,
		                           _sent_messages[frame_id].msg_type,
															 _sent_messages[frame_id].payload,
															 _sent_messages[frame_id].payload_len);
	}
	else {
		return -EINVAL;
	}
}



static void _add_sent_message(uint8_t frame_id, uint8_t msg_type, uint8_t * data, uint16_t len, uint64_t address) {
	int i;
	for (i = 0; i < len; i++) {
		_sent_messages[frame_id].payload[i] = data[i];
	}
	_sent_messages[frame_id].payload_len = len;
	_sent_messages[frame_id].address = address;
	_sent_messages[frame_id].msg_type = msg_type;
	_sent_messages[frame_id].noack = 1;
}

static void _ack_sent_message(uint8_t frame_id) {
	_sent_messages[frame_id].noack = 0;
}
