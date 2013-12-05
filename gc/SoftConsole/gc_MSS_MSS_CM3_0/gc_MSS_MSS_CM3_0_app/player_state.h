// mttschlt
// Used to keep track of a player's state

#ifndef __MATT_PLAYER_STATE__
#define __MATT_PLAYER_STATE__

enum PLAYER_MOTOR_SPEED {NORMAL, SLOWED, BOOSTED};
// BRAKE is just free roll
enum PLAYER_MOTOR_DIRECTION {BRAKE = 0, FORWARD = 1, REVERSE = -1};
enum PLAYER_SERVO_DIRECTION {STRAIGHT = 0, LEFT = -1, RIGHT = 1};

struct player_state;

// Effects: Resets the player state to beginning state
//          Motor off, Servo straight
//          No item
void PLAYER_STATE_reset(struct player_state*);

// Effects: updates the player state. This should be running in the main loop
void PLAYER_STATE_update(struct player_state*);

// Effects: applies the state to the motor and servos
void PLAYER_STATE_apply(struct player_state*);

// Effects: adds a modification to the player and a timeout
void PLAYER_STATE_set_modification(struct player_state*, void (*mod)(struct player_state*), uint32_t millisecs);
#endif
