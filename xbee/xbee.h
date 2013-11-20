#ifndef __XBEE_UMKART_H_
#define __XBEE_UMKART_H_
#include <inttypes.h>

/* MAX_XBEE_PACKET_SIZE given on page 11 of of XBEE 802.15.4 spec */
/* 100 Bytes payload + 10 bytes overhead possible */
#define MAX_XBEE_PAYLOAD_SIZE 110

#define  XBEE_START_BYTE    ((uint8_t) 0x7E)
#define  XBEE_ESCAPE_BYTE   ((uint8_t) 0x7D)
#define  XBEE_XON_BYTE      ((uint8_t) 0x11)
#define  XBEE_XOFF_BYTE     ((uint8_t) 0x13)
#define  XBEE_XOR_BYTE      ((uint8_t) 0x20)

#define  LOWBYTE(v)         ((uint8_t) (v))
#define  HIGHBYTE(v)        ((uint8_t) (((uint16_t) (v)) >> 8))

#define XBEE_API_MODEM      ((uint8_t) 0x8A)
#define XBEE_API_AT_COMMAND ((uint8_t) 0x08)
#define XBEE_API_AT_QUEUE_COMMAND ((uint8_t) 0x09)

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

struct xp_at_command {
  uint8_t cmdId[2]; /* Commands are two bytes always */
  uint8_t frameId;
  uint8_t *param;
  uint16_t param_len;
};

uint8_t xbee_packet_api_id(struct xbee_packet * xp);
enum XBeeModemStatus xbee_packet_modem_status(struct xbee_packet * xp); 
void xbee_packet_at_command(struct xp_at_command * xpac, struct xbee_packet * xp); 
void xbee_packet_at_command_queue(struct xp_at_command * xpac, struct xbee_packet * xp); 

#endif
