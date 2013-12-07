#include "xbee_interface.h"
#include "xbee_reader.h"
#include "mss_uart.h"
#include "circle_buffer.h"
#include "convert.h"
#include <stdio.h>

#define MSS_UART_RX_BUF_SIZE 16

static void _xbee_interface_rx_handler(mss_uart_instance_t * this_uart);

static uint8_t _overflow_buffer[16]; /* used to keep reading in from UART if out of memory */

/* Receive buffer */
static struct {
	void * circle_buf_mem[XBEE_PACKET_BUF_SIZE];
	CircularBuffer circle_buf;
	struct xbee_reader xr;
	uint8_t buf[MSS_UART_RX_BUF_SIZE];
	enum {
		XBEE_RX_READY, XBEE_RX_INPROGRESS,
	} state;
	struct xbee_packet_received * xpr;
} _xbee_rx;

void _xbee_interface_rx_init() {
	CircularBufferInit(&_xbee_rx.circle_buf, _xbee_rx.circle_buf_mem,
			XBEE_PACKET_BUF_SIZE);

	_xbee_rx.state = XBEE_RX_READY;
	MSS_UART_set_rx_handler(&g_mss_uart1, _xbee_interface_rx_handler,
			MSS_UART_FIFO_SINGLE_BYTE);
	MSS_UART_enable_irq(&g_mss_uart1, MSS_UART_RBF_IRQ);
}

struct xbee_packet_received * xbee_read() {
	return CircularBufferRead(&(_xbee_rx.circle_buf));
}

static void _xbee_interface_rx_handler(mss_uart_instance_t * this_uart) {
	size_t bytes_from_uart, i;
	uint8_t *end_of_read;
	int err;
	int bytes_remaining; /* Bytes remaining from the last read. If we complete packet */
	MSS_UART_disable_irq(this_uart, MSS_UART_RBF_IRQ);
	bytes_remaining = 0;
	do {
		/* If in a ready state, we need to allocate a new packet to store the
		 * information */
		if (_xbee_rx.state == XBEE_RX_READY) {
			_xbee_rx.xpr = xbee_interface_create_receive_packet();
			if (!_xbee_rx.xpr) {
				/* Drop the packets on the floor, we really can't do anything better
				 * than that at this moment because we're out of memory */
				MSS_UART_get_rx(this_uart, _overflow_buffer,
						sizeof(_overflow_buffer));
				return;
			}
			_xbee_rx.xpr->flags = 0;
			XBeeReaderInit(&(_xbee_rx.xr), &(_xbee_rx.xpr->xp));
			_xbee_rx.state = XBEE_RX_INPROGRESS;
		}

		/* Read up to first 16 bytes from the serial port queue */
		if (MSS_UART_RX_BUF_SIZE > bytes_remaining) {
			bytes_from_uart = MSS_UART_get_rx(this_uart, _xbee_rx.buf
					+ bytes_remaining, MSS_UART_RX_BUF_SIZE - bytes_remaining);
		}

		/* Parse packets from uart into our packet */
		end_of_read = XBeeReaderRead(&(_xbee_rx.xr), _xbee_rx.buf, _xbee_rx.buf
				+ bytes_from_uart + bytes_remaining);

		/* Deal with completed packet */
		if (XBeeReaderDone(&(_xbee_rx.xr))) {
			if (XBeeReaderGoodOrBadCheckSum(&(_xbee_rx.xr))) {
				_xbee_rx.xpr->flags = (XBeeReaderCheckCheckSum(&(_xbee_rx.xr))) ? 0 : XBEE_RX_BAD_PACKET;
				err = CircularBufferWrite(&(_xbee_rx.circle_buf), _xbee_rx.xpr);
				if (err < 0) {
					printf("XBee Interface RX out of memory\n\r");
					xbee_interface_free_packet_received(_xbee_rx.xpr);
				}
			} else {
				printf("XBee interface RX received bad packet\n\r");
				xbee_interface_free_packet_received(_xbee_rx.xpr);
			}
			_xbee_rx.state = XBEE_RX_READY;
			/* Determine the bytes remaining that were read from the UART Buffer  and
			 * not consumed by the xbee_packet being created */
		}
		bytes_remaining = bytes_from_uart + bytes_remaining - (end_of_read - _xbee_rx.buf);
		/* Copy on used bytes to front of buffer */
		for (i = 0; i < bytes_remaining; i++) {
			_xbee_rx.buf[i] = *end_of_read++;
		}
	} while (bytes_remaining > 0);
	MSS_UART_enable_irq(this_uart, MSS_UART_RBF_IRQ);
}
