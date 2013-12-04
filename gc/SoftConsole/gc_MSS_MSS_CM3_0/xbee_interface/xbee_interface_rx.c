#include "xbee_interface.h"
#include "xbee_reader.h"
#include "mss_uart.h"
#include "circle_buffer.h"

#define MSS_UART_RX_BUF_SIZE 16

static void _xbee_interface_rx_handler(mss_uart_instance_t * this_uart);

static uint8_t _overflow_buffer[16];

/* Receive buffer */
static struct {
	void * circle_buf_mem[XBEE_PACKET_BUF_SIZE];
	CircularBuffer circle_buf;
	struct xbee_reader xr;
	uint8_t buf[MSS_UART_RX_BUF_SIZE];
	enum {
		XBEE_RX_READY, XBEE_RX_INPROGRESS,
	} state;
	uint8_t flags;
} _xbee_rx;

void _xbee_interface_rx_init() {
	CircularBufferInit(&_xbee_rx.circle_buf, _xbee_rx.circle_buf_mem,
			XBEE_PACKET_BUF_SIZE);

	_xbee_rx.flags = 0;
	_xbee_rx.state = XBEE_RX_READY;
	MSS_UART_set_rx_handler(&g_mss_uart1, _xbee_interface_rx_handler,
			MSS_UART_FIFO_SINGLE_BYTE);
	MSS_UART_enable_irq(&g_mss_uart1, MSS_UART_RBF_IRQ);
}

struct xbee_packet * xbee_read() {
	return CircularBufferRead(&(_xbee_rx.circle_buf));
}

uint8_t xbee_read_get_errors() {
	return _xbee_rx.flags;
}

void xbee_read_clear_errors() {
	_xbee_rx.flags = 0;
}

static void _xbee_interface_rx_handler(mss_uart_instance_t * this_uart) {
	size_t bytes_from_uart, i;
	uint8_t *end_of_read;
	struct xbee_packet * xp;
	int err;
	int bytes_remaining; /* Bytes remaining from the last read. If we complete packet */
	MSS_UART_disable_irq(this_uart, MSS_UART_RBF_IRQ);
	bytes_remaining = 0;
	do {
		/* If in a ready state, we need to allocate a new packet to store the
		 * information */
		if (_xbee_rx.state == XBEE_RX_READY) {
			xp = xbee_interface_create_packet();
			if (!xp) {
				_xbee_rx.flags |= XBEE_RX_OUTOF_MEMORY;
				/* Drop the packets on the floor, we really can't do anything better
				 * than that at this moment because we're out of memory */
				MSS_UART_get_rx(this_uart, _overflow_buffer,
						sizeof(_overflow_buffer));
				return;
			}
			XBeeReaderInit(&(_xbee_rx.xr), xp);
			_xbee_rx.state = XBEE_RX_INPROGRESS;
		}

		/* Read up to first 16 bytes from the serial port queue */
		bytes_from_uart = MSS_UART_get_rx(this_uart, _xbee_rx.buf
				+ bytes_remaining, MSS_UART_RX_BUF_SIZE - bytes_remaining);

		/* Parse packets from uart into our packet */
		end_of_read = XBeeReaderRead(&(_xbee_rx.xr), _xbee_rx.buf, _xbee_rx.buf
				+ bytes_from_uart + bytes_remaining);

		/* Deal with completed packet */
		if (XBeeReaderDone(&(_xbee_rx.xr))) {
			if (XBeeReaderGood(&(_xbee_rx.xr))) {
				err = CircularBufferWrite(&(_xbee_rx.circle_buf),
						_xbee_rx.xr.xp);
				/* Error check: Mark error if our circular buffer is full */
				if (err < 0) {
					_xbee_rx.flags |= XBEE_RX_OVERFLOW_PACKET_LOSS;
					xbee_interface_free_packet(_xbee_rx.xr.xp);
				}
			} else {
				/* Error check for a bad packet (bad checksum) */
				_xbee_rx.flags |= XBEE_RX_BAD_PACKET;
				xbee_interface_free_packet(_xbee_rx.xr.xp);
			}
			_xbee_rx.state = XBEE_RX_READY;
			/* Determine the bytes remaining that were read from the UART Buffer  and
			 * not consumed by the xbee_packet being created */
			bytes_remaining = bytes_from_uart + bytes_remaining - (end_of_read - _xbee_rx.buf);
			/* Copy on used bytes to front of buffer */
			for (i = 0; i < bytes_remaining; i++) {
				_xbee_rx.buf[i] = *end_of_read++;
			}
		}
	} while (bytes_remaining > 0);
	MSS_UART_enable_irq(this_uart, MSS_UART_RBF_IRQ);
}

