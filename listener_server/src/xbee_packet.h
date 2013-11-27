#ifndef __XBEE_PACKET_INT_H_
#define __XBEE_PACKET_INT_H_

#include "xbee.h"
#include "serial.h"
#include <inttypes.h>

int xbee_get_next_packet(xbee_serial_t *serial, struct xbee_packet * xp); 
int xbee_received_packet(struct xbee_packet *xp); 

#endif
