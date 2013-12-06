#include "xbee_interface.h"
#include "xbee_writer.h"
#include "mss_uart.h"
#include "mss_rtc.h"
#include "circle_buffer.h"
#include "atomics.h"

#define MSS_UART_QUEUE_CAPACITY 16

static void _xbee_interface_tx_handler(mss_uart_instance_t * this_uart);
static inline void _xbee_interface_tx_start(struct xbee_packet * xp);
void _xbee_interface_tx_init();

/* Send buffer Variables */
static struct {
	void * circle_buf_mem[XBEE_PACKET_BUF_SIZE];
	CircularBuffer circle_buf;
	uint8_t uart_queue[MSS_UART_QUEUE_CAPACITY];
	size_t  uart_queue_size;
	unsigned char lock;
	struct {
		void * circle_buf_mem[XBEE_PACKET_BUF_SIZE];
		CircularBuffer circle_buf;
		unsigned char lock;
		uint64_t rtc_count;
	} wait_for_response;
	struct xbee_writer xw;
	struct xbee_packet *xp;
} _xbee_tx;

void  _xbee_interface_tx_init() {
	CircularBufferInit(&_xbee_tx.circle_buf,
					 _xbee_tx.circle_buf_mem,
					 XBEE_PACKET_BUF_SIZE);
	CircularBufferInit(&_xbee_tx.wait_for_response.circle_buf,
	                   _xbee_tx.wait_for_response.circle_buf_mem,
					   XBEE_PACKET_BUF_SIZE);
	_xbee_tx.uart_queue_size = 0;
	_xbee_tx.lock = 0;
	_xbee_tx.wait_for_response.lock = 0;

	MSS_UART_set_tx_handler(&g_mss_uart1, _xbee_interface_tx_handler);
	MSS_UART_disable_irq(&g_mss_uart1, MSS_UART_TBE_IRQ );
}

int xbee_send(struct xbee_packet *xp) {
	int err;
	err = CircularBufferWrite(&_xbee_tx.circle_buf, xp);
	if (err == 0 && (atomic_lock_test_and_set_1(&(_xbee_tx.lock)) == 0)) {
		if ((xp = CircularBufferRead(&(_xbee_tx.circle_buf)))) {
			_xbee_interface_tx_start(xp);
			_xbee_interface_tx_handler(&g_mss_uart1);
		}
		else {
			/* Low probability, that someting else clears buffer between appending
			 * and grabbing lock
			 */
			atomic_lock_release(&(_xbee_tx.lock));
		}
	}
	return err;
}

void xbee_interface_tx_unlock_wait() {
	_xbee_tx.wait_for_response.lock = 0;
	MSS_UART_enable_irq( &g_mss_uart1, MSS_UART_TBE_IRQ );
}

struct xbee_packet * xbee_interface_tx_next_status_packet() {
	return CircularBufferRead(&(_xbee_tx.wait_for_response.circle_buf));
}

static inline void _xbee_interface_tx_start(struct xbee_packet * xp) {
	XBeeWriterInit(&(_xbee_tx.xw), xp);
	_xbee_tx.uart_queue_size = 0;
	_xbee_tx.xp = xp;
}

static void _xbee_interface_tx_handler(mss_uart_instance_t * this_uart) {
	struct xbee_packet * xp;
	uint8_t * start_ptr, * end_ptr, * move_ptr;
	size_t items_in_fifo;
	int err;
	MSS_UART_disable_irq( this_uart, MSS_UART_TBE_IRQ );

	if (atomic_lock_test_and_set_1(&(_xbee_tx.lock)) == 0) {
		if ((xp = CircularBufferRead(&(_xbee_tx.circle_buf)))) {
			_xbee_interface_tx_start(xp);
		}
		else {
			__sync_lock_release(&(_xbee_tx.lock));
			return;
		}
	}

	if (_xbee_tx.wait_for_response.lock) {
		/* 1 second = a difference of 256 */
		return;
		if (_xbee_tx.wait_for_response.rtc_count - MSS_RTC_get_rtc_count() > 128) {
			_xbee_tx.wait_for_response.lock = 0;
			xbee_interface_free_packet(xbee_interface_tx_next_status_packet());
			/* Need some form of error reporting ! */
			/* @TODO Add some error reporting mechanism */
		}
		else {
			/* We need to wait for a response, so disable interrupts until we get
			 * an RX response */
			return;
		}
	}

	/* Copy data from packet into queue to be moved into xbee buffer */
	start_ptr = _xbee_tx.uart_queue + _xbee_tx.uart_queue_size;
	end_ptr = _xbee_tx.uart_queue + sizeof(uint8_t) * MSS_UART_QUEUE_CAPACITY;
	end_ptr = XBeeWriterWrite(&(_xbee_tx.xw), start_ptr, end_ptr);

	/* Add items to fifo_queue, and then copy data in array to front if not
	 * added to the fifo_queue */
	items_in_fifo = MSS_UART_fill_tx_fifo(this_uart, _xbee_tx.uart_queue, end_ptr - _xbee_tx.uart_queue);
	move_ptr = _xbee_tx.uart_queue + items_in_fifo;
	_xbee_tx.uart_queue_size = 0;
	while (move_ptr != end_ptr) {
		_xbee_tx.uart_queue[_xbee_tx.uart_queue_size++] = *move_ptr++;
	}
	/* Check to see if a complete packet has been sent, which means there is
	 * nothing left in the the uart_queue, and the XBeeWriter says it's done */
	if (_xbee_tx.uart_queue_size == 0 && XBeeWriterDone(&(_xbee_tx.xw))) {
		if (XBEE_INTERFACE_EXPECT_RESPONSE(_xbee_tx.xw.xp)) {
			err = CircularBufferWrite(&_xbee_tx.wait_for_response.circle_buf, _xbee_tx.xp);
			if (err == 0) {
				_xbee_tx.wait_for_response.lock = 1;
				_xbee_tx.wait_for_response.rtc_count = MSS_RTC_get_rtc_count();
			}
			else {
				xbee_interface_free_packet(_xbee_tx.xw.xp);
			}
		}
		else {
			xbee_interface_free_packet(_xbee_tx.xw.xp);
		}
		__sync_lock_release(&(_xbee_tx.lock));
	}
	MSS_UART_enable_irq( this_uart, MSS_UART_TBE_IRQ );
}

