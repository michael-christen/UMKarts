// mttschlt
// Used to keep track of a player's state

#ifndef __MATT_PLAYER_STATE__
#define __MATT_PLAYER_STATE__
#include "item.h"

enum PLAYER_MOTOR_SPEED {NORMAL, SLOWED, BOOSTED};
enum PLAYER_MOTOR_DIRECTION {BRAKE = 0, FORWARD = 1, REVERSE = -1, FREE_ROLL = 2};
enum PLAYER_SERVO_DIRECTION {STRAIGHT = 0, LEFT = -1, RIGHT = 1};

struct player_state;

void PLAYER_STATE_reset(player_state* state);
void PLAYER_STATE_update(player_state* state);
void PLAYER_STATE_apply(player_state* state);
void PLAYER_STATE_set_modification(player_state* state, void (*mod)(player_state*), uint32_t millisecs);
#endif
