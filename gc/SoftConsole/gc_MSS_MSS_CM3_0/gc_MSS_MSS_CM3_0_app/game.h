#ifndef __GAME_H_
#define __GAME_H_

#include <inttypes.h>

#define GAME_HOST_ANNOUNCE_DELAY 1
#define GAME_JOIN_ACK_TIMEOUT    3

enum GameState {
	GAME_WAIT,
	GAME_HOST,
	GAME_JOIN,
	GAME_IN_GAME,
	GAME_OVER,
};

extern enum GameState g_game_state;
extern const char *g_game_state_str[];
extern uint64_t g_game_host;

void game_init();

int game_trans_wait_to_host();
int game_trans_host_to_join();
int game_trans_host_to_in_game();
int game_trans_join_to_in_game();
int game_trans_in_game_to_over();
int game_trans_over_to_wait();
int game_trans_host_to_wait();
int game_trans_join_to_host();
int game_trans_join_to_wait();

int game_trans_start_game();

uint8_t game_host_announce_wait_long_enough();
void game_host_announce_set_last_announce();

uint8_t game_join_ack_timeout();
void game_join_set_last_ack();

#endif /* __GAME_H_ */
