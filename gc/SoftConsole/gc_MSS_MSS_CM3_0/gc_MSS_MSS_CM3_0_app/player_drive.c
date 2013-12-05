/*
 * PLAYER_DRIVE.c
 *
 *  Created on: Dec 4, 2013
 *      Author: mttschlt
 */

#include "drivers/mss_rtc/mss_rtc.h"

#include "controller.h"
#include "item.h"
#include "motor.h"
#include "player_drive.h"

// Millisecs to rtc conversion factor
static const int MS_TO_COUNT = 33;


struct PLAYER_DRIVE {
	enum PLAYER_MOTOR_SPEED speed;
	enum PLAYER_MOTOR_DIRECTION motor_direction;
	enum PLAYER_SERVO_DIRECTION servo_direction;
	void (*mod)();
	uint64_t mod_stop;
};

// The global that will be our driver
static struct PLAYER_DRIVE player_driver = {NORMAL, BRAKE, STRAIGHT, 0, 0};

void PLAYER_DRIVE_reset() {
	player_driver.speed = NORMAL;
	player_driver.motor_direction = BRAKE;
	player_driver.servo_direction = STRAIGHT;
	player_driver.mod = 0;
	player_driver.mod_stop = 0;
}

static void PLAYER_DRIVE_update_from_controller() {
	CONTROLLER_load();
	if (CONTROLLER->a && CONTROLLER->b){
		player_driver.motor_direction = BRAKE;
	} else if (CONTROLLER->a) {
		player_driver.motor_direction = FORWARD;
	} else if (CONTROLLER->b) {
		player_driver.motor_direction = REVERSE;
	} else {
		player_driver.motor_direction = BRAKE;
	}

	if (CONTROLLER->a || CONTROLLER->b) {
		player_driver.speed = NORMAL;
	} else {
		player_driver.speed = BRAKE;
	}

	if (CONTROLLER->d_right || CONTROLLER->joystick_x > 158) {
		player_driver.servo_direction = RIGHT;
	} else if (CONTROLLER->d_left || CONTROLLER->joystick_x < 98) {
		player_driver.servo_direction = LEFT;
	} else {
		player_driver.servo_direction = STRAIGHT;
	}

	if (CONTROLLER->l) {
		useCurrentItem();
	} else if (CONTROLLER->x) {
		handleItemGrab();
	} else if (CONTROLLER->y) {
		use_green_shell();
	}
}

void PLAYER_DRIVE_update() {
	PLAYER_DRIVE_update_from_controller();
	if (player_driver.mod == 0) return;

	// Deal with modifications
	if (MSS_RTC_get_seconds_count() >= player_driver.mod_stop) {
		player_driver.mod = 0;
		player_driver.mod_stop = 0;
	} else {
		player_driver.mod();
	}
}

void PLAYER_DRIVE_set_modification(void (*mod)(), uint32_t secs) {
	player_driver.mod = mod;
	player_driver.mod_stop = MSS_RTC_get_seconds_count() + secs;
}

void PLAYER_DRIVE_apply() {
	// Deal with the servo
	MOTOR_set_servo_direction((int)player_driver.servo_direction);

	// Deal with the motor
	double speed = ((int)(player_driver.speed)) / 10.0;
	MOTOR_set_speed(speed * (int)(player_driver.motor_direction));
}

void mod_disable_motors_and_servos() {
	player_driver.motor_direction = BRAKE;
	player_driver.servo_direction = STRAIGHT;
}

void mod_speed_boost() {
	player_driver.speed = BOOSTED;
}

void mod_speed_slow() {
	player_driver.speed = SLOWED;
}
