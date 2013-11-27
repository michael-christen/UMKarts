#include "xbee.h"
#include "xbee_interface.h"
#include "mario_xbee.h"

void mario_xbee_intepret_packet(struct xbee_packet * xp) {
	switch (xbee_packet_api_id(xp)) {
	case XBEE_API_AT_COMMAND_RESPONSE:

	case XBEE_API_MODEM:
		/* Ignore for now */
		break;
	case XBEE_API_TX_STATUS:
		break;
	}
}

/*
 * This function will be called when we are looking for other available
 * cards on the network.
 */
void mario_xbee_network_discovery() {
}
