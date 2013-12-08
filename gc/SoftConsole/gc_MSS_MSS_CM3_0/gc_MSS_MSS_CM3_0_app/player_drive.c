/*
 * PLAYER_DRIVE.c
 *
 *  Created on: Dec 4, 2013
 *      Author: mttschlt
 */

#include "drivers/mss_rtc/mss_rtc.h"
#include "drivers/mss_gpio/mss_gpio.h"

#include "controller.h"
#include "item.h"
#include "motor.h"
#include "player_drive.h"
#include "sound.h"

#include <stdio.h>

// Millisecs to rtc conversion factor
static const int MS_TO_COUNT = 33;


struct PLAYER_DRIVE {
	enum PLAYER_MOTOR_SPEED speed;
	enum PLAYER_MOTOR_DIRECTION motor_direction;
	enum PLAYER_SERVO_DIRECTION servo_direction;
	void (*mod)();
	uint64_t mod_stop;
	uint8_t invincible;
};

// The global that will be our driver
static struct PLAYER_DRIVE player_driver = {NORMAL, FREEROLL, STRAIGHT, 0, 0};

void PLAYER_DRIVE_reset() {
	player_driver.speed = NORMAL;
	player_driver.motor_direction = FREEROLL;
	player_driver.servo_direction = STRAIGHT;
	player_driver.mod = 0;
	player_driver.mod_stop = 0;
	player_driver.invincible = 0;
}

static void PLAYER_DRIVE_update_from_controller() {
	CONTROLLER_load();

	// Set Motor Direction
	if (CONTROLLER->a && CONTROLLER->b){
		player_driver.motor_direction = FREEROLL;
	} else if (CONTROLLER->a) {
		player_driver.motor_direction = FORWARD;
	} else if (CONTROLLER->b) {
		player_driver.motor_direction = REVERSE;
	} else {
		player_driver.motor_direction = FREEROLL;
	}

	// Set Motor Speed
	if (CONTROLLER->a || CONTROLLER->b) {
		player_driver.speed = NORMAL;
	} else {
		player_driver.speed = FREEROLL;
	}

	// Set Servo Direction
	if (CONTROLLER->d_right || CONTROLLER->joystick_x > 158) {
		player_driver.servo_direction = RIGHT;
	} else if (CONTROLLER->d_left || CONTROLLER->joystick_x < 98) {
		player_driver.servo_direction = LEFT;
	} else {
		player_driver.servo_direction = STRAIGHT;
	}

	// Deal with Items
	if (CONTROLLER->l) {
		useCurrentItem();
	} else if (CONTROLLER->x) {
		handleItemGrab();
	} else if (CONTROLLER->y) {
		use_green_shell();
	}
}

static void PLAYER_DRIVE_remove_modification() {
	if (player_driver.mod == mod_star) {
		sound_stop();
	} else if (player_driver.mod == mod_hit_by_shell) {
		MSS_GPIO_set_output(MSS_GPIO_3, 1);
	}
	player_driver.mod = 0;
	player_driver.mod_stop = 0;
}

void PLAYER_DRIVE_update() {
	PLAYER_DRIVE_update_from_controller();
	player_driver.invincible = 0;
	if (player_driver.mod == 0) return;

	// Deal with modifications
	if (player_driver.mod && MSS_RTC_get_seconds_count() >= player_driver.mod_stop) {
		PLAYER_DRIVE_remove_modification();
	} else {
		player_driver.mod();
	}
}

void PLAYER_DRIVE_set_modification(void (*mod)(), uint32_t secs) {
	PLAYER_DRIVE_remove_modification();
	player_driver.mod = mod;
	player_driver.mod_stop = MSS_RTC_get_seconds_count() + secs;
}

void PLAYER_DRIVE_apply() {
	// Deal with the servo
	MOTOR_set_servo_direction((int)player_driver.servo_direction);

	double speed = ((int)(player_driver.speed)) / 10.0;
	MOTOR_set_speed(speed * (int)(player_driver.motor_direction));
}

uint8_t PLAYER_DRIVE_is_invincible() {
	return player_driver.invincible;
}

void PLAYER_DRIVE_set_invincible() {
	player_driver.invincible = 1;
}

void mod_disable_motors_and_servos() {
	player_driver.motor_direction = FREEROLL;
	player_driver.speed = 0;
	player_driver.servo_direction = STRAIGHT;
}

void mod_speed_boost() {
	player_driver.speed = BOOSTED;
}

void mod_speed_slow() {
	player_driver.speed = SLOWED;
}

void mod_star() {
	mod_speed_boost();
	player_driver.invincible = 1;
}

void mod_hit_by_shell() {
	mod_disable_motors_and_servos();
	player_driver.invincible = 1;
	MSS_GPIO_set_output(MSS_GPIO_3, 0);
}
void mod_hit_by_lightning() {
	mod_speed_slow();
	player_driver.invincible = 1;
	MSS_GPIO_set_output(MSS_GPIO_3, 0);
}
