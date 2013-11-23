#ifndef MOTOR_H_  // Only define once
#define MOTOR_H_  // Only define once

#include "CMSIS/a2fxxxm3.h"

#define MOTOR_BASE (FPGA_FABRIC_BASE + 0x100)
volatile uint32_t MOTOR_period, MOTOR_cmpVal;
// The technique of using a structure declaration
// to describe the device register layout and names is
// very common practice. Notice that there aren't actually
// any objects of that type defined, so the declaration
// simply indicates the structure without using up any store.

typedef struct
{
    uint32_t overflow; // Offset 0x0
    uint32_t counter; // Offset 0x4
    uint32_t control; // Offset 0x8
    uint32_t compare; // Offset 0xC
    uint32_t interrupt; //Offset0x10
    uint32_t captureSync; //Offset 0x14
    uint32_t captureAsync; //Offset 0x18
    uint32_t direction; //offset 0x1c
} motor_t;

#define MOTOR_ENABLE_MASK 0x00000001UL

// Using the mytimer_t structure we can make the
// compiler do the offset mapping for us.
// To access the device registers, an appropriately
// cast constant is used as if it were pointing to
// such a structure, but of course it points to memory addresses instead.
// Look at at mytimer.c
// Look at the the functions's disassembly
// in .lst file under the Debug folder

#define MOTOR ((motor_t *) MOTOR_BASE)

/**
 * Initialize the MYTIMER
*/
void MOTOR_init();

/**
 * Start MYTIMER
*/

void MOTOR_enable();
/**
 * Stop MYTIMER
*/

void MOTOR_disable();

/**
 * Set the limit to which the timer counts.
*/

void MOTOR_setOverflowVal(uint32_t value);

/**
 * Enable all interrupts
 */
void MOTOR_enable_allInterrupts();
/**
 * Disable all interrupts
 */

void MOTOR_disable_allInterrupts();
/**
 * Enable compare interrupt
 */

void MOTOR_enable_compareInt();
/**
 * Disable compare interrupt
 */

void MOTOR_disable_compareInt();
/**
 * Set Compare value
 */

void MOTOR_setCompareVal(uint32_t compare);
/**
 * Enable overflow interrupt
 */

void MOTOR_enable_overflowInt();
/**
 * Disable overflow interrupt
 */

void MOTOR_disable_overflowInt();
/**
  * Interrupt status
*/

uint32_t MOTOR_getInterrupt_status();

void MOTOR_enable_pwm();

void MOTOR_disable_pwm();

// < 0 is rev, == 0 is brake, > 0 is fwd
void MOTOR_set_motor_direction(int dir);

//-1 -> 1 sets speed and direction of motors
void MOTOR_set_speed(double percentage);

// < 0 is rev, == 0 is brake, > 0 is fwd
void MOTOR_set_servo_direction(int dir);

#endif /* MYTIMER_H_ */
