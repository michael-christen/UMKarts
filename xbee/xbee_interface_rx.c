#include "xbee_interface.h"

/* Receive buffer */
static struct {
  void * circle_buf_mem[XBEE_PACKET_BUF_SIZE];
  CircularBuffer circle_buf;
} _xbee_rx;

void  _xbee_interface_rx_init() {
  CircularBufferInit(&_xbee_rx.circle_buf,
                     _xbee_rx.circle_buf_mem,
                     XBEE_PACKET_BUF_SIZE);

  MSS_UART_set_rx_handler(&g_mss_uart1, _xbee_interface_rx_handler, MSS_UART_FIFO_SINGLE_BYTE);
}
