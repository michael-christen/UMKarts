/*
 * player_state.c
 *
 *  Created on: Dec 4, 2013
 *      Author: mttschlt
 */

#include "drivers/mss_rtc/mss_rtc.h"
#include "controller.h"

#include "player_state.h"

static const int MS_TO_COUNT = 33;

struct player_state {
	PLAYER_MOTOR_SPEED speed;
	PLAYER_MOTOR_DIRECTION motor_direction;
	PLAYER_SERVO_DIRECTION servo_direction;
	item i;
	void (*mod)(player_state*);
	uint64_t mod_stop;
};

void PLAYER_STATE_reset(player_state* state) {
	state->speed = NORMAL;
	state->motor_direction = BRAKE;
	state->servo_direction = STRAIGHT;
	state->i = MAX_NUM_ITEMS;
	state->mod = 0;
	state->mod_stop = 0;
}

void PLAYER_STATE_update_from_controller(player_state* state) {
	CONTROLLER_load();
	if (CONTROLLER->a && CONTROLLER->b)
		state->motor_direction = BRAKE;
	else if (CONTROLLER->a) {
		state->motor_directon = FORWARD;
	} else if (CONTROLLER->b) {
		state->motor_directon = FORWARD;
	} else {
		state->motor_direction = FREE_ROLL;
	}

	if (CONTROLLER->d_right || CONTROLLER->joystick_x > 158) {
		state->servo_direction = RIGHT;
	} else if (CONTROLLER->d_left || CONTROLLER->joystick_x < 98) {
		state->servo_direction = LEFT;
	} else {
		state->servo_direction = STRAIGHT;
	}

	if (CONTROLLER->l) {
		useCurrentItem();
		state->i = MAX_NUM_ITEMS;
	} else if (CONTROLLER->x) {
		handleItemGrab();
	} else if (CONTROLLER->y) {
		use_green_shell();
	}
}

void PLAYER_STATE_update(player_state* state) {
	PLAYER_STATE_update_from_controller(state);
	if (state->mod == 0) return;
	if (MSS_RTC_get_rtc_count() >= state->mod_stop) {
		state->mod = 0;
		state->mod_stop = 0;
	} else {
		state->mod(state);
	}
}

void PLAYER_STATE_set_modification(player_state* state, void (*mod)(player_state*), uint32_t millisecs) {
	state->mod = mod;
	state->mod_stop = MSS_RTC_get_rtc_count() + (millisecs * MS_TO_COUNT);
}
