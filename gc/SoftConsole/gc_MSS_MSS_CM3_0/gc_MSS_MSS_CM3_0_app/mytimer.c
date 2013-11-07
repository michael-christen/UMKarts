#include "mytimer.h"
#include <stdio.h>


void MYTIMER_init()
{
    // we don't have to do anything.
}

void MYTIMER_enable()
{
    MYTIMER->control |= MYTIMER_ENABLE_MASK;
}

void MYTIMER_disable()
{
    MYTIMER->control &= ~MYTIMER_ENABLE_MASK;
}

void MYTIMER_setOverflowVal(uint32_t value)
{
	// Yes it's inefficient, but it's written this way to
	// show you the C to assembly mapping.
    uint32_t * timerAddr = (uint32_t*)(MYTIMER);
    *timerAddr = value; // overflowReg is at offset 0x0
}

uint32_t MYTIMER_getCounterVal()
{

// Yes it's inefficient, but it's written this way to
// show you the C to assembly mapping.
    uint32_t * timerAddr = (uint32_t*)(MYTIMER);
    return *(timerAddr+1); // counterReg is at offset 0x4
}

void MYTIMER_enable_allInterrupts()
{
	MYTIMER->control |= 14;
}

void MYTIMER_disable_allInterrupts()
{
	MYTIMER->control &= 49;
}

void MYTIMER_enable_compareInt()
{
	MYTIMER->control |= 6;
}

void MYTIMER_disable_compareInt()
{
	MYTIMER->control &= 59;
}

void MYTIMER_setCompareVal(uint32_t compare)
{
	MYTIMER->compare = compare;
}

void MYTIMER_enable_overflowInt()
{
	MYTIMER->control |= 10;
}

void MYTIMER_disable_overflowInt()
{
	MYTIMER->control &= 55;
}

uint32_t MYTIMER_getInterrupt_status()
{
	return MYTIMER->interrupt;
}

void MYTIMER_enable_pwm()
{
	MYTIMER->control |= 16;
}

void MYTIMER_disable_pwm()
{
	MYTIMER->control &= 47;
}

/**
 * Enable Capture
 */
void MYTIMER_enable_capture()
{
	MYTIMER->control |= 32;
}
/**
 * Disable Capture
 */
void MYTIMER_disable_capture()
{
	MYTIMER->control &= 31;
}

/**
 * Read the synchronous capture value
*/

uint32_t MYTIMER_get_sync_capture()
{
	return MYTIMER->captureSync;
}
/**
 * Read the asynchronous capture value
*/

uint32_t MYTIMER_get_async_capture()
{
	return MYTIMER->captureAsync;
}

void MYTIMER_set_motor_direction(int dir)
{
	if(dir < 0) {
		MYTIMER->direction &= 12;
		MYTIMER->direction |= 1;
	}
	else if(dir == 0) {
		MYTIMER->direction &= 12;
	}
	else {
		MYTIMER->direction &= 12;
		MYTIMER->direction |= 2;
	}
}

void MYTIMER_set_servo_direction(int dir)
{
	if(dir < 0) {
		MYTIMER->direction &= 3;
		MYTIMER->direction |= 4;
	}
	else if(dir == 0) {
		MYTIMER->direction &= 3;
	}
	else {
		MYTIMER->direction &= 3;
		MYTIMER->direction |= 8;
	}
}
