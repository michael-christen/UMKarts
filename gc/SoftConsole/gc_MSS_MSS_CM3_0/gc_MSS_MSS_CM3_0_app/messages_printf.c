#include "messages.h"
#include <stdarg.h>
#include <stdio.h>


int xbee_printf(const char * format_string, ...) {
	uint64_t dest_address = 0x0013A20040A711E0ull;
	uint8_t buf[XBEE_MESSAGE_MAX_SIZE + 1]; /* Plus one for null */
	int err;
	int size;
	va_list varargs;
	va_start(varargs, format_string);
	err = vsnprintf(buf, XBEE_MESSAGE_MAX_SIZE, format_string, varargs); /* -1 for null terminator */
	if (err < 0) {
		goto xbee_printf_exit;
	}
	if (err > XBEE_MESSAGE_MAX_SIZE) {
		size = XBEE_MESSAGE_MAX_SIZE;
	}
	else {
		size = err;
	}

	err = send_message_address(dest_address, XBEE_MESSAGE_PRINTF, XBEE_APP_OPT_NO_ACK, buf, size);
xbee_printf_exit:
	va_end(varargs);

	return err;
}
