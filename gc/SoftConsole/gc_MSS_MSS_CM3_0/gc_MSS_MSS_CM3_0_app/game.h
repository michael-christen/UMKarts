#ifndef __GAME_H_
#define __GAME_H_

#include <inttypes.h>

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
#endif /* __GAME_H_ */
