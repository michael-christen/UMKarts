#ifndef __PLAYER_H_
#define __PLAYER_H_

#define MAX_PLAYERS 16

struct Player {
	uint64_t address;
	uint8_t id;
};

struct PlayerTable {
	struct Player players[MAX_PLAYERS];
	uint8_t size;
};

void player_discovery();

#endif /* End _PLAYER_H */
