/*
 * player_state.c
 *
 *  Created on: Dec 4, 2013
 *      Author: mttschlt
 */

#include "drivers/mss_rtc/mss_rtc.h"

#include "controller.h"
#include "item.h"
#include "motor.h"
#include "player_state.h"


static const int MS_TO_COUNT = 33;

struct player_state {
	enum PLAYER_MOTOR_SPEED speed;
	enum PLAYER_MOTOR_DIRECTION motor_direction;
	enum PLAYER_SERVO_DIRECTION servo_direction;
	void (*mod)(struct player_state*);
	uint64_t mod_stop;
};

void PLAYER_STATE_reset(struct player_state* state) {
	state->speed = NORMAL;
	state->motor_direction = BRAKE;
	state->servo_direction = STRAIGHT;
	state->mod = 0;
	state->mod_stop = 0;
}

void PLAYER_STATE_update_from_controller(struct player_state* state) {
	CONTROLLER_load();
	if (CONTROLLER->a && CONTROLLER->b)
		state->motor_direction = BRAKE;
	else if (CONTROLLER->a) {
		state->motor_direction = FORWARD;
	} else if (CONTROLLER->b) {
		state->motor_direction = FORWARD;
	} else {
		state->motor_direction = BRAKE;
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
	} else if (CONTROLLER->x) {
		handleItemGrab();
	} else if (CONTROLLER->y) {
		use_green_shell();
	}
}

void PLAYER_STATE_update(struct player_state* state) {
	PLAYER_STATE_update_from_controller(state);
	if (state->mod == 0) return;

	// Deal with modifications
	if (MSS_RTC_get_rtc_count() >= state->mod_stop) {
		state->mod = 0;
		state->mod_stop = 0;
	} else {
		state->mod(state);
	}
}

void PLAYER_STATE_set_modification(struct player_state* state, void (*mod)(struct player_state*), uint32_t millisecs) {
	state->mod = mod;
	state->mod_stop = MSS_RTC_get_rtc_count() + (millisecs * MS_TO_COUNT);
}

// Item pick up is dealt with separately!
void PLAYER_STATE_apply(struct player_state* state) {
	// Deal with the servo
	MOTOR_set_servo_direction((int)state->servo_direction);

	// Deal with the motor
	double speed = 0;
	if (state->speed == BOOSTED) {
		speed = 1.0;
	} else if (state->speed == SLOWED) {
		speed = 0.5;
	} else {
		speed = 0.7;
	}
	MOTOR_set_speed(speed * (int)(state->motor_direction));
}
