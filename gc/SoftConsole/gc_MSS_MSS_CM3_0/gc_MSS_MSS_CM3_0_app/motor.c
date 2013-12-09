#include "motor.h"
#include <stdio.h>


void MOTOR_init()
{
    MOTOR_setOverflowVal(MOTOR_period);
    MOTOR_setCompareVal(MOTOR_cmpVal);

    MOTOR_enable_allInterrupts();
    MOTOR_enable_pwm();

    MOTOR_enable();
}

void MOTOR_enable()
{
    MOTOR->control |= MOTOR_ENABLE_MASK;
}

void MOTOR_disable()
{
    MOTOR->control &= ~MOTOR_ENABLE_MASK;
}

void MOTOR_setOverflowVal(uint32_t value)
{
	MOTOR->overflow = value;
}

void MOTOR_enable_allInterrupts()
{
	MOTOR->control |= 14;
}

void MOTOR_disable_allInterrupts()
{
	MOTOR->control &= 49;
}

void MOTOR_enable_compareInt()
{
	MOTOR->control |= 6;
}

void MOTOR_disable_compareInt()
{
	MOTOR->control &= 59;
}

void MOTOR_setCompareVal(uint32_t compare)
{
	MOTOR->compare = compare;
}

void MOTOR_enable_overflowInt()
{
	MOTOR->control |= 10;
}

void MOTOR_disable_overflowInt()
{
	MOTOR->control &= 55;
}

uint32_t MOTOR_getInterrupt_status()
{
	return MOTOR->interrupt;
}

void MOTOR_enable_pwm()
{
	MOTOR->control |= 16;
}

void MOTOR_disable_pwm()
{
	MOTOR->control &= 47;
}

void MOTOR_set_motor_direction(int dir)
{
	if(dir < 0) {
		MOTOR->direction &= 12;
		MOTOR->direction |= 1;
	}
	else if(dir == 0) {
		MOTOR->direction &= 12;
	}
	else {
		MOTOR->direction &= 12;
		MOTOR->direction |= 2;
	}
	//printf("Direction %d\n\r",(int)MOTOR->direction);
}

void MOTOR_set_servo_direction(int dir)
{
	if(dir < 0) {
		MOTOR->direction &= 3;
		MOTOR->direction |= 4;
	}
	else if(dir == 0) {
		MOTOR->direction &= 3;
	}
	else {
		MOTOR->direction &= 3;
		MOTOR->direction |= 8;
	}
	//printf("Direction %d\n\r",(int)MOTOR->direction);
}

void MOTOR_set_speed(double percentage) {
	volatile uint32_t lengthOfLow;
	int isNegative = percentage < 0;
	if(isNegative) {
			percentage *= -1;
			MOTOR_set_motor_direction(-1);
	}
	else if(percentage >= 0.125){
			MOTOR_set_motor_direction(1);
	}
	if(percentage < 0.125) {
			MOTOR_set_motor_direction(0);
	}
	lengthOfLow = MOTOR_period*(1-percentage);
	if(percentage <= 1 && percentage >= -1) {
			MOTOR_setCompareVal(lengthOfLow);
	}
}
