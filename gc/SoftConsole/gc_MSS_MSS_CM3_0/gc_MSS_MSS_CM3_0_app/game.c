#include "game.h"
#include "player.h"
#include "messages.h"
#include "mss_rtc.h"
#include "sound.h"
#include "player_drive.h"
#include "sound_samples.h"
#include <errno.h>

enum GameState g_game_state;
uint64_t g_game_host;
const char *g_game_state_str[] = {
	"GAME WAIT",
	"GAME HOST",
	"GAME JOIN",
	"GAME IN GAME",
	"GAME OVER",
};

static uint32_t _game_host_announce_last_time;
static uint32_t _game_join_last_ack_time;

void game_init() {
	g_game_state = GAME_WAIT;
	g_game_host = 0;
	_game_host_announce_last_time = 0;
}

int game_trans_wait_to_host() {
	if (g_game_state == GAME_WAIT && player_address_complete()) {
		player_add_player(player_get_address_from_driver(DRIVER));
		g_game_host = player_get_address_from_driver(DRIVER);
		g_game_state = GAME_HOST;
		/* Don't send a message here. Messages will be sent in the main loop */
		xbee_printf("GAME STATE TRANS: WAIT -> HOST");
		return 0;
	}
	return -EINVAL;
}

int game_trans_host_to_join(uint64_t host) {
	if (g_game_state == GAME_HOST) {
		g_game_host = host;
		g_game_state = GAME_JOIN;
		xbee_printf("GAME STATE TRANS: HOST -> JOIN");
		game_join_set_last_ack();
		return 0;
	}
	return -EINVAL;
}

int game_trans_join_to_host() {
	if (g_game_state == GAME_JOIN) {
		g_game_state = GAME_HOST;
		player_add_player(player_get_address_from_driver(DRIVER));
		g_game_host = player_get_address_from_driver(DRIVER);
		xbee_printf("GAME STATE TRANS: JOIN -> HOST");
		return 0;
	}
	return -EINVAL;
}

int game_trans_join_to_wait() {
	if (g_game_state == GAME_JOIN) {
		g_game_state = GAME_WAIT;
		xbee_printf("GAME STATE TRANS: JOIN -> WAIT");
		return 0;
	}
	return -EINVAL;
}

int game_trans_join_to_in_game() {
	if (g_game_state == GAME_JOIN) {
		g_game_state = GAME_IN_GAME;
		xbee_printf("GAME STATE TRANS: JOIN -> IN GAME");
		game_trans_start_game();
		return 0;
	}
	return -EINVAL;
}

int game_trans_host_to_in_game() {
	if (g_game_state == GAME_HOST) {
		g_game_state = GAME_IN_GAME;
		xbee_printf("GAME STATE TRANS: HOST -> IN GAME");
		game_trans_start_game();
		return 0;
	}
	return -EINVAL;
}

int game_trans_host_to_wait() {
	if (g_game_state == GAME_HOST) {
		g_game_state = GAME_WAIT;
		xbee_printf("GAME STATE TRANS: HOST -> WAIT");
		return 0;
	}
	return -EINVAL;
}

int game_trans_in_game_to_over() {
	if (g_game_state == GAME_IN_GAME) {
		g_game_state = GAME_OVER;
		return 0;
	}
	return -EINVAL;
}

int game_trans_over_to_wait() {
	if (g_game_state == GAME_OVER) {
		g_game_state = GAME_WAIT;
		return 0;
	}
	return -EINVAL;
}

int game_trans_start_game() {
	sound_play(START_BEGIN, START_END);
	PLAYER_DRIVE_set_modification(mod_disable_motors_and_servos, 3);
	return 0;
}

uint8_t game_host_announce_wait_long_enough() {
	if (MSS_RTC_get_seconds_count() - _game_host_announce_last_time > GAME_HOST_ANNOUNCE_DELAY) {
		return 1;
	}
	return 0;
}

void game_host_announce_set_last_announce() {
	_game_host_announce_last_time = MSS_RTC_get_seconds_count();
}

uint8_t game_join_ack_timeout() {
	if (MSS_RTC_get_seconds_count() - _game_join_last_ack_time > GAME_JOIN_ACK_TIMEOUT) {
		return 1;
	}
	return 0;
}

void game_join_set_last_ack() {
	_game_join_last_ack_time = MSS_RTC_get_seconds_count();
}
