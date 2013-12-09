#include "player.h"
//#include "oled.h"
#include "xbee.h"
#include "xbee_interface.h"
#include <errno.h>
#include <string.h>

struct PlayerTable g_player_table;

const char* driver_names[DRIVER_INVALID] = {
		"Mario",
		"Luigi",
		"Wario",
		"Peach",
};

static struct {
	uint64_t _address;
	enum {
		ADDRESS_UNSET, ADDRESS_LOW_SET, ADDRESS_HIGH_SET, ADDRESS_DONE,
	} _state;
} _myPlayerInfo;

static struct {
	uint64_t _address;
	Driver _driver;
} _drive_lookup_table[] = { { 0x0013A200407A866BULL, MARIO }, {
		0x0013A200407A8640ULL, LUIGI }, { 0x0013A200407A8676ULL, WARIO }, {
		0x0013A20040AE5BE4ULL, PEACH }, };

Driver DRIVER = PEACH;

int player_add_player(uint64_t p) {
	int i;
	if (player_get_driver_from_address(p) == DRIVER_INVALID) {
		return -EINVAL;
	}
	/* Check to see if already exists */
	for (i = 0; i < g_player_table.size; i++) {
		if (g_player_table.players[i] == p)
			return 0;
	}
	g_player_table.size += 1;
	g_player_table.players[i] = p;
	return 0;
}

int player_remove_player(uint64_t p) {
	int i, state;
	state = 0;
	for (i = 0; i < g_player_table.size; i++) {
		if (!state) {
			if (g_player_table.players[i] == p)
				state = 1;
		}
		else {
			g_player_table.players[i - 1] = g_player_table.players[i];
		}
	}
	if (state) {
		g_player_table.size -= 1;
		return 0;
	}
	else {
		return -EINVAL;
	}
}

int player_exists(uint64_t p) {
	int i;
	for (i = 0; i < g_player_table.size; i++) {
		if (g_player_table.players[i] == p)
			return 1;
	}
	return 0;
}

Driver player_get_driver_from_address(uint64_t p) {
	int i;
	for (i = 0; i < sizeof(_drive_lookup_table)
			/ sizeof(_drive_lookup_table[0]); i++) {
		if (_drive_lookup_table[i]._address == p) {
			return _drive_lookup_table[i]._driver;
		}
	}
	return DRIVER_INVALID;
}

uint64_t player_get_address_from_driver(Driver d) {
	int i;
	for (i = 0; i < sizeof(_drive_lookup_table)
			/ sizeof(_drive_lookup_table[0]); i++) {
		if (_drive_lookup_table[i]._driver == d) {
			return _drive_lookup_table[i]._address;
		}
	}
	/* Not really possible to make it here, but just in case */
	return 0ull;
}

void player_init() {
	_myPlayerInfo._state = ADDRESS_UNSET;
	memset(&g_player_table, 0, sizeof(g_player_table));
}

uint8_t player_address_complete() {
	return _myPlayerInfo._state == ADDRESS_DONE;
}

void player_set_low_address(uint32_t low) {
	if (_myPlayerInfo._state == ADDRESS_HIGH_SET) {
		_myPlayerInfo._address += (0xFFFFFFFFull & low);
		_myPlayerInfo._state = ADDRESS_DONE;
		/*
		if (DRIVER != player_get_driver_from_address(_myPlayerInfo._address)) {
			struct oled_data write_data;

			write_data.line1 = FIRST_LINE;
			write_data.char_offset1 = 0;

			write_data.contrast_val = 0x01;

			write_data.string1 = driver_names[player_get_driver_from_address(_myPlayerInfo._address)];
			OLED_write_data(&write_data, FIRST_LINE);
		}
		*/
		DRIVER = player_get_driver_from_address(_myPlayerInfo._address);
	} else if (_myPlayerInfo._state == ADDRESS_UNSET) {
		_myPlayerInfo._address = (0xFFFFFFFFull & low);
		_myPlayerInfo._state = ADDRESS_LOW_SET;
	}
}

void player_set_high_address(uint32_t high) {
	if (_myPlayerInfo._state == ADDRESS_LOW_SET) {
		_myPlayerInfo._address += ((uint64_t)(0xFFFFFFFF00000000ull
				& ((uint64_t) high << 32)));
		_myPlayerInfo._state = ADDRESS_DONE;
		/*
		if (DRIVER != player_get_driver_from_address(_myPlayerInfo._address)) {
			struct oled_data write_data;

			write_data.line1 = FIRST_LINE;
			write_data.char_offset1 = 0;

			write_data.contrast_val = 0x01;

			write_data.string1 = driver_names[player_get_driver_from_address(_myPlayerInfo._address)];
			OLED_write_data(&write_data, FIRST_LINE);
		}
		*/
		DRIVER = player_get_driver_from_address(_myPlayerInfo._address);
	} else if (_myPlayerInfo._state == ADDRESS_UNSET) {
		_myPlayerInfo._address = ((uint64_t)(0xFFFFFFFF00000000ull
				& ((uint64_t) high << 32)));
		_myPlayerInfo._state = ADDRESS_HIGH_SET;
	}
}

int driver_discovery() {
	int err;
	struct xbee_packet *xp;
	xp = xbee_interface_create_packet();
	if (!xp)
		return -ENOMEM;
	xp->payload[0] = XBEE_API_AT_COMMAND;
	xp->payload[1] = xbee_interface_next_frame_id();
	xp->payload[2] = 'S';
	xp->payload[3] = 'H';
	xp->len = 4;
	err = xbee_send(xp);
	if (err < 0) {
		xbee_interface_free_packet(xp);
	} else {
		xp = xbee_interface_create_packet();
		if (!xp)
			return -ENOMEM;
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

