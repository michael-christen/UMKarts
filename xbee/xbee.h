#ifndef __XBEE_UMKART_H_
#define __XBEE_UMKART_H_
#include "firmware/drivers/mss_uart/mss_uart.h"

/* MAX_XBEE_PACKET_SIZE given on page 11 of of XBEE 802.15.4 spec */
#define MAX_XBEE_PACKET_SIZE 100
#define MAX_XBEE_PAYLOAD_SIZE ((MAX_XBEE_PACKET_SIZE - 4) / 2)

#define  XBEE_START_BYTE   ((uint8_t) 0x7E)
#define  XBEE_ESCAPE_BYTE  ((uint8_t) 0x7D)
#define  XBEE_XON_BYTE     ((uint8_t) 0x11)
#define  XBEE_XOFF_BYTE    ((uint8_t) 0x13)
#define  XBEE_XOR_BYTE     ((uint8_t) 0x20)

#define EXBEEINUSE  0x01

/*
 * Initializes the MSS_UART1 device, and configures it for use with our XBee
 */
int xbee_init();
int xbee_send(


#endif
