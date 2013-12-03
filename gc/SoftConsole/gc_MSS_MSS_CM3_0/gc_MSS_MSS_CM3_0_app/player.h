#ifndef __PLAYER_H_
#define __PLAYER_H_

#include <inttypes.h>

#define MAX_PLAYERS 16

typedef enum {
    MARIO,
    LUIGI,
    WARIO,
    PEACH,
		DRIVER_INVALID,
} Driver;

extern Driver DRIVER;

struct Player {
	uint64_t address;
	Driver Driver;
	uint8_t valid;
};

struct PlayerTable {
	struct Player players[MAX_PLAYERS];
	uint8_t size;
};

struct PlayerTableIter {
	int _pos;
	struct Player * (*next)();
	struct PlayerTable * _table;
};

/**
 * Sends out an xbee packet to find all other cars that are in the area.
 */
void player_discovery();

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
void player_set_low_address(uint8_t low); 

/**
 * Sets the high bits of our xbee address. Also marks the high bits as complete
 * if we have already set the high bits
 */
void player_set_high_address(uint8_t high); 

struct PlayerTableIter player_table_iter();

#endif /* End _PLAYER_H */
