#ifndef __PLAYER_H_
#define __PLAYER_H_

#define MAX_PLAYERS 16

struct Player {
	uint64_t address;
	uint8_t id;
	uint8_t valid;
};

struct PlayerTable {
	struct Player players[MAX_PLAYERS];
	uint8_t size;
};

struct PlayerTableIter {
	int _pos;
	(struct Player *) (*next)();
	struct PlayerTable * _table;
};

void player_discovery();

struct PlayerTableIter player_table_iter();

#endif /* End _PLAYER_H */
