#include "xbee_interface.h"

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

