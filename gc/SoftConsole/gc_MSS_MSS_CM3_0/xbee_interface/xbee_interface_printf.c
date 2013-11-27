#include <stdio.h>
#include <stdarg.h>

int xbee_printf(const char * format_string, ...) {
	int err;
	int size;
	va_list varargs;
	va_start(varargs, format_string);
	struct xbee_packet * xp = xbee_interface_create_packet();
	if (!xp) {
		return -EMEM;
	}
	err = vsnprintf(xbee_txpt_payload_start(xp), MAX_XBEE_TX_PAYLOAD_SIZE, format_string, varargs);
	if (err < 0) {
		return err;
	}
	if (err > MAX_XBEE_TX_PAYLOAD_SIZE) {
		size = MAX_XBEE_TX_PAYLOAD_SIZE;
	}
	else {
		size = err;
	}

	xbee_txpt_set_payload_size(xp, size);
	xbee_txpt_set_frame_id(xp, xbee_next_frame_id());
	xbee_txpt_set_options(xp, 0);
	xbee_txpt_set_radius(xp, 0);
	/* TODO: Remove hardcode of destenation address */
	xbee_txpt_set_destaddress(xp, 0x0013A20040A711E0);
	xbee_txpt_init(xp);
	xbee_send(xp);

}

