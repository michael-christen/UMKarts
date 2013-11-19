#ifndef _XBEE_INTERFACE_
#define _XBEE_INTERFACE_
#include "xbee.h"

/* Can't be higher than 32, because allocator can only hold 32 of an item */
#define XBEE_PACKET_BUF_SIZE 32 
#if XBEE_PACKET_BUF_SIZE > 32
#error "XBEE_PACKET_BUF_SIZE must be less than or equal to 32 because SimpleAllocator restrictions"
#endif

/*
 * Allocate the xbee_interface
 */
int xbee_interface_init();

/* Allocator for XBee packets */
struct xbee_packet * xbee_interface_create_packet(); 
void xbee_interface_free_packet(struct xbee_packet *xp); 

#endif
