#ifndef _XBEE_INTERFACE_
#define _XBEE_INTERFACE_
#include "xbee.h"

#define EXBEEALLOCMEM 0x2

#define XBEE_RX_OVERFLOW_PACKET_LOSS 0x01
#define XBEE_RX_BAD_PACKET           0x02
#define XBEE_RX_OUTOF_MEMORY         0x04

/* Can't be higher than 32, because allocator can only hold 32 of an item */
#define XBEE_PACKET_BUF_SIZE 32
#if XBEE_PACKET_BUF_SIZE > 32
#error "XBEE_PACKET_BUF_SIZE must be less than or equal to 32 because SimpleAllocator restrictions"
#endif

#define XBEE_INTERFACE_EXPECT_RESPONSE(xp) (xp->payload[1] != 0x0)

/*
 * Allocate the xbee_interface
 */
int xbee_interface_init();
uint8_t xbee_interface_next_frame_id();

/* Allocator for XBee packets */
struct xbee_packet * xbee_interface_create_packet();
void xbee_interface_free_packet(const struct xbee_packet *xp);

/* XBee send and receive functions */

/**
 * Adds this xbee_packet to the queue to be sent.
 *
 * We add the passed in xbee_packet to the queue of packets to be sent, and
 * start sending it if no other packet it currently being sent. We require
 * that the xbee_packet was allocated using xbee_interface_create_packet,
 * because we try to deallocate it at the end of the function. (Actually, this
 * will probably work just fine if we can't de allocate, but that isn't
 * explictily supported). HOWEVER, we require that the memory passed into this
 * function will exist after the packet is put into the queue. We don't copy
 * the memory. In the event of an error, it is up to the programmer to
 * deallocate the xbee_packet, because we do not take care of it.
 *
 * @xp - Packet, allocated with xbee_interface_create_packet, that we want to
 *       add to the queue.
 * @ret - 0 if Not error. -EBUFFULL in case the buffer is already full and we
 *        can't insert a new item. If we can't insert a new item, the caller
 *        must deallocated xbee_packet, otherwise it will be deallocated when
 *        the entire packet is sent.
 */
int xbee_send(struct xbee_packet *);

/**
 * When sending, we lock the uart device for up to 0.5 seconds to wait for a
 * response from the XBee. when we are expeciting a response. This function
 * lifts that lock, and is mostly called from the xbee received uart interrupt.
 *
 * This is part of a collection of functions that insures that status packets
 * will be received in the order they are sent. When you receive a rx status
 * packet or a AT command response, just call
 * xbee_interface_tx_next_status_packet() and you will be given the
 * corresponding status packet.
 */
void xbee_interface_tx_unlock_wait();
struct xbee_packet * xbee_interface_tx_next_status_packet() {

/**
 * Reads the next available packet from the buffer.
 *
 * This returns a struct of the next available packet, or NULL if the buffer
 * is currently empty and there are no packets to read. The packet returned
 * was created with xbee_interface_create_packet, and it is expected that the
 * returned packet be freed when it is no longer useful to the programmer
 *
 * @ret - Valid xbee_packet if there is one available, else NULL
 */
struct xbee_packet * xbee_read();

/**
 * Get the error flags from the xbee_reader. This will tell us if we've received
 * any bad packets, or had any errors in receiving packets.
 *
 * EX:
 *   xbee_read_get_errors() & XBEE_RX_OVERFLOW_PACKET_LOSS - Checks for packet loss
 *   xbee_read_get_errors() & XBEE_RX_BAD_PACKET - Check for bad checksum
 *   xbee_read_get_errors() & XBEE_RX_OUTOF_MEMORY - Check out of memory
 */
uint8_t xbee_read_get_errors();

/**
 * Clear the received error flags
 */
void xbee_read_clear_errors();

#endif
