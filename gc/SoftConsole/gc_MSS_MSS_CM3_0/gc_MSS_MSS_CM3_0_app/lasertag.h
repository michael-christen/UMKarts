// Wrapper for IR_LED class to play laser tag
#include <stdbool.h>

/*
 * Initializes the Laser tag system
 */
void LASER_TAG_init();

bool LASER_TAG_shoot();

/*
 * Returns whether or not you got a valid hit
 */
uint8_t LASER_TAG_hit();
