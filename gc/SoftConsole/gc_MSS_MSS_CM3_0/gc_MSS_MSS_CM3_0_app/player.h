#ifndef __PLAYER_H_
#define __PLAYER_H_

#include <inttypes.h>

#define MAX_PLAYERS 16

typedef enum {
	MARIO, LUIGI, WARIO, PEACH, DRIVER_INVALID,
} Driver;

struct PlayerTable {
	uint64_t players[MAX_PLAYERS];
	uint8_t size;
};

extern Driver DRIVER;
extern struct PlayerTable g_player_table;

void player_init();

/**
 * Sends out an xbee packet to find all other cars that are in the area.
 */
int player_discovery();

/**
 * Sends out an xbee packet to figure out who we are. This needs to be called
 * almost immediately because it's important that we figure out who we are so
 * we know what sound to play
 */
int driver_discovery();

/**
 * Returns true or false to let use know if we have received our address yet or
 * not.
 */
uint8_t player_address_complete();

/**
 * Sets the low bits of our address. Also marks address as complete if we have
 * already set the high bits.
 */
void player_set_low_address(uint32_t low);

/**
 * Sets the high bits of our xbee address. Also marks the high bits as complete
 * if we have already set the high bits
 */
void player_set_high_address(uint32_t high);

/**
 * Gets a driver from the address. Returns DRIVER_INVALID if the address does
 * not correspond to an actual address.
 */
Driver player_get_driver_from_address(uint64_t p);

/**
 * Gets the address from a driver. Returns 0 if passed DRIVER_INVALID, because
 * that is not a valid driver. (duh)
 */
uint64_t player_get_address_from_driver(Driver d);

/**
 * Tests to see if a player is registered in our player table.
 */
int player_exists(uint64_t p);

/**
 * Adds a player to our player table.
 */
int player_remove_player(uint64_t p);

/**
 * Removes a player from our player table.
 */
int player_add_player(uint64_t p);

#endif /* End _PLAYER_H */
