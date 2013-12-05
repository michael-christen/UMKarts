#include "xbee_reader.h"
#include "log.h"
#include "xbee_packet.h"
#include "convert.h"
#include <stdlib.h>

static int _xbee_printf(uint64_t address, const uint8_t * data, uint16_t len) {
	int err, i;
	err = printf("0x%" PRIX64 ": ", address);
	if (err < 0) return err;
	for (i = 0; i < len; i++) {
		err = printf("%c", data[i]);
		if (err < 0) {
			return err;
		}
	}
	err = printf("\n");
	return err;
}

static uint64_t _xbee_tx_packet_get_address(uint8_t * start) {
	int i;
	uint64_t ret = 0;
	for (i = 0; i < sizeof(uint64_t); i++) {
		ret += (((uint64_t) start[i]) << i*8) & (0xFF << i*8);
	}
	return ret;
}

int xbee_get_next_packet(xbee_serial_t *serial, struct xbee_packet * xp, unsigned char debug) {
	struct xbee_reader xr;
	int result;
	uint8_t ch;
	XBeeReaderInit(&xr, xp);
	while (!XBeeReaderDone(&xr)) {
		result = xbee_ser_getchar(serial);
		if (result >= 0) {
			ch = result & 0xFF;
			if (debug) printf("%c\n", ch);
			XBeeReaderRead(&xr, &ch, &ch + 1);
		}
		else if (result != -ENODATA) {
			Log(EERROR, "Unable to read packet because %s", strerror(errno));
		}
	}

	if (XBeeReaderGood(&xr)) {
		return 0;
	}
	else {
		return -1;
	}
}

int xbee_received_packet(struct xbee_packet *xp) {
	uint8_t * data;
	uint16_t data_len;
	data_len = xbee_rxpt_get_payload_size(xp);
	if (data_len > 0) {
		data = xbee_rxpt_payload_start(xp);
		switch (*data) {
			case 0x00:
				_xbee_printf(bytes_to_uint64_t(xp->payload + 1), data + 3, data_len - 3);
				break;
			case 0x02:
				printf("0x%" PRIX64 " is trying to host a game\n", bytes_to_uint64_t(xp->payload + 1));
				break;
			default:
				Log(EINFO, "Recevied a tx packet that we have not configured for %x", *data);
		}
	}
	return 0;
}

