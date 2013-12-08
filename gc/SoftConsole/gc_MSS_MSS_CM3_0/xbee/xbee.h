#ifndef __XBEE_UMKART_H_
#define __XBEE_UMKART_H_
#include <inttypes.h>

/* MAX_XBEE_PACKET_SIZE given by NP command of AT Command mode */
/* 73 byte payload + 14 bytes overhead possible */
#define MAX_XBEE_PAYLOAD_SIZE    87
#define MAX_XBEE_TX_PAYLOAD_SIZE 73

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
#define XBEE_API_EXPLICIT_TX ((uint8_t) 0x11)

#define XBEE_API_AT_COMMAND_RESPONSE ((uint8_t) 0x88)


#define XBEE_API_AT_RESPONSE ((uint8_t) 0x88)
#define XBEE_API_MODEM      ((uint8_t) 0x8A)
#define XBEE_API_TX_STATUS  ((uint8_t) 0x8B)
#define XBEE_API_RX         ((uint8_t) 0x90)

#define XBEE_BROADCAST_ADDRESS (0x000000000000FFFFull)

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

uint8_t xbee_packet_api_id(struct xbee_packet * xp);

enum XBeeModemStatus xbee_packet_modem_status(struct xbee_packet * xp);

/* TX Commands */
/**
 * Need to use all of these functions in order to create a valid xbee_txpt packet.
 * you first must initialize, then you would set the frame id and the options and
 * then radius. Then you set the dest address. The last thing you do is get the start
 * address of payload, and then write up to the MAX_PAYLOAD_SIZE of data into the xbee
 * payload, then set the size of the data you wrote. Then you're done! Yeah
 */
void xbee_txpt_init(struct xbee_packet * xp);
void xbee_txpt_set_frame_id(struct xbee_packet * xp, uint8_t frameId);
void xbee_txpt_set_options(struct xbee_packet * xp, uint8_t opts);
void xbee_txpt_set_radius(struct xbee_packet *xp, uint8_t rad);
void xbee_txpt_set_destaddress(struct xbee_packet *xp, uint64_t address);
uint8_t * xbee_txpt_payload_start(struct xbee_packet *xp);
void xbee_txpt_set_payload_size(struct xbee_packet *xp, uint16_t payload_size);
/* TX Commands End */

/* RX Commands */
uint8_t xbee_rxpt_get_frame_id(struct xbee_packet *xp);
uint8_t *xbee_rxpt_payload_start(struct xbee_packet *xp);
uint16_t xbee_rxpt_get_payload_size(struct xbee_packet *xp);

#endif
