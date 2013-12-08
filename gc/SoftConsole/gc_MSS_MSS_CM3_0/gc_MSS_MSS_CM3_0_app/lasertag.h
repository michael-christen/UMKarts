// Wrapper for IR_LED class to play laser tag
#ifndef __MATT_LASER_TAG__
#define __MATT_LASER_TAG__
#include <stdbool.h>
#include <stdint.h>

/*
 * Initializes the Laser tag system
 */
void LASER_TAG_init();

bool LASER_TAG_shoot();

/*
 * Returns whether or not you got a valid hit
 */
uint8_t LASER_TAG_hit();

/*
 * Sets the LED in the IR Reciever on or off
 *
 * @param int on 1 = on
 *               0 = off
 *               All other input is invalid
 */
void LASER_TAG_set_hit_LED(uint8_t on);
#endif
