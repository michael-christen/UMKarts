#ifndef __XBEE_UMKART_H_
#define __XBEE_UMKART_H_
#include <inttypes.h>

/* MAX_XBEE_PACKET_SIZE given on page 11 of of XBEE 802.15.4 spec */
#define MAX_XBEE_PAYLOAD_SIZE 100

#define  XBEE_START_BYTE   ((uint8_t) 0x7E)
#define  XBEE_ESCAPE_BYTE  ((uint8_t) 0x7D)
#define  XBEE_XON_BYTE     ((uint8_t) 0x11)
#define  XBEE_XOFF_BYTE    ((uint8_t) 0x13)
#define  XBEE_XOR_BYTE     ((uint8_t) 0x20)

#define  LOWBYTE(v)        ((uint8_t) (v))
#define  HIGHBYTE(v)       ((uint8_t) (((uint16_t) (v)) >> 8))

struct xbee_packet {
  uint16_t len;
  uint8_t payload[MAX_XBEE_PAYLOAD_SIZE];
};

int xbee_send(struct xbee_packet *);

#endif
