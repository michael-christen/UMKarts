#ifndef MARIO_XBEE_H_
#define MARIO_XBEE_H_
#include "xbee.h"

int mario_xbee_interpret_packet(struct xbee_packet *);

/**
 * Prints over xbee a message to be printed by someone else, probably the listener.
 *
 * This function prints a message over the network, to the specified listener. It should
 * be used for debugging purposes, and can be called from wherever you want whenver you want,
 * it should deal with interrupts no problem. The constraint is that the final length of the
 * message MUST be less than max payload size of one xbee packet. If it is great, the message
 * will be truncated, and no error will be returned.
 * TODO: Return an error if message is truncated.
 */
int xbee_printf(const char * format_string, ...);

#endif /* MARIO_XBEE_H_ */
