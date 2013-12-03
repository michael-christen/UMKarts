#include "xbee.h"
#include "xbee_interface.h"
#include "mario_xbee.h"
#include "player.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define MAX_PRINTF_SIZE MAX_XBEE_TX_PAYLOAD_SIZE - 1

void mario_xbee_intepret_packet(struct xbee_packet * xp) {
	switch (xbee_packet_api_id(xp)) {
	case XBEE_API_AT_COMMAND_RESPONSE:
		if (xp->payload[4] != 0x00) {
			xbee_printf("Invalid xbee packet: %c%c, %d\r\n", xp->payload[2], xp->payload[3], xp->payload[4]);
		}
		else if (xp->payload[2] == "S" && xp->payload[3] == "H") {
			player_set_high_address(xp->payload[5]);
		}
		else if (xp->payload[2] == "S" && xp->payload[3] == "L") {
			player_set_low_address(xp->payload[5]);
		}
		else {
			/* Should we print an error in this case? */
		}
		break;
	case XBEE_API_MODEM:
		xbee_printf("XBee Modem Status: %d\n", xp->payload[1]);
		break;
	case XBEE_API_TX_STATUS:
		break;
	}
}

int xbee_printf(const char * format_string, ...) {
	uint64_t dest_address = 0x0013A20040A711E0LL;
	int err;
	int size;
	uint8_t * payload_start;
	va_list varargs;
	va_start(varargs, format_string);
	struct xbee_packet * xp = xbee_interface_create_packet();
	if (!xp) {
		err = -ENOMEM;
		goto xbee_printf_exit;
	}
	payload_start = xbee_txpt_payload_start(xp);
	*payload_start = 0x00; /* Set frame id to say that this is a printf packet */
	err = vsnprintf(payload_start + 1, MAX_PRINTF_SIZE - 1, format_string, varargs); /* -1 for null terminator */
	if (err < 0) {
		goto xbee_printf_exit;
	}
	if (err > MAX_PRINTF_SIZE) {
		size = MAX_PRINTF_SIZE;
	}
	else {
		size = err;
	}

	xbee_txpt_init(xp);
	xbee_txpt_set_payload_size(xp, size + 1);
	xbee_txpt_set_frame_id(xp, xbee_interface_next_frame_id());
	xbee_txpt_set_options(xp, 0);
	xbee_txpt_set_radius(xp, 0);
	xbee_txpt_set_destaddress(xp, dest_address);

	err = xbee_send(xp);

xbee_printf_exit:
	va_end(varargs);

	return err;
}
