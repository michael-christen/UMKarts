#include "game.h"
#include "player.h"
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

void game_init() {
	g_game_state = GAME_WAIT;
	g_game_host = 0;
}

int game_trans_wait_to_host() {
	if (g_game_state == GAME_WAIT && player_address_complete()) {
		g_game_host = player_get_address_from_driver(DRIVER);
		g_game_state = GAME_HOST;
		/* Don't send a message here. Messages will be sent in the main loop */
		return 0;
	}
	return -EINVAL;
}

int game_trans_host_to_join(uint64_t host) {
	if (g_game_state == GAME_HOST) {
		g_game_host = host;
		g_game_state = GAME_JOIN;
		return 0;
	}
	return -EINVAL;
}

int game_trans_join_to_in_game() {
	if (g_game_state == GAME_JOIN) {
		g_game_state = GAME_IN_GAME;
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
