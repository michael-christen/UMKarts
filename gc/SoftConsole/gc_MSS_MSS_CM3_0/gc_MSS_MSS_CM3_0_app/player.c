#include "player.h"
#include "xbee.h"
#include "xbee_interface.h"
#include <errno.h>
#include <string.h>

static struct PlayerTable _players;

static struct {
	uint64_t _address;
	enum {
		ADDRESS_UNSET,
		ADDRESS_LOW_SET,
		ADDRESS_HIGH_SET,
		ADDRESS_DONE,
	} _state;
} _myPlayerInfo;

static struct {
	uint64_t _address;
	Driver _driver;
} _drive_lookup_table[] = {
	{0x0013A200407A866BULL, MARIO},
	{0x0013A200407A8640ULL, LUIGI},
	{0x0013A200407A8676ULL, WARIO},
	{0x0013A20040AE5BE4ULL, PEACH},
};

Driver DRIVER = PEACH;

static struct Player * _iter_next(struct PlayerTableIter * self) {
	struct Player *player;
	for( ; self->_pos < MAX_PLAYERS; self->_pos++) {
		if (self->_table->players[self->_pos].valid) {
			player = &(self->_table->players[self->_pos]);
			self->_pos++;
			return player;
		}
	}
	return NULL;
}

Driver player_get_driver_from_address(uint64_t p) {
	int i;
	for (i = 0; i < sizeof(_drive_lookup_table) / sizeof(_drive_lookup_table[0]); i++) {
		if (_drive_lookup_table[i]._address == p) {
			return _drive_lookup_table[i]._driver;
		}
	}
	return DRIVER_INVALID;
}

uint64_t player_get_address_from_driver(Driver d) {
	int i;
	for (i = 0; i < sizeof(_drive_lookup_table) / sizeof(_drive_lookup_table[0]); i++) {
		if (_drive_lookup_table[i]._driver == d) {
			return _drive_lookup_table[i]._address;
		}
	}
	/* Not really possible to make it here, but just in case */
	return 0ull;
}

void player_init() {
	_myPlayerInfo._state = ADDRESS_UNSET;
	memset(&_players, 0, sizeof(_players));
}

uint8_t player_address_complete() {
	return _myPlayerInfo._state == ADDRESS_DONE;
}

void player_set_low_address(uint32_t low) {
	if (_myPlayerInfo._state == ADDRESS_HIGH_SET) {
		_myPlayerInfo._address += (0xFFFFFFFFull & low);
		_myPlayerInfo._state = ADDRESS_DONE;
		DRIVER = player_get_driver_from_address(_myPlayerInfo._address);
	}
	else if (_myPlayerInfo._state == ADDRESS_UNSET) {
		_myPlayerInfo._address = (0xFFFFFFFFull & low);
		_myPlayerInfo._state = ADDRESS_LOW_SET;
	}
}

void player_set_high_address(uint32_t high) {
	if (_myPlayerInfo._state == ADDRESS_LOW_SET) {
		_myPlayerInfo._address += ((uint64_t) (0xFFFFFFFF00000000ull & ((uint64_t) high << 32)));
		_myPlayerInfo._state = ADDRESS_DONE;
		DRIVER = player_get_driver_from_address(_myPlayerInfo._address);
	}
	else if (_myPlayerInfo._state == ADDRESS_UNSET) {
		_myPlayerInfo._address = ((uint64_t) (0xFFFFFFFF00000000ull & ((uint64_t) high << 32)));
		_myPlayerInfo._state = ADDRESS_HIGH_SET;
	}
}


struct PlayerTableIter player_table_iter() {
	struct PlayerTableIter it;
	it._pos = 0;
	it._table = &_players;
	it.next = _iter_next;
	return it;
}

int driver_discovery() {
	int err;
	struct xbee_packet *xp;
	xp = xbee_interface_create_packet();
	if (!xp) return -ENOMEM;
	xp->payload[0] = XBEE_API_AT_COMMAND;
	xp->payload[1] = xbee_interface_next_frame_id();
	xp->payload[2] = 'S';
	xp->payload[3] = 'H';
	xp->len = 4;
	err = xbee_send(xp);
	if (err < 0) {
		xbee_interface_free_packet(xp);
	}
	else {
		xp = xbee_interface_create_packet();
		if (!xp) return -ENOMEM;
		xp->payload[0] = XBEE_API_AT_COMMAND;
		xp->payload[1] = xbee_interface_next_frame_id();
		xp->payload[2] = 'S';
		xp->payload[3] = 'L';
		xp->len = 4;
		err = xbee_send(xp);
		if (err < 0) {
			xbee_interface_free_packet(xp);
		}
	}
	return err;
}
