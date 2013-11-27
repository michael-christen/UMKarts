#ifndef __XBEE_UMKART_H_
#define __XBEE_UMKART_H_
#include <inttypes.h>

/* MAX_XBEE_PACKET_SIZE given on page 11 of of XBEE 802.15.4 spec */
/* 100 Bytes payload + 14 bytes overhead possible */
/* TODO: Check the xbee spec, see if we can make packet size even bigger for printf */
#define MAX_XBEE_PAYLOAD_SIZE 114
#define MAX_XBEE_TX_PAYLOAD_SIZE 100

#define  XBEE_START_BYTE    ((uint8_t) 0x7E)
#define  XBEE_ESCAPE_BYTE   ((uint8_t) 0x7D)
#define  XBEE_XON_BYTE      ((uint8_t) 0x11)
#define  XBEE_XOFF_BYTE     ((uint8_t) 0x13)
#define  XBEE_XOR_BYTE      ((uint8_t) 0x20)

#define  LOWBYTE(v)         ((uint8_t) (v & 0xff))
#define  HIGHBYTE(v)        ((uint8_t) (((uint16_t) (v)) >> 8))

#define XBEE_API_AT_COMMAND ((uint8_t) 0x08)
#define XBEE_API_AT_QUEUE_COMMAND ((uint8_t) 0x09)
#define XBEE_API_TX_REQUEST ((uint8_t) 0x10)

#define XBEE_API_AT_COMMAND_RESPONSE ((uint8_t) 0x88)


#define XBEE_API_AT_RESPONSE ((uint8_t) 0x88)
#define XBEE_API_MODEM      ((uint8_t) 0x8A)
#define XBEE_API_TX_STATUS  ((uint8_t) 0x8B)

enum XBeeModemStatus {
  EXBeeModemStatus_HardwareReset,
  EXBeeModemStatus_WatchdogReset,
  EXBeeModemStatus_Associated,
  EXBeeModemStatus_Disassociated,
  EXBeeModemStatus_SyncLost,
  EXBeeModemStatus_CoordinatorRealign,
  EXBeeModemStatus_CoordinatorStart,
};

struct xbee_packet {
  uint16_t len;
  uint8_t payload[MAX_XBEE_PAYLOAD_SIZE];
};


struct xp_tx_request {
  uint64_t dest_addres;
  uint8_t frameId;
  uint8_t broadcast_radius; /* Maximum hops in transmit. 0 means maximum hops */
  uint8_t txoptions; /* bit 0: Disable ACK. bit 1: Don't attempt route discovery */
  uint8_t *rfdata;
  uint16_t data_len;
};

uint8_t xbee_packet_api_id(struct xbee_packet * xp);
uint8_t xbee_packet_frame_id(struct xbee_packet * xp);
enum XBeeModemStatus xbee_packet_modem_status(struct xbee_packet * xp); 


void xbpt_create_tx_request(const struct xp_tx_request * xptx, struct xbee_packet * xp);

#endif
