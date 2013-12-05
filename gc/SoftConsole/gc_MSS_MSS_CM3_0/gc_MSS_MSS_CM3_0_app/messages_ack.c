#include "messages.h"

int message_ack(uint64_t address, uint8_t frame_id, uint8_t msg_id, uint8_t failure) {
	int err;
	uint8_t buf[3];
	buf[0] = frame_id;
	buf[1] = msg_id;
	buf[2] = failure;

	err = send_message_address(address, XBEE_MESSAGE_ACK, XBEE_APP_OPT_NO_ACK, buf, 3);
	return err;
}
