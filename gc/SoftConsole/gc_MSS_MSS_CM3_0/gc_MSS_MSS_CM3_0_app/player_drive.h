// mttschlt
// Used to keep track of a player's state

#ifndef __MATT_PLAYER_STATE__
#define __MATT_PLAYER_DRIVE__

// Scale is 0 - 10 for MOTOR_SPEED (its really 0 - 1)
enum PLAYER_MOTOR_SPEED {NORMAL = 7, SLOWED = 5, BOOSTED = 10};
// BRAKE is just free roll
enum PLAYER_MOTOR_DIRECTION {BRAKE = 0, FORWARD = 1, REVERSE = -1};
enum PLAYER_SERVO_DIRECTION {STRAIGHT = 0, LEFT = -1, RIGHT = 1};


// Effects: Resets the player state to beginning state
//          Motor off, Servo straight
//          No item
void PLAYER_DRIVE_reset();

// Effects: updates the player state. This should be running in the main loop
void PLAYER_DRIVE_update();

// EFF: Applies the state to the motor and servos. Should be called after PLAYER_DRIVE_update
void PLAYER_DRIVE_apply();

// Effects: adds a modification to the player and a timeout. This will overwrite any previous modifications
void PLAYER_DRIVE_set_modification(void (*mod)(), uint32_t secs);

// Start list of modification functions
void mod_disable_motors_and_servos();
void mod_speed_boost();
void mod_speed_slow();
void mod_star();
void mod_hit_by_shell();
#endif
