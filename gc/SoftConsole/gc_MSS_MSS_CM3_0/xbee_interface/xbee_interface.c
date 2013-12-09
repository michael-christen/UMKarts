#include "xbee_interface.h"
#include "mss_uart.h"
#include "atomics.h"

static uint8_t _XBeeFrameId = 1;
/* 
 * Declare initialization functions so we can call them. No in xbee_interface.h
 * because they are really global functions
 */
void  _xbee_interface_tx_init(); 
void  _xbee_interface_rx_init(); 
int _xbee_interface_alloc_init();

int xbee_interface_init() {
  int check;
  check = _xbee_interface_alloc_init();
  if (check < 0) return check;

  MSS_UART_init(&g_mss_uart1,
                MSS_UART_57600_BAUD,
                MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

  _xbee_interface_rx_init();
  _xbee_interface_tx_init();

  return 0;
}

uint8_t xbee_interface_next_frame_id() {
	uint8_t ret_val, new_val;
	do {
		ret_val = (*((volatile uint8_t *) &_XBeeFrameId));
		new_val = ret_val + 1;
		if (new_val == 0) new_val = 1;
	}while (atomic_cmpxchg_1(&_XBeeFrameId, new_val, ret_val) != 0);
	return ret_val;
}
