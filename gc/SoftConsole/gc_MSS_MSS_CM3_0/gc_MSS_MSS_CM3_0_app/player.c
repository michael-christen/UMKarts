#include "player.h"

static struct PlayerTable _players;


static struct Player * _iter_next(struct PlayerTableIter * self) {
	struct Player *player;
	for( ; self->_pos < MAX_PLAYERS; self->_pos++) {
		if (self->_table[self->_pos].valid) {
			player = self->_table[self->_pos];
			self->_pos++;
			return player;
		}
	}
	return NULL;
}


struct struct PlayerTableIter player_table_iter() {
	struct PlayerTableIter it;
	it._pos = 0;
	it._table = _players;
	it.next = _iter_next;
	return it;
}
