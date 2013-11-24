#include "ir_led.h"
#include <stdio.h>
#include "drivers/mss_gpio/mss_gpio.h"
#include "CMSIS/a2fxxxm3.h"

/*
 * Message - 8 bit message to send over IR
 * status - reads back the status of IR LED / RECV
 * status[0] = are we currently sending?
 * status[1] = currently recieving
 * status[2] = newdata?
 *
 *
 */

typedef struct {
	// FPGA_FABRIC_BASE
	volatile uint16_t message;
} ir_led;

static ir_led* const LASER_TAG_LED_write = (ir_led*)(FPGA_FABRIC_BASE + 0x300);
static const ir_led* const LASER_TAG_LED_read = (ir_led*)(FPGA_FABRIC_BASE + 0x300);

static uint8_t IR_LED_buf_full() {
	return (LASER_TAG_LED_read->message >> 8) & 0x01;
}

// Note: THIS DOES NOT BLOCk
//       If you call IR_LED_write multiple times quickly,
//       it will only write once.
//       One write takes about 5ms
uint8_t IR_LED_write(uint8_t msg) {
	// Blocks until we are done sending
	if (IR_LED_buf_full()) return 0;
	LASER_TAG_LED_write->message = msg;
	return 1;
}

// Should only be called when interrupt fired
// Guarented not to change until next interrupt (or reset)
uint8_t IR_LED_read() {
	return (LASER_TAG_LED_read->message & 0x00FF);
}
__attribute__ ((interrupt)) void GPIO0_IRQHandler( void ) {
	volatile uint8_t data = IR_LED_read();
	printf("Data: %x\r\n", data);
	MSS_GPIO_clear_irq( MSS_GPIO_0 );
}
